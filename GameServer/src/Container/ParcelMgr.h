/*
 * ParcelMgr.h
 *
 *  Created on: 2014年4月10日
 *      Author: helh
 */

#ifndef PARCELMGR_H_
#define PARCELMGR_H_

#include "ContainerBase.h"
#include "Timer/TimerInterface.h"

using namespace std;
using namespace CommBaseOut;

// 包裹管理器
class ArticleParcelMgr : public ArticleBaseMgr
{
	enum
	{
		Type = ePackageType
	};

public:
	ArticleParcelMgr(Player* player, size_t MaxContainerSize);
	~ArticleParcelMgr();

	void ReInit();
	void Release();

	virtual void SetArticleInfo(PlayerInfo::ContainerInfo *content);
	virtual void GetArticleInfo(google::protobuf::Message *info);
	virtual void GetArticleInfo(CharLogin::EquipQuickInfo *content){}
	virtual void SendArticleInfo();
	virtual void StartTime();
	virtual void ReStartTIme();
	// 针对背包的 设置开启时间 接口 PARAM: IN OUT 间隔时间   OUT间隔时间因VIP而改变
	void SetOpenTimeByInter(int64& intervalTime);

	virtual BYTE GetType()
	{
		return ArticleParcelMgr::Type;
	}

	void AddPackSize(int nTime);
	//定时开启一个格子
	void OpenPpack(void* arg);
	void AddBackPackSize(DWORD addNum);

	// VIP升级 背包开启时间改变
	// 操作VIP升级事件
	bool HandleUpdateVipLv(const EventArgs& e);

	void getAllTaskItem(map<int,int>& mapList);
	void GetAllIem(map<int, int>& itemList);

	void DiscardGoods(int container,  int gride, int& goodsID, int& num, int& err);

	bool IsGirdFull();
protected:

	// 当格子发生锁定
	virtual void OnLockGrid(BYTE pos, bool bLock);
	//丢弃物品
	int DiscardGoods(int container, int gride, int& goodsID, int& num);

private:
	TimerConnection m_timer;
	EventConnection m_vipEvent;
};




#endif /* PARCELMGR_H_ */
