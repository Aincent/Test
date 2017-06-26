/*
 * OrderMgr.cpp
 *
 *  Created on: 2014年6月27日
 *      Author: root
 */

#include "OrderMgr.h"
#include "StrArgPkg.h"
#include "../Object/Player/PlayerMessage.h"
#include "../Mail/MailMessage.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "FightDefined.h"
#include "CounterDefine.h"
#include "../DanLu/DanLuMessager.h"
#include "../DanLu/DanLuMgr.h"
#include "../Horse/HorseManager.h"
#include "../Horse/HorseMessage.h"
#include "../MagicWeapon/MagicMessage.h"
#include "../MagicWeapon/MagicMgr.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../Attribute/AttributeCreature.h"
#include "../CJobLucky/CJobLuckyMgr.h"
#include "../FestivalUI/ChristmasManage.h"
#include "FileLoader/FestivalUIFile.h"
#include "../FestivalUI/NewYearActivity.h"
#include "../Clan/ClanMessage.h"

OrderMgr::OrderMgr()
{
}

OrderMgr::~OrderMgr()
{

}
bool OrderMgr::OrderPlayer(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player> &player,string str)
{
#ifdef USE_GM
	vector<int> vecType;
	int tempf = 0;

	StrArgPkg funPkgItem(":", str);
	string name = getArg(funPkgItem,0).c_str();
	if(name == "xbst" ||name == "XBST")
	{
		for(BYTE i = 1 ; i < funPkgItem.size() ; i++)
		{
			StrArgPkg funPkgItemTemp("~",getArg(funPkgItem,i).c_str());
			string temp =getArg(funPkgItemTemp,0).c_str();
			if(temp == "exp" || temp == "EXP")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				player->AddExp(tempf);
			}
			else if(temp == "gexp" || temp == "GEXP")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				Smart_Ptr<GirlWar> girl = player->GetGirlWarManager()->GetGirlWar();
				if(girl)
				{
					girl->AddExp(tempf);
				}
			}
			else if(temp == "golden" || temp == "GOLDEN")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				player->ChangeGolden(tempf);
				//只为测试充值方便,实际运营的时候只能从后台充值
				player->SynchRechargeToWS(tempf);

				if(player->GetVipPart())
				{
					player->GetVipPart()->AddUpGold(atoi(getArg(funPkgItemTemp,1).c_str()));

					//暂时把加元宝 设置充值次数
					player->GetCounterService()->IncreasePlayerCounter(CHARGE_RMB_COUNT, 1);
					// 是否是首充
					if (!player->GetSignIn()->IsGetFirstAward() &&
							1 == player->GetCounterService()->GetNumById(CHARGE_RMB_COUNT))
					{
						player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, true);
					}
				}
				vecType.push_back(eCharGolden);
			}
			else if(temp == "ach" || temp == "ACH")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				player->AddPrestige(tempf);
				vecType.push_back(ePrestige);

				player->ChangeCredit(tempf);
				vecType.push_back(eCredit);

				player->SetDataFlag(ePrestigeInfo);
			}
			else if(temp == "bindgolden" || temp == "BINDGOLDEN")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				player->ChangeBindGolden(tempf);
				vecType.push_back(eCharBindGolden);
			}
			else if(temp == "money" || temp == "MONEY")
			{
				tempf = atoi(getArg(funPkgItemTemp,1).c_str());
				if(tempf < 0 || tempf > 999999999)tempf = 999999999;
				player->ChangeMoney(tempf);
				vecType.push_back(eCharMoney);
			}
			else if(temp == "force" || temp == "FORCE")
			{
				player->ChangeCurForce(atoi(getArg(funPkgItemTemp,1).c_str()));
				vecType.push_back(eCharForce);
			}
			else if(temp == "equip" || temp == "EQUIP")
			{
				DWORD itemID;
				DWORD itemNum;
				vector<int> vecLock;

				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				itemID  = atoi(getArg(funPkgEquipArg,0).c_str());
				itemNum = atoi(getArg(funPkgEquipArg,1,"1").c_str());

				player->GetContainer(ePackageType)->AddItem(itemID,itemNum,vecLock,0,0,true,npc_fly_type);
			}
			else if(temp == "change" || temp == "CHANGE")
			{
				MapStaticInfo *info;
				info = CMapConfig::GetInstance()->GetStaticInfo(atoi(getArg(funPkgItemTemp,1).c_str()));
				if(atoi(getArg(funPkgItemTemp,2).c_str()) == 0 && atoi(getArg(funPkgItemTemp,3).c_str()) == 0)
				{
					if(info)
					{
						PlayerMessage::GetInstance()->FastChangeMap( player, atoi(getArg(funPkgItemTemp,1).c_str()),
								info->reliveXPos,info->reliveYPos,true);
					}
				}
				else
				{
					if(info)
					{
						PlayerMessage::GetInstance()->FastChangeMap(player, atoi(getArg(funPkgItemTemp,1).c_str()),
								atoi(getArg(funPkgItemTemp,2).c_str()),atoi(getArg(funPkgItemTemp,3).c_str()),true);
					}
				}
			}
			else if(temp == "mail" || temp == "MAIL")
			{
				vector<int64> goods;
				for(BYTE i = 1 ; i < funPkgItemTemp.size(); i+=2)
				{
					int64 num = atoi(getArg(funPkgItemTemp,i+1).c_str());
					int64 item = atoi(getArg(funPkgItemTemp,i).c_str()) | (num<<32);
					goods.push_back(item);
				}
				MailMessageMgr::GetInstance()->SendSysMail(goods,player->GetName());
			}
			else if(temp == "wexp" || temp == "WEXP")
			{
				int exp = atoi(getArg(funPkgItemTemp,1).c_str());
				player->GetMagicWeapon()->AddWeaponExp(exp);
			}
			else if(temp == "dexp" || temp == "DEXP")
			{
				int exp = atoi(getArg(funPkgItemTemp,1).c_str());
				player->GetDanLuManager()->AddDanLuExp(exp);
			}
			else if(temp == "finishquest")
			{
				int tempType = atoi(getArg(funPkgItemTemp,1).c_str());
				player->GetQuestMgr()->gmFinishTask((QuestType)tempType);
			}
			else if(temp == "receivequest")
			{
				int tempQuest = atoi(getArg(funPkgItemTemp,1).c_str());
				player->GetQuestMgr()->gmReceiveTask(tempQuest);
			}
			else if(temp == "credit" || temp == "CREDIT")
			{
				int credit = atoi(getArg(funPkgItemTemp,1).c_str());
				player->ChangeCredit(credit);
				vecType.push_back(eCredit);

				player->AddPrestige(credit);
				vecType.push_back(ePrestige);
			}

			else if(temp == "attribute")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempType  = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempValue = atoi(getArg(funPkgEquipArg,1).c_str());
				player->getAttManage()->setValue((ECharAttrType)tempType,tempValue);

				vecType.push_back(tempType);
				player->SynCharAttribute(vecType);
			}
			else if(temp == "Officialjob")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempID    = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempExp   = atoi(getArg(funPkgEquipArg,1).c_str());
				int tempIndex = atoi(getArg(funPkgEquipArg,2).c_str());

				player->GetCJobLucky()->gmAddJob(tempID,tempExp,tempIndex);
			}
			else if(temp == "christmasday")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempID    = atoi(getArg(funPkgEquipArg,0).c_str());

				player->GetChristmas()->gmSetLoginDay(tempID);
			}
			else if(temp == "probability")
			{
				int tempID    = atoi(getArg(funPkgItemTemp,1).c_str());

				FestivalUIFile::GetInstance()->gmProbabilityTest(tempID);
			}
			else if(temp == "newyear")
			{
				int tempID    = atoi(getArg(funPkgItemTemp,1).c_str());
				player->GetNewYear()->gmSetContinueDay(tempID);	
			}
			else if(temp == "upstar")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempId  = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempValue = atoi(getArg(funPkgEquipArg,1).c_str());
				player->GetGirlWarManager()->SetStarrankByOrder(tempId,tempValue);
			}
			else if(temp == "upstar")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempId  = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempValue = atoi(getArg(funPkgEquipArg,1).c_str());
				player->GetGirlWarManager()->SetStarrankByOrder(tempId,tempValue);
			}
			else if(temp == "upuality")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempId  = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempValue = atoi(getArg(funPkgEquipArg,1).c_str());
				player->GetGirlWarManager()->SetQualityByOrder(tempId,tempValue);
			}
			else if(temp == "girllv")
			{
				StrArgPkg funPkgEquipArg(",",getArg(funPkgItemTemp,1).c_str());

				int tempId  = atoi(getArg(funPkgEquipArg,0).c_str());
				int tempValue = atoi(getArg(funPkgEquipArg,1).c_str());
				player->GetGirlWarManager()->SetLvByOrder(tempId,tempValue);
			}
			else if(temp == "godtree")
			{
				int tLevel =atoi(getArg(funPkgItemTemp,1).c_str());
				ClanMessage::GetInstance()->makeItemTest(player,tLevel);	
			}

			player->SetDataFlag(eBaseInfo);
			player->SynCharAttribute(vecType);
		}

		return true;
	}

#endif

	return false;
}








