/*
 * EquipSmithExMgr.cpp
 *
 *  Created on: 2016年9月10日
 *      Author: root
 */

#include "EquipSmithExMgr.h"
#include <cstdlib>
#include "Log/Logger.h"
#include "Network/Message_Facade.h"
#include "../../MessageBuild.h"
#include "../../ServerManager.h"
#include "MessageCommonRet.h"
#include "MessageStruct/CharLogin/Smith.pb.h"
#include "../../Object/Player/PlayerMessage.h"
#include "FileLoader/GoodsLoader.h"
#include "Goods/PropDefine.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "../../Object/Player/Player.h"
#include "CharDefine.h"
#include "../../Map/SceneManager.h"
#include "./FileLoader/EquipSmithExLoader.h"
#include "../../Chat/ChatMgr.h"
#include "../../Daily/DailyMgr.h"
#include "util.h"
#include "../../Container/ContainerMessage.h"
#include "../../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "MessageStruct/StatisticInfo.pb.h"
#include "EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "../../ServerEventDefine.h"
#include "../FileLoader/MallLoader.h"
#include "../../Container/ArticleEquipMgr.h"
#include "FightDefined.h"
#include "FileLoader/GameParaLoader.h"
#include "../../VIP/PlayerVipPart.h"
#include "../../Attribute/AttributeCreature.h"
#include "../../Container/ParcelMgr.h"
#include "FileLoader/MaterialSynthesisLoader.h"
#include "../../StatisticMgr/StatisticHandle.h"

EquipSmithExMgr::EquipSmithExMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithExMgr, MSG_REQ_C2GM_INLAYQUICKSYNTH);
	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithExMgr, MSG_REQ_C2GM_UPGRADEEQUIP);
	DEF_MSG_QUICK_REQUEST_REG_FUN(EquipSmithExMgr, MSG_REQ_C2GM_INALYSTONEINFO); // 宝石镶嵌的所有宝石信息
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_STONEDAADEL);
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_STRENGTHEQUIP);
	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_GT2GM_REFINE);
	DEF_MSG_REQUEST_REG_FUN(eGameServer,MSG_REQ_GT2GM_AFFIRMREFINE);
}

EquipSmithExMgr::~EquipSmithExMgr()
{

}


void EquipSmithExMgr::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void EquipSmithExMgr::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_STONEDAADEL);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_STRENGTHEQUIP);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_REFINE);
	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_AFFIRMREFINE);
	DEF_SWITCH_TRY_DISPATCH_END
}

/////////////////////////////////////////////////镶嵌快速升级宝石///////////////////////////////////////////////////////////////////
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithExMgr, MSG_REQ_C2GM_INLAYQUICKSYNTH)
{
	EquipSmith::ClientReqUpgradeStone req;
	EquipSmith::RetUpgradeStone toClient;

	if (!player)
	{
		return;
	}

	if (player->IsOpenSystem(eEquipInlayLv))
	{
		req.ParseFromString(content);

		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_InlayQuickUp, MONITOR);

		int ret = InlayQuickUpStone(player, req);
		toClient.set_err(ret);


	}
	else
	{
		toClient.set_err(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_INLAYQUICKSYNTH);
}

int EquipSmithExMgr::InlayQuickUpCheck(Smart_Ptr<Player>& player, int curStoneID, int curStonePos, int equipPos)
{
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);

	if (!parcelContainer || !equipContainer)
	{
		return ePlayerPackageFail;
	}

	Smart_Ptr<EquipProp> equip = equipContainer->GetItemDataPtr(equipPos);
	if (!equip)
	{
		return eClientSendDataError;
	}

	const GoodsInfo* stoneInfo = NULL;
	const GoodsInfo* equipInfo = NULL;

	stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(curStoneID);
	equipInfo = GoodsLoader::GetInstance()->GetItemDataByID(equip->GetID());
	if (NULL == stoneInfo || NULL == equipInfo)
	{
		return eItemError;
	}

	// 查找装备的镶嵌孔位置是否有这个宝石
	int realStoneID = equip->IsHaveStone(curStonePos);
	if (0 == realStoneID || realStoneID != curStoneID)
	{
		return eClientSendDataError;
	}

	// 获取合成信息
	vector<MeterialSynthesisItem> destItemInfo;

	CMeterialSynthesisLoader::GetInstance()->GetInfoBySrcMaterialID(curStoneID, destItemInfo);
	if (0 >= destItemInfo.size())
	{
		return eItemError;
	}

	// 因为宝石系列 一种宝石不可能可以合成多个种类 目标合成材料只会有一种  所以取第一个
	MeterialSynthesisItem& info = *destItemInfo.begin();

	const GoodsInfo* destStoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(info.destMeterialID);
	if (NULL == destStoneInfo)
	{
		return eItemError;
	}

	// 因为宝石系列 一种宝石的源合成材料也只会有一种
	vector<pair<int, int> >& needMaterial = info.srcMeterialID;
	if (1 != needMaterial.size())
	{ // 宝石只能有一种源材料
		return eItemError;
	}

	const GoodsInfo* srcMaterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(needMaterial.begin()->first);
	if (NULL == srcMaterialInfo)
	{
		return eItemError;
	}

	if (needMaterial.begin()->first == curStoneID)
	{ // 原材料中有当前宝石ID的
		// 镶嵌孔上有一个了 所以减1
		if ((needMaterial.begin()->second) - 1 > (parcelContainer->FindPropCount(needMaterial.begin()->first)))
		{
			return eItemNotEnough;
		}
	}
	else
	{
		return eItemError;
	}

	int costMoney = 0;
	// 铜钱是否不足
	costMoney += info.costMoney;
	if (player->getAttManage()->getValue<int>(eCharMoney) < costMoney)
	{
		return eNotEnoghMoney;
	}

	return 0;
}

int EquipSmithExMgr::InlayQuickUpStone(Smart_Ptr<Player>& player, EquipSmith::ClientReqUpgradeStone req)
{
	int ret = InlayQuickUpCheck(player, req.stoneid(), req.stonepos(), req.equippos());
	if (0 != ret)
	{
		return ret;
	}

	int curStoneID = req.stoneid(), curStonePos = req.stonepos(), equipPos = req.equippos();
	vector<DWORD> vec;

	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<EquipProp> equip = equipContainer->GetItemDataPtr(equipPos);


	// 获取合成信息
	vector<MeterialSynthesisItem> destItemInfo;
	CMeterialSynthesisLoader::GetInstance()->GetInfoBySrcMaterialID(curStoneID, destItemInfo);

	// 因为宝石系列 一种宝石不可能可以合成多个种类 目标合成材料只会有一种  所以取第一个
	MeterialSynthesisItem& info = *destItemInfo.begin();

	// 因为宝石系列 一种宝石的源合成材料也只会有一种 所以取第一个
	vector<pair<int, int> > needMaterial = info.srcMeterialID;
	parcelContainer->DeleteGoodsById(needMaterial.begin()->first, needMaterial.begin()->second - 1,  true);

	int costMoney = 0;

	costMoney += info.costMoney;

	// 将宝石位置清除
	equip->ClearStone(curStonePos);

	const GoodsInfo* destStoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(info.destMeterialID);

	// 镶嵌
	equip->InlayStone(destStoneInfo->itemID,curStonePos);

	if (0 < costMoney)
	{
		player->MoneyChange(costMoney, false);
	}

	vec.push_back((eEquipQuickType << 16) | equipPos);
	player->SendSectorChange(vec);
	player->SetDataFlag(ePackageInfo);
	player->SetDataFlag(eBaseInfo);

	if(equip->GetID() != -1)
	{
		equipContainer->calculateFight();
		equipContainer->ChangePlayerAttr(destStoneInfo->stoneType, destStoneInfo->GetStoneAttr(), true);

	}

	{
		ComposeArgs e(player,destStoneInfo->useLvReq);
		player->FireEvent(PLAYER_SMELT_COMPOSE,e);
	}

	return 0;
}

///////////////////////////////////////////////////宝石镶嵌的所有位置宝石信息////////////////////////////////////////////////////////////////////
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithExMgr, MSG_REQ_C2GM_INALYSTONEINFO)
{
	EquipSmith::ReqInlayStoneInfo req;
	EquipSmith::RetInlayStoneInfo ret;

	req.ParseFromString(content);

	if (!player)
	{
		LOG_ERROR(FILEINFO, "client request server inlay stone info error, no player data!!!");
		return;
	}

	CharLogin::EquipQuickInfo equipInfo;
	player->GetContainer(eEquipQuickType)->GetArticleInfo(&equipInfo);
	for (int i = 0; i < equipInfo.equip_size(); ++i)
	{
		CharLogin::ClientItemInfo itemInfo = equipInfo.equip(i);
		EquipSmith::InlayStoneInfo* inlayStoneInfo = NULL;

		inlayStoneInfo = ret.add_inlaystoneinfo();

		inlayStoneInfo->set_pos(i);

		if (itemInfo.inlaypos_size() != itemInfo.stoneid_size())
		{
			LOG_ERROR(FILEINFO, "client request server inlay stone info data error!!!");
			return;
		}

		for (int j = 0; j < itemInfo.inlaypos_size(); ++j)
		{
			inlayStoneInfo->add_stonepos(itemInfo.inlaypos(j));
			inlayStoneInfo->add_stoneid(itemInfo.stoneid(j));
		}
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_INALYSTONEINFO);
}

///////////////////////////////////////////装备升级//////////////////////////////////////////////////////
DEF_MSG_QUICK_REQUEST_DEFINE_FUN(EquipSmithExMgr,MSG_REQ_C2GM_UPGRADEEQUIP)
{
	LOG_DEBUG(FILEINFO, "client req gt get upgrade equip");
	EquipSmith::ClientReqUpgradeEquip req;
	ServerReturn::ServerRetInt ack;

	req.ParseFromString(content);

	if(player && player->IsOpenSystem(eEquipUpgradeLv))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Upgrade, MONITOR);;

		ack.set_ret(UpgradeEquip(player, req));


	}
	else
	{
		ack.set_ret(eNotOpen);
	}

	REQ_MSG_QUICK_ACK_FUN(message, &ack, MSG_REQ_C2GM_UPGRADEEQUIP);
}

// 升级检查
int EquipSmithExMgr::UpgradeEquipCheck(Smart_Ptr<Player>& player, int ePos, bool bGolden, bool isProtect)
{
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	if (!equipContainer || !parcelContainer)
	{
		return ePlayerPackageFail;
	}

	vector<DWORD> vec;
	int haveStoneCount = 0;
	int golden = 0; // 需扣的元宝
	Smart_Ptr<EquipProp> equipProp;
	equipProp = equipContainer->GetItemDataPtr(ePos);

	if (!equipProp)
	{
		return ePlayerPackageFail;
	}

	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
	if (NULL == goodsInfof)
	{
		return eItemError;
	}

	if(goodsInfof->useLvReq == GameParaLoader::GetInstance()->getEquipMaxLv())
	{
		return eEquipLvFull;
	}

	const GoodsInfo* goodsInfos = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID()+1);
	if (NULL == goodsInfos)
	{
		return eItemError;
	}

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

	if(goodsInfos->useLvReq > player->getAttManage()->getValue<int>(eCharLv))
	{
		return eLVNotEnough;
	}

	int nEquipType = goodsInfof->suitID > 0 ?2 :1;
	const UpgradeBaseParam* pEquiplv = EquipSmithExLoader::GetInstance()->GetUpgradeBaseParam(nEquipType,goodsInfof->itemQuality,goodsInfof->useLvReq);
	if(NULL == pEquiplv)
		return eItemError;

	haveStoneCount = parcelContainer->FindPropCount(pEquiplv->nItemId);

	if(haveStoneCount < pEquiplv->nItemNum)
	{
		if (bGolden) // 自动购买  材料不足元宝代替
		{
			//判断是否拥有vip打折权限;所有商品打8折
			MallGoodsInfo info;
			MallLoader::GetInstance()->GetItemTypeDataByID(pEquiplv->nItemId,MallType_EquipStrength,info);
			if (0 > info.id)
			{
				return eItemError;
			}
			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
			if ((bool)vip)
			{
				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pEquiplv->nItemNum - haveStoneCount));
			}
		}
		else
		{
			return eUpGradeStoneEnough;
		}
	}

	if (!player->IsEnoughGolden(golden))
	{
		return eNotEnogGolden;
	}

	// 消耗金钱
	if (player->getAttManage()->getValue<int>(eCharMoney) < pEquiplv->nMoney)
	{
		return eNotEnoghMoney;
	}
	return 0;
}


//升级
int EquipSmithExMgr::UpgradeEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqUpgradeEquip req)
{
	int ret = UpgradeEquipCheck(player, req.equippos(), req.bgolden(), req.isprotect());
	if (0 != ret)
	{
		return ret;
	}

	vector<DWORD> vec;
	int haveStoneCount = 0;
	int golden = 0; // 需扣的元宝
	Smart_Ptr<EquipProp> equipProp;
	bool bind = false;
	Smart_Ptr<ArticleEquipMgr> equipMgr = player->GetContainer(eEquipQuickType);
	equipProp = player->GetContainer(eEquipQuickType)->GetItemDataPtr(req.equippos());

	const GoodsInfo* goodsInfof = GoodsLoader::GetInstance()->GetItemDataByID(equipProp->GetID());
	int nEquipType = goodsInfof->suitID > 0 ?2 :1;
	const UpgradeBaseParam* pEquiplv = EquipSmithExLoader::GetInstance()->GetUpgradeBaseParam(nEquipType,goodsInfof->itemQuality,goodsInfof->useLvReq);
	if(NULL == pEquiplv)
		return eItemError;

	haveStoneCount = player->GetContainer(ePackageType)->FindPropCount(pEquiplv->nItemId);

	if (req.bgolden() && haveStoneCount < pEquiplv->nItemNum)
	{
		//判断是否拥有vip打折权限;所有商品打8折
		MallGoodsInfo info;
		MallLoader::GetInstance()->GetItemTypeDataByID(pEquiplv->nItemId,MallType_EquipStrength,info);

		if (0 > info.id)
		{
			return eItemError;
		}

		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
		if ((bool)vip)
		{
			golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pEquiplv->nItemNum - haveStoneCount));
		}
	}

	if (player->getAttManage()->getValue<int>(eCharMoney) < pEquiplv->nMoney)
	{
		return eNotEnoghMoney;
	}
	player->MoneyChange(pEquiplv->nMoney, false);
	//扣宝石
	bind = player->GetContainer(ePackageType)->DeleteGoodsById(pEquiplv->nItemId,pEquiplv->nItemNum,equipProp->GetBindType());

	//转化绑定类型
	if(equipProp->GetBindType() == false)
		equipProp->SetBind(bind);

	//扣除元宝
	if(golden != 0)
	{
		vector<int> vecGolden;

		//扣除元宝
		int tempbuckleGolden 	 = 0;
		int tempbuckleBindGolden = 0;

		player->DecreaseGoldenCost(golden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			vecGolden.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			vecGolden.push_back(eCharBindGolden);

		player->SynCharAttribute(vecGolden);
	}

	//更新玩家属性
	Smart_Ptr<ArticleEquipMgr> bagMgr = player->GetContainer(eEquipQuickType);
	if(false == bool(bagMgr))
		return ePlayerPackageFail;

	bagMgr->ChangeAttrbuteByEquip(false, equipProp,req.equippos());

	//升级
	equipProp->SetID(equipProp->GetID()+1);

	bagMgr->ChangeAttrbuteByEquip(true, equipProp,req.equippos());

	OneParmaEventArgs e(player->GetMyself());
	player->FireEvent(PLAYER_EQUIP_UPGRADE,e);

	vec.push_back((eEquipQuickType << 16) | req.equippos());
	player->SynCharAttribute();
	player->SetDataFlag(eEquipQuickInfo);
	player->SendSectorChange(vec,npc_fly_type);

	//完成活跃次数
	player->GetVitalityMgr()->FinshVitality(eViEquipUp);

	return eRetSuccess;
}

/////////////////////////////////////////////强化//////////////////////////////////////////////////////////////////////
DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithExMgr, MSG_REQ_GT2GM_STRENGTHEQUIP)
{
	LOG_DEBUG(FILEINFO, "Player start to strength equip");
	EquipSmith::ClientReqStrengthEquip meContent;
	ServerReturn::ServerThreeInt ack;

	char *con;
	int len = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);


	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Strength, MONITOR);;

		int equipUpgrade = 0, surplusProtect = 0;
		if(player->IsOpenSystem(eEquipStrengtLv))
		{
			ret = StrengthEquip(player, meContent, equipUpgrade, surplusProtect);
		}
		else
		{
			ret= eNotOpen;
		}

		ack.set_retf(ret);
		ack.set_rets(meContent.onekeystrenght());
		ack.set_rett(surplusProtect);

		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STRENGTHEQUIP, message, &ack);
		Message_Facade::Send(messRet);

			//强化
		if(ret == 0)
		{
			StrengthEquipArgs e(player,equipUpgrade);
			player->FireEvent(PLAYER_SMELT_STRONG,e);
		}

		//有些事件是需要升星和掉星都监听
		if(ret ==0 || ret == eStrengthEquipDropStar)
		{
			StrengthEquipArgs e(player,equipUpgrade);
			player->FireEvent(PLAYER_EQUIP_STRONG,e);
		}
	}
}

int EquipSmithExMgr::StrengthEquipCheck(Smart_Ptr<Player> &player, int ePos, bool bmeterial ,bool bGolden)
{
	Smart_Ptr<EquipProp> Equip = NULL;
	vector<DWORD> vec;
	int stoneCount = 0;
	int golden = 0;

	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);

	if (!equipContainer || !parcelContainer)
	{
		return ePlayerPackageFail;
	}

	Equip = equipContainer->GetItemDataPtr(ePos);

	//没有该装备
	if (!Equip)
	{
		return eItemError;
	}

	//装备满级了
	if ((uint)Equip->GetUpgrade() >= Strength_Max_Lv)
	{
		return eEquipLvLimit;
	}

	const StrengthBaseParam* pStrength = EquipSmithExLoader::GetInstance()->GetStrengthBaseParam(Equip->GetUpgrade());
	if(!pStrength)
	{
		return eItemError;
	}

	const GoodsInfo* strengthItemInfo = GoodsLoader::GetInstance()->GetItemDataByID(pStrength->nItemId);

	if (0 >= pStrength->nItemId || pStrength->nItemNum <= 0 || NULL == strengthItemInfo)
	{
		return eItemError;
	}

	stoneCount = parcelContainer->FindPropCount(pStrength->nItemId);

	//强化石少于需要量
	if(stoneCount < pStrength->nItemNum)
	{
		if(!bGolden)
		{
			return eStoneNumLimit;
		}
		else	//不足扣元宝
		{
			MallGoodsInfo info;
			MallLoader::GetInstance()->GetItemTypeDataByID(pStrength->nItemId,MallType_EquipStrength,info);
			//判断是否拥有vip打折权限;
			Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
			if ((bool)vip)
			{
				golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pStrength->nItemNum - stoneCount));
			}
		}
	}

	if(0 < golden && !player->IsEnoughGolden(golden))
	{
		return eNotEnogGolden;
	}

	//判断铜钱消耗
	if(player->getAttManage()->getValue<int>(eCharMoney) < pStrength->nMoney)
	{
		return eNotEnoghMoney;
	}

	return 0;
}

int EquipSmithExMgr::StrengthEquip(Smart_Ptr<Player> &player, EquipSmith::ClientReqStrengthEquip content, int& equipUpgrade, int& surplusProtect)
{
	int ret = StrengthEquipCheck(player, content.equippos(), content.bmeterial(), content.bgolden());
	if (0 != ret)
	{
		return ret;
	}

	Smart_Ptr<EquipProp> Equip = NULL;
	vector<DWORD> vec;
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Equip = equipContainer->GetItemDataPtr(content.equippos());

	const StrengthBaseParam* pStrength = EquipSmithExLoader::GetInstance()->GetStrengthBaseParam(Equip->GetUpgrade());
	int stoneCount = 0, golden = 0;

	// 强化石数量
	stoneCount = player->GetContainer(ePackageType)->FindPropCount(pStrength->nItemId);
	// 需要的强化石数量
	if (stoneCount < pStrength->nItemNum && content.bgolden())
	{ // 数量不足 用元宝花费
		MallGoodsInfo info;
		MallLoader::GetInstance()->GetItemTypeDataByID(pStrength->nItemId,MallType_EquipStrength,info);

		//判断是否拥有vip打折权限;
		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
		if ((bool)vip)
		{
			golden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pStrength->nItemNum - stoneCount));
		}
	}

	//扣除宝石
	bool bind = false;
	bind = player->GetContainer(ePackageType)->DeleteGoodsById(pStrength->nItemId, pStrength->nItemNum, Equip->GetBindType());

	//转化绑定类型
	if (!Equip->GetBindType())
		Equip->SetBind(bind);


	player->MoneyChange(pStrength->nMoney, false);

	//扣除元宝
	if(golden != 0)
	{
		vector<int> vecGolden;

		//扣除元宝
		int tempbuckleGolden 	 = 0;
		int tempbuckleBindGolden = 0;

		player->DecreaseGoldenCost(golden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			vecGolden.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			vecGolden.push_back(eCharBindGolden);

		player->SynCharAttribute(vecGolden);
	}

	//统计一天强化消耗的铜钱数
	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eEquipSmith, GlobalType::Minor::eEquipSmith_StrengthCostMoney, StatistSaveType::Int64_Data(pStrength->nMoney));

	equipContainer->ChangeAttrbuteByEquip(false,Equip,content.equippos());
	Equip->AddUpgrade();
	equipContainer->ChangeAttrbuteByEquip(true,Equip,content.equippos());

	if((uint)Equip->GetUpgrade() >= Strength_Max_Lv)
	{
		string strId = ChatMgr::GetInstance()->GetStringByInt(Equip->GetID());
		string strLv = ChatMgr::GetInstance()->GetStringByInt(Equip->GetUpgrade());
		ChatMgr::GetInstance()->Marquee(eMarqueeStrength,player->GetName(),strId,strLv);
	}

	equipUpgrade = Equip->GetUpgrade();
	vec.push_back((eEquipQuickType << 16) | content.equippos());
	player->SetDataFlag(eEquipQuickInfo);
	player->SendSectorChange(vec);

	player->SynCharAttribute();
	//完成活跃次数
	player->GetVitalityMgr()->FinshVitality(eViEquipStrengthen);

	return 0;
}



/////////////////////////////////////////////////////////镶钳////////////////////////////////////////////////////////////////////////////////////
DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithExMgr, MSG_REQ_GT2GM_STONEDAADEL)
{
	EquipSmith::AddDelStone  meContent;
	ServerReturn::ServerRetInt ack;
	char *con;
	int len = 0;
	int64 ret = 0;

	con = message->GetBuffer(len);
	meContent.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player add or delete stone on stone");

	int64 charID = message->GetMessageTime();
	Smart_Ptr<Player> player;

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
//		if(meContent.equip().container() < 0 || meContent.equip().container() >= eArticleMax)
//		{
//			LOG_ERROR(FILEINFO, "add or delete stone but container type is error");
//
//			return;
//		}

		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Inlay, MONITOR);
		int stoneID = 0;

		if(player->IsOpenSystem(eEquipInlayLv))
		{
			ret = AddDeleteStoneFromEquip(player, meContent, &stoneID);
		}
		else
		{
			ret = eNotOpen;
		}

		ack.set_ret(ret);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STONEDAADEL, message, &ack);
		Message_Facade::Send(messRet);

		if (1 == (ret >> 32))
		{
			//镶嵌
//			if(!meContent.type())
			{
				UseItemEventArgs e(player, stoneID, 1);
				player->FireEvent(PLAYER_SMELT_INSERT,e);
			}
//			else
//			{
//				UseItemEventArgs e(player, stoneID, 1);
//				player->FireEvent(PLAYER_TAKE_OFF_STONE,e);
//			}
		}
	}
}

int EquipSmithExMgr::AddDeleteStoneCheck(Smart_Ptr<Player>& player, bool type, int equipPos, int stonePos,int inlayPos)
{
	Smart_Ptr<PropBase> stone = NULL;
	Smart_Ptr<PropBase> equip = NULL;


	const GoodsInfo* stoneInfo = NULL;
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	if (!equipContainer || ! parcelContainer)
	{
		return ePlayerPackageFail;
	}

	equip = equipContainer->GetItemDataPtr(equipPos);
	if(!equip)
		return ePlayerPackageFail;

	Smart_Ptr<EquipProp> equipItem = equip;
	vector<DWORD> vec;

	// 镶嵌
	stone = parcelContainer->GetItemDataPtr(stonePos);
	if(!stone)
		return eItemError;

	if(stone->GetAmount() <= 0)
		return eItemCountNull;

	stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stone->GetID());
	if(stoneInfo==NULL || 	stoneInfo->type != eStone || stoneInfo->stoneType != EquipSmithExLoader::GetInstance()->GetInlayStoneType(equipPos))
		return eGoodsTypeError;

	if(player->getAttManage()->getValue<int>(eCharLv) < EquipSmithExLoader::GetInstance()->GetInlayPosLv(equipPos,inlayPos))
		return eInlayNotOpen;

//	// 镶嵌不能满
//	if(equipItem->IsAddStoneFull())
//	{
//		return eHoleFull;
//	}

	int stoneID = equipItem->IsHaveStone(inlayPos);
	if(0 != stoneID)
	{
		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stoneID);
		if(stoneInfo==NULL || 	stoneInfo->type != eStone)
			return eGoodsTypeError;

		int ret = parcelContainer->IsBagEnought(stoneInfo->itemID, 1);
		if (0 != ret)
		{
			return ret;
		}
	}

	return 0;

}

int64 EquipSmithExMgr::AddDeleteStoneFromEquip(Smart_Ptr<Player> &player, EquipSmith::AddDelStone req, int* stoneID)
{
	Smart_Ptr<ArticleEquipMgr> equipMgr;
	Smart_Ptr<PropBase> stone = NULL;
	Smart_Ptr<EquipProp> equipItem = NULL;
	const GoodsInfo* stoneInfo = NULL;
	int hPos = 0;
	vector<DWORD> vec;
	int equipPos = 0, stonePos = 0,inlayPos = -1;
	bool type = false;
	equipPos = req.equip().container();
	inlayPos = req.equip().pos();
	stonePos = req.stonepos();
	type = req.type();

	int64 ret = AddDeleteStoneCheck(player, type, equipPos, stonePos,inlayPos);
	if (0 != ret)
	{
		return ret;
	}
	equipMgr = player->GetContainer(eEquipQuickType);
	equipItem = player->GetContainer(eEquipQuickType)->GetItemDataPtr(equipPos);
	Smart_Ptr<ArticleEquipMgr> ArticleEquip_ptr = player->GetContainer(eEquipQuickType);

	// 摘取
	*stoneID = equipItem->IsHaveStone(inlayPos);
	if(*stoneID > 0)
	{
		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(*stoneID);

		vector<int> vecPos;
		int can_ret = player->GetContainer(ePackageType)->AddItem(*stoneID,1,vecPos,1,0,false);
		if(can_ret != 0)
			return can_ret;

		//对相应的 位置  清零   再 把之后的 位置 重置
		equipItem->ClearStone(inlayPos);
		//
//		vec.push_back((eEquipQuickType << 16) | equipPos);

		vector<int>::iterator itVec = vecPos.begin();
		for(; itVec != vecPos.end(); ++itVec)
		{
			vec.push_back((ePackageType << 16) | (*itVec));
		}

		hPos = stonePos;

		ArticleEquip_ptr->ChangePlayerAttr(stoneInfo->stoneType, stoneInfo->GetStoneAttr(), false);
	}


	// 镶嵌
	{
		stone = player->GetContainer(ePackageType)->GetItemDataPtr(stonePos);
		stoneInfo = GoodsLoader::GetInstance()->GetItemDataByID(stone->GetID());

		hPos = equipItem->InlayStone(stone->GetID(),inlayPos); // 镶嵌

		BaseArgs args;
		args.GoodsID = stoneInfo->itemID;
		args.Num =1;
		args.Pos = stonePos;
		player->GetContainer(ePackageType)->DeleteGoods(&args);

		vec.push_back((eEquipQuickType << 16) | equipPos);
		vec.push_back((ePackageType << 16) | stonePos);

		player->SetDataFlag(ePackageInfo);
		player->SetDataFlag(eEquipQuickInfo);
		*stoneID = stone->GetID();

		ArticleEquip_ptr->ChangePlayerAttr(stoneInfo->stoneType, stoneInfo->GetStoneAttr(), true);
	}

	ret = ((int64)1 << 32) | hPos;
	player->SendSectorChange(vec);

	return ret;
}

///////////////////////////////////////////////////////////////洗炼//////////////////////////////////////////////////////////////////////////////////////////
DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithExMgr,MSG_REQ_GT2GM_REFINE)
{
	EquipSmith::ClientReqRefineEquip reqGm;
	EquipSmith::ReturnNewAttr ret;
	LOG_DEBUG(FILEINFO, "Player GT2GM refine");
	int len = 0;
	char *str = message->GetBuffer(len);

	reqGm.ParseFromArray(str, len);

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if(!player)
	{
		return;
	}

	int state = 0;
	if(player->IsOpenSystem(eEquipRefineLv))
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSmith, BehaviorType::Minor::eSmith_Refine, MONITOR);

		if(!ContainerMessage::GetInstance()->CalContainerType(reqGm.container()))
		{
			state = eItemError;
		}
		else
		{
			state = RefineEquip(player, reqGm, ret);
		}
	}
	else
	{
		ret.set_status(eNotOpen);
	}

	ret.set_status(state);

	//返回网关消息
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_REFINE, message, &ret);
	Message_Facade::Send(messRet);
}

int EquipSmithExMgr::RefineEquipCheck(Smart_Ptr<Player>& player, int pos, vector<int>& topLevelPos, bool isMustTopLv,
		bool isAutoBuy)
{
	vector<DWORD> vec;
	vector<int> vecTypes;
	// 临时洗练属性
	map<int, ReGetAttr> tempAttr;

	// 所用的元宝
	int useGolden = 0;


	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	if (!equipContainer || !parcelContainer)
	{
		return ePlayerPackageFail;
	}

	//得到装备指针
	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(pos);

	if(!(bool)equipProp)
	{
		return eItemError;
	}

	//得到装备配置表
	const RefineBaseParam* pRefine = EquipSmithExLoader::GetInstance()->GetRefineBaseParam();

	if(player->getAttManage()->getValue<int>(eCharMoney) < pRefine->nMoney)
	{
		return eNotEnoghMoney;
	}

	//得到宝石数量
	int refineCount = parcelContainer->FindPropCount(pRefine->nItemId);
	//强化石少于需要量
	if(refineCount < pRefine->nItemNum)
	{
		MallGoodsInfo info;
		MallLoader::GetInstance()->GetItemTypeDataByID(pRefine->nItemId ,MallType_EquipStrength,info);
		if (info.id < 0)
		{
			return eItemError;
		}

		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
		if ((bool)vip)
		{
			useGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pRefine->nItemNum - refineCount));
		}

		if (isAutoBuy && !player->IsEnoughGolden(useGolden))
		{ // 是否自动购买
			return eNotEnoughGold;
		}
		else if (!isAutoBuy)
		{
			return eStoneNumLimit;
		}
	}

	return 0;
}

int EquipSmithExMgr::RefineEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqRefineEquip refineInfo, EquipSmith::ReturnNewAttr& ret)
{
	vector<int> topLvPos;
	for (int i = 0; i < refineInfo.toplvpos_size(); ++i)
	{
		topLvPos.push_back(refineInfo.toplvpos(i));
	}

	int err = RefineEquipCheck(player,refineInfo.equipid(), topLvPos, refineInfo.ismusttoplv(), refineInfo.isautobuy());
	if (0 != err)
	{
		return err;
	}

	// 临时洗练属性
	map<int, ReGetAttr> tempAttr;
	vector<DWORD> vec;
	vector<int> vecTypes;

	int useGolden = 0;
	//得到装备指针
	Smart_Ptr<EquipProp> equipProp = player->GetContainer(eEquipQuickType)->GetItemDataPtr(refineInfo.equipid());
	//得到装备配置表
	const RefineBaseParam* pRefine = EquipSmithExLoader::GetInstance()->GetRefineBaseParam();
	//得到宝石数量
	int refineCount = player->GetContainer(ePackageType)->FindPropCount(pRefine->nItemId);
	//强化石少于需要量
	if(refineCount < pRefine->nItemNum)
	{
		MallGoodsInfo info;
		MallLoader::GetInstance()->GetItemTypeDataByID(pRefine->nItemId ,MallType_EquipStrength,info);
		if (info.id < 0)
		{
			return eItemError;
		}

		Smart_Ptr<PlayerVipPart> vip = player->GetVipPart();
		if ((bool)vip)
		{
			useGolden += PlayerVipPart::CalBuyCostGolden(vip, info.price * (pRefine->nItemNum - refineCount));
		}
	}

	// 获得洗练属性
	if (-1 == EquipSmithExLoader::GetInstance()->CalEquipRefineAttr(tempAttr))
	{
		return eEquipUnRefine;
	}

	if (refineCount < pRefine->nItemNum && refineInfo.isautobuy())
	{
		int tempGolden	   = 0;
		int tempBindGolden = 0;
		player->DecreaseGoldenCost(useGolden,tempGolden,tempBindGolden);

		if(tempGolden > 0)
			vecTypes.push_back(eCharGolden);

		if(tempBindGolden > 0)
			vecTypes.push_back(eCharBindGolden);
	}

	//扣除宝石
	player->GetContainer(ePackageType)->DeleteGoodsById(pRefine->nItemId, pRefine->nItemNum, equipProp->GetBindType());

	if (0 < vecTypes.size())
	{
		player->SynCharAttribute(vecTypes);
	}

	player->MoneyChange(pRefine->nMoney,false);
	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eEquipSmith, GlobalType::Minor::eEquipSmith_RefineCostMoney, StatistSaveType::Int64_Data(pRefine->nMoney));

	for (map<int, ReGetAttr>::iterator it = tempAttr.begin(); it != tempAttr.end(); ++it)
	{
		EquipSmith::RefineAttr* refineAttr = ret.add_attrinfo();
		refineAttr->set_attrtype(it->second.type);
		refineAttr->set_attrvalue(it->second.value);
		refineAttr->set_attrlevel(it->second.level);
		refineAttr->set_attrpos(it->first);
	}

	ret.set_container(refineInfo.container());
	ret.set_equipid(refineInfo.equipid());

	// 保存临时洗练属性
	equipProp->SetTempAttr(tempAttr);

	SmeltArgs e(player,equipProp->GetLegendAttrCount());
	player->FireEvent(PLAYER_SMELT_SOPHISTICATED,e);

	LOG_DEBUG(FILEINFO, "refine GM TO GAT");

	return eRetSuccess;
}


DEF_MSG_REQUEST_DEFINE_FUN(EquipSmithExMgr,MSG_REQ_GT2GM_AFFIRMREFINE)
{
	EquipSmith::ClintReqAffirmRefine reqGm;
	EquipSmith::ReturnAffirmRefine ret;
	int len = 0;
	int state = 0;

	char *str = message->GetBuffer(len);
	reqGm.ParseFromArray(str, len);

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();
	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);

	if((bool)player && player->IsOpenSystem(eEquipRefineLv))
	{
		if(!ContainerMessage::GetInstance()->CalContainerType(reqGm.container()))
		{
			state = eItemError;
		}
		else
		{
			if((bool)player)
			{
				state = AffirmRefine(player, reqGm);
			}
			else
			{
				state = eAffirmError;
			}
		}

		ret.set_status(state);
		ret.set_container(reqGm.container());
		ret.set_equipid(reqGm.equipid());
		LOG_DEBUG(FILEINFO, "return AffirmRefine state = %d",state);
	}
	else
	{
		ret.set_status(eNotOpen);
	}
	//返回网关消息
	Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_AFFIRMREFINE, message, &ret);
	Message_Facade::Send(messRet);
}

int EquipSmithExMgr::AffirmRefineCheck(Smart_Ptr<Player>& player, int pos, vector<int> lockPos, bool isAutoBuy)
{
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);
	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	if (!equipContainer || !parcelContainer)
	{
		return ePlayerPackageFail;
	}

	vector<DWORD> vec;
	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(pos);
	if(!(bool)equipProp)
	{
		return eAffirmError;
	}

	const RefineBaseParam* pRefine = EquipSmithExLoader::GetInstance()->GetRefineBaseParam();
	int needGolden = 0;
	vector<int> vecTypes;

	if(!equipProp->IsHaveRefine())
		return eAffirmNull;


	if (lockPos.size() > 0)
	{
		needGolden += lockPos.size() * pRefine->nGolden;
		if (!player->IsEnoughGolden(needGolden))
		{
			return eNotEnoughGold;
		}
	}

	return 0;
}

//替换洗练属性
int EquipSmithExMgr::AffirmRefine(Smart_Ptr<Player> &player, EquipSmith::ClintReqAffirmRefine content)
{
	vector<int> vecTypes;
	int needGolden = 0;
	vector<int> lockPos;
	vector<DWORD> vec;
	for (int i = 0; i < content.lockpos_size(); ++i)
	{
		lockPos.push_back(content.lockpos(i));
	}

	// 检测
	int err = AffirmRefineCheck(player, content.equipid(), lockPos, content.isautobuy());
	if (0 != err)
	{
		return err;
	}

	Smart_Ptr<ArticleParcelMgr> parcelContainer = player->GetContainer(ePackageType);
	Smart_Ptr<ArticleEquipMgr> equipContainer = player->GetContainer(eEquipQuickType);

	Smart_Ptr<EquipProp> equipProp = equipContainer->GetItemDataPtr(content.equipid());
//	//得到装备配置表
	const RefineBaseParam* pRefine = EquipSmithExLoader::GetInstance()->GetRefineBaseParam();

	if (0 < lockPos.size())
	{
		needGolden += lockPos.size() * pRefine->nGolden;
	}

	if (needGolden > 0)
	{
		//扣除元宝
		int tempbuckleGolden 	 = 0;
		int tempbuckleBindGolden = 0;

		player->DecreaseGoldenCost(needGolden,tempbuckleGolden,tempbuckleBindGolden);

		if(tempbuckleGolden > 0)
			vecTypes.push_back(eCharGolden);

		if(tempbuckleBindGolden > 0)
			vecTypes.push_back(eCharBindGolden);
	}

	equipContainer->ChangeAttrbuteByEquip(false, equipProp,content.equipid());
	// 替换
	equipProp->AffirmRefine(lockPos);
	equipContainer->ChangeAttrbuteByEquip(true, equipProp,content.equipid());

	player->SynCharAttribute();
	player->SetDataFlag(eEquipQuickInfo);

	if (0 < vecTypes.size())
	{
		player->SynCharAttribute(vecTypes);
	}
	vec.push_back((eEquipQuickType << 16) | content.equipid());
	player->SendSectorChange(vec,npc_fly_type);

	SmeltArgs e(player,equipProp->GetLegendAttrCount());
	player->FireEvent(PLAYER_SMELT_SMELT,e);

	return eRetSuccess;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
