/*
 * ArticleEquipMgr.h
 *
 *  Created on: 2014年4月11日
 *      Author: helh
 */

#ifndef ARTICLEEQUIPMGR_H_
#define ARTICLEEQUIPMGR_H_

#include "ContainerBase.h"
#include "FileLoader/EquipSmithExLoader.h"

using namespace std;
using namespace CommBaseOut;

struct QuickSector
{
	BYTE type;
	int value;

	QuickSector():type(0),value(0)
	{

	}
};

enum UpdateQuickSector
{
	REMOVE_QUICK_SECTOR = 1,
	ADD_QUICK_SECTOR = 2,
};

enum EQuickLan
{
	e_QuickLan_move =0,
	e_QuickLan_skill =1,
	e_QuickLan_good =2,
	e_QuickLan_clear =3,
};

#define QUICK_SECTOR_COUNT 10
#define EQUIPITEM_SECTOR_COUNT 12

#define MAINEQUIPPOS 2   //主武器 位置
#define COSTUMEPOS 4     //衣服 位置

class ArticleEquipMgr : public ArticleBaseMgr
{
	enum
	{
		Type = eEquipQuickType
	};

public:
	ArticleEquipMgr(Player* player, size_t MaxContainerSize);
	 ~ArticleEquipMgr();

	 void ReInit();
	 void Release();

	virtual BYTE GetType()
	{
		return ArticleEquipMgr::Type;
	}

	virtual void SetArticleInfo(PlayerInfo::ContainerInfo *content);
	virtual void GetArticleInfo(google::protobuf::Message *info);
//	virtual void GetArticleInfo(CharLogin::ClientContainerInfo *content);
	virtual void GetArticleInfoByPos(CharLogin::ClientContainerInfo *content,PlayerInfo::ContainerInfo *info);
	virtual void GetArticleInfo(CharLogin::EquipQuickInfo *content);
	virtual void SetArticleInfo(const PlayerInfo::EquipQuickInfo *content);

	//上线计算属性
	void onlineCalculateAtt();

	//计算一次战斗力
	int calculateFight();

	//获得某件装备的所有属性，套装的不算
	void getOneEquipAtt(int nPos,Smart_Ptr<EquipProp> equip,map<ECharAttrType,int64>& attList);

	virtual void SendArticleInfo();
	virtual void StartTime(){}
	virtual void ReStartTIme(){}
	QuickSector GetQuickSector(int gride);
	void ChangeQuickSector(int srcGride, int destGride);
	void SetQuickSector(int gride, int type, int value);
	void ClearQuickSector(int gride);
	int IsEmptyQuickSector(int gride);
	int GetQuickSectorSize();
	void UpdateQuickSectorById(int goodsId, int type);
	int MoveQuickSector(int type, int arg1, int arg2, int container);
	//获取已装备的传说级别装备数量和传说级别且满级装备数量
	void GetLegendEquipCount(int & legendEquipCount,int & legendAndFullStrengthEquipCount);
	//获得已装备的装备数量
	DWORD GetEquipCountWeared(void);
	//获得已装备的大于等于指定强化级数的装备
	DWORD GetEquipCountStrength();
	//获得大于等于指定洗练星级的洗练条数
	DWORD GetRefineCount();
	//获取装备等级大于多少级且品质大于多岁的已装备装备数量
	DWORD GetEquipLvCount();
	//获取已镶嵌在已装备的装备上的指定等级的宝石个数
	DWORD GetStoneCount();
	DWORD GetStoneCount(int nLv);

	// 装备栏  更改属性
	int ChangeAttr_FromEquip(const int &pos,const int&before_ID, const int &after_ID);
	void ChangePlayerAttr(const int &stone_ID, bool isAdd);
	void ChangePlayerAttr(int type, int value, bool isAdd);
	void ChangePlayerAttr(int type, int value, bool isAdd,std::vector<int> &vec,std::vector<int> &vecBro);
	void ChangeAttrbuteByEquip(bool isAdd,const Smart_Ptr<EquipProp> &propBase,int nPos = 0);

	void InitEquipItem(PlayerInfo::ItemInfo *info);

	int WearEquip(int type, int gride, int goodsId);

	int CheckWearEquip(int goodsId,const GoodsInfo &ginfo);

	//
	int ChangeEquipPos(const int &startContainer, const int & startGride,
													const int &endContainer, const int &endGride, vector<DWORD> &vec);

	void ChangeNewEuipStone(const Smart_Ptr<EquipProp> &equip_ptr, Smart_Ptr<EquipProp> &new_pro);

	void ChangeNewEuipAttr(const Smart_Ptr<EquipProp> &equip_ptr, Smart_Ptr<EquipProp> &new_pro);

	Smart_Ptr<EquipProp> NewEquipPropStone(Smart_Ptr<EquipProp> equipProp);

	// 装备能强化，镶嵌，洗练，升级
	void IsAllEquipCanStrength(int& can, vector<bool>* canIndex = NULL);
	void IsAllEquipCanInlay(int& canInlay, int& quickUp, vector<bool>* canIndex = NULL);
	void IsAllEquipCanRefine(int& can, vector<bool>* canIndex = NULL);
	void IsAllEquipCanUpgrade(int& can, vector<bool>* canIndex = NULL);

	void initQuickSkill(int skillID);
//	void UpdateEquipUI();
	
	void syncEquip(vector<eItemPlace>& poss);

protected:

	// 当格子发生锁定
	virtual void OnLockGrid(BYTE pos, bool bLock){}

	//获得穿上或脱下一件装备的套装属性
	void getSuitAttList(bool isDrop,const Smart_Ptr<EquipProp> equipPtr,map<ECharAttrType,int64>& attList);

	//获得身上某套装件数
	int getSuitIDCounts(int suitID,int pos,bool calculate);

	//计算身上所有套装属性
	void calculateAllSuitAtt(map<ECharAttrType,int64>& attList);

private:

	vector<QuickSector> m_quick; //快捷栏数据
};



#endif /* ARTICLEEQUIPMGR_H_ */
