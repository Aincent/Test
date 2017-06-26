/*
 * PlayerTitle.h
 *
 *  Created on: 2014年6月26日
 *      Author: root
 */

#ifndef PLAYERTITLE_H_
#define PLAYERTITLE_H_

#include "define.h"
#include <map>
#include <vector>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/ServerReturn3Int.pb.h"
#include "MessageStruct/ServerReturnInt.pb.h"

using namespace std;
using namespace CommBaseOut;

class Player;

enum eTileStatus
{
	eTitleNotActive,		//未获得
	eTitleNotGet,			//获得没领取
	eTitleHasGet,			//获得已领取
};

struct sTitleStatus
{
	int titleId;
	int titleStatus;		//0表示未获得，1表示获得未领取，2表示已领取
	int titleActive;	//0未激活 1激活
	sTitleStatus():titleId(0),titleStatus(0),titleActive(0)
	{
	}
	sTitleStatus(PlayerInfo::TitleStatus &info)
	{
		titleId = info.titleid();
		titleStatus = info.titlestatus();
		titleActive = info.titleactive();
	}
	void SetsTitleStatus(PlayerInfo::TitleStatus *info)
	{
		info->set_titleid(titleId);
		info->set_titlestatus(titleStatus);
		info->set_titleactive(titleActive);
	}
};

class Title
{
public:
	typedef std::map<int,sTitleStatus> TitleMap;
private:
	TitleMap		m_titleMap;
	Player*		m_owner;
public:
	Title(Player * player);
	~Title();

	void ReInit();
	void Release();

public:
	//	// 数据库相关
	void SetTitleToDB(PlayerInfo::TitleInfoList *TitleInfo);
	void InitTitleInfo(const PlayerInfo::TitleInfoList &info);

	//上线计算属性
	void onlineCalculateAtt();

	//计算战斗力
	int calculateFight();

	//增加战斗力排名的称号，如果有变更，需删除原称号并添加新称号
	int addRankingTitle(int titleId);
	//清楚战斗力排名称号
	int DeleteRankingTitle();
	int addTitle(int titleId);
	void ActiveTitle(int &titleId, int &errorType);
	void ActiveTitleRanking(int titleId, int &errorType);

	//声望达到获得称号
	void GetTitleWithPrestige();

	bool IsHaveTitle(int titleId);

	void GetTitleInfo(std::vector<ServerReturn::ServerThreeInt> &list);

	//领取称号奖励
	bool DrawTitlePrize(uint titleId, int &errorType);

	//统计称号相关数据
//	void StatisticTitle();

	//设置称号
	void SetActiveTitleID(int value);
	//熟悉变更
	void ChangeAttrbuteBy(bool isAdd,int titleID);
	//更新
	void UpDate(ServerReturn::ServerDoubleIntArray &ret,bool isInit);
};


#endif /* PLAYERTITLE_H_ */
