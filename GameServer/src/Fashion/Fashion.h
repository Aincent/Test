/*
 * Fashion.h
 *
 *  Created on: 2016年4月7日
 *      Author: root
 */

#ifndef FASHION_H_
#define FASHION_H_

#include "Common.h"
#include "define.h"
#include "Fashion/FashionMessage.pb.h"
#include "FashionLoader/FashionLoader.h"

using namespace std;
using namespace CommBaseOut;

struct Fashion
{
	enum E_FASHION_STATE
	{
		eFashion_Default = 0,
		eFashion_Drop = 0,
		eFashion_Wear,
		eFashion_Display = eFashion_Wear, // 显示/隐藏 隐藏：只是player的m_fashionID 为0
		eFashion_Hide,
	};
	int ID; // 时装ID
	int strengthLv; // 强化Lv
	E_FASHION_STATE state;

//	enum E_FASHION_DISPLAY
//	{
//		eFashionDisplay_Show = 0,
//		eFashionDisplay_Hide,
//	};
};
class Player;
class FashionShow
{
public:
	FashionShow(Player* player);
	~FashionShow();

	void ReInit();
	void Release();

	void InitWardrobe(const FashionMsgProto::FashionShow& wardrobe);
	void SaveWardrobe(FashionMsgProto::FashionShow& wardrobe) const;

	// 时装秀
	void Show(FashionMsgProto::RetFashionInfo& show) const;
	void Show(const int& fashionID, FashionMsgProto::FashionInfo& show) const;

	// 激活
	int Activate(const int& fashionID);
	int Activate(const int& fashionID, FashionMsgProto::FashionInfo& show);
	// 装扮
	int Dressup(const int& fashionID, Fashion::E_FASHION_STATE is);
	int Dressup(const int& fashionID, Fashion::E_FASHION_STATE is, FashionMsgProto::FashionInfo& show);
	// 强化
	int Strength(const int& fashionID);
	int Strength(const int& fashionID, FashionMsgProto::FashionInfo& show);

	// 显示或隐藏时装  仅仅是模型ID为0
	int Display(const int& fashionID, Fashion::E_FASHION_STATE is);

	// 上线属性计算
	void OnlineCalAttr();
	// 计算战斗力
	void CalFighting();
	//获得当前时装所增加的属性
	void GetCurAdditionalAttr(map<ECharAttrType,int64>& attr);

protected:

	// 计算战斗力
	void CalFighting(map<ECharAttrType,int64>& attr);
	// 激活中 消耗与检查
	int Activating(const int& fashionID);
	// 强化中 消耗与检查
	int Strengthening(const int& fashionID);
	// 装扮中
	int Dressing(const int& fashionID, Fashion::E_FASHION_STATE is);
	// 显示/隐藏时装
	int Displaying(const int& fashionID, Fashion::E_FASHION_STATE is);

	// 消耗
	void Consume(const FashionInfo::Conditions& cond, map<int, bool>& isBind);
	// 检查消耗
	int ConsumeCheck(const FashionInfo::Conditions& cond, map<int, bool>& isBind) const;


	//------------------------------------------------------------//

	// 激活
	void Activated(const int& fashionID);
	// 穿戴
	void Wore(const int& fashionID);
	// 脱下
	void Dropped(const int& fashionID);
	// 强化
	void Strengthened(const int& fashionID);
	// 显示/隐藏
	void Display(const int& fashionID);
	void Hide(const int& fashionID);

private:

	// 广播装扮
	void BroadDress();
	// 产生效果
	void ActivateEffect(const int& fashionID);
	void StrengthenEffect(const int& fashionID);
	void DressEffect(const int& fashionID, Fashion::E_FASHION_STATE is);

	// 获得已穿戴时装
	int GetWoreID();

private:

	typedef map<int, Fashion> Wardrobe;
	Wardrobe m_wardrobe; // 衣橱
//	int m_woreFashion; // 已穿着的时装

	Player* m_owner;
};

#endif /* FASHION_H_ */
