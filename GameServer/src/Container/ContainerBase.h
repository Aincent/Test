/*
 * ContainerBase.h
 *
 *  Created on: 2014年3月31日
 *      Author: helh
 */
/**********************************************/
//说明：容器－背包
/*********************************************/

#ifndef CONTAINERBASE_H_
#define CONTAINERBASE_H_

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "Smart_Ptr.h"
#include "Goods/PropData.h"
#include "Goods/EquipData.h"
#include "Goods/TimeData.h"
#include "define.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "./funUse.h"
#include "EventSet/EventArgs.h"
#include "EventSet/EventSet.h"
#include "FileLoader/GoodsLoader.h"
#include "../EventSet/EventDefine.h"
#include "FightDefined.h"

using namespace std;
using namespace CommBaseOut;

enum EArticleType
{
	ePackageType = 0,
	eStoreHouseType ,
	eEquipQuickType ,
	eArticleMax,
};

enum EWearEquipOperation  //装备穿戴枚举
{
	e_EquipOperation_Wear =1,  //穿
	e_EquipOperation_Drop =2,   //脱

};

const int Once_Max_AddItem_Count  = 99;//一次最多 能够 增加的 叠加数

class Player;

class ArticleBaseMgr
{
public:
	ArticleBaseMgr(Player * player, size_t MaxContainerSize);
	virtual ~ArticleBaseMgr();

	void ReInit();
	void Release();

	virtual BYTE GetType() = 0;
	virtual void StartTime() = 0;
	virtual void ReStartTIme() = 0;
	virtual void SetArticleInfo(PlayerInfo::ContainerInfo *content) = 0;
	virtual void GetArticleInfo(google::protobuf::Message *info) = 0;
	virtual void GetArticleInfo(CharLogin::EquipQuickInfo *content) = 0;
	virtual void SetArticleInfo(const PlayerInfo::EquipQuickInfo *content){};
	virtual void SendArticleInfo() = 0;

	virtual void AddPackSize(int nTime)
	{
		return;
	}

	// 得到道具指针
	const Smart_Ptr<PropBase> GetItemDataPtr(size_t pos) const
	{
		if (IsValidPos(pos) == false)
			return 0;

		return m_gridList[pos];
	}

	Smart_Ptr<PropBase>  GetItemDataPtr(size_t pos)
	{
		if (IsValidPos(pos) == false)
			return 0;

		return m_gridList[pos];
	}
	Smart_Ptr<PropBase> GetItemDataPtrByID(int &id);

	inline bool IsValidPos(size_t pos) const
	{
		return pos < m_gridList.size();
	}

	inline bool IsEmpty(size_t pos) const
	{
		return GetItemDataPtr(pos)->IsValid() == true;
	}

	inline int GetMaxContainerSize()
	{
		return m_maxContainerSize;
	}

	inline size_t GetSize() const
	{
		return m_gridList.size();
	}

	// 判断是否锁定
	inline bool IsLock(BYTE pos) const
	{
		return m_lockGridList.count(pos) == 0 ? false : true;
	}

	// 设置锁定,是否通知客户端
	void SetLock(BYTE pos, bool bLock, bool bfire);

	// 锁定的数量
	inline size_t GetLockSize() const
	{
		return m_lockGridList.size();
	}

	//检测时限类物品
	void checkTime();
	bool isTimeOut(int pos);

	//---------------------------背包操作接口------------------------------------------
	//保存物品信息
	int SaveClientItemInfo(PlayerInfo::ItemInfo *info,vector<int> &vec);
	//获取某个格子号对应的物品信息
	void GetClientItemInfo(int gride, CharLogin::ClientItemInfo *clientItemInfo);
	//格子物品数量减一
	void SubmitGrideItem(int pos);
	//删除格子物品数
	void DeleteGrideItem(int pos,int num);
	//更换容易位置的物品
	void SaveGrideItemInfo(int gride, Smart_Ptr<PropBase> &propBase);
	//清楚某个位置的物品
	void ClearGrideItemInfo(int gride);
	//根据物品ID找到它所在的格子号,优先不邦定,可以根据自己的需求传入需要找到的是否是邦定的物品
	int FindPosByPropID(int id, int isBind = 0);
	//得到该物品的所有格子号，返回总数量
	int FindPosByPropID(int id , vector<int> &vec , bool flag);
	//得到物品数量，通过ID
	int FindPropCount(int id);
	//寻找空格子
	int FindNullPos();
	//剩余的背包格子数
	int GetFreeSize();
	//整理背包
	int ArrangeContainer(int type);
	//整理背包
	void SortBag();

	//-------------------	//初始化  玩家 背包 容器 数据    接口//---------------------------------------------//

	void InitItem(const PlayerInfo::ItemInfo *info);

	void InitEquipItem( PlayerInfo::ItemInfo *info);

//----------------------------------------------------------------//

	//此接口 为  加入 物品 ( 可以一次 加入 大于 99的 物品  )
	int Base_AddItem(int id, vector<int> &vec, int count, WORD flag, int strengthLv);

	//正常 增加
	int Base_AddSimpleItem(const GoodsInfo &ginfo, vector<int> &vec, int count,WORD flag, int strengthLv);

//----------------------添加  新物品 接口  ---勿使用 其他---------------------------------------------------------------------------

	//添加一个物品到容器,isSyc表示是否要通知客户端启动引导使用物品
	bool Synch_AddItem(Smart_Ptr<PropBase> &data, int isFly=0);

	//这接口 的 vector都是  对应的   idList 物品ID   numList数量   vecPos添加成功后 所在的 位置  strengthLv 强化等级   isBind是否 帮定
	int AddItem(const vector<DWORD>& idList,const vector<DWORD> &numList, vector<int>& vecPos,const vector<WORD>& flagList,
			const vector<DWORD> &strengthLv,bool isSend = true,fightDefined fightType = efightDefinedNULL);

	int AddItem(DWORD id, DWORD num, vector<int> &vecPos
			,WORD flag = 0,DWORD strenght = 0,bool isSend = true,fightDefined fightType = efightDefinedNULL);

	//判断背包是否足够存放物品  统一接口 勿使用  其他
	int IsBagEnought(const vector<DWORD>& idList,const vector<DWORD>& numList,const vector<WORD>& flagList);

	int IsBagEnought(DWORD id, DWORD num,WORD flag = 0);

	//-----------------------------------------------------------------------

	//添加物品到背包根据CharLogin::ClientItemInfo(从邮件中提取 物品  特例 )
	bool Mail_AddItems(const CharLogin::ClientItemInfo *Item,vector<DWORD> &vec);

	//根据ClientItemInfo得到 ItemInfo
	void GetItemInfoByClItInfo(const CharLogin::ClientItemInfo *Item,PlayerInfo::ItemInfo &info);

	void FilterVector(vector<DWORD> &idList, vector<DWORD> &numList, vector<DWORD> fristIdList, vector<DWORD> secondIdList,
												vector<DWORD> fristNumList, vector<DWORD> secondNumList);

	//---------------------------物品操作接口------------------------------------------
	//扩充一个格子
	int AddBackPackSize(int container, int num, Goods::GTAckToCOpenGoodsRet &toGate);

	//丢弃物品
//	int DiscardGOODS(int container, int gride, Goods::DiscardUpdateGoodsRet &Ret);
	//丢弃物品包括装备
	int DiscardEquipOrGoods(int goodID,int container,Goods::DiscardUpdateGoodsRet * Ret);

	//移动物品----------------------------------------
	//判断 是否 可以 作  移动 操作
	int CalCanDoMove(const int &startContainer, const int &endContainer);

	int MoveGoods(int type, int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec);
	//容器之间互移
	int MoveGoods(int type, int startContainer, int startGride, int endContainer, vector<DWORD> &vec);
	//判断是否能够容器间互移
	int CanMoveGoods(int startContainer, int startGride, int endContainer);
	//移动物品时判断  如果是仓库中的操作 是否 可以操作
	int IsCanMoveToHouseHandle(const GoodsInfo *good_ptr);

	//交换位置
	int ChangeGoods(int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec);
	//合并
	int MergeGoods(int startContainer, int startGride, int endContainer, int endGride, vector<DWORD> &vec);

	//穿戴转变
	int WearEquip(int type, int gride, int goodsId);
	//检查装备
	int CheckWearEquip(int goodsId, GoodsInfo &ginfo);
	//丢弃装备
//	int DiscardEquip(int gride);
	//分割物品
	int SplitGoods(int container, int gride, int num, vector<DWORD> &vec);
	//发送到客户端添加一个物品
	void SendAddItem(Goods::GTAckToCUpdateGoodsRet &info);

	//使用物品
	int UseGoods(BaseArgs *checkArgs);
	int CheckGoods(BaseArgs *checkArgs, const GoodsInfo &ginfo);

	//--------------------------------------------------------------------------------------------

	//删除 某件 物品   统一接口   如果  有特例 的 话 请标注
	int DeleteGoods(const BaseArgs *checkArgs,bool isSend = true);
	// 删除 某些物品    byIDOrPos: true为ID  false为pos
	int DeleteGoods(vector<BaseArgs>& checkArgs, bool byIDOrPos);

	//此方法有多少扣多少优先扣除指定帮定类型（适用于材料不足元宝代替）返回绑定类型 一个绑定则返回帮定 ******** (特例******)
	bool DeleteGoodsById(int goodsId, int num , bool idBind);

	int BaseArgs_DeleteGoodsById(int goodsId, int num,bool isSend);
	int BaseArgs_DeleteGoodsByPos(int goodsId, int pos, int num,bool isSend);
	// 支持多个不同goodsID 的删除 并且是统一发送消息  注意消息大小
	int BaseArgs_DeleteGoodsByID(vector<BaseArgs>& checkArgs);
	int BaseArgs_DeleteGoodsByPos(vector<BaseArgs>& checkArgs);

	//-------------------------------------------------------------------


	//---------------------------------------------------------
	//是否 有 足够的 物品
	bool HaveGoodsEnought(int goodsId, int num, vector<size_t> &epos);
	//---------------------------------------------------------

	//-----------根据 物品ID  获取 其数量 ---------------------------
	int GetGoodsNum(int goodsId);

	//---------------------------------------------------------

	//重组成 一个 新的 物品
	Smart_Ptr<PropBase> NewGoods(Smart_Ptr<PropBase> & propBase);
	//fillEnum
	Smart_Ptr<PropBase> Base_NewEquipProp(Smart_Ptr<EquipProp> propBase);

	Smart_Ptr<PropBase> Base_NewTimeProp(Smart_Ptr<TimeProp> propBase);
	Smart_Ptr<PropBase> Base_NewPropBase(Smart_Ptr<PropBase> & propBase);

	//-------------------------------------------------------------------------------------------------------

	// 装备栏  更改属性
	int ChangeAttr_FromEquip(const int &pos, const int &goodID, bool isAdd);
	void ChangePlayerAttr(int type, int value, bool isAdd);

	void SetOpenTime(int64 time);
	int64 GetOpenTimeStamp();
protected:

	// 整理容器
	static bool NeatenContainer(vector<Smart_Ptr<PropBase> >& gridList);

	virtual void OnLockGrid(BYTE pos, bool bLock) = 0;

protected:
	vector<Smart_Ptr<PropBase> > m_gridList; // 格子链表
	set<BYTE>	m_lockGridList; // 当前锁定的格子

	Player *m_player;   // 玩家指针
	bool m_openFlag;	//开格子模式

	size_t m_curContainerSize;//当前容量
	size_t m_maxContainerSize; //最大容量
	int64 m_openTime;
};

#endif /* CONTAINERBASE_H_ */
