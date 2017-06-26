/*
 * ArticleStoreHouseMgr.h
 *
 *  Created on: 2014年4月10日
 *      Author: helh
 */

#ifndef ARTICLESTOREHOUSEMGR_H_
#define ARTICLESTOREHOUSEMGR_H_

#include "ContainerBase.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

// 物品仓库管理器
class ArticleStorehouseMgr : public ArticleBaseMgr
{
	enum
	{
		Type = eStoreHouseType
	};

public:
	ArticleStorehouseMgr(Player* player, size_t MaxContainerSize);
	~ArticleStorehouseMgr();

	void ReInit();
	void Release();

	virtual BYTE GetType()
	{
		return ArticleStorehouseMgr::Type;
	}

	virtual void SetArticleInfo(PlayerInfo::ContainerInfo *content);
	virtual void GetArticleInfo(google::protobuf::Message *info);
	virtual void GetArticleInfo(CharLogin::EquipQuickInfo *content){}
	virtual void SendArticleInfo();
	virtual void StartTime();
	virtual void ReStartTIme();

	// 针对仓库的 设置开启时间 接口 PARAM: IN OUT 间隔时间   OUT间隔时间因VIP而改变
	void SetOpenTimeByInter(int64& intervalTime);

	void AddBackPackSize(DWORD addNum);
	//发送快捷消息  通知客户端 有增加的格子数量
	void SynCharStorehouseAddPack(int addNum);

	//定时开启一个格子
	void OpenPpack(void* arg);

	// VIP升级 仓库开启格子时间需改变
	// 操作VIP升级事件
	bool HandleUpdateVipLv(const EventArgs& e);

protected:

	// 当格子发生锁定
	virtual void OnLockGrid(BYTE pos, bool bLock)
	{
	}


private:

	TimerConnection m_timer;
	EventConnection m_vipEvent;
};



#endif /* ARTICLESTOREHOUSEMGR_H_ */
