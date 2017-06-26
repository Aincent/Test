/*
 * funUse.cpp
 *
 *  Created on: 2014年4月30日
 *      Author: root
 */

#include "funUse.h"
#include "../Object/Player/Player.h"
#include "FileLoader/GoodsLoader.h"
#include "Goods/PropDefine.h"
#include "../CDTime/CDMgr.h"
#include "MessageCommonRet.h"
#include "../Horse/HorseManager.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "FightDefined.h"
#include "../QuestNew/QuestPart.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include <math.h>
#include "../Horse/HorseLoader/HorseLoader.h"
#include "FileLoader/GameParaLoader.h"
#include "../Attribute/AttributeCreature.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include "../StatisticMgr/StatisticHandle.h"

using namespace CommBaseOut;

int global_check_glod(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_double_exp(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_double_mry(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_cottage_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_send_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_megaphone_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_strengthen_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_succinct_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_protect_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_prestige_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_check_buff_prop(Player *player, BaseArgs *useArgs)
{
	int count = 1;
	
	if(player->IsInCreatureState(ERS_Silent)||
       player->IsInCreatureState(ERS_Stun)||
	   player->IsInCreatureState(ERS_Sleep))
	{
		return eStateNotUseItem;
	}

	if(useArgs==NULL)
	{
		return eItemError;
	}
	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	if(player->IsDead())
	{
		return eCreatureDead;
	}

	if(ginfo->telCD > 0)
	{
		if(!player->GetCDMgr()->IsCDEnd(ginfo->telCD))
		{
			return eUseGoodsCdLimit;
		}

		//hp检测
		if(GameParaLoader::GetInstance()->getHPCD()==ginfo->telCD)
		{
				if(player->getAttManage()->getValue<int>(eCharHP) >= player->getAttManage()->getValue<int>(eCharHPMax))
				{
						return eReachMaxHP;
				}

				if(useArgs->Num > 1)
				{
					const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(ginfo->buffID);
					if(info == NULL)
					{
						LOG_ERROR(FILEINFO, "add buff but buff is null");

						return false;
					}

					switch(info->useType)
					{
						case eChangeHP:
						{
							count = (player->getAttManage()->getValue<int>(eCharHPMax) - player->getAttManage()->getValue<int>(eCharHP)) / (int)info->m_pParam->m_Param;
							if(((player->getAttManage()->getValue<int>(eCharHPMax) - player->getAttManage()->getValue<int>(eCharHP)) % (int)info->m_pParam->m_Param) != 0)
							{
								count += 1;
							}

							break;
						}
						case eScaleChangeHP:
						{
							int curHp = player->getAttManage()->getValue<int>(eCharHP);
							int maxHP = player->getAttManage()->getValue<int>(eCharHPMax);
							while(count <=  useArgs->Num)
							{
								curHp += (maxHP * (int)info->m_pParam->m_Param) / 100;
								if(curHp >= maxHP)
								{
									break;
								}

								count++;
							}

							break;
						}
					}

					if(count < useArgs->Num)
						useArgs->Num = count;
				}
		}

		//mp检测
		if(GameParaLoader::GetInstance()->getMPCD()==ginfo->telCD)
		{
				if(player->getAttManage()->getValue<int>(eCharMP) >= player->getAttManage()->getValue<int>(eCharMPMax))
				{
						return eReachMaxMP;
				}

				if(useArgs->Num > 1)
				{
					const BuffInfo* info = SkillLoader::GetInstance()->GetBuffInfoPtrByID(ginfo->buffID);
					if(info == NULL)
					{
						LOG_ERROR(FILEINFO, "add buff but buff is null");

						return false;
					}

					switch(info->useType)
					{
						case eChangeMP:
						{
							count = (player->getAttManage()->getValue<int>(eCharMPMax) - player->getAttManage()->getValue<int>(eCharMP)) / (int)info->m_pParam->m_Param;
							if(((player->getAttManage()->getValue<int>(eCharMPMax) - player->getAttManage()->getValue<int>(eCharMP)) % (int)info->m_pParam->m_Param) != 0)
							{
								count += 1;
							}

							break;
						}
						case eScaleChangeMP:
						{
							int curMp = player->getAttManage()->getValue<int>(eCharMP);
							int maxMP = player->getAttManage()->getValue<int>(eCharMPMax);
							while(count <=  useArgs->Num)
							{
								curMp += (maxMP * (int)info->m_pParam->m_Param) / 100;
								if(curMp >= maxMP)
								{
									break;
								}

								count++;
							}

							break;
						}
					}

					if(count < useArgs->Num)
						useArgs->Num = count;
				}
		}
	}

	//挑战副本药品使用限制(竞技场也一样不能使用)
	if(GET_MAP_TYPE(player->GetMapID()) >= eCopyMap)
	{
		unsigned int instanceIndex = GET_MAP_ID(player->GetMapID());
		InstanceMapEntry* entry = InstanceLoader::GetInstance()->GetEntry(instanceIndex);
		if (!entry)
		{
			return eInstanceInfoError;
		}
	}

	return 0;
}
int global_check_horse_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}

int global_check_gift_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
			return eItemError;
	}
	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	if (0 < ginfo->price)
	{
		int cost = ginfo->price * useArgs->Num;
		if (0 >= cost)
		{
			return eClientSendDataError;
		}

		if (cost > player->getAttManage()->getValue<int>(eCharGolden))
		{
			return eNotEnogGolden;
		}
	}

	vector<WORD> tempFlagList;
	vector<DWORD> tempItemList;
	vector<DWORD> tempNumList;

	ginfo->m_ContainData.getItemList(tempItemList,tempNumList,(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));
	tempFlagList.resize(tempItemList.size(),1);

	if (1 < useArgs->Num)
	{
		for (uint i = 0; i < tempNumList.size(); ++i)
		{
			tempNumList[i] *= useArgs->Num;
		}
	}

	int ret = player->GetContainer(ePackageType)->IsBagEnought(tempItemList,tempNumList,tempFlagList);
	if (0 != ret)
	{
		return ret;
	}

	return 0;
}

int global_check_task_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
			return eItemError;
	}

	vector<int> vecPos;
	vector<DWORD> vec;
	int goodsID = useArgs->GoodsID;

	if(!player)
	{
		return eNull;
	}

	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}
	Smart_Ptr<QuestPart> quest = player->GetQuestMgr();
	if(quest)
	{
		DWORD questId = quest->getAcceptedQuests().findMainQuest();
		Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(questId);
		if(info==NULL)
		{
			return eQuestNotExist;
		}

		int xPos = (DWORD)player->GetXNewPos();
		int yPos = (DWORD)player->GetYNewPos();
		int itemX = info->m_nTaskXpos;
		int itemY = info->m_nTaskYpos;
		int diffx = abs(xPos-itemX);
		int diffy = abs(yPos-itemY);
		double diff = sqrt(diffx * diffx - diffy * diffy);

		if(player->GetContainer(ePackageType))
		{
			if((questId == (DWORD)ginfo->questId) && (diff < GameParaLoader::GetInstance()->getCanFinishQuestDiff()) && (GET_MAP_ID(player->GetMapID()) ==  info->m_nTaskMap))
			{
				return 0;
			}
			else
			{
				return eTaskItemAddressError;
			}
		}
	}
	return eQuestNotExist;
}

int global_check_girl_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
		return eItemError;
	}

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);

	if(ginfo!=NULL &&  !player->GetGirlWarManager()->IsHave(ginfo->buffID))
	{
		return eGirlWarNotActive;
	}

	return 0;
}
int global_check_skin_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
		return eItemError;
	}

	if(false == player->IsOpenSystem(eStHorse))
		return eNotOpen;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo==NULL)
		return eItemError;

	//是否拥有这个坐骑
	if(HorseLoader::GetInstance()->IsUseModel(player->GetHorseManager()->GetId(), ginfo->buffID))
	{
		return eHorseExist;
	}

	if(player->GetHorseManager()->IsHaveThisSkin(ginfo->buffID))
	{
		hSkinInfo skinInfo;
		player->GetHorseManager()->GetHorseSkinInfo(ginfo->buffID,skinInfo);
		if(skinInfo.skinid != -1 && skinInfo.endTime == 0)
		{
			return eHaveSkinForever;
		}
	}

	return 0;
}
int global_check_gifts_prop(Player *player, BaseArgs *useArgs)
{
	if(player->GetContainer(ePackageType)->GetFreeSize() < 2)
	{
		return eContainerFull;
	}
	return 0;
}
//int global_check_fashions_prop(Player *player, BaseArgs *useArgs)
//{
//	return 0;
//}
//int global_check_quantity_prop(Player *player, BaseArgs *useArgs)
//{
//	return 0;
//}

int global_use_glod(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
		return eItemError;
	}
	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}
	player->MoneyChange(ginfo->buffID * useArgs->Num,true);

//	if(ginfo->buffID * useArgs->Num > 0)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,ginfo->buffID * useArgs->Num);
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Money,eStaMinor_Money_UseMoneyItem,&costAndGotInfo);
//	}
	return 0;

}
int global_use_double_exp(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_double_mry(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_cottage_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_send_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_megaphone_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_strengthen_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_succinct_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
int global_use_protect_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}

int global_use_prestige_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}

int global_use_buff_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
		return eItemError;
	}

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo==NULL)
	{
		return eItemError;
	}
	Smart_Ptr<CreatureObj> obj = player->GetMyself();
	if(obj)
	{
		player->AddBuff(ginfo->buffID, obj, useArgs->Num);
	}
	if(ginfo->telCD > 0)
	{
		player->GetCDMgr()->StartCD(ginfo->telCD, ginfo->useCD);
	}
	player->SetDataFlag(eBaseInfo);
	return 0;
}
int global_use_horse_prop(Player *player, BaseArgs *useArgs)
{
	player->GetHorseManager()->AddHorse();

	return 0;
}

int global_use_gift_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
	{
		return eItemError;
	}

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo== NULL)
		return eItemError;

	int cost = 0;
	if (0 < ginfo->price)
	{
		cost = ginfo->price * useArgs->Num;
		if (0 >= cost)
		{
			return eClientSendDataError;
		}

		if (cost > player->getAttManage()->getValue<int>(eCharGolden))
		{
			return eNotEnogGolden;
		}
	}

	if (0 < cost)
	{
		player->ChangeGolden(cost, false);
	}

	int money = 0;
	int bindGolden = 0;

	vector<DWORD> goodsIdList;
	vector<DWORD> numList;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> flagVec;

	for(int i = 0; i < useArgs->Num; i++)
	{
			ginfo->m_ContainData.getItemList(goodsIdList,numList,(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));
	}

	strengthLvVec.resize(goodsIdList.size(), ginfo->equipStrengthLv);
	flagVec.resize(goodsIdList.size(),1);

	player->GetContainer(ePackageType)->AddItem(goodsIdList, numList, vecPos,flagVec,strengthLvVec,true,gift_open_type);
	for(size_t i = 0;i < goodsIdList.size();++i)
	{
		if(goodsIdList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeMoneyItemId()))
			money += numList[i];
		else if(goodsIdList[i] == DWORD(GameParaLoader::GetInstance()->getExchangeBindGoldenItemId()))
			bindGolden += numList[i];;

		const GoodsInfo* goods = GoodsLoader::GetInstance()->GetItemDataByID(goodsIdList[i]);
		if (NULL == goods)
		{
			return eItemError;
		}
		if (goods->needToBroadCast)
		{
			string strItem = ChatMgr::GetInstance()->GetStringByInt(goodsIdList[i]);
			string strOpen = ChatMgr::GetInstance()->GetStringByInt(goodsID);
			ChatMgr::GetInstance()->Marquee(eMarqueeOpenGift, player->GetName(), strOpen, strItem);
		}
	}

//	if(money > 0)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,money);
//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Money,eStaMinor_Money_UseGift,&costAndGotInfo);
//	}

//	if(bindGolden > 0)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharBindGolden,bindGolden);
//		if(eAccountType_User == player->GetAccountType())
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_BindGolden,eStaMinor_BindGolden_UseGift,&costAndGotInfo);
//	}

	return 0;
}

int global_use_task_prop(Player *player, BaseArgs *useArgs)
{
	if(player==NULL || useArgs==NULL)
	{
		return eNull;
	}

	vector<int> vecPos;
	vector<DWORD> vec;
	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	int questid = player->GetQuestMgr()->getAcceptedQuests().findMainQuest();
	Entry_Quest *info = QuestLoader::GetInstance()->GetQuestInfo(questid);
	if(!info)
	{
		return eQuestNotExist;
	}

	int xPos = player->GetXNewPos();
	int yPos = (DWORD)player->GetYNewPos();
	int itemX = info->m_nTaskXpos;
	int itemY = info->m_nTaskYpos;
	int diffx = abs(xPos-itemX);
	int diffy = abs(yPos-itemY);
	double diff = sqrt(diffx * diffx - diffy * diffy);

	if(player->GetContainer(ePackageType))
	{
		if((questid == ginfo->questId) && (diff < GameParaLoader::GetInstance()->getCanFinishQuestDiff()) &&
				(GET_MAP_ID(player->GetMapID()) ==  info->m_nTaskMap))
		{
			BaseArgs deleArg;

			deleArg.GoodsID = goodsID;
			deleArg.Num = 1;
			deleArg.Pos = -1;

			int ret = player->GetContainer(ePackageType)->DeleteGoods(&deleArg);

			if(0 != ret)
				return ret;
		}
		else
		{
			return eQuestNotExist;
		}
	}

	return 0;
}

int global_use_decrease_sin_prop(Player *player, BaseArgs *useArgs)
{
	vector<int> vecPos;
	vector<DWORD> vec;

	if(NULL==player || useArgs==NULL)
	{
		return eNull;
	}

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	if(player->getAttManage()->getValue<float>(eSinValue) <= 0.0f)
	{
		return eSinIsZero;
	}

	//减罪恶值
	player->ReduceSinValue(int(ginfo->buffID) * useArgs->Num);

//	StatisticInfo::CostAndGotInfo costAndGotInfo;
//	StatisticInfo::CostItemInfo * costInfo = costAndGotInfo.add_costlist();
//	StatisticMgr::SetItemCost(costInfo,useArgs->GoodsID,useArgs->Num);
//	if(eAccountType_User == player->GetAccountType())
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_RedName,eStaMinor_RedName_ClearRedNameCost,&costAndGotInfo);

	return 0;
}

int global_use_girl_prop(Player *player, BaseArgs *useArgs)
{
	int num = 0;
	vector<int> vecLock;
	if(useArgs==NULL)
		return -1;
	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo==NULL)
		return -1;

	//player->GetGirlWarManager()->GreateGirlWar(ginfo->buffID);
	const sGirlWarStarrank *pStarInfo= GirlWarLoader::GetInstance()->GetGirlWarStarrank(ginfo->buffID);
	if(pStarInfo==NULL)
		return -1;

	const GoodsInfo* pInfo = GoodsLoader::GetInstance()->GetItemDataByID(pStarInfo->nDebrisId);
	if(pInfo == NULL)
		return -1;

	num = pStarInfo->nResolveNum * useArgs->Num;
	if(player->GetContainer(ePackageType)->IsBagEnought(pStarInfo->nDebrisId, num,pInfo->getFlag()) != 0)
	{
//		vector<int64> getAward;
//		int64 good = num;
//		good = good << 32;	//物品数量
//		good = good | pStarInfo->nDebrisId;
//		getAward.push_back(good);
//
//		MailMessageMgr::GetInstance()->SendSysMail(getAward, player->GetName());

		return ePackageFull;
	}

	player->GetContainer(ePackageType)->AddItem(pStarInfo->nDebrisId,num,vecLock,pInfo->bindTyle,pInfo->equipStrengthLv,true, gift_open_type);

	return 0;
}

int global_use_skin_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return -1;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo==NULL)
		return -1;

	player->GetHorseManager()->GetHorseSkin(ginfo->buffID,ginfo->valideSecs,useArgs->Num);
	return 0;
}

int global_use_gifts_prop(Player *player, BaseArgs *useArgs)
{
	return 0;
}
//
//int global_use_fashions_prop(Player *player, BaseArgs *useArgs)
//{
//	if(useArgs==NULL)
//			return eItemError;
//
//	int goodsID = useArgs->GoodsID;
//	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
//	if(ginfo == NULL)
//		return eItemError;
//
//	return player->GetFashionMgr()->Activate(ginfo->buffID);
//}

//int global_use_quantity_prop(Player *player, BaseArgs *useArgs)
//{
//	if(useArgs==NULL)
//			return eItemError;
//
//	int goodsID = useArgs->GoodsID;
//	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
//	if(ginfo == NULL)
//		return eItemError;
//
//	return player->GetFashionMgr()->AddQuantity(ginfo->buffID,useArgs->Num);
//}

int global_check_sin_prop(Player* player, BaseArgs* useArgs)
{
	if(!player || useArgs==NULL)
	{
		return eNull;
	}

	if(player->getAttManage()->getValue<float>(eSinValue) <= 0.0f)
		return eSinIsZero;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
		return eItemError;

	float sinvalue = player->getAttManage()->getValue<float>(eSinValue);
	int needNum = int(ceil(sinvalue/int(ginfo->buffID)));
	int haveNum = useArgs->Num;

	useArgs->Num = (needNum > haveNum)?haveNum:needNum;

	return 0;
}

int global_check_vip_card_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	if(player->getAttManage()->getValue<DWORD>(eVIPLv) >= (DWORD)GameParaLoader::GetInstance()->getVipCardLv())
	{
		return eVipLvHigherThanVipCardLv;
	}

	if(ginfo->telCD > 0)
	{
		if(!player->GetCDMgr()->IsCDEnd(ginfo->telCD))
		{
			return eUseGoodsCdLimit;
		}
	}

	return 0;
}

int global_check_box_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	return 0;
}

int global_use_exp_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
		return eItemError;

	player->AddExp(ginfo->buffID * useArgs->Num);

	return 0;
}

int global_use_force_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
		return eItemError;

	player->ChangeCurForce(ginfo->buffID * useArgs->Num);
	player->SynCharAttribute(eCharForce, player->getAttManage()->getValue<int>(eCharForce));
	return 0;
}

int global_use_golden_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
				return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
		return eItemError;

	player->ChangeGolden(ginfo->buffID * useArgs->Num);
	player->SynCharAttribute(eCharGolden, player->getAttManage()->getValue<int>(eCharGolden));

	return 0;
}

int global_use_vip_card_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}
	Smart_Ptr<CreatureObj> obj = player->GetMyself();
	if(obj)
	{
		player->GetVipPart()->SetVipCard(ginfo->valideSecs);
	}
	if(ginfo->telCD > 0)
	{
		player->GetCDMgr()->StartCD(ginfo->telCD, ginfo->useCD);
	}
	player->SetDataFlag(eBaseInfo);

	return 0;
}

int global_use_box_prop(Player *player, BaseArgs *useArgs)
{
	int exp = 0;
	int force = 0;

	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	exp = 2000 * useArgs->Num;
	force = 1500 * useArgs->Num;

	player->ChangeCurForce(force,true);
//	if(force > 0)
//	{
//		StatisticInfo::CostAndGotInfo costAndGotInfo;
//		StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//		StatisticMgr::AddItemGotInfo(gotInfo,eCharForce,force);
//		StatisticMgr::GetInstance()->StatisticPlayerBehavior(player->GetID(),eStatic_Force,eStaMinor_Force_Box,&costAndGotInfo);
//	}
	player->SynCharAttribute(eCharForce, player->getAttManage()->getValue<int>(eCharForce));
	player->AddExp(exp);

	return 0;
}

int global_check_countryBossBox_prop(Player* player, BaseArgs* useArgs)
{
	if(useArgs==NULL)
	{
			return eItemError;
	}

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo == NULL)
	{
		return eItemError;
	}

	vector<WORD> tempFlagList;
	vector<DWORD> tempItemList;
	vector<DWORD> tempNumList;

	ginfo->m_ContainData.getItemList(tempItemList,tempNumList,(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));
	tempFlagList.resize(tempItemList.size(),1);

	if (1 < useArgs->Num)
	{
		for (uint i = 0; i < tempNumList.size(); ++i)
		{
			tempNumList[i] *= useArgs->Num;
		}
	}

	int ret = player->GetContainer(ePackageType)->IsBagEnought(tempItemList,tempNumList,tempFlagList);
	if (0 != ret)
	{
		return ret;
	}

	return 0;
}

int global_check_title_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo== NULL)
		return eItemError;

	int titleID = ginfo->buffID;
	if(titleID <= 0)
		return ePlayerTitleIDERROR;
	//已经拥有了
	if(player->GetTitle()->IsHaveTitle(titleID))
		return eAlreadyHaveThisTitle;

	return 0;
}

int global_use_countryBossBox_prop(Player* player, BaseArgs* useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo== NULL)
		return eItemError;

	bool flag = false;

	vector<DWORD> goodsIdList;
	vector<DWORD> numList;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> flagVec;

	for(int i = 0; i < useArgs->Num; i++)
	{
			ginfo->m_ContainData.getItemList(goodsIdList,numList,(ECharProf)player->getAttManage()->getValue<BYTE>(eCharProfession));
	}

	//获取的物品需要跟宝箱的绑定类型一致
	int bindType = ginfo->getFlag();
	strengthLvVec.resize(goodsIdList.size(), ginfo->equipStrengthLv);
	flagVec.resize(goodsIdList.size(),bindType);

	player->GetContainer(ePackageType)->AddItem(goodsIdList, numList, vecPos,flagVec,strengthLvVec,true,gift_open_type);

	if (!flag)
	{
		for (uint i = 0; i < goodsIdList.size(); ++i)
		{
			const GoodsInfo* goods = GoodsLoader::GetInstance()->GetItemDataByID(goodsIdList[i]);
			if (NULL == goods)
			{
				return eItemError;
			}
			if (goods->needToBroadCast)
			{
				string strItem = ChatMgr::GetInstance()->GetStringByInt(goodsIdList[i]);
				string strOpen = ChatMgr::GetInstance()->GetStringByInt(goodsID);
				ChatMgr::GetInstance()->Marquee(eMarqueeOpenGift, player->GetName(), strOpen, strItem);
			}
		}
		flag = true;
	}

	return 0;
}

int global_use_title_prop(Player *player, BaseArgs *useArgs)
{
	if(useArgs==NULL)
			return eItemError;

	int goodsID = useArgs->GoodsID;
	const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(goodsID);
	if(ginfo== NULL)
		return eItemError;

	int titleID = ginfo->buffID;
	if(titleID <= 0)
		return ePlayerTitleIDERROR;

	int ret = player->GetTitle()->addTitle(titleID);
	if(ret)
		return ret;

	return 0;
}
