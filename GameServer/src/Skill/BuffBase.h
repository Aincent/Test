/*
 * BuffBase.h
 *
 *  Created on: 2014年2月12日
 *      Author: helh
 */

#ifndef BUFFBASE_H_
#define BUFFBASE_H_

#include "define.h"
#include <vector>
#include "Smart_Ptr.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"


using namespace std;
using namespace CommBaseOut;

class CreatureObj;
class AttrChange;
class Effect;
struct BuffInfo;

class BuffBase
{
public:
	friend class BuffFactory;

	virtual void Init(Smart_Ptr<CreatureObj> & owner, Smart_Ptr<CreatureObj> & caster, DWORD onlyID, int id, int alltime, int timer, int num) = 0;

	void InitInfo(Smart_Ptr<CreatureObj>& owner, int key, int id, DWORD onlyID);

	virtual void Release();

	//获得结束时间
	virtual int64 GetEndTime() = 0;

	//获得类型
	virtual int GetType() = 0;

	//重启BUFF时间
	virtual bool ReStart() = 0;

	// 开始
	virtual bool Start(bool isDel = true) = 0;

	// 结束,外部调用结束
	virtual void End() = 0;

	//保存数据
	virtual void SetInfo(PlayerInfo::SkillInfo *info, bool isDel) = 0;

	// 判断BUFF是否已经结束
	inline bool IsEnd() const
	{
		return m_bEnd;
	}

	// 得到唯一的ID
	inline DWORD GetOnlyID() const
	{
		return m_lOnlyID;
	}

	// 得到静态ID
	inline int GetID() const
	{
		return m_id;
	}

	// 得到拥有者
	Smart_Ptr<CreatureObj>& GetOwner()
	{
		return m_owner;
	}

	//buff操作消息
	void SendBuffOperateMessage(bool type, bool isSyn = false, int time = 0);

	const BuffInfo* GetBuffInfoPtrByID();

protected:
	BuffBase(Smart_Ptr<CreatureObj>& owner, int key, int id, DWORD onlyID);

	virtual ~BuffBase();

	// 执行功能
	void Execute();

	//效果
	virtual void ResultEffect(Smart_Ptr<CreatureObj> target,const BuffInfo* info, vector<AttrChange> &toMyself, vector<AttrChange> &toOther);

	//还原效果
	void ResultReverseEffect(int type, vector<AttrChange> &toMyself, vector<AttrChange> &toOther);

	//发送属性改变
	void SendAttrChange(Smart_Ptr<CreatureObj>& fire, vector<AttrChange> &toMyself, vector<AttrChange> &toOther);

	// 也即还原
	void Reverse();

	// 结束
	void OnEnd();

	// BUFF结束了
	virtual void OnEndImp() = 0;

	Smart_Ptr<CreatureObj> m_owner;     // 拥有者,即此BUFFER对谁产生作用
	int  m_casterKey; // 即谁释放的
	int m_id; // Buff的ID

	// 当前执行的次数
	size_t m_curCount;

	// BUFF的唯一ID，整个服唯一
	DWORD m_lOnlyID;

	// BUFF是否已经结束了
	bool m_bEnd;
	int m_value; ///需要还原的值
	int m_values;	//第二个需要还原的值
	float m_ReverseValue;//需要还原的浮点数

	Effect* m_Effect;	//该指针目前有些效果用到
};



#endif /* BUFFBASE_H_ */
