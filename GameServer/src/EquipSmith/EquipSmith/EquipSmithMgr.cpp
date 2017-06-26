///*
// * EquipSmithMgr.cpp
// *
// *  Created on: 2014年4月26日
// *      Author: helh
// */
//
//#include <cstdlib>
//#include "EquipSmithMgr.h"
//#include "Log/Logger.h"
//#include "Network/Message_Facade.h"
//#include "../../MessageBuild.h"
//#include "../../ServerManager.h"
//#include "MessageCommonRet.h"
//#include "MessageStruct/CharLogin/Smith.pb.h"
//#include "../../Object/Player/PlayerMessage.h"
//#include "FileLoader/GoodsLoader.h"
//#include "Goods/PropDefine.h"
//#include "MessageStruct/ServerReturnInt.pb.h"
//#include "../../Object/Player/Player.h"
//#include "CharDefine.h"
//#include "../../Map/SceneManager.h"
//#include "./FileLoader/EquipSmithLoader.h"
//#include "../../Chat/ChatMgr.h"
//#include "../../Daily/DailyMgr.h"
//#include "util.h"
//#include "../../Container/ContainerMessage.h"
//#include "../../StatisticMgr/StatisticMgr.h"
//#include "StatisticBase/StatisticDefine.h"
//#include "MessageStruct/StatisticInfo.pb.h"
//#include "EventSet/EventArgs.h"
//#include "EventSet/EventSet.h"
//#include "../../ServerEventDefine.h"
//#include "../FileLoader/MallLoader.h"
//#include "../../Container/ArticleEquipMgr.h"
//#include "FightDefined.h"
//#include "FileLoader/GameParaLoader.h"
//#include "../../VIP/PlayerVipPart.h"
//#include "../../Attribute/AttributeCreature.h"
//#include "../../Container/ParcelMgr.h"
//#include "FileLoader/MaterialSynthesisLoader.h"
//#include "../../StatisticMgr/StatisticHandle.h"
//
//EquipSmithMgr::EquipSmithMgr()
//{
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithMgr, MSG_REQ_C2GM_INLAYQUICKSYNTH);
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithMgr, MSG_REQ_C2GM_UPGRADEEQUIP);
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithMgr, MSG_REQ_C2GM_SUCCEEDSTRENGTH);
//	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithMgr, MSG_REQ_C2GM_INALYSTONEINFO); // 宝石镶嵌的所有宝石信息
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_STONEDAADEL);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_STRENGTHEQUIP);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_GT2GM_REFINE);
//	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_GT2GM_AFFIRMREFINE);
//}
//
//EquipSmithMgr::~EquipSmithMgr()
//{
//
//}
//
//
//void EquipSmithMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//
//	DEF_SWITCH_TRY_DISPATCH_END
//}
//
//void EquipSmithMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
//{
//	DEF_SWITCH_TRY_DISPATCH_BEGIN
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_STONEDAADEL);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_STRENGTHEQUIP);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_REFINE);
//	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_AFFIRMREFINE);
//	DEF_SWITCH_TRY_DISPATCH_END
//}
//
//
//int EquipSmithMgr::AddDeleteStoneCheck(Smart_Ptr<Player>& player, bool type, int equipPos, int stonePos)
//{
//	Smart_Ptr<PropBase> stone = NULL;
//	Smart_Ptr<PropBase> equip = NULL;
//
//
//	const GoodsInfo* stoneInfo = NULL;
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	if (!equipContainer || ! parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	equip = equipContainer->GetItemDataPtr(equipPos);
//	if(!equip)
//		return ePlayerPackageFail;
//
//	const GoodsInfo* equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID());
//
//	if (equipInfo == NULL && equip->GetID() >= 0)
//		return eItemError;
//
//	if (equip->GetID() >=0 && equipInfo->type != eWeapon && equipInfo->type != eArmor && equipInfo->type != eJewelry )
//		return eGoodsTypeError;
//
//	Smart_Ptr<EquipProp> equipItem = equip;
//	vector<DWORD> vec;
//
//	if(!type)
//	{ // 镶嵌
//		stone = parcelContainer->GetItemDataPtr(stonePos);
//		if(!stone)
//			return eItemError;
//
//		if(stone->GetAmount() <= 0)
//			return eItemCountNull;
//
//		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stone->GetID());
//		if(stoneInfo==NULL || 	stoneInfo->type != eStone)
//			return eGoodsTypeError;
//
//		int money = 0;
//		money += stoneInfo->useLvReq * GameParaLoader::GetInstance()->getCostMoneyRate();
//
//		if (money > player->getAttManage()->getValue<int>(eCharMoney))
//		{
//			return eNotEnoghMoney;
//		}
//
//		int can_ret = this->IsCanAdd(equipItem, stone->GetID());
//		if(can_ret != 0)
//			return can_ret;
//	}
//	else
//	{ // 摘取
//		int stoneID = equipItem->IsHaveStone(stonePos);
//		if(0 == stoneID)
//				return eInlayedStoneNull;
//
//		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stoneID);
//		if(stoneInfo==NULL || 	stoneInfo->type != eStone)
//			return eGoodsTypeError;
//
//		int ret = parcelContainer->IsBagEnought(stoneInfo->itemID, 1);
//		if (0 != ret)
//		{
//			return ret;
//		}
//	}
//
//	return 0;
//
//}
//
//int EquipSmithMgr::RefineEquipCheck(Smart_Ptr<Player>& player, int pos, vector<int>& topLevelPos, bool isMustTopLv,
//		bool isAutoBuy)
//{
//	vector<DWORD> vec;
//	vector<int> vecTypes;
//	// 临时洗练属性
//	map<int, ReGetAttr> tempAttr;
//
//	// 所用的元宝
//	int useGolden = 0;
//
//
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	if (!equipContainer || !parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	//得到装备指针
//	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(pos);
//
//	if(!(bool)equipProp || equipProp->IsValid())
//	{
//		return eItemError;
//	}
//
//	//得到装备配置表
//	const GoodsInfo* goodsInfo=GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//	if(goodsInfo==NULL)
//	{
//		return eItemError;
//	}
//
//	if(goodsInfo->type != eWeapon &&  goodsInfo->type != eArmor &&goodsInfo->type != eJewelry)
//	{
//		return eGoodsTypeError;
//	}
//
//	// 顶级需扣的元宝
//	int topCostGolden = EquipSmithLoader::GetInstance()->GetTopCostByEquipLv(goodsInfo->useLvReq);
//	if (-1 == topCostGolden)
//	{
//		return eItemError;
//	}
//
//	if(player->getAttManage()->getValue<int>(eCharMoney)<goodsInfo->reflushUseMon)
//	{
//		return eNotEnoghMoney;
//	}
//
//	//得到宝石位置
//	int posId = parcelContainer->FindPosByPropID(goodsInfo->reflushItem,equipProp->GetFlag());
//
//	Smart_Ptr<PropBase> propBase = parcelContainer->GetItemDataPtr(posId);
//
//	if (!(bool)propBase)
//	{
//		MallGoodsInfo info;
//		MallLoader::GetInstance()->GetItemTypeDataByID(goodsInfo->reflushItem ,MallType_EquipStrength,info);
//		if (info.id < 0)
//		{
//			return eItemError;
//		}
//
//		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//		if ((bool)vip)
//		{
//			useGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//		}
//
//		if (isAutoBuy && !player->IsEnoughGolden(useGolden))
//		{ // 是否自动购买
//			return eNotEnoughGold;
//		}
//		else if (!isAutoBuy)
//		{
//			return eRefineStoneNull;
//		}
//	}
//
//	// 洗练出顶极的扣元宝
//	if (isMustTopLv)
//	{
//		useGolden += topCostGolden;
//		if (!player->IsEnoughGolden(useGolden))
//		{
//			return eNotEnoughGold;
//		}
//	}
//
//	return 0;
//}
//
//int EquipSmithMgr::AffirmRefineCheck(Smart_Ptr<Player>& player, int pos, vector<int> lockPos, bool isAutoBuy)
//{
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	if (!equipContainer || !parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	vector<DWORD> vec;
//	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(pos);
//	if(!(bool)equipProp)
//	{
//		return eAffirmError;
//	}
//
//	int needGolden = 0;
//	vector<int> vecTypes;
//
//	// 替换锁ID
//	int replaceItemID = EquipSmithLoader::GetInstance()->GetRefineReplaceLockID();
//	const GoodsInfo* replaceItem = GoodsLoader::GetInstance()->GetItemDataByID(replaceItemID);
//
//	//得到装备配置表
//	const GoodsInfo* goodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//	if (NULL == goodsInfo || NULL == replaceItem)
//	{
//		return eItemError;
//	}
//
//	if(goodsInfo->type != eWeapon &&  goodsInfo->type != eArmor &&goodsInfo->type != eJewelry)
//	{
//		return eGoodsTypeError;
//	}
//
//	if(!equipProp->IsHaveRefine())
//		return eAffirmNull;
//
//	if (lockPos.size() > 0)
//	{
//		uint num = 0;
//
//		//得到替换锁
//		num = parcelContainer->FindPropCount(replaceItemID);
//
//		if(num < lockPos.size())
//		{
//			if (isAutoBuy)
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(replaceItemID,MallType_EquipStrength, info);
//				if  (info.id < 0)
//				{
//					return eItemError;
//				}
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					needGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (lockPos.size() - num));
//				}
//
//				if (!player->IsEnoughGolden(needGolden))
//				{
//					return eNotEnoughGold;
//				}
//			}
//			else
//			{
//				return eLockNull;
//			}
//		}
//
//	}
//	return 0;
//}
//
//int EquipSmithMgr::StrengthEquipCheck(Smart_Ptr<Player> &player, int ePos, bool bmeterial ,bool bGolden)
//{
//	Smart_Ptr<EquipProp> Equip = NULL;
//	Smart_Ptr<PropBase> meterial = NULL;
//
//	vector<DWORD> vec;
//	int meterialGride = 0;
//	int stoneCount = 0;
//	int needStoneCount = 0;
//	int golden = 0;
//	int surplusProtect = 0;
//
//	int protectItemID = EquipSmithLoader::GetInstance()->GetStrengthProtectID(); // 保护符物品ID
//
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//
//	if (!equipContainer || !parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	Equip = equipContainer->GetItemDataPtr(ePos);
//
//	//没有该装备
//	if (!Equip || Equip->IsValid() )
//	{
//		return eItemError;
//	}
//	const GoodsInfo* equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(Equip->GetID());
//	if (equipInfo == NULL)
//	{
//		return eItemError;
//	}
//
//	//装备满级了
//	if (Equip->GetUpgrade() >= EquipSmithLoader::GetInstance()->GetStrengthMaxLv())
//	{
//		return eEquipLvLimit;
//	}
//
//	// 装备等级是否达到强化条件的等级以上
//	if (Equip->GetUpgrade()+1 > EquipSmithLoader::GetInstance()->GetStrengthMaxLvByLv(equipInfo->useLvReq))
//	{
//		return eEquipLvNotStrength;
//	}
//
//	needStoneCount = EquipSmithLoader::GetInstance()->GetStrengNeedStoneCount(Equip->GetUpgrade());
//
//	int strengthItemID = EquipSmithLoader::GetInstance()->GetStrengthItemID(Equip->GetUpgrade());
//	const GoodsInfo* strengthItemInfo = GoodsLoader::GetInstance()->GetItemDataByID(strengthItemID);
//
//	if (0 >= strengthItemID || needStoneCount <= 0 || NULL == strengthItemInfo ||
//			NULL == GoodsLoader::GetInstance()->GetItemDataByID(protectItemID))
//	{
//		return eItemError;
//	}
//
//	surplusProtect = parcelContainer->GetGoodsNum(protectItemID); // 保护符剩余数量
//	stoneCount = parcelContainer->FindPropCount(strengthItemID);
//
//	//强化石少于需要量
//	if(stoneCount < needStoneCount)
//	{
//		if(!bGolden)
//		{
//			return eStoneNumLimit;
//		}
//		else	//不足扣元宝
//		{
//			MallGoodsInfo info;
//			MallLoader::GetInstance()->GetItemTypeDataByID(strengthItemID,MallType_EquipStrength,info);
//			//判断是否拥有vip打折权限;
//			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//			if ((bool)vip)
//			{
//				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (needStoneCount - stoneCount));
//			}
//		}
//	}
//
//	if (bmeterial)
//	{
//		meterialGride = parcelContainer->FindPosByPropID(protectItemID);
//		meterial = parcelContainer->GetItemDataPtr(meterialGride);
//
//		const GoodsInfo* meterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(protectItemID);
//		if (meterialInfo == NULL)
//		{
//			return eItemError;
//		}
//		//使用保护符但是背包没有该物品
//		if (!meterial || meterial->IsValid())
//		{
//			if (bGolden)
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//				if(info.id < 0)
//				{
//					return eItemError;
//				}
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//			else
//			{
//				return ePurpleStoneNull;
//			}
//		}
//	}
//
//	if(0 < golden && !player->IsEnoughGolden(golden))
//	{
//		return eNotEnogGolden;
//	}
//
//	//判断铜钱消耗
//	int costMoney = EquipSmithLoader::GetInstance()->GetStrengNeedMoney(Equip->GetUpgrade());
//
//	if(player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
//	{
//		return eNotEnoghMoney;
//	}
//
//	return 0;
//}
//
//// 升级检查
//int EquipSmithMgr::UpgradeEquipCheck(Smart_Ptr<Player>& player, int ePos, bool bGolden, bool isProtect)
//{
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	if (!equipContainer || !parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	vector<DWORD> vec;
//	int haveStoneCount = 0;
//	int needStoneCount = 0;
//	int golden = 0; // 需扣的元宝
//	Smart_Ptr<EquipProp> equipProp;
//	int protectItemID = EquipSmithLoader::GetInstance()->GetUpProtectItemID(); // 升级保护符唯一ID
//	Smart_Ptr<PropBase> protectProp; //
//	int protectPropPos = 0; // 升级保护符位置
//
//	equipProp = equipContainer->GetItemDataPtr(ePos);
//
//	if (!equipProp)
//	{
//		return ePlayerPackageFail;
//	}
//
//	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//	const GoodsInfo* protectItemInfo = GoodsLoader::GetInstance()->GetItemDataByID(protectItemID);
//	if (NULL == goodsInfof || NULL == protectItemInfo)
//	{
//		return eItemError;
//	}
//
//	if(goodsInfof->useLvReq == GameParaLoader::GetInstance()->getEquipMaxLv())
//	{
//		return eEquipLvFull;
//	}
//
//	const GoodsInfo* goodsInfos = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID()+1);
//	if (NULL == goodsInfos)
//	{
//		return eItemError;
//	}
//
//	vector<int> notUpItemID;
//	// 获取不能升级的装备ID
//	EquipSmithLoader::GetInstance()->GetNotUpItemID(notUpItemID);
//
//	if (0 < notUpItemID.size())
//	{
//		for (uint i = 0; i < notUpItemID.size(); ++i)
//		{
//			if (goodsInfof->itemID == notUpItemID[i])
//			{
//				return eEquipNotUpgrade;
//			}
//		}
//	}
//
//	if(goodsInfos->useLvReq > player->getAttManage()->getValue<int>(eCharLv))
//	{
//		return eLVNotEnough;
//	}
//
//	int upItemID = EquipSmithLoader::GetInstance()->GetUpItemID(goodsInfof->itemQuality); // 升级需要的升级石
//
//	needStoneCount = EquipSmithLoader::GetInstance()->GetUpgradeNeedStoneCount(goodsInfof->useLvReq);
//	haveStoneCount = parcelContainer->FindPropCount(upItemID);
//
//	if(haveStoneCount < needStoneCount)
//	{
//		if (bGolden) // 自动购买  材料不足元宝代替
//		{
//			//判断是否拥有vip打折权限;所有商品打8折
//			MallGoodsInfo info;
//			MallLoader::GetInstance()->GetItemTypeDataByID(upItemID,MallType_EquipStrength,info);
//			if (0 > info.id)
//			{
//				return eItemError;
//			}
//			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//			if ((bool)vip)
//			{
//				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (needStoneCount - haveStoneCount));
//			}
//		}
//		else
//		{
//			return eUpGradeStoneEnough;
//		}
//	}
//	if (isProtect)
//	{
//		protectPropPos = parcelContainer->FindPosByPropID(protectItemID);
//		protectProp = parcelContainer->GetItemDataPtr(protectPropPos);
//		if (-1 == protectPropPos || !protectProp || protectProp->IsValid())
//		{ // 如果没有保护符
//			if (bGolden)
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//				if  (info.id < 0)
//				{
//					return eItemError;
//				}
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//			else
//			{
//				return eNotUpgradeProtect;
//			}
//		}
//	}
//	if (!player->IsEnoughGolden(golden))
//	{
//		return eNotEnogGolden;
//	}
//	// 消耗金钱
//	int costMoney = EquipSmithLoader::GetInstance()->GetUpgradeNeedMoney(goodsInfof->useLvReq);
//	if (player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
//	{
//		return eNotEnoghMoney;
//	}
//	return 0;
//}
//
//int EquipSmithMgr::SucceedStrengthCheck(Smart_Ptr<Player>& player, int containerf, int ePosF, int containers, int ePosS,
//		bool bGolden, bool isRefine, bool isProtect)
//{
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	if (!equipContainer || !parcelContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	Smart_Ptr<EquipProp> equipPropF;
//	Smart_Ptr<EquipProp> equipPropS;
//	vector<DWORD> vec;
//	int haveStoneCount = 0;
//	int golden = 0; // 需扣的元宝
//	int protectItemID = EquipSmithLoader::GetInstance()->GetSucceedProtectItemID(); //  继承保护符唯一ID
//	Smart_Ptr<PropBase> protectProp;
//	int protectPropPos; // 继承保护符位置
////	int succeedItemID = EquipSmithLoader::GetInstance()->GetSucceedItemID(); // 继承需要的宝石ID
//
//	equipPropF = parcelContainer->GetItemDataPtr(ePosF);
//	equipPropS = equipContainer->GetItemDataPtr(ePosS);
//
//	if(!equipPropF || !equipPropS)
//	{
//		return eClientSendDataError;
//	}
//
//	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipPropF->GetID());
//	const GoodsInfo* goodsInfos = GoodsLoader::GetInstance()->GetItemDataByID(equipPropS->GetID());
//	if(NULL == goodsInfof || NULL == goodsInfos)
//	{
//		return eItemError;
//	}
//
//	vector<int> notSucItemID;
//	EquipSmithLoader::GetInstance()->GetNotSucceedItemID(notSucItemID);
//	if (0 < notSucItemID.size())
//	{
//		for (uint i = 0; i < notSucItemID.size(); ++i)
//		{
//			if (goodsInfof->itemID == notSucItemID[i] || goodsInfos->itemID == notSucItemID[i])
//			{
//				return eEquipNotSucceed;
//			}
//		}
//	}
//
//	if(equipPropS->GetUpgrade() < equipPropF->GetUpgrade() || equipPropS->GetUpgrade() <= 0)
//	{
//		return eStrengNull;
//	}
//
//	if(goodsInfof->itemPlace != goodsInfos->itemPlace)
//	{
//		return eDiffEquipType;
//	}
//
//	if(goodsInfof->itemQuality < goodsInfos->itemQuality)
//	{
//		return eEquipQualityEnough;
//	}
//
//	if (goodsInfof->itemQuality == goodsInfos->itemQuality)
//	{
//		if (goodsInfof->useLvReq <= goodsInfos->useLvReq)
//		{
//			return eEquipLvNotMatchCond;
//		}
//	}
//
//	int succeedItemID = EquipSmithLoader::GetInstance()->GetSucceedItemID(goodsInfos->itemQuality); // 继承需要的宝石ID
//	if (0 == succeedItemID)
//	{
//		return eItemError;
//	}
//
//	const InheritData* tempData = EquipSmithLoader::GetInstance()->getInheritData(equipPropS->GetUpgrade());
//	if (NULL == tempData)
//		return eItemError;
//
//	haveStoneCount = parcelContainer->FindPropCount(succeedItemID);
//
//	//扣宝石
//	int tempItemCounts = tempData->m_NeedItemCounts;
//	//橙装翻倍
//	if(eGoodsQualityOrange == goodsInfos->itemQuality)
//		tempItemCounts += tempItemCounts;
//
//	//　继承石
//	if(haveStoneCount < tempItemCounts)
//	{
//		if (bGolden)
//		{
//			// 自动购买 材料不足元宝代替
//			// 判断是否拥有vip打折权限;所有商品打8折
//			MallGoodsInfo info;
//			MallLoader::GetInstance()->GetItemTypeDataByID(succeedItemID,MallType_EquipStrength,info);
//			if (-1 == info.id)
//			{
//				return eItemError;
//			}
//			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//			if ((bool)vip)
//			{
//				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (tempItemCounts - haveStoneCount));
//				tempItemCounts = haveStoneCount;
//			}
//		}
//		else
//		{
//			return eSucceeStoneEnough;
//		}
//	}
//
//	// 保护符
//	if (isProtect)
//	{
//		const GoodsInfo* goodsInfo =	GoodsLoader::GetInstance()->GetItemDataByID(protectItemID);
//		if (goodsInfo->itemID <= 0)
//		{
//			return eItemError;
//		}
//
//		protectPropPos = parcelContainer->FindPosByPropID(protectItemID);
//		protectProp = parcelContainer->GetItemDataPtr(protectPropPos);
//		if (protectPropPos == -1 || !protectProp || protectProp->IsValid())
//		{
//			if (bGolden)
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//				if  (info.id < 0)
//				{
//					return eItemError;
//				}
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//			else
//			{
//				return eNotSucceedProtect;
//			}
//		}
//	}
//
//	// 继承洗练
//	if (isRefine)
//	{
//		golden += EquipSmithLoader::GetInstance()->GetSucceedRefineCost();
//	}
//
//	if (0 < golden)
//	{
//		if(!player->IsEnoughGolden(golden))
//		{
//			return eNotEnogGolden;
//		}
//	}
//
//// 消耗金钱
//	int costMoney = goodsInfos->useLvReq * 5000 * (goodsInfos->itemQuality + 1);
//
//	if(player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
//	{
//		return eNotEnoghMoney;
//	}
//
//	return 0;
//}
//
//
//void EquipSmithMgr::RefineSortNewAttr(int equipLv, map<int, ReGetAttr>& vecAttr, vector<int>& topLevelPos, map<int, ReGetAttr>& newAttr)
//{
//	vector<ReGetAttr> topLvAttrs;
//	vector<ReGetAttr> notTopLvAttrs;
//
//	int other = 0, maxLevel = 0;
//	maxLevel = EquipSmithLoader::GetInstance()->GetRefineMaxLvByLv(equipLv);
//	for (map<int, ReGetAttr>::iterator it = vecAttr.begin(); it != vecAttr.end(); ++it)
//	{
//		if (maxLevel == it->second.level && (uint)other < topLevelPos.size())
//		{
//			topLvAttrs.push_back(it->second);
//			++other;
//		}
//		else
//		{
//			notTopLvAttrs.push_back(it->second);
//		}
//	}
//
//
//	vector<ReGetAttr>::iterator notTopLvIt = notTopLvAttrs.begin();
//	vector<ReGetAttr>::iterator topLvIt = topLvAttrs.begin();
//
//	for (int i = 0, topLvIndex = 0; i < (int)vecAttr.size(); ++i)
//	{
//		if (topLvIndex < (int)topLvAttrs.size())
//		{
//			if (i == topLevelPos[topLvIndex] || (i != topLevelPos[topLvIndex] && i >= (int)notTopLvAttrs.size()))
//			{
//				newAttr[topLevelPos[topLvIndex]] = *topLvIt;
//				++topLvIndex;
//				++topLvIt;
//				continue;
//			}
//		}
//
//		newAttr[i] = *notTopLvIt;
//		++notTopLvIt;
//	}
//}
//
//int EquipSmithMgr::IsCanAdd(const Smart_Ptr<EquipProp> &equipItem, const int &stoneID)
//{
//	// 装备等级限制
//	const GoodsInfo *destInfo =	GoodsLoader::GetInstance()->GetItemDataByID(stoneID);
//	const GoodsInfo* equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equipItem->GetID());
//	if (NULL == destInfo || NULL == equipInfo)
//	{
//		return eItemError;
//	}
//
//	if (EquipSmithLoader::GetInstance()->GetStoneLvByLv(equipInfo->useLvReq) < destInfo->useLvReq)
//	{ // 宝石物品是宝石所能镶嵌的装备的使用等级
//		return eEquipLvNotInlay;
//	}
//
//	// 所有宝石不能同一类型
//	if(equipItem->IsSameTypeStone(stoneID))
//	{
//		return eSameStone;
//	}
//
//	// 镶嵌不能满
//	if(equipItem->IsAddStoneFull())
//	{
//		return eHoleFull;
//	}
//
//	return 0;
//}
//
//int64 EquipSmithMgr::AddDeleteStoneFromEquip(Smart_Ptr<Player> &player, EquipSmith::AddDelStone req, int* stoneID)
//{
//	Smart_Ptr<ArticleEquipMgr> equipMgr;
////	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	Smart_Ptr<PropBase> stone = NULL;
//	Smart_Ptr<EquipProp> equipItem = NULL;
//	const GoodsInfo* stoneInfo = NULL;
//	int hPos = 0;
//	vector<DWORD> vec;
//	int equipPos = 0, stonePos = 0;
//	bool type = false;
//	equipPos = req.equip().pos();
//	stonePos = req.stonepos();
//	type = req.type();
//
//	int64 ret = AddDeleteStoneCheck(player, type, equipPos, stonePos);
//	if (0 != ret)
//	{
//		return ret;
//	}
//	equipMgr = player->GetContainer(eEquipQuickType);
//	equipItem = player->GetContainer(eEquipQuickType)->GetItemDataPtr(equipPos);
//
//
//	if (!type)
//	{ // 镶嵌
//		int money = 0;
//		stone = player->GetContainer(ePackageType)->GetItemDataPtr(stonePos);
//
//		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stone->GetID());
//
//		money = stoneInfo->useLvReq * GameParaLoader::GetInstance()->getCostMoneyRate();
//
////		StatisticInfo::CostItemInfo * costItem = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCostBefore(costItem,eCharMoney,money,player);
//
//		player->MoneyChange(money,false);
//
////		StatisticMgr::SetItemCostAfter(costItem,eCharMoney,player);
//
//		hPos = equipItem->InlayStone(stone->GetID()); // 镶嵌
//
//		BaseArgs args;
//		args.GoodsID = stoneInfo->itemID;
//		args.Num =1;
//		args.Pos = stonePos;
//		player->GetContainer(ePackageType)->DeleteGoods(&args);
//
//		vec.push_back((eEquipQuickType << 16) | equipPos);
//		vec.push_back((ePackageType << 16) | stonePos);
//
//		player->SetDataFlag(ePackageInfo);
//		player->SetDataFlag(eEquipQuickInfo);
//
////		if(eAccountType_User == player->GetAccountType())
////			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_StoneInsetEvent,&costAndGotInfo,stone->GetID());
//		*stoneID = stone->GetID();
//
//
//	}
//	else
//	{ // 摘取
//		*stoneID = equipItem->IsHaveStone(stonePos);
//		if(*stoneID == 0)
//				return eInlayedStoneNull;
//
//		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(*stoneID);
//
//		vector<int> vecPos;
//		int can_ret = player->GetContainer(ePackageType)->AddItem(*stoneID,1,vecPos,1,0,false);
//		if(can_ret != 0)
//			return can_ret;
//
//		//对相应的 位置  清零   再 把之后的 位置 重置
//		equipItem->ClearStone(stonePos);
//		//
//		vec.push_back((eEquipQuickType << 16) | equipPos);
//
//		vector<int>::iterator itVec = vecPos.begin();
//		for(; itVec != vecPos.end(); ++itVec)
//		{
//			vec.push_back((ePackageType << 16) | (*itVec));
//		}
//
//		hPos = stonePos;
//
////		if(eAccountType_User == player->GetAccountType())
////			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_StoneRemoveEvent,&costAndGotInfo,*stoneID);
//	}
//
//	if(equipItem->GetID() != -1)
//	{
//		Smart_Ptr<ArticleEquipMgr> ArticleEquip_ptr = player->GetContainer(eEquipQuickType);
//
//		ArticleEquip_ptr->calculateFight();
//		if (!type)
//		{
//			ArticleEquip_ptr->ChangePlayerAttr(stoneInfo->stoneType, stoneInfo->GetStoneAttr(), true);
//		}
//		else
//		{
//			ArticleEquip_ptr->ChangePlayerAttr(stoneInfo->stoneType, stoneInfo->GetStoneAttr(), false);
//		}
//	}
//
//	ret = ((int64)1 << 32) | hPos;
//	player->SendSectorChange(vec);
//
////	bool inlayTop = equipMgr->IsAllEquipInlayTop();
////	player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_InlayUnLvCeil), !inlayTop);
//
//	return ret;
//
//}
//
//int EquipSmithMgr::StrengthEquip(Smart_Ptr<Player> &player, EquipSmith::ClientReqStrengthEquip content, int& equipUpgrade, int& surplusProtect)
//{
//	int ret = StrengthEquipCheck(player, content.equippos(), content.bmeterial(), content.bgolden());
//	if (0 != ret)
//	{
//		return ret;
//	}
//
//	Smart_Ptr<EquipProp> Equip = NULL;
//	int ration = 0;
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Equip = equipContainer->GetItemDataPtr(content.equippos());
//
//	Smart_Ptr<PropBase> meterial = NULL;
//	int meterialGride = 0; // 保护符格子位置
//	vector<DWORD> vec;
//
//	int stoneCount = 0, needStoneCount = 0, strengthItemID = 0, protectItemID = 0, golden = 0;
//	int costMoney = 0, tempDropCounts = 0;
//
//	// 获得相应的强化石ID
//	strengthItemID = EquipSmithLoader::GetInstance()->GetStrengthItemID(Equip->GetUpgrade());
//	// 强化石数量
//	stoneCount = player->GetContainer(ePackageType)->FindPropCount(strengthItemID);
//	// 需要的强化石数量
//	needStoneCount = EquipSmithLoader::GetInstance()->GetStrengNeedStoneCount(Equip->GetUpgrade());
//
//	if (stoneCount < needStoneCount && content.bgolden())
//	{ // 数量不足 用元宝花费
//		MallGoodsInfo info;
//		MallLoader::GetInstance()->GetItemTypeDataByID(strengthItemID,MallType_EquipStrength,info);
//
//		//判断是否拥有vip打折权限;
//		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//		if ((bool)vip)
//		{
//			golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (needStoneCount - stoneCount));
//		}
//	}
//
//	protectItemID = EquipSmithLoader::GetInstance()->GetStrengthProtectID(); // 保护符物品ID
//	if (content.bmeterial())
//	{ // 使用保护符
//		meterialGride = player->GetContainer(ePackageType)->FindPosByPropID(protectItemID);
//		meterial = player->GetContainer(ePackageType)->GetItemDataPtr(meterialGride);
//		if (!meterial || meterial->IsValid())
//		{ // 使用保护符但是背包没有该物品
//			if (content.bgolden())
//			{ // 保护符不足用元宝花费
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//		}
//		const GoodsInfo* meterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(protectItemID);
//
//		ration += meterialInfo->buffID * 100;
//	}
//
//	costMoney = EquipSmithLoader::GetInstance()->GetStrengNeedMoney(Equip->GetUpgrade());
//
//
//
////	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
////	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();
//
//	//扣除宝石
//	bool bind = false;
//	bind = player->GetContainer(ePackageType)->DeleteGoodsById(strengthItemID, needStoneCount, Equip->GetBindType());
//
////	StatisticInfo::CostItemInfo * costStoneInfo = costAndGotInfo->add_costlist();
////	StatisticMgr::SetItemCost(costStoneInfo, strengthItemID, needStoneCount);
//
//	//转化绑定类型
//	if (!Equip->GetBindType())
//		Equip->SetBind(bind);
//
//	//扣除保护符
//	if ((bool)meterial)
//	{
//		meterial->SubAmount();
//		vec.push_back((ePackageType << 16) | meterialGride);
//
////		StatisticInfo::CostItemInfo * costMeterialInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCost(costMeterialInfo,protectItemID,1);
//
//		surplusProtect = player->GetContainer(ePackageType)->GetGoodsNum(protectItemID);
//	}
//
////	StatisticInfo::CostItemInfo * costMoneyInfo = costAndGotInfo->add_costlist();
////	StatisticMgr::SetItemCostBefore(costMoneyInfo,eCharMoney,costMoney,player);
//	//扣除money
//	player->MoneyChange(costMoney, false);
//
////	StatisticMgr::SetItemCostAfter(costMoneyInfo,eCharMoney,player);
//	//扣除元宝
//	if(golden != 0)
//	{
////		StatisticInfo::CostItemInfo * costGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costGoldenInfo,eCharGolden,player);
//
////		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,player);
//
//		vector<int> vecGolden;
//
//		//扣除元宝
//		int tempbuckleGolden 	 = 0;
//		int tempbuckleBindGolden = 0;
//
//		player->DecreaseGoldenCost(golden,tempbuckleGolden,tempbuckleBindGolden);
//
//		if(tempbuckleGolden > 0)
//			vecGolden.push_back(eCharGolden);
//
//		if(tempbuckleBindGolden > 0)
//			vecGolden.push_back(eCharBindGolden);
//
//		player->SynCharAttribute(vecGolden);
//
////		StatisticMgr::SetItemCostAfter(costGoldenInfo,eCharGolden,tempbuckleGolden,player);
//
////		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,player);
//	}
//	//统计一天强化消耗的铜钱数
////	if(eAccountType_User == player->GetAccountType())
////	{
////		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Forge,eStaMinor_Forge_StrenCostMoney,costMoney);
//		GLOBAL_STATISTIC_GM(player, GlobalType::Main::eEquipSmith, GlobalType::Minor::eEquipSmith_StrengthCostMoney, StatistSaveType::Int64_Data(costMoney));
////	统计一天强化消耗的物品数
////		StatisticMgr::GetInstance()->StatisticPlayerItemsMap(player,eStatic_Forge,eStaMinor_Forge_StrenCostItems,strengthItemID,1);
////	}
//
////	idAndCostAndGotInfo.set_beforeid(Equip->GetUpgrade());
//
//	const GoodsInfo* equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(Equip->GetID());
//
//	int temp = 0;
//	eStrengthType tempResultType = EquipSmithLoader::GetInstance()->GetStrengthRation(Equip->GetUpgrade(), ration, tempDropCounts);
//
//	if(tempResultType == eStrengthNull)
//		return eSystemError;
//
//	switch(tempResultType)
//	{
//	case eStrengthGeneral:  //普通
//		{
//			temp = eStrengthEquipFail;
//			break;
//		}
//	case eStrenghtDrop: 	//掉星
//		{
//			temp = eStrengthEquipDropStar;
//			int tempCurrentGrade = Equip->GetUpgrade();
//			if(tempCurrentGrade <= 0)
//				break;
//
//			if(tempDropCounts <= 0)
//				break;
//
//			tempCurrentGrade -= tempDropCounts;
//			if(tempCurrentGrade < 0)
//				tempCurrentGrade = 0;
//
//			equipContainer->ChangeAttrbuteByEquip(false, *equipInfo, Equip);
//			Equip->SetUpgrade(tempCurrentGrade);
//			equipContainer->ChangeAttrbuteByEquip(true, *equipInfo, Equip);
//
//			break;
//		}
//	case eStrenghSuccess:   //成功
//		{
//			equipContainer->ChangeAttrbuteByEquip(false, *equipInfo, Equip);
//			Equip->AddUpgrade();
//			equipContainer->ChangeAttrbuteByEquip(true, *equipInfo, Equip);
//
//			if(Equip->GetUpgrade() >= EquipSmithLoader::GetInstance()->GetStrengthMaxLv())
//			{
//				string strId = ChatMgr::GetInstance()->GetStringByInt(Equip->GetID());
//				string strLv = ChatMgr::GetInstance()->GetStringByInt(Equip->GetUpgrade());
//				ChatMgr::GetInstance()->Marquee(eMarqueeStrength,player->GetName(),strId,strLv);
//			}
//			equipUpgrade = Equip->GetUpgrade();
//			temp = 0;
//			break;
//		}
//	default:
//		{
//			break;
//		}
//	}
//	vec.push_back((eEquipQuickType << 16) | content.equippos());
//	player->SetDataFlag(eEquipQuickInfo);
//	player->SendSectorChange(vec);
//	if (eStrengthEquipFail != temp)
//	{
//		player->SynCharAttribute();
//	}
//
////	idAndCostAndGotInfo.set_afterid(Equip->GetUpgrade());
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_AboutGolden_StrengthEquipEvent,&idAndCostAndGotInfo,equipInfo->itemID);
//
//	//完成活跃次数
//	player->GetVitalityMgr()->FinshVitality(eViEquipStrengthen);
//
////	bool strenthTop = equipContainer->IsAllEquipStrengthTop();
////	if (strenthTop)
////	{
////		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_StrengthUnLvCeil), 1, false);
////	}
//
//	return temp;
//}
//
//DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithMgr, MSG_REQ_GT2GM_STONEDAADEL)
//{
//	EquipSmith::AddDelStone  meContent;
//	ServerReturn::ServerRetInt ack;
//	char *con;
//	int len = 0;
//	int64 ret = 0;
//
//	con = message->GetBuffer(len);
//	meContent.ParseFromArray(con, len);
//
//	LOG_DEBUG(FILEINFO, "Player add or delete stone on stone");
//
//	int64 charID = message->GetMessageTime();
//	Smart_Ptr<Player> player;
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
//	if((bool)player)
//	{
//		if(meContent.equip().container() < 0 || meContent.equip().container() >= eArticleMax)
//		{
//			LOG_ERROR(FILEINFO, "add or delete stone but container type is error");
//
//			return;
//		}
//
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Inlay, MONITOR);
//		int stoneID = 0;
//
//		if(player->IsOpenSystem(eEquipInlayLv))
//		{
//			ret = AddDeleteStoneFromEquip(player, meContent, &stoneID);
//		}
//		else
//		{
//			ret = eNotOpen;
//		}
//		ack.set_ret(ret);
//
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STONEDAADEL, message, &ack);
//		Message_Facade::Send(messRet);
//
//		if (1 == (ret >> 32))
//		{
//			//镶嵌
//			if(!meContent.type())
//			{
//				UseItemEventArgs e(player, stoneID, 1);
//				player->FireEvent(PLAYER_SMELT_INSERT,e);
//			}
//			else
//			{
//				UseItemEventArgs e(player, stoneID, 1);
//				player->FireEvent(PLAYER_TAKE_OFF_STONE,e);
//			}
//		}
//
//
//	}
//}
//
//DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithMgr,MSG_REQ_GT2GM_REFINE)
//{
//	EquipSmith::ClientReqRefineEquip reqGm;
//	EquipSmith::ReturnNewAttr ret;
//	LOG_DEBUG(FILEINFO, "Player GT2GM refine");
//	int len = 0;
//	char *str = message->GetBuffer(len);
//
//	reqGm.ParseFromArray(str, len);
//
//	Smart_Ptr<Player> player;
//	int64 charID = message->GetMessageTime();
//	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
//	if(!player)
//	{
//		return;
//	}
//
//	int state = 0;
//	if(player->IsOpenSystem(eEquipRefineLv))
//	{
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Refine, MONITOR);
//
//		if(!ContainerMessage::GetInstance()->CalContainerType(reqGm.container()))
//		{
//			state = eItemError;
//		}
//		else
//		{
//			vector<int> topLvPos;
//			for (int i = 0; i < reqGm.toplvpos_size(); ++i)
//			{
//				topLvPos.push_back(reqGm.toplvpos(i));
//			}
//			state = RefineEquip(player, reqGm, ret);
//
//		}
//
//
//	}
//	else
//	{
//		ret.set_status(eNotOpen);
//	}
//
//	ret.set_status(state);
//
//	//返回网关消息
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_REFINE, message, &ret);
//	Message_Facade::Send(messRet);
//}
//DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithMgr,MSG_REQ_GT2GM_AFFIRMREFINE)
//{
//	EquipSmith::ClintReqAffirmRefine reqGm;
//	EquipSmith::ReturnAffirmRefine ret;
//	int len = 0;
//	int state = 0;
//
//	char *str = message->GetBuffer(len);
//	reqGm.ParseFromArray(str, len);
//
//	Smart_Ptr<Player> player;
//	int64 charID = message->GetMessageTime();
//	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
//
//	if((bool)player && player->IsOpenSystem(eEquipRefineLv))
//	{
//		if(!ContainerMessage::GetInstance()->CalContainerType(reqGm.container()))
//		{
//			state = eItemError;
//		}
//		else
//		{
//			vector<int> lockPos;
//			if((bool)player)
//			{
//				for (int i = 0; i < reqGm.lockpos_size(); ++i)
//				{
//					lockPos.push_back(reqGm.lockpos(i));
//				}
//				state = AffirmRefine(player, reqGm);
//			}
//			else
//			{
//				state = eAffirmError;
//			}
//		}
//
//		ret.set_status(state);
//		ret.set_container(reqGm.container());
//		ret.set_equipid(reqGm.equipid());
//		LOG_DEBUG(FILEINFO, "return AffirmRefine state = %d",state);
//	}
//	else
//	{
//		ret.set_status(eNotOpen);
//	}
//	//返回网关消息
//	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_AFFIRMREFINE, message, &ret);
//	Message_Facade::Send(messRet);
//}
//DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithMgr, MSG_REQ_GT2GM_STRENGTHEQUIP)
//{
//	LOG_DEBUG(FILEINFO, "Player start to strength equip");
//	EquipSmith::ClientReqStrengthEquip meContent;
//	ServerReturn::ServerThreeInt ack;
//
//	char *con;
//	int len = 0;
//	int ret = 0;
//
//	con = message->GetBuffer(len);
//	meContent.ParseFromArray(con, len);
//
//
//	Smart_Ptr<Player> player;
//	int64 charID = message->GetMessageTime();
//
//	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
//	if((bool)player)
//	{
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Strength, MONITOR);;
//
//		int equipUpgrade = 0, surplusProtect = 0;
//		if(player->IsOpenSystem(eEquipStrengtLv))
//		{
////			ret = StrengthEquip(player, meContent.container(), meContent.equippos(), meContent.bmeterial() , meContent.bgolden(), equipUpgrade, surplusProtect);
//			ret = StrengthEquip(player, meContent, equipUpgrade, surplusProtect);
//		}
//		else
//		{
//			ret= eNotOpen;
//		}
//		ack.set_retf(ret);
//		ack.set_rets(meContent.onekeystrenght());
//		ack.set_rett(surplusProtect);
//
//		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STRENGTHEQUIP, message, &ack);
//		Message_Facade::Send(messRet);
//
//			//强化
//		if(ret == 0)
//		{
//			StrengthEquipArgs e(player,equipUpgrade);
//			player->FireEvent(PLAYER_SMELT_STRONG,e);
//		}
//
//		//有些事件是需要升星和掉星都监听
//		if(ret ==0 || ret == eStrengthEquipDropStar)
//		{
//			StrengthEquipArgs e(player,equipUpgrade);
//			player->FireEvent(PLAYER_EQUIP_STRONG,e);
//		}
//
//
//	}
//}
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithMgr,MSG_REQ_C2GM_UPGRADEEQUIP)
//{
//	LOG_DEBUG(FILEINFO, "client req gt get upgrade equip");
//	EquipSmith::ClientReqUpgradeEquip req;
//	ServerReturn::ServerRetInt ack;
//
//	req.ParseFromString(content);
//
//	if(player && player->IsOpenSystem(eEquipUpgradeLv))
//	{
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Upgrade, MONITOR);;
//
//		ack.set_ret(UpgradeEquip(player, req));
//
//
//	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}
//
//	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_UPGRADEEQUIP);
//}
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithMgr,MSG_REQ_C2GM_SUCCEEDSTRENGTH)
//{
//	LOG_DEBUG(FILEINFO, "client req gt get succeed strength");
//	EquipSmith::ClientReqSucceedEquip req;
//	ServerReturn::ServerRetInt ack;
////	ServerMessage::ClientSingleMessage toClient;
//
//	req.ParseFromString(content);
//
//	if(player && player->IsOpenSystem(eEquipSucceedLv))
//	{
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Succeed, MONITOR);
//
//		ack.set_ret(SucceedStrength(player, req));
//
//
//	}
//	else
//	{
//		ack.set_ret(eNotOpen);
//	}
//
//	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_SUCCEEDSTRENGTH);
//}
//
//
//int EquipSmithMgr::RefineEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqRefineEquip refineInfo, EquipSmith::ReturnNewAttr& ret)
//{
//	vector<int> topLvPos;
//	for (int i = 0; i < refineInfo.toplvpos_size(); ++i)
//	{
//		topLvPos.push_back(refineInfo.toplvpos(i));
//	}
//	int err = RefineEquipCheck(player,refineInfo.equipid(), topLvPos, refineInfo.ismusttoplv(), refineInfo.isautobuy());
//	if (0 != err)
//	{
//		return err;
//	}
//
//	// 临时洗练属性
//	map<int, ReGetAttr> tempAttr;
//	vector<DWORD> vec;
//	vector<int> vecTypes;
//
//	int useGolden = 0;
//	//得到装备指针
//	Smart_Ptr<EquipProp> equipProp = player->GetContainer(eEquipQuickType)->GetItemDataPtr(refineInfo.equipid());
//	//得到装备配置表
//	const GoodsInfo* goodsInfo=GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//	// 得到宝石位置
//	int posId = player->GetContainer(ePackageType)->FindPosByPropID(goodsInfo->reflushItem,equipProp->GetFlag());
//	Smart_Ptr<PropBase> propBase = player->GetContainer(ePackageType)->GetItemDataPtr(posId);
//	if (!propBase)
//	{
//		MallGoodsInfo info;
//		MallLoader::GetInstance()->GetItemTypeDataByID(goodsInfo->reflushItem ,MallType_EquipStrength,info);
//
//		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//		if ((bool)vip && refineInfo.isautobuy())
//		{
//			useGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//		}
//	}
//
//	// 获得洗练属性
//	if (-1 == EquipSmithLoader::GetInstance()->CalEquipRefineAttr(goodsInfo->useLvReq, goodsInfo->itemQuality, refineInfo.ismusttoplv(), tempAttr,
//			(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession)))
//	{
//		return eEquipUnRefine;
//	}
//
//	// 顶级需扣的元宝
//	int topCostGolden = EquipSmithLoader::GetInstance()->GetTopCostByEquipLv(goodsInfo->useLvReq);
//
//	// 洗练出顶极的扣元宝
//	if (refineInfo.ismusttoplv())
//	{
//		useGolden += topCostGolden;
//	}
//
////	StatisticInfo::CostAndGotInfo costAndGotInfo;
//
//	if ((!(bool)propBase && refineInfo.isautobuy()) || refineInfo.ismusttoplv())
//	{
////		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCostBefore(costItemInfo,eCharGolden,player);
//
////		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,player);
//
//		int tempGolden	   = 0;
//		int tempBindGolden = 0;
//		player->DecreaseGoldenCost(useGolden,tempGolden,tempBindGolden);
//
//		if(tempGolden > 0)
//			vecTypes.push_back(eCharGolden);
//
//		if(tempBindGolden > 0)
//			vecTypes.push_back(eCharBindGolden);
//
////		StatisticMgr::SetItemCostAfter(costItemInfo,eCharGolden,tempGolden,player);
//
////		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempBindGolden,player);
//	}
//	if ((bool)propBase)
//	{
////		StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
////		StatisticMgr::SetItemCost(costItemInfo,propBase->GetID(),1);
//
//		player->GetContainer(ePackageType)->DeleteGoodsById(propBase->GetID(), 1, propBase->GetBindType());
//	}
//
//	if (0 < vecTypes.size())
//	{
//		player->SynCharAttribute(vecTypes);
//	}
//
//	player->MoneyChange(goodsInfo->reflushUseMon,false);
////	StatisticInfo::CostItemInfo * costItemInfo = costAndGotInfo.add_costlist();
////	StatisticMgr::SetItemCostBefore(costItemInfo,eCharMoney,goodsInfo->reflushUseMon,player);
////	StatisticMgr::SetItemCostAfter(costItemInfo,eCharMoney,player);
//
//	//统计一天洗练消耗的铜钱数
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Forge,eStaMinor_Forge_SuccinctCostMoney,goodsInfo->reflushUseMon);
//	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eEquipSmith, GlobalType::Minor::eEquipSmith_RefineCostMoney, StatistSaveType::Int64_Data(goodsInfo->reflushUseMon));
//
//
//	if ((bool)propBase)
//	{
////		统计一天洗练消耗的物品数1
////		StatisticMgr::GetInstance()->StatisticPlayerItemsMap(player,eStatic_Forge,eStaMinor_Forge_SuccinctCostItem,propBase->GetID(),1);
//
//		vec.push_back((ePackageType << 16) | posId);
//		player->SendSectorChange(vec);
//		player->SetDataFlag(ePackageInfo);
//
//	}
//
//	map<int, ReGetAttr> newAttr;
//	if (0 < topLvPos.size())
//	{
//		RefineSortNewAttr(goodsInfo->useLvReq, tempAttr, topLvPos, newAttr);
//
//	}
//	else
//	{
//		newAttr = tempAttr;
//	}
//
//	equipProp->SetLegendAttrCount(0);
//
//	for (map<int, ReGetAttr>::iterator it = newAttr.begin(); it != newAttr.end(); ++it)
//	{
//		EquipSmith::RefineAttr* refineAttr;
//		refineAttr = ret.add_attrinfo();
//		refineAttr->set_attrtype(it->second.type);
//		refineAttr->set_attrvalue(it->second.value);
//		refineAttr->set_attrlevel(it->second.level);
//		refineAttr->set_attrpos(it->first);
//
//		if (it->second.level == EquipSmithLoader::GetInstance()->GetRefineAttrMaxLv())
//		{ // 计算顶级属性 计算成就用到
//			equipProp->IncreaseLegendAttrCount();
//		}
//	}
//
//	ret.set_container(refineInfo.container());
//	ret.set_equipid(refineInfo.equipid());
//
//	// 保存临时洗练属性
//	equipProp->SetTempAttr(newAttr);
//
//	SmeltArgs e(player,equipProp->GetLegendAttrCount());
//	player->FireEvent(PLAYER_SMELT_SOPHISTICATED,e);
//
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_SuccinctEvent,&costAndGotInfo,equipProp->GetID());
//
//	LOG_DEBUG(FILEINFO, "refine GM TO GAT");
//
//	return eRetSuccess;
//}
//
////替换洗练属性
//int EquipSmithMgr::AffirmRefine(Smart_Ptr<Player> &player, EquipSmith::ClintReqAffirmRefine content)
//{
//	vector<int> vecTypes;
//	int needGolden = 0;
//	vector<int> lockPos;
//	vector<DWORD> vec;
//	for (int i = 0; i < content.lockpos_size(); ++i)
//	{
//		lockPos.push_back(content.lockpos(i));
//	}
//
//	// 检测
//	int err = AffirmRefineCheck(player, content.equipid(), lockPos, content.isautobuy());
//	if (0 != err)
//	{
//		return err;
//	}
//
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//
//	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(content.equipid());
////	//得到装备配置表
//	const GoodsInfo* goodsInfo = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//
//	// 替换锁ID
//	int replaceItemID = EquipSmithLoader::GetInstance()->GetRefineReplaceLockID();
//	//得到替换锁
//	uint num = parcelContainer->FindPropCount(replaceItemID);
//
//	if (0 < lockPos.size())
//	{
//		if(num < lockPos.size())
//		{
//			if (content.isautobuy())
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(replaceItemID,MallType_EquipStrength, info);
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					needGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (lockPos.size() - num));
//				}
//			}
//		}
//		//扣除替换锁
//		parcelContainer->DeleteGoodsById(replaceItemID, lockPos.size(), equipProp->GetBindType());
//	}
//
//	if (content.isautobuy() && needGolden > 0)
//	{ //
//		//扣除元宝
//		int tempbuckleGolden 	 = 0;
//		int tempbuckleBindGolden = 0;
//
//		player->DecreaseGoldenCost(needGolden,tempbuckleGolden,tempbuckleBindGolden);
//
//		if(tempbuckleGolden > 0)
//			vecTypes.push_back(eCharGolden);
//
//		if(tempbuckleBindGolden > 0)
//			vecTypes.push_back(eCharBindGolden);
//	}
//
//	equipContainer->ChangeAttrbuteByEquip(false, *goodsInfo, equipProp);
//	// 替换
//	equipProp->AffirmRefine(lockPos);
//	equipContainer->ChangeAttrbuteByEquip(true, *goodsInfo, equipProp);
//
//	player->SynCharAttribute();
//	player->SetDataFlag(eEquipQuickInfo);
//
//	if (0 < vecTypes.size())
//	{
//		player->SynCharAttribute(vecTypes);
//	}
//	vec.push_back((eEquipQuickType << 16) | content.equipid());
//	player->SendSectorChange(vec,npc_fly_type);
//
//	SmeltArgs e(player,equipProp->GetLegendAttrCount());
//	player->FireEvent(PLAYER_SMELT_SMELT,e);
//
//	equipProp->SetLegendAttrCount(0);
//
////	bool refineTop = equipContainer->IsAllEquipRefineTop();
////	if (refineTop)
////	{
////		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_RefineUnLvCeil), 1, false);
////	}
//
//
//	return eRetSuccess;
//}
//
////升级
//int EquipSmithMgr::UpgradeEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqUpgradeEquip req)
//{
//	int ret = UpgradeEquipCheck(player, req.equippos(), req.bgolden(), req.isprotect());
//	if (0 != ret)
//	{
//		return ret;
//	}
//
//	vector<DWORD> vec;
//	int haveStoneCount = 0;
//	int needStoneCount = 0;
//	int golden = 0; // 需扣的元宝
//	Smart_Ptr<EquipProp> equipProp;
//	bool bind = false;
//	int protectItemID = EquipSmithLoader::GetInstance()->GetUpProtectItemID(); // 升级保护符唯一ID
//	Smart_Ptr<PropBase> protectProp; //
//	int protectPropPos = 0; // 升级保护符位置
//	Smart_Ptr<ArticleEquipMgr> equipMgr = player->GetContainer(eEquipQuickType);
//
//	equipProp = player->GetContainer(eEquipQuickType)->GetItemDataPtr(req.equippos());
//
//	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
//	const GoodsInfo* goodsInfos = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID() + 1);
//
//	int upItemID = EquipSmithLoader::GetInstance()->GetUpItemID(goodsInfof->itemQuality); // 升级需要的升级石
//
////	StatisticInfo::IDAndCostAndGotInfo idAndCostAndGotInfo;
////	idAndCostAndGotInfo.set_beforeid(goodsInfof->itemID);
////	StatisticInfo::CostAndGotInfo * costAndGotInfo = idAndCostAndGotInfo.mutable_costandgotlist();
//
//	needStoneCount = EquipSmithLoader::GetInstance()->GetUpgradeNeedStoneCount(goodsInfof->useLvReq);
//	haveStoneCount = player->GetContainer(ePackageType)->FindPropCount(upItemID);
//
//	if (req.bgolden() && haveStoneCount < needStoneCount)
//	{
//		//判断是否拥有vip打折权限;所有商品打8折
//		MallGoodsInfo info;
//		MallLoader::GetInstance()->GetItemTypeDataByID(upItemID,MallType_EquipStrength,info);
//
//		if (0 > info.id)
//		{
//			return eItemError;
//		}
//
//		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//		if ((bool)vip)
//		{
//			golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (needStoneCount - haveStoneCount));
//		}
//	}
//
//	if (req.isprotect())
//	{
//
//		protectPropPos = player->GetContainer(ePackageType)->FindPosByPropID(protectItemID);
//		protectProp = player->GetContainer(ePackageType)->GetItemDataPtr(protectPropPos);
//		if (-1 == protectPropPos || !protectProp || protectProp->IsValid())
//		{ // 如果没有保护符
//			if (req.bgolden())
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//		}
//	}
//
//	// 消耗金钱
//	int costMoney = EquipSmithLoader::GetInstance()->GetUpgradeNeedMoney(goodsInfof->useLvReq);
//
////	StatisticInfo::CostItemInfo * costMoneyInfo = costAndGotInfo->add_costlist();
////	StatisticMgr::SetItemCostBefore(costMoneyInfo, eCharMoney, costMoney, player);
//
//	if (player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
//	{
//		return eNotEnoghMoney;
//	}
//	player->MoneyChange(costMoney, false);
////	StatisticMgr::SetItemCostAfter(costMoneyInfo, eCharMoney, player);
//	//扣宝石
//	bind = player->GetContainer(ePackageType)->DeleteGoodsById(upItemID,needStoneCount,equipProp->GetBindType());
//
////	StatisticInfo::CostItemInfo * costStoneInfo = costAndGotInfo->add_costlist();
////	StatisticMgr::SetItemCost(costStoneInfo,upItemID,needStoneCount);
//
//	if (req.isprotect() && (bool)protectProp)
//	{ // 删除保护符
//		protectProp->SubAmount();
//		vec.push_back((ePackageType << 16) | protectPropPos);
//
////		StatisticInfo::CostItemInfo * protectProp = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCost(protectProp,protectItemID,1);
//	}
//
//	//转化绑定类型
//	if(equipProp->GetBindType() == false)
//		equipProp->SetBind(bind);
//
//	//扣除元宝
//	if(golden != 0)
//	{
////		StatisticInfo::CostItemInfo * costGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costGoldenInfo,eCharGolden,player);
//
////		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,player);
//
//		vector<int> vecGolden;
//
//		//扣除元宝
//		int tempbuckleGolden 	 = 0;
//		int tempbuckleBindGolden = 0;
//
//		player->DecreaseGoldenCost(golden,tempbuckleGolden,tempbuckleBindGolden);
//
//		if(tempbuckleGolden > 0)
//			vecGolden.push_back(eCharGolden);
//
//		if(tempbuckleBindGolden > 0)
//			vecGolden.push_back(eCharBindGolden);
//
//		player->SynCharAttribute(vecGolden);
//
////		StatisticMgr::SetItemCostAfter(costGoldenInfo,eCharGolden,tempbuckleGolden,player);
//
////		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,player);
//
////		idAndCostAndGotInfo.set_afterid(goodsInfos->itemID);
//
////		if(eAccountType_User == player->GetAccountType())
////			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_UpGradeEvent,&idAndCostAndGotInfo);
//	}
//
//	//更新玩家属性
//	Smart_Ptr<ArticleEquipMgr> bagMgr = player->GetContainer(eEquipQuickType);
//	if(false == bool(bagMgr))
//		return ePlayerPackageFail;
//
//	bagMgr->ChangeAttrbuteByEquip(false, *goodsInfof, equipProp);
//
//	//升级
//	equipProp->SetID(equipProp->GetID()+1);
//
//	//掉星
//	if(equipProp->GetUpgrade()>1)
//	{
//		if (!req.isprotect())
//		{
//			int tempUpGrade = equipProp->GetUpgrade();
//			int tempDrop    = EquipSmithLoader::GetInstance()->GetUpgradeDropStar(goodsInfof->useLvReq);
//
//			tempUpGrade -= tempDrop;
//
//			if(tempUpGrade < 0)
//				tempUpGrade = 0;
//
//			equipProp->SetUpgrade(tempUpGrade);
//		}
//	}
//
//	bagMgr->ChangeAttrbuteByEquip(true, *goodsInfos, equipProp);
//
//	OneParmaEventArgs e(player->GetMyself());
//	player->FireEvent(PLAYER_EQUIP_UPGRADE,e);
//
//	vec.push_back((eEquipQuickType << 16) | req.equippos());
//	player->SynCharAttribute();
//	player->SetDataFlag(eEquipQuickInfo);
//	player->SendSectorChange(vec,npc_fly_type);
//
//	//完成活跃次数
//	player->GetVitalityMgr()->FinshVitality(eViEquipUp);
//
////	bool upgradeTop = equipMgr->IsAllEquipUpgradeTop();
////	if (upgradeTop)
////	{
////		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_EquipSmith, eUIHLSmall_EquipSmith_UpgradeUnLvCeil), 1, false);
////	}
//
//	return eRetSuccess;
//}
////继承
//int EquipSmithMgr::SucceedStrength(Smart_Ptr<Player> &player, EquipSmith::ClientReqSucceedEquip req)
//{
//	int ret = SucceedStrengthCheck(player, req.containerf(), req.posf(), req.containers(), req.poss(), req.bgolden(), req.isrefine(), req.isprotect());
//	if (0 != ret)
//	{
//		return ret;
//	}
//
//	Smart_Ptr<EquipProp> equipPropF;
//	Smart_Ptr<EquipProp> equipPropS;
//	vector<DWORD> vec;
//	int haveStoneCount = 0;
//	int golden = 0; // 需扣的元宝
//	bool bind = false;
//	int protectItemID = EquipSmithLoader::GetInstance()->GetSucceedProtectItemID(); //  继承保护符唯一ID
//	Smart_Ptr<PropBase> protectProp;
//	int protectPropPos; // 继承保护符位置
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//
//	equipPropF = parcelContainer->GetItemDataPtr(req.posf());
//	equipPropS = equipContainer->GetItemDataPtr(req.poss());
//
//	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipPropF->GetID());
//	const GoodsInfo* goodsInfos = GoodsLoader::GetInstance()->GetItemDataByID(equipPropS->GetID());
//
//	int succeedItemID = EquipSmithLoader::GetInstance()->GetSucceedItemID(goodsInfos->itemQuality); // 继承需要的宝石ID
//
//	const InheritData* tempData = EquipSmithLoader::GetInstance()->getInheritData(equipPropS->GetUpgrade());
//
//	haveStoneCount = parcelContainer->FindPropCount(succeedItemID);
//
//	//扣宝石
//	int tempItemCounts = tempData->m_NeedItemCounts;
//	//橙装翻倍
//	if(eGoodsQualityOrange == goodsInfos->itemQuality)
//		tempItemCounts += tempItemCounts;
//
//	if(haveStoneCount < tempItemCounts)
//	{
//		if (req.bgolden())
//		{
//			// 自动购买 材料不足元宝代替
//			// 判断是否拥有vip打折权限;所有商品打8折
//			MallGoodsInfo info;
//			MallLoader::GetInstance()->GetItemTypeDataByID(succeedItemID,MallType_EquipStrength,info);
//
//			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//			if ((bool)vip)
//			{
//				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (tempItemCounts - haveStoneCount));
//				tempItemCounts = haveStoneCount;
//			}
//		}
//	}
//
//	if (req.isprotect())
//	{
//		protectPropPos = parcelContainer->FindPosByPropID(protectItemID);
//		protectProp = parcelContainer->GetItemDataPtr(protectPropPos);
//		if (protectPropPos == -1 || !protectProp || protectProp->IsValid())
//		{
//			if (req.bgolden())
//			{
//				MallGoodsInfo info;
//				MallLoader::GetInstance()->GetItemTypeDataByID(protectItemID, MallType_EquipStrength, info);
//
//				Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
//				if ((bool)vip)
//				{
//					golden += PlayerVipPart::CalBuyCostGolden(vip, info.price);
//				}
//			}
//		}
//	}
//
//	if (req.isrefine())
//	{
//		golden += EquipSmithLoader::GetInstance()->GetSucceedRefineCost();
//	}
//
////	StatisticInfo::SucceedStrengthInfo succeedStrengthInfo;
////	succeedStrengthInfo.set_beforeid(goodsInfos->itemID);
////	StatisticInfo::CostAndGotInfo * costAndGotInfo = succeedStrengthInfo.mutable_costandgotlist();
//
//// 消耗金钱
//	int costMoney = goodsInfos->useLvReq * 5000 * (goodsInfos->itemQuality + 1);
//
////	StatisticInfo::CostItemInfo * costMoneyInfo = costAndGotInfo->add_costlist();
////	StatisticMgr::SetItemCostBefore(costMoneyInfo, eCharMoney, costMoney, player);
//
//	player->MoneyChange(costMoney, false);
//
////	StatisticMgr::SetItemCostAfter(costMoneyInfo, eCharMoney, player);
//
//	if(tempItemCounts > 0)
//	{
//		bind = player->GetContainer(ePackageType)->DeleteGoodsById(succeedItemID,tempItemCounts,equipPropS->GetBindType());
////		StatisticInfo::CostItemInfo * costStoneInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCost(costStoneInfo,succeedItemID,tempItemCounts);
//	}
//
//	//转化绑定类型
//	if(bind == false && equipPropS->GetBindType() == true)
//		bind = true;
//
//	if(equipPropF->GetBindType() == false)
//		equipPropF->SetBind(bind);
//
//	//扣除元宝
//	if (0 < golden)
//	{
////		StatisticInfo::CostItemInfo * costGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costGoldenInfo, eCharGolden, player);
//
////		StatisticInfo::CostItemInfo * costBGoldenInfo = costAndGotInfo->add_costlist();
////		StatisticMgr::SetItemCostBefore(costBGoldenInfo,eCharBindGolden,player);
//
//		vector<int> vecGolden;
//
//		//扣除元宝
//		int tempbuckleGolden 	 = 0;
//		int tempbuckleBindGolden = 0;
//
//		player->DecreaseGoldenCost(golden,tempbuckleGolden,tempbuckleBindGolden);
//
//		if(tempbuckleGolden > 0)
//			vecGolden.push_back(eCharGolden);
//
//		if(tempbuckleBindGolden > 0)
//			vecGolden.push_back(eCharBindGolden);
//
//		player->SynCharAttribute(vecGolden);
//
////		StatisticMgr::SetItemCostAfter(costGoldenInfo, eCharGolden,tempbuckleGolden, player);
//
////		StatisticMgr::SetItemCostAfter(costBGoldenInfo,eCharBindGolden,tempbuckleBindGolden,player);
//	}
//
//	int lowerLv = 0; // 强化需要减少的等级
//	if (req.isprotect())
//	{
//		if ((bool)protectProp)
//		{// 删除保护符
//			protectProp->SubAmount();
//			vec.push_back((ePackageType << 16) | protectPropPos);
//
////			StatisticInfo::CostItemInfo * costProtectInfo = costAndGotInfo->add_costlist();
////			StatisticMgr::SetItemCost(costProtectInfo,protectItemID,1);
//		}
//		lowerLv = equipPropS->GetUpgrade();
//	}
//	else
//	{
//		lowerLv  = equipPropS->GetUpgrade() - tempData->m_DropStar;
//		if (0 > lowerLv)
//		{
//			lowerLv = 0;
//		}
//	}
//
//	vector<ReGetAttr> attrBapTizeList;
//	//更新玩家属性
//	equipContainer->ChangeAttrbuteByEquip(false, *goodsInfos, equipPropS);
//
//	equipPropF->SetUpgrade(lowerLv);
//	equipPropS->SetUpgrade(0);
//	if (req.isrefine())
//	{ // 继承洗练属性
//		equipPropS->GetAttrBaptizeList(attrBapTizeList);
//
//		vector<ReGetAttr> newAttr;
//		equipPropF->GetAttrBaptizeList(newAttr);
//
//		EquipSmithLoader::GetInstance()->GetRefineAttrByLQ(goodsInfof->useLvReq, goodsInfof->itemQuality, attrBapTizeList, newAttr,
//				(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));
//
//		//attrBapTizeList = newAttr;
//
//		equipPropF->SetAttrBaptizeList(newAttr);
//		equipPropS->InitAttrBaptizeList();
//	}
//
//	equipContainer->ChangeAttrbuteByEquip(true, *goodsInfos, equipPropS);
//
//	vec.push_back((ePackageType << 16) | req.posf());
//	vec.push_back((eEquipQuickType << 16) | req.poss());
//	player->SynCharAttribute();
//	player->SetDataFlag(ePackageInfo);
//	player->SetDataFlag(eEquipQuickInfo);
//	player->SendSectorChange(vec);
//
//	EventArgs tempArgs;
//	player->FireEvent(PLAYER_EQUIP_INHERIT,tempArgs);
//
//	CharLogin::ClientItemInfo clidenItemInfo;
//	equipPropF->SetClientInfo(&clidenItemInfo);
////	StatisticInfo::ClientItemInfo * staticClientItemInfo = succeedStrengthInfo.mutable_afteritem();
////	StatisticMgr::SetStatisticItemInfo(clidenItemInfo,staticClientItemInfo);
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_SucceedEquipEvent,&succeedStrengthInfo);
//
//	return eRetSuccess;
//}
//
//int EquipSmithMgr::InlayQuickUpStone(Smart_Ptr<Player>& player, EquipSmith::ClientReqUpgradeStone req)
//{
//	int ret = InlayQuickUpCheck(player, req.stoneid(), req.stonepos(), req.equippos());
//	if (0 != ret)
//	{
//		return ret;
//	}
//
//	int curStoneID = req.stoneid(), curStonePos = req.stonepos(), equipPos = req.equippos();
//	vector<DWORD> vec;
//
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//	Smart_Ptr<EquipProp> equip = equipContainer->GetItemDataPtr(equipPos);
//
//
//	// 获取合成信息
//	vector<MeterialSynthesisItem> destItemInfo;
//	CMeterialSynthesisLoader::GetInstance()->GetInfoBySrcMaterialID(curStoneID, destItemInfo);
//
//	// 因为宝石系列 一种宝石不可能可以合成多个种类 目标合成材料只会有一种  所以取第一个
//	MeterialSynthesisItem& info = *destItemInfo.begin();
//
//	// 因为宝石系列 一种宝石的源合成材料也只会有一种 所以取第一个
//	vector<pair<int, int> > needMaterial = info.srcMeterialID;
//	parcelContainer->DeleteGoodsById(needMaterial.begin()->first, needMaterial.begin()->second - 1,  true);
//
//	int costMoney = 0;
//
//	costMoney += info.costMoney;
//
//	// 将宝石位置清除
//	equip->ClearStone(curStonePos);
//
//	const GoodsInfo* destStoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(info.destMeterialID);
//
//	// 镶嵌
//	equip->InlayStone(destStoneInfo->itemID);
//
//	// 统计
////	StatisticInfo::CostAndGotInfo costAndGotInfo;
////	StatisticInfo::CostItemInfo * costStoneInfo = costAndGotInfo.add_costlist();
////	StatisticMgr::SetItemCost(costStoneInfo,needMaterial.begin()->first, needMaterial.begin()->second - 1);
//
//	//统计一天合成消耗的物品数
////	StatisticMgr::GetInstance()->StatisticPlayerItemsMap(player,eStatic_Forge,eStaMinor_Forge_ComposeCostItem,
////	needMaterial.begin()->first, needMaterial.begin()->second - 1);
//
////	StatisticInfo::CostItemInfo * costMoneyInfo = costAndGotInfo.add_costlist();
////	StatisticMgr::SetItemCostBefore(costMoneyInfo,eCharMoney,costMoney,player);
//
//
//	if (0 < costMoney)
//	{
//		player->MoneyChange(costMoney, false);
//	}
//
////	StatisticMgr::SetItemCostAfter(costMoneyInfo,eCharMoney,player);
////
////	if(eAccountType_User == player->GetAccountType())
////		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_ComposeEvent,&costAndGotInfo);
//
//	vec.push_back((eEquipQuickType << 16) | equipPos);
//	player->SendSectorChange(vec);
//	player->SetDataFlag(ePackageInfo);
//	player->SetDataFlag(eBaseInfo);
//
//	if(equip->GetID() != -1)
//	{
//		equipContainer->calculateFight();
//		equipContainer->ChangePlayerAttr(destStoneInfo->stoneType, destStoneInfo->GetStoneAttr(), true);
//
//	}
//
//	{
//		ComposeArgs e(player,destStoneInfo->useLvReq);
//		player->FireEvent(PLAYER_SMELT_COMPOSE,e);
//	}
//
//	return 0;
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithMgr, MSG_REQ_C2GM_INALYSTONEINFO)
//{
//	EquipSmith::ReqInlayStoneInfo req;
//	EquipSmith::RetInlayStoneInfo ret;
//
//	req.ParseFromString(content);
//
//
//
//	if (!player)
//	{
//		LOG_ERROR(FILEINFO, "client request server inlay stone info error, no player data!!!");
//		return;
//	}
//
//	CharLogin::EquipQuickInfo equipInfo;
//	player->GetContainer(eEquipQuickType)->GetArticleInfo(&equipInfo);
//	for (int i = 0; i < equipInfo.equip_size(); ++i)
//	{
//		CharLogin::ClientItemInfo itemInfo = equipInfo.equip(i);
//		EquipSmith::InlayStoneInfo* inlayStoneInfo = NULL;
//
//		inlayStoneInfo = ret.add_inlaystoneinfo();
////		inlayStoneInfo = ret.mutable_inlaystoneinfo(itemInfo.num());
//
//		inlayStoneInfo->set_pos(i);
//
//		if (itemInfo.inlaypos_size() != itemInfo.stoneid_size())
//		{
//			LOG_ERROR(FILEINFO, "client request server inlay stone info data error!!!");
//			return;
//		}
//
//		for (int j = 0; j < itemInfo.inlaypos_size(); ++j)
//		{
//			inlayStoneInfo->add_stonepos(itemInfo.inlaypos(j));
//			inlayStoneInfo->add_stoneid(itemInfo.stoneid(j));
//		}
//
//	}
//
//
//	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_INALYSTONEINFO);
//}
//
//int EquipSmithMgr::InlayQuickUpCheck(Smart_Ptr<Player>& player, int curStoneID, int curStonePos, int equipPos)
//{
//	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
//	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
//
//	if (!parcelContainer || !equipContainer)
//	{
//		return ePlayerPackageFail;
//	}
//
//	Smart_Ptr<EquipProp> equip = equipContainer->GetItemDataPtr(equipPos);
//	if (!equip)
//	{
//		return eClientSendDataError;
//	}
//
//	const GoodsInfo* stoneInfo = NULL;
//	const GoodsInfo* equipInfo = NULL;
//
//	stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(curStoneID);
//	equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID());
//	if (NULL == stoneInfo || NULL == equipInfo)
//	{
//		return eItemError;
//	}
//
//	// 查找装备的镶嵌孔位置是否有这个宝石
//	int realStoneID = equip->IsHaveStone(curStonePos);
//	if (0 == realStoneID || realStoneID != curStoneID)
//	{
//		return eClientSendDataError;
//	}
//
//	// 获取合成信息
//	vector<MeterialSynthesisItem> destItemInfo;
//
//	CMeterialSynthesisLoader::GetInstance()->GetInfoBySrcMaterialID(curStoneID, destItemInfo);
//	if (0 >= destItemInfo.size())
//	{
//		return eItemError;
//	}
//
//	// 因为宝石系列 一种宝石不可能可以合成多个种类 目标合成材料只会有一种  所以取第一个
//	MeterialSynthesisItem& info = *destItemInfo.begin();
//
//	const GoodsInfo* destStoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(info.destMeterialID);
//	if (NULL == destStoneInfo)
//	{
//		return eItemError;
//	}
//
//	int stoneLv = EquipSmithLoader::GetInstance()->GetStoneLvByLv(equipInfo->useLvReq);
//	if (-1 == stoneLv)
//	{
//		return eItemError;
//	}
//
//	if (stoneLv < destStoneInfo->useLvReq)
//	{
//		return eEquipLvNotInlay;
//	}
//
//	// 因为宝石系列 一种宝石的源合成材料也只会有一种
//	vector<pair<int, int> >& needMaterial = info.srcMeterialID;
//	if (1 != needMaterial.size())
//	{ // 宝石只能有一种源材料
//		return eItemError;
//	}
//
//	const GoodsInfo* srcMaterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(needMaterial.begin()->first);
//	if (NULL == srcMaterialInfo)
//	{
//		return eItemError;
//	}
//
//	if (needMaterial.begin()->first == curStoneID)
//	{ // 原材料中有当前宝石ID的
//		// 镶嵌孔上有一个了 所以减1
//		if ((needMaterial.begin()->second) - 1 > (parcelContainer->FindPropCount(needMaterial.begin()->first)))
//		{
//			return eItemNotEnough;
//		}
//	}
//	else
//	{
//		return eItemError;
//	}
//
//	int costMoney = 0;
//	// 铜钱是否不足
//	costMoney += info.costMoney;
//	if (player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
//	{
//		return eNotEnoghMoney;
//	}
//
//	return 0;
//}
//
//DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithMgr, MSG_REQ_C2GM_INLAYQUICKSYNTH)
//{
//	EquipSmith::ClientReqUpgradeStone req;
//	EquipSmith::RetUpgradeStone toClient;
//
//	if (!player)
//	{
//		return;
//	}
//
//	if (player->IsOpenSystem(eEquipInlayLv))
//	{
//		req.ParseFromString(content);
//
//		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_InlayQuickUp, MONITOR);
//
//		int ret = InlayQuickUpStone(player, req);
//		toClient.set_err(ret);
//
//
//	}
//	else
//	{
//		toClient.set_err(eNotOpen);
//	}
//
//	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_INLAYQUICKSYNTH);
//}
//
//void EquipSmithMgr::ReplaceAttr(const vector<ReGetAttr>& srcAttr, vector<ReGetAttr>& dest)
//{
//
//}
//
//
//
//
//
//
//
//
//
//
//
//
