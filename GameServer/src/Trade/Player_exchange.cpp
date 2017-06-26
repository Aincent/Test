////
///* ★Name				:   Player_exchange.cpp
// *  Created on: 2014.5.8
// *      Author: cz
// */
////			:	玩家间交易处理
////////////////////////////////////////////////////////////////////////////

#include "../Object/Player/Player.h"
#include "../Map/GameZone.h"
#include "../Map/SceneManager.h"
#include "msg_Exchang.h"
#include "Exchang_define.h"
#include "Exchang.h"
#include "define.h"
#include "MessageCommonRet.h"

//------------------------------------------------------------------------
// 请求交易
//------------------------------------------------------------------------
DWORD Player::ProcExchangeReq(Player* &pTarget, DWORD dwTgtPlayerID)
{
	// 是否自己和自己交易
	if((DWORD)GetKey() == dwTgtPlayerID)
	{
		return eNotWithMe;
	}

	// 获取地图
   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
   if(!zone)
	 {
	   return E_Exchange_PlayeLogOut;
	 }

	// 被申请者检查
	Smart_Ptr<CreatureObj> tempObj = pTarget;
	zone->GetObject(dwTgtPlayerID, tempObj);
	pTarget = (Player*)tempObj.Get();
	if(!pTarget)
	{
		// 被申请者不在线,或与申请者不在同一张地图中
		return E_Exchange_NotInSame_Map;
	}

	// 申请者是否可以发起交易
	if(!CanExchange())
	{
		return E_Exchange_SelfBusy;
	}

	if(IsDead())
	{
		return E_Exchange_PlayeDead;
	}

	if(pTarget->IsDead())
	{
		return E_Exchange_PlayeDead;
	}

	// 被申请者是否可交易
	if(!pTarget->CanExchange())
	{
		return E_Exchange_PlayerBusy;
	}

	// 交易距离
	if(IsInDistance(pTarget, (DWORD)MAX_EXCHANGE_DISTANCE) == false)
	{
		return E_Exchange_OutOfRange;
	}

	// 设置申请者交易状态
	BeginExchange(dwTgtPlayerID);

	// 设置被申请者状态
	pTarget->GetExchMgr().SetTgtPlayerKey(GetKey());

	return 0;
}

Smart_Ptr<PlayerCampShopMgr> & Player::getCampShopMgr()
{
		return m_campShopMgr;
}

Smart_Ptr<BaseShopMgr> & Player::GetBaseShopMgr()
{
	return m_baseShopMgr;
}

//
////------------------------------------------------------------------------
//// 玩家对请求交易的反馈
////------------------------------------------------------------------------
DWORD Player::ProcExchangeReqRes(Player* &pApplicant, DWORD dwTgtPlayerID, DWORD dwErrorCode)
{
	// 是否自己和自己交易
	if((DWORD)GetKey() == dwTgtPlayerID)
	{
		return eNotWithMe;
	}

	// 判断交易对象是否正确
	if((DWORD)GetExchMgr().GetTgtPlayerKey() != dwTgtPlayerID)
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	//// 再次判断是否忙
	//if(!CanExchange())
	//{
	//	return E_Exchange_PlayerBusy;
	//}

	// 获取地图
   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
   if(!zone)
	 {
	   return E_Exchange_PlayeLogOut;
	 }

	// 交易申请者检查
   Smart_Ptr<CreatureObj> tempObj;
	zone->GetObject(dwTgtPlayerID, tempObj);
	pApplicant = (Player*)(tempObj.Get());
	if(!pApplicant || pApplicant->GetKey() < 0)
	{
		// 被申请者不在线,或与申请者不在同一张地图中
		return E_Exchange_NotInSame_Map;
	}

	// 申请者的交易目标是否变化
	ExchangeMgr &exchMgrApp = pApplicant->GetExchMgr();
	if(pApplicant->IsExchanging() && exchMgrApp.GetTgtPlayerKey() != GetKey())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	// 检查交易返回码
	if(dwErrorCode != E_Exchange_Accept)
	{
		// 结束交易发起者的交易状态
		pApplicant->EndExchange();
		return dwErrorCode;
	}

	// 申请者交易状态检查
	if(!pApplicant->IsExchanging())
	{
		exchMgrApp.SetTgtPlayerKey(-1);
		return E_Exchange_PlayerBusy;
	}

	// 交易距离
	if(IsInDistance(pApplicant, (DWORD)MAX_EXCHANGE_DISTANCE) == false)
	{
		return E_Exchange_OutOfRange;
	}

	// 设置申请者交易状态
	BeginExchange(dwTgtPlayerID);

	return E_Exchange_Accept;
}
//
////------------------------------------------------------------------------
//// 添加交易物品
////------------------------------------------------------------------------
DWORD Player::ProcExchangeAdd(Player* &pTarget, uint &pItemIndex, int &nInsIndex)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != 0)
	{
		return dwErrorCode;
	}

	// 取得交易物品
	   Smart_Ptr<ArticleBaseMgr> pPacket = GetContainer(ePackageType);
	   if(!pPacket)
	   	{
	   		return eNull;
	   	}

	   Smart_Ptr<PropBase> itemInfo = pPacket->GetItemDataPtr(pItemIndex);
	   if(!itemInfo)
	   	   {
		   return eNull;
	   	   }
	   if(itemInfo->GetID() < 0 || pPacket->IsLock(pItemIndex))
	   	   {
		   return E_Exchange_ItemCanNot_Exchange;
	   	   }


	   if(itemInfo->IsBind())
	   	   {
		   return E_Exchange_ItemCanNot_Exchange;
	   	   }


		// 放入到待交易物品中
		nInsIndex = GetExchMgr().AddItem(pItemIndex, itemInfo->GetAmount());
		if(-1 == nInsIndex)
		{
			// 已在交易列表中，或交易物品个数已达到上限
			return E_Exchange_ItemCanNot_Find;
		}

		// 解除目标锁定状态
		//pTarget->GetExchMgr().Unlock();

	return 0;
}
//
////------------------------------------------------------------------------
//// 取消交易物品
////------------------------------------------------------------------------
//DWORD Player::ProcExchangeDec(Player* &pTarget, INT64 n64Serial)
//{
//	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
//	if(dwErrorCode != E_Success)
//	{
//		return dwErrorCode;
//	}
//
//	// 解除目标锁定状态
//	pTarget->GetExchMgr().Unlock();
//
//	return GetExchMgr().DecItem(n64Serial);
//}
//
////------------------------------------------------------------------------
//// 修改交易金钱
////------------------------------------------------------------------------
//DWORD Player::ProcExchangeMoney(Player* &pTarget, INT64 n64Silver)
//{
//	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
//	if(dwErrorCode != E_Success)
//	{
//		return dwErrorCode;
//	}
//
//	// 解除目标锁定状态
//	pTarget->GetExchMgr().Unlock();
//
//	// 背包中是否有足够的金钱
//	CurrencyMgr &CurMgr = GetCurMgr();
//	if(n64Silver > CurMgr.GetBagSilver())
//	{
//		return E_Exchange_NotEnough_Money;
//	}
//
//	GetExchMgr().ResetMoney(n64Silver);
//
//	return E_Success;
//}
//
////------------------------------------------------------------------------
//// 锁定交易
////------------------------------------------------------------------------
DWORD Player::ProcExchangeLock(Player* &pTarget)
{
	DWORD dwErrorCode = ProcPrepareExchange(pTarget);
	if(dwErrorCode != 0)
	{
		return dwErrorCode;
	}

	GetExchMgr().Lock();

	return 0;
}
//
////------------------------------------------------------------------------
//// 玩家取消交易
////------------------------------------------------------------------------
DWORD Player::ProcExchangeCancel(Player* &pTarget)
{
	// 是否处于交易状态
	if(!IsExchanging())
	{
		return E_Exchange_SelfBusy;
	}

	// 获得交易管理器和交易对象id
	ExchangeMgr &exchMgr = GetExchMgr();
	DWORD dwTargetPlayerID = exchMgr.GetTgtPlayerKey();

	// 取消交易
	EndExchange();

	//// 交易数据是否处于锁定状态
	//if(exchMgr.IsLock())
	//{
	//	return -1;
	//}

	// 获取地图
   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
   if(!zone)
	 {
	   return E_Exchange_PlayeLogOut;
	 }

	// 交易申请者检查
   Smart_Ptr<CreatureObj> tempObj;
	zone->GetObject(dwTargetPlayerID, tempObj);
	pTarget = (Player*)(tempObj.Get());
	if(!pTarget || pTarget->GetID() < 0)
	{
		// 被申请者不在线,或与申请者不在同一张地图中
		return E_Exchange_NotInSame_Map;
	}

	// 目标没有处于交易状态，或正在和别人交易
	if(!pTarget->IsExchanging() || pTarget->GetExchMgr().GetTgtPlayerKey() != GetKey())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	pTarget->EndExchange();

	return 0;
}
//
////------------------------------------------------------------------------
//// 确认交易
////------------------------------------------------------------------------
DWORD Player::ProcExchangeVerify(Player* &pTarget, DWORD &dwFailedPlayerID)
{
	DWORD dwErrorCode = 0;


	// 获得交易管理器
	ExchangeMgr &exchMgr = GetExchMgr();
	// 获取地图
   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
   if(!zone)
	 {
	   return E_Exchange_PlayeLogOut;
	 }

   Smart_Ptr<CreatureObj> tempObj;
	zone->GetObject(exchMgr.GetTgtPlayerKey(), tempObj);
	pTarget = (Player*)(tempObj.Get());

	if(!pTarget)
	{
		// 被申请者不在线,或与申请者不在同一张地图中
		dwFailedPlayerID	= exchMgr.GetTgtPlayerKey();
		dwErrorCode		= E_Exchange_NotInSame_Map;
//		EndExchange();

	}

	// 是否处于交易状态
	if(!IsExchanging())
	{
		return E_Exchange_SelfBusy;
	}

	// 交易数据是否处于锁定状态
	if(!exchMgr.IsLock())
	{
		return ePlayerIsNotLock;
	}

	// 目标没有处于交易状态，或正在和别人交易
	if(!pTarget->IsExchanging() || pTarget->GetExchMgr().GetTgtPlayerKey() != GetKey())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	// 目标是否处于锁定状态
	if(!pTarget->GetExchMgr().IsLock())
	{
		return ePlayerIsNotLock;
	}

	exchMgr.Verify();
	if(!pTarget->GetExchMgr().IsVerify())
	{
		return E_Exchange_NotAnswer;
	}

	// 交易距离确认
	if(IsInDistance(pTarget, (DWORD)MAX_EXCHANGE_DISTANCE) == false)
	{
		return dwErrorCode = E_Exchange_OutOfRange;
	}

	// 背包空间检查 -- 背包空间不足时，不清楚交易数据
	//取出来为空时竟然是－1，先加1从0开始算
	int byExTypeNum = 0;
	byExTypeNum = exchMgr.GetItemTypeNum()+1;
	int byTgtExTypeNum	= pTarget->GetExchMgr().GetItemTypeNum()+1;
	if(byExTypeNum > byTgtExTypeNum)
	{
		// 检查对方背包
		Smart_Ptr<ArticleBaseMgr> pPacket = pTarget->GetContainer(ePackageType);
//		int tempsize = pPacket->GetFreeSize();
		if(byExTypeNum - byTgtExTypeNum > pPacket->GetFreeSize())
		{
			// 解锁
			//exchMgr.Unlock();
			//pTarget->GetExchMgr().Unlock();
//			EndExchange();
//
//			if(pTarget)
//			{
//				pTarget->EndExchange();
//			}

			dwFailedPlayerID = pTarget->GetKey();
			return E_Exchange_NotEnough_BagSpace;
		}
	}
	else
	{
		// 检查自己背包
		Smart_Ptr<ArticleBaseMgr> pPacket = GetContainer(ePackageType);
//		int tempsize = pPacket->GetFreeSize();
		if(byTgtExTypeNum - byExTypeNum > pPacket->GetFreeSize())
		{
			// 解锁
//			exchMgr.Unlock();
//			pTarget->GetExchMgr().Unlock();

//			EndExchange();
//
//			if(pTarget)
//			{
//				pTarget->EndExchange();
//			}

			dwFailedPlayerID = GetKey();
			return E_Exchange_NotEnough_BagSpace;
		}
	}

	/*************************************************
	* 交易物品处理
	*************************************************/

	PropBase *pSrcItem[MAX_EXCHANGE_ITEM];
	PropBase *pDstItem[MAX_EXCHANGE_ITEM];
	memset(pSrcItem, 0, sizeof(PropBase*) * MAX_EXCHANGE_ITEM);
	memset(pDstItem, 0, sizeof(PropBase*) * MAX_EXCHANGE_ITEM);

	int srcdwErrorCode = 0;
	int tgtdwErrorCode = 0;
	int *srcItemsPos = exchMgr.GetSerialArray();
	int *traderItemsPos = pTarget->GetExchMgr().GetSerialArray();

	for(size_t i = 0; i < MAX_EXCHANGE_ITEM; i++)
	{
	   Smart_Ptr<ArticleBaseMgr> pSrcPacket = GetContainer(ePackageType);
	   if(!pSrcPacket)
		{
		   srcdwErrorCode =  E_Exchange_ItemCanNot_Find;
		   //不跳过,一个空指针继续使用将导致宕机
		   continue;
		}

	   if(srcItemsPos[i] < 0)
		   {
			   continue;
		   }
	   Smart_Ptr<PropBase> srcitemInfo = pSrcPacket->GetItemDataPtr(srcItemsPos[i]);
	   if(!srcitemInfo || srcitemInfo->GetID() < 0)
		   {
		   tgtdwErrorCode =  E_Exchange_ItemCanNot_Find;
		   	continue;
		   }

	   pSrcItem[i] = srcitemInfo.Get();

	}

	for(size_t i = 0; i < MAX_EXCHANGE_ITEM; i++)
	{
	   Smart_Ptr<ArticleBaseMgr> pTgtPacket = pTarget->GetContainer(ePackageType);
	   if(!pTgtPacket)
		{
		   srcdwErrorCode =  E_Exchange_ItemCanNot_Find;
		   continue;
		}
	   if(traderItemsPos[i] < 0)
		   {
			   continue;
		   }
	   Smart_Ptr<PropBase> tgtitemInfo = pTgtPacket->GetItemDataPtr(traderItemsPos[i]);
	   if(!tgtitemInfo || tgtitemInfo->GetID() < 0)
		   {
		   tgtdwErrorCode =  E_Exchange_ItemCanNot_Find;
		   continue;
		   }
	   pDstItem[i] = tgtitemInfo.Get();
	}


	// 交易物品检查
	if(srcdwErrorCode != 0)
	{
		dwFailedPlayerID = GetKey();
//		EndExchange();
//
//		if(pTarget)
//		{
//			pTarget->EndExchange();
//		}
		return dwErrorCode;
	}

	if(tgtdwErrorCode != 0)
	{
		dwFailedPlayerID = pTarget->GetKey();
//		EndExchange();
//
//		if(pTarget)
//		{
//			pTarget->EndExchange();
//		}
		return dwErrorCode;
	}

	// 4.移动物品

	// 4.1 金钱 -- 先减金钱,避免损失;策划现在定的只作物品交换和钱的交换
	uint srchadMoney =  GetTypeValue(eCharMoney);
	uint tgthadMoney =  pTarget->GetTypeValue(eCharMoney);
	if (srchadMoney < exchMgr.GetMoney())
	{
	    return E_Exchange_NotEnough_Money;
	}

	if (tgthadMoney < pTarget->GetExchMgr().GetMoney())
	{
	    return E_Exchange_NotEnough_Money;
	}

	std::vector<int> vecType;
	vecType.push_back(eCharMoney);
	ChangeMoney(srchadMoney, false);
	SynCharAttribute(vecType);
	pTarget->ChangeMoney(srchadMoney);
	pTarget->SynCharAttribute(vecType);


	pTarget->ChangeMoney(tgthadMoney, false);
	SynCharAttribute(vecType);
	ChangeMoney(tgthadMoney);
	SynCharAttribute(vecType);


	// 4.2.2 将交易物品放到玩家身上
	for(uint i = 0; i < MAX_EXCHANGE_ITEM; i++)
	{
		PropBase *srcitem = pSrcItem[i];

		if(!srcitem || srcItemsPos[i] < 0)
		{
			continue;
		}

		// 先从玩家身上删除交易物品 -- 避免可堆叠物品处理出错
	   BaseArgs srcargs;
	   srcargs.GoodsID = srcitem->GetID();
	   srcargs.Num = srcitem->GetAmount();
	   srcargs.Pos = srcItemsPos[i];

		Smart_Ptr<ArticleBaseMgr> pSrcPacket = GetContainer(ePackageType);

		Smart_Ptr<ArticleBaseMgr> ptgtPacket = pTarget->GetContainer(ePackageType);

		vector<int> tempPosList;
		if(ptgtPacket->AddItem(srcitem->GetID(),srcitem->GetAmount(),tempPosList) != 0)
		{
			 return eContainerFull;
		}

		pSrcPacket->DeleteGoods(&srcargs);
	}

	for(uint i = 0; i < MAX_EXCHANGE_ITEM; i++)
	{
		PropBase *tgtitem = pDstItem[i];

		if(!tgtitem || traderItemsPos[i] < 0)
		{
			continue;
		}

	   BaseArgs tgtargs;
	   tgtargs.GoodsID = tgtitem->GetID();
	   tgtargs.Num = tgtitem->GetAmount();
	   tgtargs.Pos = traderItemsPos[i];

		Smart_Ptr<ArticleBaseMgr> pSrcPacket = GetContainer(ePackageType);
		Smart_Ptr<ArticleBaseMgr> ptgtPacket = pTarget->GetContainer(ePackageType);

		vector<int> tempPosList;
		if(!pSrcPacket->AddItem(tgtitem->GetID(),tgtitem->GetAmount(),tempPosList))
		{
			 return eContainerFull;
		}
		ptgtPacket->DeleteGoods(&tgtargs);
	}

	return dwErrorCode;
}

//------------------------------------------------------------------------
// 检查玩家身上的交易物品是否齐全
//------------------------------------------------------------------------
DWORD Player::VerifyExchangeData(PropBase* pItem[])
{
	// 获得交易管理器
	ExchangeMgr &exchMgr = GetExchMgr();
	// 1.金钱是否还够
	uint hadMoney =  GetTypeValue(eCharMoney);
		if (hadMoney < exchMgr.GetMoney())
		{
		    return E_Exchange_NotEnough_Money;
		}

	//// 2.身上是否有足够的空间
	//if(GetItemMgr().GetBagFreeSize() < exchMgr.GetItemTypeNum())
	//{
	//	return E_Exchange_NotEnough_BagSpace;
	//}

	// 3.待交易物品是否还在身上, 若在，得到物品指针
//	if(!GetItemMgr().CheckExistInBag(pItem, exchMgr.GetSerialArray(), exchMgr.GetNumArray(), MAX_EXCHANGE_ITEM))
//	{
//		return E_Exchange_ItemCanNot_Find;
//	}

	return 0;
}
//
////------------------------------------------------------------------------
//// 修改交易数据前的检查，及得到相关对象
////------------------------------------------------------------------------
DWORD Player::ProcPrepareExchange(Player* &pTarget)
{
	// 是否处于交易状态
	if(!IsExchanging())
	{
		return E_Exchange_SelfBusy;
	}

	// 获得交易管理器
	ExchangeMgr &exchMgr = GetExchMgr();

	// 交易数据是否处于锁定状态
	if(exchMgr.IsLock())
	{
		return ePlayerIsNotLock;
	}

	// 获取地图
	// 获取地图
   Smart_Ptr<CGameZone> zone = CSceneMgr::GetInstance()->GetGameZone(GetMapID());
   if(!zone)
	 {
	   return E_Exchange_PlayeLogOut;
	 }

   Smart_Ptr<CreatureObj> tempObj;
	zone->GetObject(exchMgr.GetTgtPlayerKey(), tempObj);
	pTarget = (Player*)(tempObj.Get());
	if(!pTarget || pTarget->GetKey() < 0)
	{
		// 被申请者不在线,或与申请者不在同一张地图中
		return E_Exchange_NotInSame_Map;
	}

	// 目标没有处于交易状态，或正在和别人交易
	if(!pTarget->IsExchanging() || pTarget->GetExchMgr().GetTgtPlayerKey() != GetKey())
	{
		return E_Exchange_ApplicantTgt_Change;
	}

	return 0;
}
