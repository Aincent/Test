/*
 * Fashion.cpp
 *
 *  Created on: 2016年4月7日
 *      Author: root
 */

#include "Fashion.h"
#include "Log/Logger.h"
#include "../Attribute/AttributeType.h"
#include "../Object/Player/Player.h"
#include "../Attribute/AttributeManage.h"
#include "../Fighting/Fighting.h"
#include "../Map/SceneManager.h"

FashionShow::FashionShow(Player* player)
{
	// TODO Auto-generated constructor stub
	m_owner = player;
}

FashionShow::~FashionShow()
{
	// TODO Auto-generated destructor stub
}

void FashionShow::ReInit()
{
	m_wardrobe.clear();
}

void FashionShow::Release()
{
	m_wardrobe.clear();
}

void FashionShow::InitWardrobe(const FashionMsgProto::FashionShow& wardrobe)
{
	for (int i = 0; i < wardrobe.list_size(); ++i)
	{
		const FashionMsgProto::FashionInfo& info = wardrobe.list(i);
		Fashion fashion;
		fashion.ID = info.id();
		fashion.strengthLv = info.strengthlv();
		fashion.state = (Fashion::E_FASHION_STATE)(info.status());

		m_wardrobe[fashion.ID] = fashion;
	}

	// 存储初始数据
	m_owner->SetDataFlag(eFashionInfo);
}

void FashionShow::SaveWardrobe(FashionMsgProto::FashionShow& wardrobe) const
{
	Wardrobe::const_iterator it = m_wardrobe.begin();
	Wardrobe::const_iterator end = m_wardrobe.end();
	for (; it != end; ++it)
	{
		FashionMsgProto::FashionInfo* info = wardrobe.add_list();
		info->set_id(it->second.ID);
		info->set_strengthlv(it->second.strengthLv);
		info->set_status(it->second.state);
	}
}

void FashionShow::Show(FashionMsgProto::RetFashionInfo& show) const
{
	Wardrobe::const_iterator it = m_wardrobe.begin();
	Wardrobe::const_iterator end = m_wardrobe.end();
	for (; it != end; ++it)
	{
		FashionMsgProto::FashionInfo* info = show.add_show();
		info->set_id(it->second.ID);
		info->set_strengthlv(it->second.strengthLv);
		info->set_status(it->second.state);
	}
}

void FashionShow::Show(const int& fashionID, FashionMsgProto::FashionInfo& show) const
{
	const Wardrobe::const_iterator it = m_wardrobe.find(fashionID);
	if (m_wardrobe.end() == it)
	{
		return;
	}

	show.set_id(it->second.ID);
	show.set_strengthlv(it->second.strengthLv);
	show.set_status(it->second.state);

}

// 激活
int FashionShow::Activate(const int& fashionID)
{
	// 激活检查并消耗
	int err = Activating(fashionID);
	if (0 != err)
	{
		return err;
	}

	// 激活检查消耗完毕
	// 完成激活
	Activated(fashionID);

	return 0;
}

int FashionShow::Activate(const int& fashionID, FashionMsgProto::FashionInfo& show)
{
	int err = Activate(fashionID);
	if (0 != err)
	{
		return err;
	}

	Show(fashionID, show);

	return 0;
}

int FashionShow::Dressup(const int& fashionID, Fashion::E_FASHION_STATE is)
{
	int err = Dressing(fashionID, is);
	if (0 != err)
	{
		return err;
	}

	switch (is)
	{
	case Fashion::eFashion_Wear:
		{
			Wore(fashionID);
		}
		break;
	case Fashion::eFashion_Drop:
		{
			Dropped(fashionID);
		}
		break;
	default:
		{
			return -1;
		}
	}

	return 0;
}

int FashionShow::Dressup(const int& fashionID, Fashion::E_FASHION_STATE is, FashionMsgProto::FashionInfo& show)
{
	int err = Dressup(fashionID, is);
	if (0 != err)
	{
		return err;
	}

	Show(fashionID, show);

	return 0;
}

int FashionShow::Strength(const int& fashionID)
{
	int err = Strengthening(fashionID);
	if (0 != err)
	{
		return err;
	}

	// 强化完成
	Strengthened(fashionID);

	return 0;
}

int FashionShow::Strength(const int& fashionID, FashionMsgProto::FashionInfo& show)
{
	int err = Strength(fashionID);
	if (0 != err)
	{
		return err;
	}

	Show(fashionID, show);

	return 0;
}

int FashionShow::Display(const int& fashionID, Fashion::E_FASHION_STATE is)
{
	int err = Displaying(fashionID, is);
	if (0 != err)
	{
		return err;
	}

	switch (is)
	{
	case Fashion::eFashion_Display:
		{
			Display(fashionID);
		}
		break;
	case Fashion::eFashion_Hide:
		{
			Hide(fashionID);
		}
		break;
	default:
		{
			return -1;
		}
	}

	return 0;
}

// 上线计算战斗力
void FashionShow::OnlineCalAttr()
{
	map<ECharAttrType,int64> attr;

	GetCurAdditionalAttr(attr);

	eAttValueType attrValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttFashion);

	m_owner->calculateResultAddAtt(attr, attrValueType);

	CalFighting(attr);
}

void FashionShow::CalFighting()
{
	map<ECharAttrType,int64> attr;
	GetCurAdditionalAttr(attr);

	int fighting = m_owner->getFightingManage()->CalculateFighting(attr);

	m_owner->getFightingManage()->setFighting(eFightingFashion, fighting);
}

//获得当前时装所增加的属性
void FashionShow::GetCurAdditionalAttr(map<ECharAttrType,int64>& attr)
{
	Wardrobe::const_iterator it = m_wardrobe.begin();
	Wardrobe::const_iterator end = m_wardrobe.end();
	for (; it != end; ++it)
	{
		const Fashion& fashion = it->second;
		const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashion.ID);
		if (NULL == info)
		{
			return;
		}

		FashionInfo::Attr::const_iterator itAttr = info->activateInfo.add.attr.begin();
		FashionInfo::Attr::const_iterator endAttr = info->activateInfo.add.attr.end();
		for (; itAttr != endAttr; ++itAttr)
		{
			attr[itAttr->first] += itAttr->second;
		}

		if (Fashion::eFashion_Wear == fashion.state)
		{
			const FashionInfo::Config config = FashionLoader::GetInstance()->GetFashionConfig();
			for (int lv = config.strengthMinLv; lv <= fashion.strengthLv; ++lv)
			{
				const FashionInfo::Fashion::StrengthMap::const_iterator strengthIt = info->strengthInfo.find(lv);
				if (info->strengthInfo.end() == strengthIt)
				{
					return;
				}

				const FashionInfo::Strength& strengthInfo = strengthIt->second;

				itAttr= strengthInfo.add.attr.begin();
				endAttr = strengthInfo.add.attr.end();
				for (; itAttr != endAttr; ++itAttr)
				{
					attr[itAttr->first] += itAttr->second;
				}
			}
		}
	}
}

void FashionShow::CalFighting(map<ECharAttrType,int64>& attr)
{
	int fighting = m_owner->getFightingManage()->CalculateFighting(attr);

	m_owner->getFightingManage()->setFighting(eFightingFashion, fighting);
}

int FashionShow::Activating(const int& fashionID)
{
	// 检查
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
	if (NULL == info)
	{
		LOG_ERROR(FILEINFO, "Fashion info cannot find when activate fashion. Fashion ID:%d.", fashionID);
		return -1;
	}

	if (!parcelMgr)
	{
		LOG_ERROR(FILEINFO, "Fashion dressing not parcel manager when activate fashion.");
		return -1;
	}

	if (m_wardrobe.end() != m_wardrobe.find(fashionID))
	{
		return eFashionExist;
	}

	map<int, bool> isBind;

	int err = 0;
	const FashionInfo::Conditions& cond = info->activateInfo.cond;
	err = ConsumeCheck(cond, isBind);
	if (0 != err)
	{
		return err;
	}

	Consume(cond, isBind);
//	const FashionInfo::Conditions::const_iterator condIt = cond.begin();
//	const FashionInfo::Conditions::const_iterator condEnd = cond.end();
//	// 消耗物品
//	condIt = cond.begin();
//	condEnd = cond.end();
//	for (; condIt != condEnd; ++condIt)
//	{
//		switch (condIt->needBindCond)
//		{
//		case FashionInfo::Cond::eCondBind_AllCan: // 都能
//			{
		// 只调用DeleteGoodsById 鉴于已经确定了此绑定类型的物品已经足够扣除
		// 所以自信并相信这个接口不会删除错非此绑定类型的物品
//		m_owner->GetContainer(ePackageType)->DeleteGoodsById(condIt->needItemID, condIt->needItemCnt, isBind[condIt->needItemID]);

//			}
//			break;
//		case FashionInfo::Cond::eCondBind_JustIs: // 必须是绑定
//			{
//				vector<int> pos;
//				existCnt = parcelMgr->FindPosByPropID(condIt->needItemID, pos, true);

//				m_owner->GetContainer(ePackageType)->DeleteGoodsById(condIt->needItemID, condIt->needItemCnt, true);
//			}
//			break;
//		case FashionInfo::Cond::eCondBind_JustNot: // 必须非绑定
//			{
//				vector<int> pos;
//				existCnt = parcelMgr->FindPosByPropID(condIt->needItemID, pos, false);
//				m_owner->GetContainer(ePackageType)->DeleteGoodsById(condIt->needItemID, condIt->needItemCnt, false);
//			}
//			break;
//		default:
//			{
//
//			}
//		}
//	}

	return 0;
}

void FashionShow::Activated(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	if (NULL == info || m_wardrobe.find(fashionID) != m_wardrobe.end())
	{
		return;
	}
	Fashion fashion;
	fashion.ID = info->ID;
	fashion.state = Fashion::eFashion_Default;
	fashion.strengthLv = 0;

	m_wardrobe[fashionID] = fashion;

	ActivateEffect(fashionID);

	// 存储数据
	m_owner->SetDataFlag(eFashionInfo);
}

// 装扮中 消耗与检查
int FashionShow::Dressing(const int& fashionID, Fashion::E_FASHION_STATE is)
{
	// 检查
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
	if (NULL == info)
	{
		LOG_ERROR(FILEINFO, "Fashion info cannot find when dress fashion. Fashion ID:%d.", fashionID);
		return -1;
	}
	if (!parcelMgr)
	{
		LOG_ERROR(FILEINFO, "Fashion dressing not parcel manager when dress fashion.");
		return -1;
	}

	Wardrobe::iterator it = m_wardrobe.find(fashionID);
	if (m_wardrobe.end() == it)
	{
		LOG_WARNING(FILEINFO, "Cannot find id:%d when fashion dress.", fashionID);
		return -1;
	}
	if (it->second.state == is)
	{
		LOG_WARNING(FILEINFO, "The state of the fashion and will change state equal. fashionID:%d, will change state:%d.", fashionID, is);
		return -1;
	}

	return 0;
}

int FashionShow::Displaying(const int& fashionID, Fashion::E_FASHION_STATE is)
{
	Wardrobe::const_iterator it = m_wardrobe.find(fashionID);
	if (m_wardrobe.end() == it)
	{
		return -1;
	}

	switch (is)
	{
	case Fashion::eFashion_Display:
		{
			if (0 != m_owner->GetFashionID() || it->second.state == Fashion::eFashion_Display)
			{
				return -1;
			}
		}
		break;
	case Fashion::eFashion_Hide:
		{
			if (0 == m_owner->GetFashionID() || m_owner->GetFashionID() != fashionID ||
					it->second.state == Fashion::eFashion_Hide)
			{
				return -1;
			}
		}
		break;
	default:
		{
			return -1;
		}
	}

	return 0;
}

void FashionShow::Wore(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	Wardrobe::iterator it = m_wardrobe.begin();
	Wardrobe::iterator end = m_wardrobe.end();

	Dropped(GetWoreID());

	m_wardrobe[fashionID].state = Fashion::eFashion_Wear;

	m_owner->SetFashionID(fashionID);
	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SetDataFlag(eFashionInfo);

	DressEffect(fashionID, Fashion::eFashion_Wear);

	BroadDress();
}
void FashionShow::Dropped(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	m_wardrobe[fashionID].state = Fashion::eFashion_Drop;

	m_owner->SetFashionID(0);
	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SetDataFlag(eFashionInfo);

	DressEffect(fashionID, Fashion::eFashion_Drop);

	BroadDress();
}

// 强化中 消耗与检查
int FashionShow::Strengthening(const int& fashionID)
{
	// 检查
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
	if (NULL == info)
	{
		LOG_ERROR(FILEINFO, "Fashion info cannot find when strengthen fashion. Fashion ID:%d.", fashionID);
		return -1;
	}
	if (!parcelMgr)
	{
		LOG_ERROR(FILEINFO, "Fashion dressing not parcel manager when strengthen fashion.");
		return -1;
	}

	Wardrobe::iterator it = m_wardrobe.find(fashionID);
	if (m_wardrobe.end() == it)
	{
		LOG_WARNING(FILEINFO, "Cannot find id:%d when strengthen fashion.", fashionID);
		return -1;
	}
	const FashionInfo::Config config = FashionLoader::GetInstance()->GetFashionConfig();
	if (it->second.strengthLv == config.strengthMaxLv)
	{
		return eFashionStrengthTopLv;
	}

	int newLv = it->second.strengthLv + 1;
	FashionInfo::Fashion::StrengthMap::const_iterator strengthIt = info->strengthInfo.find(newLv);
	if (strengthIt == info->strengthInfo.end())
	{
		LOG_WARNING(FILEINFO, "Cannot find strength attr(id:%d, lv:%d) when strengthen fashion.", fashionID, newLv);
		return -1;
	}

	map<int, bool> isBind;
	int err = 0;
	const FashionInfo::Conditions& cond = strengthIt->second.cond;
	err = ConsumeCheck(cond, isBind);
	if (0 != err)
	{
		return err;
	}

	Consume(cond, isBind);

	return 0;
}

// 完成强化
void FashionShow::Strengthened(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	if (0 >= m_wardrobe[fashionID].strengthLv)
	{
		m_wardrobe[fashionID].strengthLv = info->strengthInfo.begin()->second.lv;
	}
	else
	{
		int oldLv = m_wardrobe[fashionID].strengthLv;
		m_wardrobe[fashionID].strengthLv = ++oldLv;
	}

	StrengthenEffect(fashionID);

	m_owner->SetDataFlag(eFashionInfo);
}

void FashionShow::Display(const int& fashionID)
{
	if (0 != m_owner->GetFashionID() || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	m_wardrobe[fashionID].state = Fashion::eFashion_Display;


	m_owner->SetFashionID(fashionID);
	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SetDataFlag(eFashionInfo);
	BroadDress();
}
void FashionShow::Hide(const int& fashionID)
{
	if (0 == m_owner->GetFashionID() || fashionID != m_owner->GetFashionID() || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	m_wardrobe[fashionID].state = Fashion::eFashion_Hide;
	m_owner->SetFashionID(0);
	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SetDataFlag(eFashionInfo);
	BroadDress();
}


void FashionShow::Consume(const FashionInfo::Conditions& cond, map<int, bool>& isBind)
{
	FashionInfo::Conditions::const_iterator condIt = cond.begin();
	FashionInfo::Conditions::const_iterator condEnd = cond.end();
	// 消耗物品
	for (; condIt != condEnd; ++condIt)
	{
		m_owner->GetContainer(ePackageType)->DeleteGoodsById(condIt->needItemID, condIt->needItemCnt, isBind[condIt->needItemID]);
	}
}

int FashionShow::ConsumeCheck(const FashionInfo::Conditions& cond, map<int, bool>& isBind) const
{
	Smart_Ptr<ArticleParcelMgr> parcelMgr = m_owner->GetContainer(ePackageType);
	if (!parcelMgr)
	{
		return -1;
	}
	int err = 0;
	FashionInfo::Conditions::const_iterator condIt = cond.begin();
	FashionInfo::Conditions::const_iterator condEnd = cond.end();
	for (; condIt != condEnd; ++condIt)
	{
		err = condIt->Check(parcelMgr, isBind);
		if (0 != err)
		{
			return err;
		}
	}

	return 0;
}

// 广播装扮
void FashionShow::BroadDress()
{
	ServerReturn::ServerRetInt ret;

	ret.set_ret(((int64)m_owner->GetFashionID() << 32) | m_owner->GetKey());
	CSceneMgr::GetInstance()->SendSynchMessage(&ret, MSG_SIM_GM2C_HANDLEFASHION, m_owner->GetMapID(), m_owner->GetKey(), m_owner->GetXNewPos(), m_owner->GetYNewPos(), true);

}

void FashionShow::ActivateEffect(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);
	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	eAttValueType attrValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttFashion);

	vector<int> attrType;
	FashionInfo::Attr::const_iterator itAttr = info->activateInfo.add.attr.begin();
	FashionInfo::Attr::const_iterator endAttr = info->activateInfo.add.attr.end();
	for (; itAttr != endAttr; ++itAttr)
	{
		m_owner->addAtt(itAttr->first, itAttr->second, true, attrValueType);
		attrType.push_back(itAttr->first);
	}

	if (0 < attrType.size())
	{
		// 重新计算战斗力
		CalFighting();

		attrType.push_back(eFightPower);
		// 同步属性
		m_owner->SynCharAttribute(attrType);
	}
}

void FashionShow::StrengthenEffect(const int& fashionID)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);

	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end() || m_wardrobe[fashionID].state != Fashion::eFashion_Wear)
	{
		return;
	}

	int strengthLv = m_wardrobe[fashionID].strengthLv;

	FashionInfo::Fashion::StrengthMap::const_iterator it = info->strengthInfo.find(strengthLv);
	if (it == info->strengthInfo.end())
	{
		return;
	}

	const FashionInfo::Strength& strengthInfo = it->second;

	eAttValueType attrValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttFashion);

	vector<int> attrType;
	FashionInfo::Attr::const_iterator itAttr= strengthInfo.add.attr.begin();
	FashionInfo::Attr::const_iterator endAttr = strengthInfo.add.attr.end();
	for (; itAttr != endAttr; ++itAttr)
	{
		m_owner->addAtt(itAttr->first, itAttr->second, true, attrValueType);
		attrType.push_back(itAttr->first);
	}

	const FashionInfo::Strength::Add::Buff& buff =  strengthInfo.add.buff;
	FashionInfo::Strength::Add::Buff::const_iterator itBuff = buff.begin();
	FashionInfo::Strength::Add::Buff::const_iterator endBuff = buff.end();
	for (; itBuff != endBuff; ++itBuff)
	{
		Smart_Ptr<CreatureObj> creatureObj = m_owner->GetCreatureSelf();
		m_owner->AddBuff(*itBuff, creatureObj);
	}

	if (0 < attrType.size())
	{
		// 重新计算战斗力
		CalFighting();

		attrType.push_back(eFightPower);
		// 同步属性
		m_owner->SynCharAttribute(attrType);
	}

}
void FashionShow::DressEffect(const int& fashionID, Fashion::E_FASHION_STATE is)
{
	const FashionInfo::Fashion* info = FashionLoader::GetInstance()->GetFashionInfo(fashionID);

	if (NULL == info || m_wardrobe.find(fashionID) == m_wardrobe.end())
	{
		return;
	}

	// 改变属性
	int strengthLv = m_wardrobe[fashionID].strengthLv;

	bool isAdd = false;
	switch (is)
	{
	case Fashion::eFashion_Wear:
		{
			isAdd = true;
		}
		break;
	case Fashion::eFashion_Drop:
		{
			isAdd = false;
		}
		break;
	default:
		{
			return ;
		}
	}
	eAttValueType attrValueType = AttributeManage::GetInstance()->getAttValueType(eEachModuleAttFashion);

	const FashionInfo::Config config = FashionLoader::GetInstance()->GetFashionConfig();
	vector<int> attrType;
	for (int lv = config.strengthMinLv; lv <= strengthLv; ++lv)
	{
		FashionInfo::Fashion::StrengthMap::const_iterator strengthIt = info->strengthInfo.find(lv);
		if (info->strengthInfo.end() == strengthIt)
		{
	//		LOG_ERROR(FILEINFO, "Cannot find this fashion id:%d strengthLv:%d when change strength attribute", fashionID, strengthLv);
			return;
		}
		const FashionInfo::Strength& strengthInfo = strengthIt->second;

		FashionInfo::Attr::const_iterator itAttr= strengthInfo.add.attr.begin();
		FashionInfo::Attr::const_iterator endAttr = strengthInfo.add.attr.end();
		for (; itAttr != endAttr; ++itAttr)
		{
			m_owner->addAtt(itAttr->first, itAttr->second, isAdd,attrValueType);
			attrType.push_back(itAttr->first);
		}

		const FashionInfo::Strength::Add::Buff& buff =  strengthInfo.add.buff;
		FashionInfo::Strength::Add::Buff::const_iterator itBuff = buff.begin();
		FashionInfo::Strength::Add::Buff::const_iterator endBuff = buff.end();
		for (; itBuff != endBuff; ++itBuff)
		{
			Smart_Ptr<CreatureObj> creature = m_owner->GetCreatureSelf();
			m_owner->AddBuff(*itBuff, creature);
		}
	}

	if (0 < attrType.size())
	{
		// 重新计算战斗力
		CalFighting();

		attrType.push_back(eFightPower);
		// 同步属性
		m_owner->SynCharAttribute(attrType);
	}
}

// 获得已穿戴时装
int FashionShow::GetWoreID()
{
	for (Wardrobe::const_iterator it = m_wardrobe.begin(); it != m_wardrobe.end(); ++it)
	{
		if (it->second.state > Fashion::eFashion_Drop)
		{
			return it->second.ID;
		}
	}

	return 0;
}



