/*
 * MoneyCarbon.h
 *
 *  Created on: 2015年9月11日
 *      Author: root
 */

#ifndef MONEYCARBON_H_
#define MONEYCARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"
#include "MoneyInstanceDefine.h"

struct TowerData
{
	TowerData()
	{
		lv = 1;
		score = 0;
		defene = 0;
		attack = 0;
		hp = 0;
		cool = 0;
		defeneUseNum = 0;
		attackUseNum = 0;
		coolUseNum = 0;
	}
	int lv;
	int score;
	int defene;
	int attack;
	int hp;
	int cool;
	int defeneUseNum;
	int attackUseNum;
	int coolUseNum;
};

struct PlayerMoneyInstanceData
{
	PlayerMoneyInstanceData()
	{
		normalMonsterNum = 0;
		bossMonsterNum = 0;
		money = 0;
	}
	int normalMonsterNum;
	int bossMonsterNum;
	int money;
};

class MoneyCarbon : public CGameDuplicate
{
public:
		enum
		{
			eFitchState_no=0,				//不能领
			eFitchState_yes=1,			//可以领
			eFitchState_success=2,//已经领取
		};
		MoneyCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
		virtual ~MoneyCarbon();

public:
		int BuyAttr(Smart_Ptr<Player>& player);
		int BuyItem(Smart_Ptr<Player>& player, int type);
		int UseSkill(Smart_Ptr<Player>& player);
		int Exit(Smart_Ptr<Player>& player);
		int Fitch(Smart_Ptr<Player>& player);
		void SendMail(Smart_Ptr<Player>& player);

		//玩家属性改变
		void InitToClient(Smart_Ptr<Player>& player);
		void SyntoClient(Smart_Ptr<Player>& player, const std::vector< pair<int,int64> >& data);


		//塔属性改变
		void InitTowerData(Smart_Ptr<Player>& player);
		void TowerChange(Smart_Ptr<Player>& player, const std::vector< pair<int,int> >& data);

protected:
		virtual int Init();

		//更新状态,false表示要删除这个场景
		virtual bool Update();

		//加入场景
		virtual int Join(Smart_Ptr<Object> & obj);
		//加入场景前处理
		virtual bool JoinHandle(Smart_Ptr<Object> & obj);
		//场景中移动
		virtual int Move(Smart_Ptr<Object> &obj);
		//离开场景，isDel是否广播移出
		virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);
		//离开场景前处理
		virtual void LeaveHandle(Smart_Ptr<Object> & obj);
		//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
		virtual void LeaveExame(Smart_Ptr<Player> & player);
		//此场景是否可以进入
		virtual bool IsCanEnter(Smart_Ptr<Player> & player);

		//赢了后的操作
		virtual void Win();
		//副本结束时间到了之后的 操作
		virtual void Over();
		//副本人数为零时的操作
		virtual void Count();
		virtual void CountEx();
		//输了之后的操作
		virtual void Failed();
		//奖励
		virtual void Award();

	private:

		//怪物死亡事件
		bool HandleMonsterDeadEvent(const EventArgs & e);

		//人物死亡
		bool HandlePlayerDeadEvent(const EventArgs & e);

		//玩家升级
		bool HandlePlayerLiveUp(const EventArgs & e);

		int  IsMoneyEnough(Smart_Ptr<Player>& player,int type, int value);
		void ReduceMoney(Smart_Ptr<Player>& player,int type, int value);
		void ChangeTowerAttr(Smart_Ptr<Player>& player,const TowerInfo* pInfo, int type);
		void Earning(Smart_Ptr<Monster>& mon, Smart_Ptr<Player>& player);
		void UpdateTowerInfoByMonDead(Smart_Ptr<Player>& player,Smart_Ptr<Monster>& mon, Smart_Ptr<CreatureObj>& other);

		//初始化塔的阵营
		void InitTowerAttr(Smart_Ptr<Player>& player);

		//移出属性
		void RemoveAttr(Smart_Ptr<Player>& player);

		//塔是否全部死亡
		bool isAllTowerDead();

		//塔升级
		void TowerLiveUp(const TowerInfo* pInfo, const TowerInfo* pInfoNext,int addHP);

		//是否结束
		bool IsEnd(){ return m_IsEnd; }

		//怪物死亡是否计算冷却度
		bool IsDeadCalCool(){ return m_IsCalCool; }

		void setDeadCoolFlag(bool flag){ m_IsCalCool = flag; }

		//结束通知
		void NoticeEnd();

		//倒计时通知
		void NoticeCoutDown();

		//死亡或领奖后终止场景所有动态事件
		void clearAll();

		//
		void onNotice(void * arg);
		void onFlushMonster(void * arg);
		void onExit(void* arg);
		void onRecoveCool(void* arg);

	private:
		TimerConnection m_NoticeTimer; //怪物提前刷新公告定时器
		TimerConnection m_FlushTimer;  //怪物刷新定时器
		TimerConnection m_CoolTimer;  //cool
		EventConnection m_playerLiveUpEvent;//玩家升级事件

		Smart_Ptr<Player> m_OwnerObj;	//副本的玩家对象

		bool m_IsCountDown;							//是否已经发送 倒计时
		bool m_IsCalCool;									//怪物死亡是否计算冷却度
		bool m_IsEnd;											//活动是否已经结束或结算
		int m_fitchFlag;									//奖励领取状态

		int m_curNode;											//当前波次
		int m_buyAttrTimes;							//本场购买了多少次
		TowerData m_TowerData;						//塔信息
		PlayerMoneyInstanceData m_PlayerData;//玩家副本数据

		std::map<int,Smart_Ptr<Monster> > m_TowerObjects; 	//修罗塔怪物
		std::map<ECharAttrType,int64> 		m_attrs_add;					//玩家增加的属性
		int m_TowerAddAttrs[eMoneyInstanceItemType_cool];		//使用元宝购买符增加修罗塔的属性
};




#endif /* MONEYCARBON_H_ */
