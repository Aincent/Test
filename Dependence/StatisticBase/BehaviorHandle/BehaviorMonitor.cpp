#include "BehaviorMonitor.h"


void BehaviorMoni::Handle(const int64& charID, const BehaviorHandType::E_Type& handle, const ::google::protobuf::Message* data)
{
	map<int64, stack<Behave::HandleData> >::iterator it = m_monitorData.find(charID);
	if (m_monitorData.end() == it || 0 >= it->second.size())
	{
		return;
	}

	Behave::HandleData handleData;
	handleData.type.Define(Behave::Type::eHandleType, handle, 0);

	char buff[NORMAL_MSG_SEND_SIZE] = {0};
	int size = data->ByteSize();
	data->SerializePartialToArray(buff, size);

	handleData.data.oth.set_data(buff, size);
	handleData.data.oth.set_len(size);
	handleData.data.oth.set_handletype(handle);

	it->second.push(handleData);
}

void BehaviorMoni::Begin(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg)
{
	Begin(charID);

	Behave::HandleData handleData;
//	handle.type.concept = Behave::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << GET_TYPE_BINARYDIGITS(DWORD) | minorType; // 主要类型<<32 | 次要类型
	handleData.type.Define(Behave::Type::eBehaviorType, mainType, minorType);
	handleData.data.req = arg;
	m_monitorData[charID].push(handleData);
}


void BehaviorMoni::End(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg)
{
	map<int64, stack<Behave::HandleData> >::iterator it = m_monitorData.find(charID);
	if (m_monitorData.end() == it || 0 >= it->second.size())
	{
		return;
	}

	Behave::HandleData handleData;
//	handle.type.concept = Behave::Type::eBehaviorType; // 声明以下值是行为类型
//	handle.type.unionType = (DWORD64)mainType << sizeof(DWORD) | minorType; // 主要类型<<32 | 次要类型
	handleData.type.Define(Behave::Type::eBehaviorType, mainType, minorType);
	handleData.data.req = arg;
	m_monitorData[charID].push(handleData);

	End(charID);
}

int BehaviorMoni::Fetch(const int64& charID, StatisticInfo::PlayerBehavior& record)
{
	map<int64, stack<Behave::HandleData> >::iterator it = m_monitorData.find(charID);

	if (m_monitorData.end() == it || 0 >= it->second.size() ||
			it->second.top().type.concept != Behave::Type::eFlagType ||
			it->second.top().type.half.first != Behave::Type::Switch::eOff
			)
	{
		return -1;
	}

	StatisticInfo::PlayerBehavior tmp;


	bool eFlag = false, bFlag = false, isFetch = true;
	while (1)
	{
		if (0 >= it->second.size() || !isFetch)
		{
			break;
		}

		const Behave::HandleData& handle = it->second.top();
		switch (handle.type.concept)
		{
		case Behave::Type::eBehaviorType:
			{
				if (eFlag)
				{
					tmp.set_maintype(handle.type.half.first);
					tmp.set_minortype(handle.type.half.second);

					eFlag = false;
					tmp.set_aftermoney(handle.data.req.money);
					tmp.set_aftergolden(handle.data.req.golden);
					tmp.set_afterforce(handle.data.req.force);
					tmp.set_afterbindgolden(handle.data.req.bindGolden);
				}
				else if (!bFlag)
				{
					if ((uint)tmp.maintype() != handle.type.half.first || (uint)tmp.minortype() != handle.type.half.second)
					{
						m_monitorData.clear();
						return -1;
					}

					bFlag = true;

					tmp.set_beforemoney(handle.data.req.money);
					tmp.set_beforegolden(handle.data.req.golden);
					tmp.set_beforeforce(handle.data.req.force);
					tmp.set_beforebindgolden(handle.data.req.bindGolden);
				}
			}
			break;
		case Behave::Type::eFlagType:
			{
				if (Behave::Type::Switch::eOff == handle.type.half.first)
				{
					eFlag = true;
				}
				else if(Behave::Type::Switch::eOn == handle.type.half.first)
				{
					bFlag = false;
					isFetch = false;
				}
			}
			break;
		case Behave::Type::eHandleType:
			{
				StatisticInfo::BehaviorOtherData* oth = tmp.mutable_othdata()->add_list();
				oth->CopyFrom(handle.data.oth);
			}
			break;
		default:
			{
			}
		}

		it->second.pop();
	}

	if (bFlag | eFlag)
	{
		return -1;
	}

	// 标记所有者
	tmp.set_charid(GET_PLAYER_CHARID(charID));
	tmp.set_time(CUtil::GetNowSecond());

	if (0 >= it->second.size())
	{
		m_monitorData.erase(it);
	}

	record = tmp;

	return 0;
}

void BehaviorMoni::Begin(const int64& charID)
{
	Behave::HandleData handleData;
//	handle.type.concept = Behave::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behave::Type::Switch::eOn << GET_TYPE_BINARYDIGITS(DWORD); // 开始行为监视类型
	handleData.type.Define(Behave::Type::eFlagType, Behave::Type::Switch::eOn, 0);
	m_monitorData[charID].push(handleData);
}

void BehaviorMoni::End(const int64& charID)
{
	Behave::HandleData handleData;
//	handle.type.concept = Behave::Type::eFlagType; // 声明以下值是标志含义 begin/end
//	handle.type.unionType = (DWORD64)Behave::Type::Switch::eOff << GET_TYPE_BINARYDIGITS(DWORD); // 结束行为监视类型
	handleData.type.Define(Behave::Type::eFlagType, Behave::Type::Switch::eOff, 0);
	m_monitorData[charID].push(handleData);
}


void Safe_BehaviorMoni::Handle(const int64& charID, const BehaviorHandType::E_Type& handle, const ::google::protobuf::Message* data)
{
	GUARD(CSimLock, obj, &m_lock);
	m_monitor.Handle(charID, handle, data);
}

// 行为开始
void Safe_BehaviorMoni::Begin(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg)
{
	GUARD(CSimLock, obj, &m_lock);
	m_monitor.Begin(charID, mainType, minorType, arg);

}
// 行为结束
void Safe_BehaviorMoni::End(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg)
{
	GUARD(CSimLock, obj, &m_lock);
	m_monitor.End(charID, mainType, minorType, arg);

}

// 行为提取
int Safe_BehaviorMoni::Fetch(const int64& charID, StatisticInfo::PlayerBehavior& record)
{
	GUARD(CSimLock, obj, &m_lock);
	return m_monitor. Fetch(charID, record);
}
