/*
 * NewGroupMgr.h
 *
 *  Created on: 2015年1月20日
 *      Author: root
 */

#ifndef NEWGROUPMGR_H_
#define NEWGROUPMGR_H_

#include "Singleton.h"
#include <list>
#include "Smart_Ptr.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/Group/GroupMessage.pb.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "../ServerOtherDefine.h"
#include "MessageCommonRet.h"


using namespace std;
using namespace CommBaseOut;

const uint GroupMember_Num = 4;  //每个队伍成员上限
const uint GroupMember_AddSomthig_Num = 2;  //可以 增加属性 的 队伍人数

const int OutputPlayerInfo_Num  = 8;  //每页显示的 玩家 数量

/////////////////////////////////////////////////////////////////////////////////////////////
// 由于 可能 会 delet 之后 忘记把 指针 置空  所以  加这个 宏控制

#define SafeDelete(p)			{ if(p) { delete (p); (p) = NULL;} }

#define SafeDeleteArray(p)		{ if(p) { delete [] (p); (p) = NULL;} }

//////////////////////////////////////////////////////////////////////////////////////


const int  BaseCreateGroupKeyNum  = 100;

class CreateGroupKeyMgr : public Singleton<CreateGroupKeyMgr>
{
public:
	CreateGroupKeyMgr():m_key(BaseCreateGroupKeyNum){}
	~CreateGroupKeyMgr(){}

	int64 CreatKey()
	{
		if(++m_key <=0)
			m_key = BaseCreateGroupKeyNum;
		return m_key;
	}

private:

	int64 m_key;
};



struct GroupMember
{
	int64 memberID; //角色ID
	bool isCaptain;  //是否 队长
	int fightPower;
	int lv ;
	int viplv;
	string name;
	BYTE profession;

	GroupMember ()
	:memberID(0),isCaptain(false),fightPower(0), lv(0),viplv(0),name(""),profession(0)
	{}

};

/*队长 地图 变更  要 更新 */
struct GroupInfo
{
	int64 groupID;
	int mapID;  //队长 所处的 场景  为 队伍 的 -----此 为 判断 是否 在 附近
	int64 captainID;

	list<GroupMember *> memberList;

	GroupInfo(const int64& id):groupID(id),mapID(0),captainID(0)
	{	}

	~GroupInfo()
	{
		Clear();
	}

	void Clear()
	{
		memberList.clear();
		groupID = 0;
		mapID = 0;
	}

	void SetMapID(const int& id){			mapID = id;	}
};

enum e_GroupReplyEnum
{
	e_group_agree =0, //同意
	e_group_noway, //不同意
};

/*
 * 此文件 为 管理 所有 本 分线 组队 信息
 * 作用  为  可以  属性 （经验 . 最大生命值）加成   ---打怪 掉落 可以 属于 队员
 * 注意:
 * 1. 玩家 下线 即认为 自动 退出
 * 2.队长 切换 地图  其他 队员 属性 变更  ----切换地图事件 中 体现
 * 3.杀怪 的经验 加成 ----------------  杀怪事件 体现
 * 4.掉落 物品 管理类 中  体现 物品 归属 -------- 杀怪事件 体现
 * 比较 要 注意 的 是  LeaveGroup()
 *
 * 本系统 的 主要 思想 是  抛弃 了 之前 组队中 对 客户端 的 操作 繁琐的
 * 只做简单 的 响应  其他  具体 队员的  的信息  都 用  通知 来作
 *
 *由于 在做完的 时候 发现 GroupInfo中 的  memberList存 new出来的 指针 好点
 *但是 都 做完了  就 没用 只能指针   －－－可以  检查这部分 的 代码... 当然 我已经 验证 OK了
 * */

class Player;

class NewGroupMgr : public Singleton<NewGroupMgr>
{
public:

	NewGroupMgr();

	~NewGroupMgr();

	void Release_NewGroupMgr();


	int CreateGroup(const Smart_Ptr<Player> &player, int64 *create_ID);
	int64 CreateGroup();

	void DeleteGroup(const int64 &groupID);
	void DeleteMember(Smart_Ptr<GroupInfo> &group_ptr, const int64 &memberID);

	int FillMemberInfo(const int64 &groupID, const Smart_Ptr<Player> &player, bool isCaptain);

	int FillMemberInfo(Smart_Ptr<GroupInfo> &group_ptr, const Smart_Ptr<Player> &player, bool isCaptain);

	void FillMemberInfo(const Smart_Ptr<Player> &player, GroupMember * ptr, bool isCaptain);

	//广播  同地图 中 自动 组队 但没有 队伍的(目前没有 用)
	void BrocastNewGroup(const int64 &groupID);

	void GetGroupPtr(const int64 &groupID, Smart_Ptr<GroupInfo> &group_ptr);
	//成员 信息
	 GroupMember *GetGroupMemberPtr(const Smart_Ptr<GroupInfo> &group_ptr, const int64 &charID);
	//队长信息
	 GroupMember *GetGroupCaptainPtr(const Smart_Ptr<GroupInfo> &group_ptr);

	//邀请 加入队伍
	int InviteToGroup(const Smart_Ptr<Player> &from_player, const Smart_Ptr<Player> &to_player);

	//发现邀请 通知
	void SendInviteMessage(const Smart_Ptr<Player> &from_player, const Smart_Ptr<Player> &to_player);

	//发送  邀请 回复
	int ReplyInviteToGroup(Smart_Ptr<Player> fromPlayer, const int& agree,
			const Smart_Ptr<Player> &toPlayer, GroupMessage::RetInviteReply &toReply);

	ServerRet inviteCreateGroup(Smart_Ptr<Player> player);

	void sendInviteResult(Smart_Ptr<Player> invitePlayer,ServerRet error,Smart_Ptr<Player> beInvitePlayer);

	//将邀请者的信发送给队员
	int SendReplyToMember(const Smart_Ptr<GroupInfo> &group_ptr, const Smart_Ptr<Player> &new_player);

	//将 其他 人 发送 给  新增成员
	int FillAnyMemberCmd(const Smart_Ptr<GroupInfo> &group_ptr,
			const Smart_Ptr<Player> &new_player,GroupMessage::RetInviteReply &toReply);


	void FillReplyToMemberCmd(const Smart_Ptr<Player> &new_player, const int &captainMapID,
			 const int64 &captainID, const uint& member_count, GroupMessage::RetPlayerStateToTeam *toMember);

	//踢出 队员
	int KickMember(const Smart_Ptr<Player> &player, const int64&charID);
	//通知 队员s
	int BrocastKickMember(const Smart_Ptr<GroupInfo> &group_ptr, const int64 & captainID, const int64 &leaveID);

	//更换 队长
	int ChangeGroupCaptain(const Smart_Ptr<Player> &player, const Smart_Ptr<Player>& willplayer);
	int64 ChangeGroupCaptain(const Smart_Ptr<GroupInfo> &group_ptr);

	void BrocastChangeCaptain(const Smart_Ptr<GroupInfo> &group_ptr, const int64 &new_captainID);

	//申请 入队
	int ApplyJoinGroup(const Smart_Ptr<Player> &player, const int64 &groupID);
	void BrocastApplyIoinGroup(const GroupMember * mem_ptr, const Smart_Ptr<Player>& appPlayer);

	//申请 回复
	int ReplyApplyJoinGroup(const Smart_Ptr<Player> &player, const Smart_Ptr<Player>& appPlayer);
	//通知  申请则 成功 入队
	void BrocastApplyer(const Smart_Ptr<GroupInfo> &group_ptr, const Smart_Ptr<Player>& appPlayer);

	// 查看附近 队伍
	int GetNearlyGroupInfoCmd(const Smart_Ptr<Player> &player, GroupMessage::RetGetTeamID &info);

	void FillGroupInfoCmd(const Smart_Ptr<Player>& player, const Smart_Ptr<GroupInfo> &group_ptr, GroupMessage::TeamInfo *info);

	void FillGroupMemberCmd(const GroupMember *ptr, GroupMessage::MemberInfo *info);

	//查看附近 玩家 信息
	int GetNearlyPlayerInfoCmd(const Smart_Ptr<Player>& player, GroupMessage::RetGetNearbyPlayerInfo &info);

	void FillNearlyPlayerInfoCmd(const Smart_Ptr<Player>& player, GroupMessage::NearbyPlayerInfo *info);

	//离开队伍
	int LeaveGroup(const Smart_Ptr<Player>& player);
	int LeaveGroup(Smart_Ptr<GroupInfo> &group_ptr, const Smart_Ptr<Player>& player);

	void BrocastLeaveGroupCmd(const Smart_Ptr<GroupInfo> &group_ptr, const int64 &leaveID);

	//模糊 查询  附近 玩家
	int GetFuzzyPlayerInfoCmd(const string &str, GroupMessage::RetGetNearbyPlayerInfo &info);

	bool ChangeMap(const EventArgs & e);

	void ChangeAttrMemberLeave(const Smart_Ptr<GroupInfo> &group_ptr);

	//同步 其他 队员 信息
	int SynchGroupMemberCmd(const Smart_Ptr<Player>& player ,GroupMessage::ToClientSysMemberMap &info);

	//查看 队伍 信息
	int FillGroupInfoCmd(const int64 &groupID, GroupMessage::RetLookforTeamInfo &info);

	// static int CalGroupExpWhenKillMonster(const int& monster_exp, const uint& member_num);
	static double GetGroupAdditionWhenKillMonster(const uint memberNum);

	bool IsSameMapGroupMember(const int& mapID,const int64 &charID,Smart_Ptr<Player>& player);

	//属性 变更  当  队伍 人数  变更
	void ChangeAttrWhenMemberChange(const Smart_Ptr<GroupInfo> &group_ptr);
	// 队员  中 有 人 切换  场景  （都 会 删除  allplayer  所以 只能 从 playercache中 判断）
	void ChangeAttrWhenMemberChangeMap(const Smart_Ptr<GroupInfo> &group_ptr, const int64 &leaveMapCharID);

	void Print_MemberInfo(const GroupMember *ptr);

	// 更改成员经验加成
	void ChangeMemExpAdd(const Smart_Ptr<GroupInfo>& group, bool flag);

	//还原pk模式
	void ReversePKType(int64 charid);

private:

	//禁止 拷贝
	NewGroupMgr(const NewGroupMgr&);

	void operator = (const NewGroupMgr& );

private:
	INT64GROUPINFOMAP m_allGroup; //所有 队伍 信息

	EventConnection m_changeMapEvent;

	//怪物被杀事件
	EventConnection m_monsterKilledEvent;
};

#define sNewGroupMgr NewGroupMgr::GetInstance()


#endif /* NEWGROUPMGR_H_ */
