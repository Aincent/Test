/*
 * CDMgr.h
 *
 *  Created on: 2014年1月20日
 *      Author: helh
 */

#ifndef CDMGR_H_
#define CDMGR_H_

#include <map>
#include "define.h"
#include "util.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "../ServerOtherDefine.h"

using namespace std;
using namespace CommBaseOut;

typedef struct sCDTime
{
	int64 startTime;
	int cdTime;
}CDTime;

// 频道类型
enum eChannelType
{
	SKILL_COMMON   = 0, // 技能公共频道
	CHAT_CHANNEL   = 1, // 聊天频道的前缀
	RELIVE_LOCAL   = 2,	// 原地复活cd频道
	RELIVE_POINT   = 3,	// 回城复活cd频道
	ARRANGE_BAG    = 4,  //整理背包CD频道
	EXCHANGE_CD    = 5,	// 下次发起交易冷却
//	WORLD_CD		  = 6,	//世界
	CD_ORDINARY = 6,				//普通聊天频道
	RANKS_CD			= 7,		//组队聊天频道
	CD_CONTINUE_BEHEADED		= 8,//连斩频道
	CD_HORN = 9,//喇叭
	BEYOND_CHANNEL = 10,//跨服战频道
};

class Player;
class CDMgr
{
public:
	CDMgr(Player * obj);
	~CDMgr();

	void ReInit();
	void Release();

	//初始化cd
	void InitCDInfo(const PlayerInfo::CDInfo &info);
	//保存数据
	void SetCDInfo(PlayerInfo::CDInfo *info);

	bool IsCDEnd(int type);

	int64 GetCDTime(int type);

	int64 GetCDEndTime(int type);

	void ResetCDTime(int type);

	void StartCD(int type, int time);

	//time为正表示加,负表示减,目前不移除CD已到的频道
	void cdTimeAlter(eChannelType type, int time,bool isSend = true);

	//time为正表示加,负表示减,目前不移除CD已到的频道
	void cdTimeAlterList(vector<eChannelType>& typeList, vector<int>& timeList,bool isSend = true);

private:

	Player * m_owner;
	CDTIMEMAP m_allCDTime;
};


#endif /* CDMGR_H_ */
