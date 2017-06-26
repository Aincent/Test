/*
 * StatistDataType.h
 *
 *  Created on: 2016年5月13日
 *      Author: root
 */

#ifndef STATISTDATATYPE_H_
#define STATISTDATATYPE_H_

#include "define.h"
#include "ServerReturnInt.pb.h"
#include "ServerReturn2Int.pb.h"
#include "ServerReturn3Int.pb.h"
#include "ServerReturnIntChar.pb.h"
#include "ServerReturnChar.pb.h"

using namespace CommBaseOut;
using namespace std;

#define SAVET_RET_PROTO_FUNC  \
	virtual const ::google::protobuf::Message* Get() const  \
	{  \
		return &_c;  \
	}

#define BEH_RET_PROTO_FUNC  \
	virtual const ::google::protobuf::Message* Get() const  \
	{  \
		return m_content.Get();  \
	}


namespace StatistSaveType
{
	enum E_TYPE
	{
		eInt64 = 1,
		eTwoDW64,
		eThrDW64,
		eTwoDW64Str,
		eStr,
		eThrDW64Str,
//		eIntToInt,
//		eVector,
//		eVecToStr,
//		eMap,
//		eDW64AndVec,
//		eVecByIntToStr,
		eSaveTypeMax,
	};

	struct Type_Data
	{
		Type_Data(E_TYPE t) : _t(t){ }
		virtual ~Type_Data() { }
		virtual const ::google::protobuf::Message* Get() const = 0;

		E_TYPE _t;

	};

	struct Int64_Data : public Type_Data
	{
		Int64_Data(int64 param) : Type_Data(eInt64)
		{
			_c.set_ret(param);
		}
		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerRetInt _c;
	};

	struct TwoDW64_Data : public Type_Data
	{
		TwoDW64_Data(DWORD64 paramF, DWORD64 paramS) : Type_Data(eTwoDW64)
		{
			_c.set_retf(paramF);
			_c.set_rets(paramS);
		}
		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerDoubleInt _c;
	};
	struct ThrDW64_Data : public Type_Data
	{
		ThrDW64_Data(DWORD64 paramF, DWORD64 paramS, DWORD64 paramT) : Type_Data(eThrDW64)
		{
			_c.set_retf(paramF);
			_c.set_rets(paramS);
			_c.set_rett(paramT);
		}

		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerThreeInt _c;
	};

	struct TwoDW64Str_Data : public Type_Data
	{
		TwoDW64Str_Data(DWORD64 paramF, DWORD64 paramS, string paramT) : Type_Data(eTwoDW64Str)
		{
			_c.set_retf(paramF);
			_c.set_rets(paramS);
			_c.set_rett(paramT);
		}

		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerDoubleIntChar _c;
	};


	struct ThrDW64Str_Data : public Type_Data
	{
		ThrDW64Str_Data(DWORD64 paramF, DWORD64 paramS, DWORD64 paramT,string paramFO) : Type_Data(eThrDW64Str)
		{
			_c.set_retf(paramF);
			_c.set_rets(paramS);
			_c.set_rett(paramT);
			_c.set_retfo(paramFO);
		}

		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerThreeIntChar _c;
	};

	struct Str_Data : public Type_Data
	{
		Str_Data(const std::string& str) : Type_Data(eStr)
		{
			_c.set_ret(str);
		}

		SAVET_RET_PROTO_FUNC

		ServerReturn::ServerRetChar _c;
	};
}


namespace BehaviorHandType
{
	enum E_Type
	{
		eCostGolden = 1,
		eCostMoney,
		eAddItem,
		eDelItem,
		eCostForce,
		eAddForce,
		eAddGolden,
		eAddMoney,
		eCostBindGolden,
		eAddBindGolden,
		eAddAcupoint,
		eCostCredit,
		eAddCredit,
		eUpVIP, // 升级VIP
		eUpClanLv, // 帮派等级升级  Int64_Data
		eAddClanMember, // 增加帮派成员
		eRedClanMember, // 减少帮派成员
		eHoldPost, // 就职
		eClanContribute, // 捐献
		eAddExp, // 增加经验
		eRechargeRes, // 充值结果
		eLoginRes, // 登陆信息
		eOfflineRes, // 下线信息
	};

	struct HandleType
	{
		HandleType(const E_Type& handleType) : m_handleType(handleType)
		{
		}
		virtual ~HandleType(){}

		const E_Type& GetHandleType() const
		{
			return m_handleType;
		}

		virtual const ::google::protobuf::Message* Get() const = 0;

		E_Type m_handleType;
	};

	struct CostGolden : public HandleType
	{
		CostGolden(const int64& cost) : HandleType(eCostGolden), m_content(cost)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};

	struct CostMoney : public HandleType
	{
		CostMoney(const int64& cost) : HandleType(eCostMoney), m_content(cost)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};

	struct AddItem : public HandleType
	{
		AddItem(const DWORD64& itemID, const DWORD64& itemNum) : HandleType(eAddItem), m_content(itemID, itemNum)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::TwoDW64_Data m_content;
	};

	struct DelItem : public HandleType
	{
		DelItem(const DWORD64& itemID, const DWORD64& itemNum) : HandleType(eDelItem), m_content(itemID, itemNum)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::TwoDW64_Data m_content;
	};
	struct CostForce : public HandleType
	{
		CostForce(const int64& cost) : HandleType(eCostForce), m_content(cost)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddForce : public HandleType
	{
		AddForce(const int64& add) : HandleType(eAddForce), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddGolden : public HandleType
	{
		AddGolden(const int64& add) : HandleType(eAddGolden), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddMoney : public HandleType
	{
		AddMoney(const int64& add) : HandleType(eAddMoney), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct CostBindGolden : public HandleType
	{
		CostBindGolden(const int64& cost) : HandleType(eCostBindGolden), m_content(cost)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddBindGolden : public HandleType
	{
		AddBindGolden(const int64& add) : HandleType(eAddBindGolden), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddAcupoint : public HandleType
	{
		AddAcupoint(const int64& add) : HandleType(eAddAcupoint), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct CostCredit : public HandleType
	{
		CostCredit(const int64& cost) : HandleType(eCostCredit), m_content(cost)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddCredit : public HandleType
	{
		AddCredit(const int64& add) : HandleType(eAddCredit), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct UpVIP : public HandleType
	{
		UpVIP(const int64& upVip) : HandleType(eUpVIP), m_content(upVip)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct UpClanLv : public HandleType
	{
		UpClanLv(const int64& clanLv) : HandleType(eUpClanLv), m_content(clanLv)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
	struct AddClanMember : public HandleType
	{
		AddClanMember(const DWORD64& clanID, const DWORD64& charID) : HandleType(eAddClanMember), m_content(clanID, charID)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::TwoDW64_Data m_content;
	};
	struct RedClanMember : public HandleType
	{
		RedClanMember(const DWORD64& clanID, const DWORD64& charID) : HandleType(eRedClanMember), m_content(clanID, charID)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::TwoDW64_Data m_content;
	};
	struct HoldPost : public HandleType
	{
		HoldPost(const DWORD64& clanID, const DWORD64& charID, const DWORD64& job) : HandleType(eHoldPost), m_content(clanID, charID, job)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::ThrDW64_Data m_content;
	};
	struct ClanContribute : public HandleType
	{
		ClanContribute(const DWORD64& money, const DWORD64& golden, const DWORD64& contri) : HandleType(eClanContribute), m_content(money, golden, contri)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::ThrDW64_Data m_content;
	};

	struct AddExp : public HandleType
	{
		AddExp(const int64& add) : HandleType(eAddExp), m_content(add)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};

	struct RechargeRes : public HandleType
	{
		RechargeRes(const DWORD64& realMoney, const DWORD64& addGolden, const DWORD64& isFirst,const string& orderId) :
			HandleType(eRechargeRes), m_content(realMoney, addGolden, isFirst,orderId)
		{

		}
		BEH_RET_PROTO_FUNC

		const StatistSaveType::ThrDW64Str_Data m_content;
	};

	struct LoginRes : public HandleType
	{
		LoginRes(const string& IP) : HandleType(eLoginRes), m_content(IP)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Str_Data m_content;
	};

	struct OfflineRes : public HandleType
	{
		OfflineRes(const int64& onlineTime) : HandleType(eOfflineRes), m_content(onlineTime)
		{
		}

		BEH_RET_PROTO_FUNC

		const StatistSaveType::Int64_Data m_content;
	};
}


#endif /* STATISTDATATYPE_H_ */
