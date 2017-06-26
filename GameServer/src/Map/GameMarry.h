/*
 * GameMarry.h
 *
 *  Created on: 2015年3月17日
 *      Author: root
 */

#ifndef GAMEMARRY_H_
#define GAMEMARRY_H_

#include "define.h"
#include "GameZone.h"
#include <map>
#include "../EventSet/EventDefine.h"
#include "Timer/TimerInterface.h"
#include "MessageStruct/Marry/MarryMessage.pb.h"
#include "GameDuplicate.h"


using namespace std;
using namespace CommBaseOut;


//副本进行阶段
enum MarryProcess
{
	eCreate=0,
	eFireworks, //烟花
	eLatern,	//放灯
	eToast,	//祝酒
	eMeteor,	//流星
	eDuplicateChange,
	eChangeWait,
	eEnd,
	eProMax,
};


class CGameMarry : public CGameZone
{
public:
	CGameMarry(int64 onlyid, int64 charid, MarryMessInfo::MarryInstance& param);
	virtual ~CGameMarry();

	virtual int Init();

	//更新状态,false表示要删除这个场景
	bool Update();

	//加入场景
	virtual int Join(Smart_Ptr<Object> & obj);
	//加入场景前处理
	bool JoinHandle(Smart_Ptr<Object> & obj);
	//场景中移动
	//离开场景，isDel是否广播移出
	virtual void Leave(Smart_Ptr<Object> & obj, bool isDel = true, bool delFromMap = false);
	//离开场景前处理
	void LeaveHandle(Smart_Ptr<Object> & obj);
	//玩家强制退出副本检验(用于判断玩家退出后副本的后续操作 )
	void LeaveExame(Smart_Ptr<Player> & player);
	//此场景是否可以进入
	bool IsCanEnter(Smart_Ptr<Player> & player);
	//获得当前进程状态
	MarryProcess GetCurDuplicateProcess()
	{
		return m_process;
	}
	//保存进入玩家的上次场景和坐标
	void SetOldMapPoint(int64 charid, int64 mapid, int xpos, int ypos)
	{
		OldMapInfo info;

		info.mapid = mapid;
		info.xpos = xpos;
		info.ypos = ypos;

		m_oldMapPoint[charid] = info;
	}

	OldMapInfo * GetOldMapPoint(int64 charid)
	{
		map<int64,OldMapInfo>::iterator it = m_oldMapPoint.find(charid);
		if(it != m_oldMapPoint.end())
			return &it->second;

		return 0;
	}

	InstanceMapEntry* GetInstanceScript() { return m_data; }

	//副本结束时间到了之后的 操作
	void Over();
	//副本人数为零时的操作
	void CountEx();
	//是否是主拥有者
	bool IsMainOwner(int64 marryId)
	{
		return m_owner == marryId;
	}

	//倒计时开始
	void CountDownStart(void * arg);
	bool ForceAllPlayerChangeMap();

	bool IsCanFire(int64 charId);

	int SetCount(int64 charId,int type);

	void ReleaseProp(int type);

	int64 GetRingId()
	{
		return m_paramf;
	}

	void FlushProcessState(void* arg);

	void GetCeleStaus(int64 charId,MarryMessInfo::GMToClientCeleTimes& info);

	bool HandleLeaveMapEvent(const EventArgs & e);
private:
	int nfFireworks; //烟花
	int nfLatern;	//放灯
	int nfToast;	//祝酒
	int nfMeteor;	//流星

	int nwFireworks; //烟花
	int nwLatern;	//放灯
	int nwToast;	//祝酒
	int nwMeteor;	//流星

	int64 m_owner;//所属者(守护秦王和竞技场为角色id，帮派战为城池id)
	InstanceMapEntry* m_data;
	MarryProcess m_process; //副本进行的阶段

	int64 m_createTime; //创建时
	int64 m_leaveTime; //玩家走光时的时间
	int64 m_startTime;
	int64 m_cdTime;

	map<int64,OldMapInfo> m_oldMapPoint; //玩家原来的点

	int64 m_paramf;
	int64 m_params;
	int64 m_fCharId;
	int64 m_wCharId;
	vector<int64> m_vInviter;
	TimerConnection m_updateID;
	EventConnection m_leaveMapEvent;//离开场景事件
};


#endif /* GAMEMARRY_H_ */
