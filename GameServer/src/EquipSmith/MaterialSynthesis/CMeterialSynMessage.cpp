/*
 * CMeterialSynMessage.cpp
 *
 *  Created on: 2015年9月14日
 *      Author: root
 */

#include "CMeterialSynMessage.h"
#include "../Dependence/FileLoader/MaterialSynthesisLoader.h"
#include "Network/Message_Facade.h"
#include "../../Object/Player/PlayerMessage.h"
#include "../../MessageBuild.h"
#include "../../ServerEventDefine.h"
#include "../../Container/ParcelMgr.h"
#include "../../StatisticMgr/StatisticHandle.h"

CMeterialSynMessage::CMeterialSynMessage()
{
	// TODO Auto-generated constructor stub
	DEF_MSG_REQUEST_REG_FUN(eGameServer, MSG_REQ_GT2GM_STONECREATE);
	DEF_MSG_QUICK_REQUEST_REG_FUN(CMeterialSynMessage, MSG_REQ_C2GM_METERIALSYNTHESIS);
}

CMeterialSynMessage::~CMeterialSynMessage() {
	// TODO Auto-generated destructor stub
}



void CMeterialSynMessage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

void CMeterialSynMessage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_REQUEST_DISPATCH_FUN(MSG_REQ_GT2GM_STONECREATE);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_REQUEST_DEFINE_FUN(CMeterialSynMessage, MSG_REQ_GT2GM_STONECREATE)
{
	return;
	EquipSmith::StoneCreate StoneCreate;
	ServerReturn::ServerRetInt ack;
	char *con;
	int len = 0;
	int ret = 0;

	con = message->GetBuffer(len);
	StoneCreate.ParseFromArray(con, len);

	LOG_DEBUG(FILEINFO, "Player add or delete stone on stone");

	Smart_Ptr<Player> player;
	int64 charID = message->GetMessageTime();

	PlayerMessage::GetInstance()->GetPlayerByCharid(charID, player);
	if((bool)player)
	{
		int stoneLv = 0;
//		if(player->IsOpenSystem(eStForging))
		{
			BEHAVIOR_MONI(player, BehaviorType::Main::eSynthesis, BehaviorType::Minor::eSynthesis_StoneSynth, MONITOR);

			ret = CreateStone(player, &StoneCreate,stoneLv);

			
		}
//		else
//		{
//			ret = eNotOpen;
//		}

		ack.set_ret(ret);
		Safe_Smart_Ptr<CommBaseOut::Message> messRet  = build_message(MSG_REQ_GT2GM_STONECREATE, message, &ack);
		Message_Facade::Send(messRet);


	}
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(CMeterialSynMessage, MSG_REQ_C2GM_METERIALSYNTHESIS)
{
	EquipSmith::ClientReqMeterialSyn req;
	EquipSmith::RetMeterialSyn toClient;
	req.ParseFromString(content);

	if (player)
	{
		BEHAVIOR_MONI(player, BehaviorType::Main::eSynthesis, BehaviorType::Minor::eSynthesis_MeterialSynth, MONITOR);;

		int ret = MeterialSynthesis(player, req);
		toClient.set_destmeterialid(req.destmeterialid());
		toClient.set_count(req.count());
		toClient.set_err(ret);

		

		REQ_MSG_QUICK_ACK_FUN(message, &toClient, MSG_REQ_C2GM_METERIALSYNTHESIS);
	}
}

int CMeterialSynMessage::CreateStone(Smart_Ptr<Player> &player, EquipSmith::StoneCreate *StoneCreate,int & stoneLv)
{
	int ret = MeterialSynthesis(player, StoneCreate->stoneid(), StoneCreate->count());
	if (0 != ret)
	{
		return ret;
	}

	const GoodsInfo* info = GoodsLoader::GetInstance()->GetItemDataByID(StoneCreate->stoneid());
	if (NULL == info)
	{
		return eItemError;
	}

	stoneLv = info->useLvReq;

	return 0;
}

int CMeterialSynMessage::MeterialSynthesis(Smart_Ptr<Player>& player, EquipSmith::ClientReqMeterialSyn& meterialSyn)
{
	int ret = MeterialSynthesis(player, meterialSyn.destmeterialid(), meterialSyn.count());
	if (0 != ret)
	{
		return ret;
	}



	return 0;
}

int CMeterialSynMessage::GetParcelCanSynth(Smart_Ptr<Player>& player)
{
	int count = 0;
	Smart_Ptr<ArticleParcelMgr> parcelCntr = player->GetContainer(ePackageType);
	if ((bool)parcelCntr)
	{
		map<int, int> itemList;
		parcelCntr->GetAllIem(itemList);
		if (0 < itemList.size())
		{
			map<int, MeterialSynthesisItem>& info = CMeterialSynthesisLoader::GetInstance()->GetMaterialInfo();
			int money = player->getAttManage()->getValue<int>(eCharMoney);
			for (map<int, MeterialSynthesisItem>::iterator it = info.begin(); info.end() != it; ++it)
			{
				bool flag = true;
				if (it->second.costMoney <= money)
				{
					vector<pair<int, int> >& srcMeterialID = it->second.srcMeterialID;
					for (uint i = 0; i < srcMeterialID.size(); ++i)
					{
						map<int, int>::iterator itTemp = itemList.find(srcMeterialID[i].first);
						if (itemList.end() == itTemp)
						{
							flag = false;
							break;
						}
						else
						{
							if (srcMeterialID[i].second > itTemp->second)
							{
								flag = false;
								break;
							}
						}
					}
				}
				else
				{
					flag = false;
				}
				if (flag)
				{
					count++;
					break;
				}
			}
		}
	}

	return count;
}
int CMeterialSynMessage::IsSynthesisCon(Smart_Ptr<Player>& player, int destMeterialID, int num)
{
	if (0 >= num)
	{
		return eClientSendDataError;
	}

	const GoodsInfo* destMeterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(destMeterialID);
	if (NULL == destMeterialInfo)
	{
		return eItemError;
	}

	MeterialSynthesisItem* itemInfo = CMeterialSynthesisLoader::GetInstance()->GetInfoByMaterialID(destMeterialID);
	if (NULL == itemInfo)
	{
		return eItemError;
	}

	Smart_Ptr<ArticleBaseMgr>& container = player->GetContainer(ePackageType);
	if (!container)
	{
		return ePlayerPackageFail;
	}

	// 花费
	if (0 > itemInfo->costMoney)
	{
		return eItemError;
	}


	int ret = container->IsBagEnought(destMeterialID, num);
	if (0 != ret)
	{
		return ret;
	}

	if (itemInfo->costMoney * num > player->getAttManage()->getValue<int>(eCharMoney))
	{
		return eNotEnoghMoney;
	}


	vector<pair<int, int> >& needMeterial =  itemInfo->srcMeterialID;
	// 需要的材料
	uint index = 0;
	for (index = 0; index < needMeterial.size(); ++index)
	{
		const GoodsInfo* srcMeterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(needMeterial[index].first);
		if (NULL == srcMeterialInfo)
		{
			return eItemError;
		}

		if ((needMeterial[index].second * num) > container->FindPropCount(needMeterial[index].first))
		{
			return eItemNotEnough;
		}
	}

	return 0;
}

int CMeterialSynMessage::MeterialSynthesis(Smart_Ptr<Player>& player, int destMeterialID, int num)
{
	int ret = IsSynthesisCon(player, destMeterialID, num);
	if (0 != ret)
	{
		return ret;
	}

	vector<int> vecType;
	MeterialSynthesisItem* itemInfo = CMeterialSynthesisLoader::GetInstance()->GetInfoByMaterialID(destMeterialID);
	const GoodsInfo* destMeterialInfo = GoodsLoader::GetInstance()->GetItemDataByID(destMeterialID);

	int costMoney = 0;
	costMoney = itemInfo->costMoney;

	vector<pair<int, int> >& needMeterial = itemInfo->srcMeterialID;
	Smart_Ptr<ArticleBaseMgr>& container = player->GetContainer(ePackageType);

	uint index = 0;
	// 扣除材料
	for (index = 0; index < needMeterial.size(); ++index)
	{
		container->DeleteGoodsById(needMeterial[index].first, needMeterial[index].second * num, true);
	}

	// 增加物品
	vector<int> vecPos;
	ret = container->AddItem(destMeterialID, num, vecPos, itemInfo->isBind);
	if (0 != ret)
	{
		return ret;
	}

	// 统计
//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//	StatisticMgr::AddItemGotInfo(gotItemInfo, destMeterialID, num);

//	for (index = 0; index < needMeterial.size(); ++index)
//	{
//		StatisticInfo::CostItemInfo * costStoneInfo = costAndGotInfo.add_costlist();
//		StatisticMgr::SetItemCost(costStoneInfo,needMeterial[index].first,needMeterial[index].second * num);

		//统计一天合成消耗的物品数
//		StatisticMgr::GetInstance()->StatisticPlayerItemsMap(player,eStatic_Forge,eStaMinor_Forge_ComposeCostItem,
//				needMeterial[index].first, needMeterial[index].second * num);
//	}
//	StatisticInfo::CostItemInfo * costMoneyInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCostBefore(costMoneyInfo,eCharMoney,costMoney,player);

	costMoney *= num;
	//扣除money
	if (0 < costMoney)
	{
		player->ChangeMoney(costMoney, false);
	}

	vecType.push_back(eCharMoney);

//	StatisticMgr::SetItemCostAfter(costMoneyInfo,eCharMoney,player);

//	if(eAccountType_User == player->GetAccountType())
//	{
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Forge,eStaMinor_Forge_MaterialComposeEvent,&costAndGotInfo);
		//统计一天合成消耗的铜钱数
//		StatisticMgr::GetInstance()->StatisticDailyTimes(eStatic_Forge,eStaMinor_Forge_ComposeCostMoney,costMoney);
	GLOBAL_STATISTIC_GM(player, GlobalType::Main::eMaterialSynth, GlobalType::Minor::eMaterialSynth_SynthCostMoney, StatistSaveType::Int64_Data(costMoney));
//	}

	player->SetDataFlag(ePackageInfo);
	player->SetDataFlag(eBaseInfo);
	player->SynCharAttribute(vecType);

	Smart_Ptr<CHighLightMgr> HLMgr = player->GetActiveHLMgr();
	if ((bool)HLMgr)
	{
		int isCan = CMeterialSynMessage::GetInstance()->GetParcelCanSynth(player);
		player->UpdateUIHighLight(eUIHLHandle_ModAndSyn, UIHLType(eUIHLBig_MaterialSynth, eUIHLSmall_MaterialSynth_HaveCanSynth), isCan);

	}

	//合成
	ComposeArgs e(player, destMeterialInfo->useLvReq);
	player->FireEvent(PLAYER_SMELT_COMPOSE,e);

	return 0;
}
