/*
 * RewardHail.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */


#include <math.h>
#include "SignIn.h"
#include "util.h"
#include "Timer/TimerInterface.h"
#include "../Map/ConfigLoader/MapConfig.h"
#include "Log/Logger.h"
#include "../Container/ContainerBase.h"
#include "../Object/Player/Player.h"
#include "MessageCommonRet.h"
#include "FileLoader/SignInLoader.h"
#include "../QuickMessage.h"
#include "CounterDefine.h"
#include "../Counter/Counter.h"
#include "../Object/FileLoader/GirlWarLoader.h"
#include "../Object/GirlWar/GirlWarManager.h"
#include "../EventSet/EventDefine.h"
#include "FightDefined.h"
#include "FileLoader/TotalLoginRewardLoader.h"
#include "../ServerEventDefine.h"
#include "FileLoader/GameParaLoader.h"
#include "OnceReward.h"
#include "../Mail/MailMessage.h"
#include "FileLoader/InternationalLoader.h"


SignIn::SignIn(Player * player):m_owner(player)
{
	ReInit();
}

SignIn::~SignIn()
{
	Release();
}

void SignIn::ReInit()
{
	m_playerTotalLogin = 1;
	m_firstChargeRewardFlag = false;
	m_mendingSignCnt = 0;

	m_playerSignIn.Init();
	m_playerReward.Init();
	m_playerTotalLoginReward.Init();

	if(!m_vipEvent.IsValid())
		m_vipEvent = RegEvent(m_owner, PLAYER_UPGRADEVIPLV_EVENT_TYPE, this, &SignIn::HandleUpdateVipLv);
}

void SignIn::Release()
{
	if(m_vipEvent.IsValid())
	{
		m_vipEvent.SetNull();
	}

	m_playerSignIn.Release();
	m_playerReward.Release();
	m_playerTotalLoginReward.Release();

}

void SignIn::SetSignInToDB(PlayerInfo::SignInInfo *signinInfo)
{
	int len = 0;
	unsigned char *signInBuf = 0;

	signInBuf = const_cast<unsigned char *>(m_playerSignIn.GetData(len));
	int signInInt = 0;
	CUtil::SafeMemmove(&signInInt, 4, signInBuf, len);
	signinInfo->set_signin(signInInt);

	len = 0;
	unsigned char *rewardBuf = 0;

	rewardBuf = const_cast<unsigned char *>(m_playerReward.GetData(len));
	int rewardInt = 0;
	CUtil::SafeMemmove(&rewardInt, 4, rewardBuf, len);
	signinInfo->set_reward(rewardInt);

	SetTotalLoginToDB(signinInfo);
}

void SignIn::SetTotalLoginToDB(PlayerInfo::SignInInfo *signinInfo)
{
	signinInfo->set_totallogin(m_playerTotalLogin);

	int len = 0;
	unsigned char *rewardBuf = 0;

	rewardBuf = const_cast<unsigned char *>(m_playerTotalLoginReward.GetData(len));
	int rewardInt = 0;
	CUtil::SafeMemmove(&rewardInt, 4, rewardBuf, len);
	signinInfo->set_totalloginreward(rewardInt);

	//-----------------------

	signinInfo->set_firstchargeoverflag(m_firstChargeRewardFlag);
}

void SignIn::InitSignInInfo(const PlayerInfo::SignInInfo &signInInfo)
{
	int signInt = 0;
	int reward =0;

	if(m_owner->GetCounterService()->getResetMothType() == eMothResetThisMoth)
	{
		signInt = signInInfo.signin();
		reward = signInInfo.reward();
	}

	//－－－－－－－－－－－－－－－－重置失败后的补救措施，重置正常后可删除－－－－－－－－－－－－－－－－－－－－－－－－－
	time_t t;
	struct tm tNow;
	time(&t);
	localtime_r(&t,&tNow);

	//到当前时间能签到的最大天数
	int tempSignInt = 0;
	int day = tNow.tm_mday;
	for(int i = 0;i < day;++i)
	{
		tempSignInt = tempSignInt | (1 << i);
	}

	//当前签到天数大于最大能签到天数，说明有问题
	if(signInt > tempSignInt)
		signInt = tempSignInt;

	//到当前时间能领取的最多奖励
	int tempReward = 0;
	const map<int, SigninStruct>& signinInfo = SignInLoader::GetInstance()->getSigninStruct();
	map<int, SigninStruct>::const_iterator it = signinInfo.begin();
	int i = 0;
	for(;it != signinInfo.end();++it)
	{
		if(day >= it->second.i_chenckInNum)
		{
			tempReward = tempReward | (1 << i);
			i++;
		}
		else
		{
			break;
		}
	}

	//当前已领奖励大于最多能领
	if(reward > tempReward)
	{
		reward = tempReward;
	}
	//－－－－－－－－－－－－－－－－重置失败后的补救措施－－－－－－－－－－－－－－－－－－－－－－－－－

	unsigned char signinBuf[4] = {0};

	CUtil::SafeMemmove(signinBuf, 4, &signInt, 4);
	m_playerSignIn.SetData(signinBuf, 4);

	unsigned char rewardBuf[4] = {0};

	CUtil::SafeMemmove(rewardBuf, 4, &reward, 4);
	m_playerReward.SetData(rewardBuf, 4);

	InitTotalLoginInfo(signInInfo);

	// 是否是VIP 补签次数
	int value = (int)m_owner->GetVipPart()->GetValueByRight(eVipMonthMendingSign);
	if (VIP_NOT_HAVE_THIS_RIGHT != value)
	{
		m_mendingSignCnt = value;
	}
}

void SignIn::InitTotalLoginInfo(const PlayerInfo::SignInInfo& signInInfo)
{
	m_playerTotalLogin = signInInfo.totallogin();

	unsigned char rewardBuf[4] = {0};
	int reward = signInInfo.totalloginreward();

	SetTotalLogin();

	if(m_playerTotalLogin <=0)
	{
		m_playerTotalLogin = 1;
		reward = 0;

		m_owner->SetDataFlag(eSignInInfo);
	}

	CUtil::SafeMemmove(rewardBuf, 4, &reward, 4);
	m_playerTotalLoginReward.SetData(rewardBuf, 4);

//	if (signInInfo.has_firstchargeoverflag() && signInInfo.firstchargeoverflag())
//	{ // 首充已经移植OnceReward模块中 此做法是为了向旧数据兼容
//		m_owner->GetOnceReward()->SetFirstChargeFlag(signInInfo.firstchargeoverflag());
//	}
	m_firstChargeRewardFlag = signInInfo.firstchargeoverflag();
}

int SignIn::SetTotalLogin()
{
	if(m_owner->GetCounterService()->getResetDayType() == eDayResetToday)
	{
		return 0;
	}

	if(m_playerTotalLogin >= MAX_TOTAL_LOGIN_COUNTER)
	{
		return 0;
	}

	this->UpdateTotalLogin(true);

	return 0;
}

void SignIn::SetSignIn(int daily,ServerReturn::ServerDoubleInt &info)
{
	//todo.
	if(m_playerSignIn.GetBit(daily))
	{
		info.set_retf(eHaveSignIn);
		return ;
	}

	int tm_mday = CounterService::getDate();
	//对未来的日子签到
	if((daily+1) > tm_mday)
	{
		info.set_retf(eSignDayAfterToday);
		return ;
	}

	if((daily+1) < tm_mday)
	{
		int MendingSignCnt = GetMendingSignCnt();
		// 是否还有补签次数
		if (MendingSignCnt > 0 && MendingSignCnt > m_owner->GetCounterService()->GetNumById(MENDING_SIGNIN_COUNT))
		{
			m_owner->GetCounterService()->IncreasePlayerCounter(MENDING_SIGNIN_COUNT, 1);
		}
		else
		{
			const int needGolen = GameParaLoader::GetInstance()->getAddSignInCost();

			int haveGolden = m_owner->getAttManage()->getValue<int>(eCharGolden);

			if(needGolen > haveGolden)
			{
				info.set_retf(eNotEnogGolden);
				return;
			}

			m_owner->ChangeGolden(needGolen,false);
		}
	}

	m_playerSignIn.SetBit(daily, true);

	SigninDailyStruct* psigninDailyStruct = NULL;
	SignInLoader::GetInstance()->GetSigninDailyStruct((daily+1),psigninDailyStruct);
	if(m_owner->GetPackage() && psigninDailyStruct)
	{
		vector<int> tempPos;
		int res = 0;
		DWORD nCount = psigninDailyStruct->nAwardNum;
		if(psigninDailyStruct->nVip && (m_owner->getAttManage()->getValue<int>(eVIPLv) > 0 || m_owner->getAttManage()->getValue<int>(eVipCardLv) > 0))
		{
			nCount = nCount*2;
		}

		res = m_owner->GetContainer(ePackageType)->IsBagEnought(psigninDailyStruct->nAward,nCount,psigninDailyStruct->nBind);
		if(res ==0)
		{
			m_owner->GetContainer(ePackageType)->AddItem(psigninDailyStruct->nAward,nCount,tempPos,psigninDailyStruct->nBind);
		}
		else
		{
			vector<int64> goods;
			int64 info = nCount;
			info = info << 32;	//物品数量
			DWORD itemid = psigninDailyStruct->nAward;
			info  = info | itemid;
			goods.push_back(info);

			if(goods.size() > 0)
			{
				string warning = InternationalLoader::GetInstance()->GetInterStr(64);
				string warning1 = InternationalLoader::GetInstance()->GetInterStr(65);
				MailMessageMgr::GetInstance()->SendSysMail(goods, m_owner->GetName(),0,warning.c_str(),warning1.c_str());
			}
		}
	}

	LOG_DEBUG(FILEINFO, "setSignin %lld day %d",m_owner->GetID(),daily);

	m_owner->GetCounterService()->IncreasePlayerCounter(TOTAL_SIGNIN_COUNTER, 1);
	m_owner->GetCounterService()->IncreasePlayerCounter(TOTAL_SIGNIN_COUNT, 1);

	if(m_owner->GetMyself())
	{
		EventArgs tArgs;
		m_owner->FireEvent(PLAYER_SIGNIN_EVENT_TYPE, tArgs);
	}

	m_owner->SetDataFlag(eSignInInfo);
	m_owner->SetDataFlag(eCounterInfo);

	info.set_retf(0);
	info.set_rets(1);
}

int SignIn::GetSignInReward(RewardHail::GetReward &reward, vector<DWORD> &goods, vector<DWORD> &num)
{
	int res = 0;
	vector<int> vecPos;
	vector<DWORD> strengthLvVec;
	vector<WORD> bindVec;

	int type = reward.list(0);
	if(type <=0)
		return eSystemError;

	if(m_playerReward.GetBit(type-1))
		return eHaveGetReward;

	vector<int> vecType;
	SigninStruct *info = SignInLoader::GetInstance()->GetSignInLoader(type);

	if(!info)
		return eRewardHailConfigError;

	int checktime = m_owner->GetCounterService()->GetNumById(TOTAL_SIGNIN_COUNTER);
	if(checktime < info->i_chenckInNum)
		return eNotGetReward;

	goods = info->m_nAward;
	num = info->m_nAwardNum;

	if(info->m_nAward.size() >0)
	{
		vector<WORD> tempFlagsList;
		tempFlagsList.resize(info->m_nAward.size(),1);

		res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_nAward,info->m_nAwardNum,tempFlagsList);
		if(res != 0)
			return res;
	}

	if (info->i_bglod > 0)
	{
		m_owner->ChangeBindGolden(info->i_bglod, true);
		vecType.push_back(eCharBindGolden);

	}
	if (info->i_money > 0)
	{
		m_owner->ChangeMoney(info->i_money, true);
		vecType.push_back(eCharMoney);

	}

	if (info->i_zhenqi > 0)
	{
		m_owner->ChangeCurForce(info->i_zhenqi, true);
		vecType.push_back(eCharForce);
	}

	bindVec.resize(goods.size(), 1);
	strengthLvVec.resize(goods.size(), 0);

	m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);


	m_owner->SynCharAttribute(vecType);

	m_playerReward.SetBit(type - 1, true);

	m_owner->SetDataFlag(eSignInInfo);
	m_owner->SetDataFlag(eBaseInfo);
	m_owner->SetDataFlag(eBattleInfo);

	EventArgs e;
	m_owner->FireEvent(PLAYER_GETSIGNIN_AWARD, e);

	return res;
}

bool SignIn::CheckSignInReward()
{
	bool bResult = false;
	int checktime = m_owner->GetCounterService()->GetNumById(TOTAL_SIGNIN_COUNTER);

	const map<int, SigninStruct>& signinInfo = SignInLoader::GetInstance()->getSigninStruct();
	map<int, SigninStruct>::const_iterator it = signinInfo.begin();
	for(;it != signinInfo.end();++it)
	{
		if(m_playerReward.GetBit(it->first -1))
			continue;

		if(checktime < it->second.i_chenckInNum)
			continue;

		bResult = true;

		break;
	}

	return bResult;
}

int SignIn::GetTolalLoginReward(RewardHail::GetReward& reward,vector<DWORD>& goods,
		vector<DWORD>& num,vector<DWORD> &strengthLvVec)
{
	int res = 0;
	vector<int> vecPos;
	vector<WORD> bindVec;

	int type = reward.list(0);
	if(m_playerTotalLogin <type || type <=0)
		return eSystemError;

	if(m_playerTotalLoginReward.GetBit(type-1))
		return eHaveGetReward;

	TotalLoginAw *info = TotalLoginRewardLoader::GetInstance()->GetTotalLoginAw(type);
	if(!info)
		return eRewardHailConfigError;

	const GoodsInfo* ginfo=NULL;
	for(uint i=0; i<info->m_totalItem.size(); ++i)
	{
		ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info->m_totalItem[i]);
		if(ginfo==NULL)
		{
			return eRewardHailConfigError;
		}
	}

	vector<WORD> tempFlagsList;
	tempFlagsList.resize(info->m_totalItem.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_totalItem, info->m_totalNum,tempFlagsList);

	goods = info->m_totalItem;
	num = info->m_totalNum;
	strengthLvVec = info->m_totalLv;

	if(!res)
	{
		vector<int> vecType;
		vector<DWORD> vec;
		m_playerTotalLoginReward.SetBit(type-1, true);
		m_owner->SetDataFlag(eSignInInfo);

//		m_owner->SynchActiveHighLight(); //........................

		if(info->bglod >0)
		{
			m_owner->ChangeBindGolden(info->bglod, true);
			vecType.push_back(eCharBindGolden);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->money >0)
		{
			m_owner->ChangeMoney(info->money, true);
			vecType.push_back(eCharMoney);
			m_owner->SetDataFlag(eBaseInfo);

//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			StatisticInfo::GotItemInfo * gotInfo = costAndGotInfo.add_gotlist();
//			StatisticMgr::AddItemGotInfo(gotInfo,eCharMoney,info->money);
//			StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_Money,eStaMinor_Money_SevenLogin,&costAndGotInfo);
		}
		if(info->zhenqi >0)
		{
			m_owner->ChangeCurForce(info->zhenqi, true);
			vecType.push_back(eCharForce);
			m_owner->SetDataFlag(eBattleInfo);
		}

		bindVec.resize(goods.size(), 1);
		m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

		if(vecType.size() >0)
		{
			m_owner->SynCharAttribute(vecType);
		}

		int status = GetTotalLoginStatus();
		vec.push_back(status);
		for(uint i=0; i<goods.size(); ++i)
		{
			vec.push_back(goods[i]);
		}
		SetRewardItemInfo(reward, vec);  //领取七日礼包时  首元素返回状态  其他为 获得 物品ID

		PlayerSevenPackage tempArgs(m_owner->GetMyself(),type);
		m_owner->FireEvent(PLAYER_SEVEN_PACKAGE,tempArgs);

		//统计 领取七日礼包
//		this->StatisticSign(eStatic_PackageGift, eStaMinor_SevenDay_GetPrize);

//		if(goods.size() == num.size())
//		{
//			StatisticInfo::CostAndGotInfo costAndGotInfo;
//			for(uint i=0; i<goods.size(); ++i)
//			{
//				StatisticInfo::GotItemInfo * gotItemInfo = costAndGotInfo.add_gotlist();
//				StatisticMgr::AddItemGotInfo(gotItemInfo,goods[i],num[i]);
//			}
//
//			if(eAccountType_User == m_owner->GetAccountType())
//				StatisticMgr::GetInstance()->StatisticPlayerBehavior(m_owner->GetID(),eStatic_PackageGift,eStaMinor_SevenDay_Event,&costAndGotInfo);
//		}

		return 0;
	}
	return res;
}

void SignIn::GetSIgnInInfo(RewardHail::ClientSignInInfo &info)
{
	int len = 0;
	unsigned char *signInBuf = 0;

	signInBuf = const_cast<unsigned char *>(m_playerSignIn.GetData(len));
	int signInInt = 0;
	CUtil::SafeMemmove(&signInInt, 4, signInBuf, len);
	info.set_signin(signInInt);

	len = 0;
	unsigned char *rewardBuf = 0;

	rewardBuf = const_cast<unsigned char *>(m_playerReward.GetData(len));
	int rewardInt = 0;
	CUtil::SafeMemmove(&rewardInt, 4, rewardBuf, len);
	info.set_reward(rewardInt);

	//补签剩余次数
	int restAddSignCount = GetMendingSignCnt() - m_owner->GetCounterService()->GetNumById(MENDING_SIGNIN_COUNT);
	info.set_restaddsigncount(restAddSignCount);
}

void SignIn::GetTolalLogin(RewardHail::OpenToalLoginAndFirstChargeRewardInfo& info)
{
//	SetTotalLogin();

	if(m_owner->IsOpenSystem(eStSevenRewardLv))
	{
		info.set_rewardinfo(GetTotalLoginStatus());
		info.set_totalcount(m_playerTotalLogin);
	}
	else
		return;

//	info.set_firstcharge((int)m_firstChargeRewardFlag);
	/*玩家 登录进来  推送 点亮图标信息*/
//	m_owner->SynchActiveHighLight(); //........................
}

int SignIn::GetTotalLoginStatus()
{
	int len = 0;
	unsigned char *rewardBuf = 0;

	rewardBuf = const_cast<unsigned char *>(m_playerTotalLoginReward.GetData(len));
	int rewardInt = 0;
	CUtil::SafeMemmove(&rewardInt, 4, rewardBuf, len);
	return rewardInt;
}

void SignIn::ResetSigIn()
{
	if(m_playerTotalLogin <MAX_TOTAL_LOGIN_COUNTER)
	{
		this->UpdateTotalLogin(false);
	}

	if(m_owner->GetCounterService()->getResetMothType() != eMothResetThisMoth)
	{
		//不同月份 重置 签到
		unsigned char signinBuf[4] = {0};
		int signInt = 0;

		CUtil::SafeMemmove(signinBuf, 4, &signInt, 4);
		m_playerSignIn.SetData(signinBuf, 4);

		unsigned char rewardBuf[4] = {0};
		int reward = 0;

		CUtil::SafeMemmove(rewardBuf, 4, &reward, 4);
		m_playerReward.SetData(rewardBuf, 4);

		m_owner->SetDataFlag(eSignInInfo);
	}
}

bool SignIn::IsFirstChargeRmb()
{
	if(m_owner->GetCounterService()->GetNumById(CHARGE_RMB_COUNT) >=1)
		return true;
	else
		return false;
}

bool SignIn::IsGetFirstAward()
{
	if (m_firstChargeRewardFlag)
	{
		return true;
	}

	return false;
}

int SignIn::GetFirstChargeRmbReward()
{
	if(!IsFirstChargeRmb())
		return eNotGetReward;
	if(m_firstChargeRewardFlag)
		return eHaveGetReward;

	int res = 0;

	FirstChargeAw *info = TotalLoginRewardLoader::GetInstance()->GetFirstChargeAw(m_owner->getAttManage()->getValue<BYTE>(eCharProfession));
	if(!info)
		return eRewardHailConfigError;

	const GoodsInfo* ginfo=NULL;
	vector<int> vecPos;

	for(uint i=0; i<info->m_totalItem.size(); ++i)
	{
		ginfo=GoodsLoader::GetInstance()->GetItemDataByID(info->m_totalItem[i]);
		if(ginfo == NULL)
		{
			return eRewardHailConfigError;
		}
	}

	vector<WORD> tempFlagsList;
	tempFlagsList.resize(info->m_totalItem.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_totalItem, info->m_totalNum,tempFlagsList);
	if(!res)
	{
		vector<WORD> bindVec;

		bindVec.resize(info->m_totalItem.size(),1);

		m_owner->GetContainer(ePackageType)->AddItem(info->m_totalItem,info->m_totalNum,vecPos,bindVec,info->m_totalLv,true,npc_fly_type);

		vector<int> vecType;

		if(info->bglod >0)
		{
			m_owner->ChangeBindGolden(info->bglod, true);
			vecType.push_back(eCharBindGolden);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->money >0)
		{
			m_owner->ChangeMoney(info->money, true);
			vecType.push_back(eCharMoney);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->zhenqi >0)
		{
			m_owner->ChangeCurForce(info->zhenqi, true);
			vecType.push_back(eCharForce);
			m_owner->SetDataFlag(eBattleInfo);
		}

		if(vecType.size() >0)
		{
			m_owner->SynCharAttribute(vecType);
		}
		return 0;
	}
	return res;
}

int SignIn::GetFirstChargeRmbReward(RewardHail::GetReward& reward,
		vector<DWORD>& goods, vector<DWORD>& num,
		vector<DWORD>& strengthLvVec)
{
	if(!IsFirstChargeRmb())
		return eNotGetReward;
	if(m_firstChargeRewardFlag)
		return eHaveGetReward;

	FirstChargeAw *info = TotalLoginRewardLoader::GetInstance()->GetFirstChargeAw(m_owner->getAttManage()->getValue<BYTE>(eCharProfession));
	if(!info)
		return eRewardHailConfigError;

	vector<int> vecPos;
	vector<WORD> bindVec;
	int res = 0;
	const GoodsInfo* ginfo=NULL;
	for(uint i=0; i<info->m_totalItem.size(); ++i)
	{
		ginfo = GoodsLoader::GetInstance()->GetItemDataByID(info->m_totalItem[i]);
		if(ginfo==NULL)
		{
			return eRewardHailConfigError;
		}
	}

	vector<WORD> tempFlagsList;
	tempFlagsList.resize(info->m_totalItem.size(),1);

	res = m_owner->GetContainer(ePackageType)->IsBagEnought(info->m_totalItem, info->m_totalNum,tempFlagsList);

	goods = info->m_totalItem;
	num = info->m_totalNum;
	strengthLvVec = info->m_totalLv;

	if(!res)
	{
		SetfirstChargeRewardFlag(true);

		vector<int> vecType;

		if(info->bglod >0)
		{
			m_owner->ChangeBindGolden(info->bglod, true);
			vecType.push_back(eCharBindGolden);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->money >0)
		{
			m_owner->ChangeMoney(info->money, true);
			vecType.push_back(eCharMoney);
			m_owner->SetDataFlag(eBaseInfo);
		}
		if(info->zhenqi >0)
		{
			m_owner->ChangeCurForce(info->zhenqi, true);
			vecType.push_back(eCharForce);
			m_owner->SetDataFlag(eBattleInfo);
		}

		bindVec.resize(goods.size(), 1);
		m_owner->GetContainer(ePackageType)->AddItem(goods, num, vecPos, bindVec,strengthLvVec,true,npc_fly_type);

		if(vecType.size() >0)
		{
			m_owner->SynCharAttribute(vecType);
		}

		SetRewardItemInfo(reward, goods);

		//统计 领取首充礼包
//		this->StatisticSign(eStatic_PackageGift, eStaMinor_Recharge_First);

		m_owner->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_FirstAward, eUIHLSmall_FirstAward_FirstRecharge), 1, false);

		return 0;
	}
	return res;
}

void SignIn::SetfirstChargeRewardFlag(bool Flag)
{
	m_firstChargeRewardFlag = Flag;
	m_owner->SetDataFlag(eSignInInfo);
}

void SignIn::SetRewardItemInfo(RewardHail::GetReward& info,
		vector<DWORD>& goods)
{
	RewardHail::GetReward tmpInfo ;
	for(uint i=0; i<goods.size(); ++i)
	{
		tmpInfo.add_list(goods[i]);
	}
	info.CopyFrom(tmpInfo);
}

int SignIn::GetSignInCanGetAward()
{
	int sigNum = 0, hadGet = 0, ret = 0;
	for(int i=0; i<31; ++i)
	{
		if(m_playerSignIn.GetBit(i))
			 sigNum++;
	}

	hadGet = CalCanReward(sigNum);
	for (int i = 0; i < hadGet; ++i)
	{
		if (!m_playerReward.GetBit(i))
			++ret;
	}

	return ret;
}

bool SignIn::TotalLogin_IsHaveRward()
{
	for(int i=0; i<m_playerTotalLogin; ++i)
	{
		if(!m_playerTotalLoginReward.GetBit(i))
			return true;
	}
	return false;
}

int SignIn::GetTLCanGetAward()
{
	int count = 0;
	for(int i=0; i<m_playerTotalLogin; ++i)
	{
		if(!m_playerTotalLoginReward.GetBit(i))
		{
			++count;
		}
	}
	return count;
}

int SignIn::CalCanReward(int sigNum)
{
//	const int signDays1 = GameParaLoader::GetInstance()->getSigNum1();
//	const int signDays2 = GameParaLoader::GetInstance()->getSigNum2();
//	const int signDays3 = GameParaLoader::GetInstance()->getSigNum3();
//	const int signDays4 = GameParaLoader::GetInstance()->getSigNum4();
//	const int signDays5 = GameParaLoader::GetInstance()->getSigNum5();
//
//	int num = 0;
//	if(sigNum >= signDays1 && sigNum < signDays2)
//		num = 1;
//	else if(sigNum >= signDays2 && sigNum < signDays3)
//		num = 2;
//	else if(sigNum >= signDays3 && sigNum < signDays4)
//		num = 3;
//	else if(sigNum >= signDays4 && sigNum < signDays5)
//		num = 4;
//	else if(sigNum >= signDays4)
//		num = 5;
//	return num;
	int num = 0;
	// 签到奖励信息
	const map<int, SigninStruct>& signinRewInfo = SignInLoader::GetInstance()->getSigninStruct();
	map<int, SigninStruct>::const_iterator it = signinRewInfo.begin();
	map<int, SigninStruct>::const_iterator end = signinRewInfo.end();
	for ( ; it != end; ++it)
	{
		if (sigNum >= it->second.i_chenckInNum)
		{
			++num;
		}
	}

	return num;
}

void SignIn::UpdateTotalLogin(bool Flag)
{
	m_playerTotalLogin += 1;  //累积登录 ＋1

	m_owner->SetDataFlag(eSignInInfo);
}

//void SignIn::StatisticSign(eStatisticMainType main_enum, eLogicRelevant fun_enum)
//{
//	StatisticMgr::GetInstance()->StatisticPlayerDailyTimes(m_owner->GetMyself(), main_enum, fun_enum, 1);
//}

bool SignIn::isTodayHaveSignin()
{
	time_t tempTime     = 0;
	struct tm tempDate;

	time(&tempTime);
	localtime_r(&tempTime,&tempDate);

	return m_playerSignIn.GetBit(tempDate.tm_mday - 1);
}


//是否领取过七日礼包，任意一种都可能过
bool SignIn::isSevenPackageHaveReceive()
{
	const map<int,TotalLoginAw>& tempInfoList    = TotalLoginRewardLoader::GetInstance()->getTotalLoginData();
	map<int,TotalLoginAw>::const_iterator tempIt = tempInfoList.begin();

	for(;tempIt != tempInfoList.end();++tempIt)
	{
		if(!m_playerTotalLoginReward.GetBit(tempIt->second.day - 1))
			continue;

		return true;
	}

	return false;
}

//是否领取过某个七日礼包，指定ID
bool SignIn::isSomeSevenPackageHaveReceive(int day)
{
	return m_playerTotalLoginReward.GetBit(day - 1);
}

// 操作VIP升级事件
bool SignIn::HandleUpdateVipLv(const EventArgs& e)
{
	PlayerAddPackSizeEventArgs* data = (PlayerAddPackSizeEventArgs*)(&e);
	if(!data)
	{
		return true;
	}

	Smart_Ptr<Player>  player = data->m_obj;
	if(player && player->GetID() == m_owner->GetID())
	{
		if(data->m_addLv > 0)
		{
			int value = (int)m_owner->GetVipPart()->GetValueByRight(eVipMonthMendingSign);
			if (VIP_NOT_HAVE_THIS_RIGHT != value)
			{
				m_mendingSignCnt = value;
			}
		}
	}

	return true;
}













