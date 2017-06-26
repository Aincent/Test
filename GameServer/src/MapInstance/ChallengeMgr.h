/*
 * ChallengeMgr.h
 *
 *  Created on: 2016年5月6日
 *      Author: root
 */

#ifndef CHALLENGEMGR_H_
#define CHALLENGEMGR_H_

#include "../QuickMessage.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../Object/Player/Player.h"

using namespace std;
using namespace CommBaseOut;

struct ChallengeLayerInfo
{
	ChallengeLayerInfo()
	{
		layer = 0;
		sweeptime = 0;
		passtime = 0;
		oneStartTimes = 0;
		twoStartTimes = 0;
		threeStartTimes = 0;
		oneStartParam = 0;
		twoStartParam = 0;
		threeStartParam = 0;
	}
	int layer;				//层数
	int sweeptime;	//扫荡时间
	int passtime;		//通关时间
	int oneStartTimes;
	int twoStartTimes;
	int threeStartTimes;
	int oneStartParam;
	int twoStartParam;
	int threeStartParam;
	std::vector<std::pair<int,int> > prizes; 	//奖励
	std::vector<pair<int,int> > targets; 				//完成目标
	std::map<int, std::vector<pair<int,int> > > startsPrizes; //星级奖励
};

//排序	道具ID	货币类型	道具单价	购买数量限制	是否绑定
struct ChallengeShop
{
	ChallengeShop()
	{
		id = 0;
		price = 0;
		bindType = 0;
		layerLimit = 0;
	}
	int id;							//物品id
	int price;					//价格
	int bindType;			//帮定方式
	int layerLimit;	//通关的关卡限制
};

struct LayerMonsterInfo
{
	int monster;
	std::vector<int> vPos;
};

struct ChallengeMonstersPos
{
	int monster;
	std::vector<pair<int,int> > vPos;
};

class ChallengeMgr : public Singleton<ChallengeMgr>, public Ack_Handler, public Request_Handler, public Message_Handler
{
public:
	ChallengeMgr();
	~ChallengeMgr();
public:
	int Init();

	int GetMaxLayers(){ return m_layerConfig.size(); }
	const ChallengeLayerInfo* GetLayerInfo(int layer);

	const ChallengeShop* GetShopInfo(int id);

	void GetMonsterPos(int layer, std::vector<ChallengeMonstersPos>& infos);

	void SendAwardToPlayer(Smart_Ptr<Player> player, bool isSuccess, int layer, int star, DWORD64 epleseTime, int fight);

	int64 FitchReWard(Smart_Ptr<Player> player, int layer, int star);

	int GetStart(int layer, DWORD64 esplese);

private:
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GS_CHALLENGEINFO);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_SWEEP_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_END_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_EXCHANGE_CHALLENGE);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_C2GS_GATHER_BOX);

	int InitLayerData();

	int InitShopData();

	int InitLua();

	void Pass(Smart_Ptr<Player> player, int layer);

private:
	std::map<int,ChallengeLayerInfo> m_layerConfig;
	std::map<int,ChallengeShop> m_ChallengeShop;
	std::map<int,std::vector<LayerMonsterInfo> > m_LayerMonsterInfo;
	std::map<int,pair<int,int> > m_PosInfo;

	CLuaInterface m_pLua;
};

#endif /* CHALLENGEMGR_H_ */
