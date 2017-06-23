#ifndef _BEHAVIORMONITOR_H
#define _BEHAVIORMONITOR_H

#include "Common.h"
#include "define.h"
#include "google/protobuf/message.h"
#include "../MsgDefineMacro.h"
#include <stack>
#include "StatisticInfo.pb.h"
#include "../SvrConfig.h"
#include "../StatistDataType.h"
#include "util.h"

using namespace CommBaseOut;
using namespace std;


namespace Behave
{
// 一个行为是一个类型，一个行为包含有多个操作，一个操作包含一个类型与数据
	struct Data
	{
		struct Require
		{
			// 行为必需数据
			int64 money;
			int64 golden;
			int64 force;
			int64 bindGolden;
			Require& operator=(const Require& obj)
			{
				this->money = obj.money;
				this->golden = obj.golden;
				this->force = obj.force;
				this->bindGolden = obj.bindGolden;
				return *this;
			}
		};
		typedef StatisticInfo::BehaviorOtherData Other;  // 其他数据

		Require req; // 存一个行为必须的数据
		Other oth; // 存一个操作的数据
	};

	struct Type
	{
		enum E_TYPE
		{
			eBehaviorType = 1,
			eFlagType,
			eHandleType,
		};

		E_TYPE concept; // 声明以下值的意义
		union
		{
			DWORD64 unionType;
			// 存值时 例如主要类型值是1 次要类型值是2      unionType = 1 | (DWORD64)2 << GET_TYPE_BINARYDIGITS(DWORD);
			// 取值时 则使用first 和 second变量
			struct
			{
				/*
				 * 类型1：first是操作类型 second是无
				 * 类型2：first是标志类型 second是无
				 * 类型3：first是主要类型 second是次要类型
				*/
				DWORD first;
				DWORD second;
			}half;
		};

		void Define(E_TYPE _concept, DWORD _f, DWORD _s)
		{
			concept = _concept;
			unionType = _f | (DWORD64)_s << GET_TYPE_BINARYDIGITS(DWORD);
		}

		struct Switch
		{ // 开关
			enum
			{
				eOff = 0,
				eOn = 1,
			};
		};

	};

	struct HandleData
	{ // 操作数据
		HandleData()
		{

		}

		Type type; // 操作类型
		Data data; // 操作数据

	};
};

class BehaviorMoni
{
public:
	// 一个操作
	void Handle(const int64& charID, const BehaviorHandType::E_Type& handle, const ::google::protobuf::Message* data);

	// 行为开始
	void Begin(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg);
	// 行为结束
	void End(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg);

	// 行为提取
	int Fetch(const int64& charID, StatisticInfo::PlayerBehavior& record);


private:
	void Begin(const int64& charID);
	void End(const int64& charID);

private:
	map<int64, stack<Behave::HandleData> > m_monitorData;

};

class Safe_BehaviorMoni
{
public:
	Safe_BehaviorMoni()
	{
	}
	~Safe_BehaviorMoni()
	{

	}

	// 一个操作
	void Handle(const int64& charID, const BehaviorHandType::E_Type& handle, const ::google::protobuf::Message* data);

	// 行为开始
	void Begin(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg);
	// 行为结束
	void End(const int64& charID, const int& mainType, const int& minorType, const Behave::Data::Require& arg);

	// 行为提取
	int Fetch(const int64& charID, StatisticInfo::PlayerBehavior& record);

private:
	BehaviorMoni m_monitor;
	CSimLock m_lock;

};
#endif // _BEHAVIORMONITOR_H
