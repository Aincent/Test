/*
 * GuideCarbon.h
 *
 *  Created on: 2015年11月9日
 *      Author: root
 */

#ifndef GUIDECARBON_H_
#define GUIDECARBON_H_

#include "define.h"
#include "../Map/GameZone.h"
#include "../MapInstance/InstanceMapLoader.h"
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "../Map/GameDuplicate.h"


class GuideCarbon : public CGameDuplicate
{
public:
		GuideCarbon(int64 onlyid, int64 charid, int64 paramf, int64 params);
		virtual ~GuideCarbon();

public:
		int Finish(Smart_Ptr<Player>& player);

		bool GetTaskState(){ return m_state; }

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

		//玩家强制退出副本检验
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

		//怪物死亡事件
		bool HandleMonsterDeadEvent(const EventArgs & e);

		//人物死亡
		bool HandlePlayerDeadEvent(const EventArgs & e);

		//刷新NPC队友
		void FlushNPC(Smart_Ptr<Player>& player);

		void FlushNPC(int profession, int sex, int country, int64 mapid, int posx, int posy, int &key);

		//刷新怪物
		void FlushSecondMonsters();

		void FlushThirdMonsters();

		void FlushMonster(int monster, int posx, int posy);

		void OnFlushTimer(void* args);

		//初始化玩家信息
		void InitPlayerInfo(Smart_Ptr<Player>& player);

		void SetTaskState(bool state){ m_state = state; }

private:
		std::vector<int> m_skillList;	//初始技能列表
		int m_KillMonsterCount;					//击杀小怪的数量
		bool m_state;											//新手副本任务是否完成了
		TimerConnection m_FlushTimer;	//怪物刷新定时器
};



#endif /* GUIDECARBON_H_ */
