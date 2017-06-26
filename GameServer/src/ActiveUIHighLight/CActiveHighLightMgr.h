/*
 * CActiveHighLightMgr.h
 *
 *  Created on: 2015年7月4日
 *      Author: root
 */

#ifndef CACTIVEHIGHLIGHTMGR_H_
#define CACTIVEHIGHLIGHTMGR_H_

#include "Network/MessageHandler.h"
#include "ServerMsgDefine.h"
#include "CharDefine.h"
#include "EventSet/EventArgs.h"
#include "../EventSet/EventDefine.h"
#include "UIHLDefine.h"
#include "MessageStruct/SystemNotice/SystemNotice.pb.h"

using namespace std;
using namespace CommBaseOut;

class Player;

struct HintValue
{

	HintValue() : fValue(0), sValue(0) { }
	HintValue(int __f, int __s) : fValue(__f), sValue(__s) { }
	HintValue(const int& __value) : fValue(__value), sValue(0) { }

	bool operator==(const HintValue& obj);
	bool operator!=(const HintValue& obj);
	bool operator<=(const HintValue& obj);
	bool operator>=(const HintValue& obj);
	bool operator>(const HintValue& obj);
	bool operator<(const HintValue& obj);
	bool operator==(const int& value);
	bool operator!=(const int& value);
	bool operator>(const int& value);
	bool operator<(const int& value);
	bool operator<=(const int& value);
	bool operator>=(const int& value);

	HintValue& operator-(const HintValue& obj);
	HintValue& operator+(const HintValue& obj);
	HintValue& operator=(const HintValue& obj);
	HintValue& operator+=(const HintValue& obj);
	HintValue& operator-=(const HintValue& obj);
	HintValue& operator=(const int& value);
	HintValue& operator+=(const int& value);
	HintValue& operator-=(const int& value);

	int fValue;
	int sValue;
};



typedef map<E_UIHL_BIGTYPE, map<E_UIHL_SMALLTYPE, HintValue> > UIHLContainer;

class CHighLightMgr
{
public:
	CHighLightMgr(Player* player);

	~CHighLightMgr();

	void ReInit();
	void Release();

	// 初始化
	// 玩家通用接口  初始化数据
	void ComTypeAddData();
	void ComTypeAddData1();
	void ComTypeAddData2();

	// 玩家通用接口  重置数据 玩家升级后 and  过了24点后 调用
	void ComTypeResetData();
	void ComTypeResetData1();
	void ComTypeResetData2();

	//
	void AddUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value);
	void AddUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int value);
//	void AddUIHLData(UIHLType type, int count);

	void ClearUIHLData();

	// 删除角色ID中某个类型
	void DeleteUIHLData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType);
	void DeleteUIHLData(E_UIHL_BIGTYPE bigType);

	// 同步所有 符合条件的 UI高亮显示
	int SynchUIHighLight();
	// 同步某UIHL类型
	int SynchUIHighLight(E_UIHL_BIGTYPE bigType);
	int SynchUIHighLight(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType);

	// 更新某活动高亮数据
	int UpdateDataAndSyn(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value, bool flag = false);

	 // 直接修改值 不同步的
	int ModifyData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value);
	int ModifyData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int value);
	int UpdateData(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue value, bool flag = false);

	void GetValueByType(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, HintValue& value);
	void GetValueByType(E_UIHL_BIGTYPE bigType, E_UIHL_SMALLTYPE smallType, int& value);


	void ActivityHLTypeAdd();

	bool HandleDataEvent(const EventArgs& e);
	bool HandlePlayerVipUp(const EventArgs& e);
	bool HandleMoneyChange(const EventArgs& e);
	bool HandlePackageGoods(const EventArgs& e);
	bool HandleForceChange(const EventArgs& e);
	bool HandleWearEquip(const EventArgs& e);
	bool HandleDropEquip(const EventArgs& e);
	bool HandleStrengthEquip(const EventArgs& e);
	bool HandleInlayEquip(const EventArgs& e);
	bool HandleRefineEquip(const EventArgs& e);
	bool HandleUpgradeEquip(const EventArgs& e);
	bool HandleRemoveStone(const EventArgs& e);
	bool HandleSigniIn(const EventArgs& e);
	bool HandleSubmitTask(const EventArgs& e);
	bool HandleEnterInst(const EventArgs& e);
	bool HandleResetProtectKing(const EventArgs& e);
	bool HandleMoneyInstBuyCnt(const EventArgs& e);
	bool HandleEscortStart(const EventArgs& e);
	bool HandlePassInst(const EventArgs& e);
	bool HandleOnekeyQuest(const EventArgs& e);
	bool HandleGetMoneyShop(const EventArgs& e);
	bool HandleResetAllInst(const EventArgs& e);
	bool HandleStartSweep(const EventArgs& e);
	bool HandleAddArenaChall(const EventArgs& e);
	bool HandlePassTarget(const EventArgs& e);
	bool HandleDrawTarget(const EventArgs& e);
	bool HandleSkillUp(const EventArgs& e);
	bool HandleGetSignAward(const EventArgs& e);
	bool HandleGetOnlineAward(const EventArgs& e);
	bool HandleGetLvAward(const EventArgs& e);
	bool HandleGetSevenLogin(const EventArgs& e);

	static vector<UIHLType> GetHLTypeByActivity(ActivityExType activityType);
protected:
	void HandleData(E_UIHL_HANDLE handle, E_UIHL_BIGTYPE bigType = eUIHLBig_Null,
			E_UIHL_SMALLTYPE smallType = eUIHLSmall_Null, int fValue = 0, bool flag = false, int sValue = 0);
	// 某状态改变公共调用
	void ChangeComCall();

	// 特例 装备穿 脱 事件都要调用 锻造 特例
	// 返回是否要同步
	bool EquipSmithUIHL(SystemNoticeProto::ToClientUIHL* toC = NULL);
	void SendEquipSmithHLToC(SystemNoticeProto::ToClientUIHL toClient);


	void UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type = UIHLType(eUIHLBig_Null, eUIHLSmall_Null),
			int count = 0,  bool flag = true, int value = 0);
	void UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type = UIHLType(eUIHLBig_Null, eUIHLSmall_Null),
				HintValue value = HintValue(), bool flag = true);
	void UpdateUIHighLight(vector<E_UIHL_HANDLE> handleTypes, vector<E_UIHL_BIGTYPE> bigTypes, vector<E_UIHL_SMALLTYPE> smallTypes,
				vector<int> counts, vector<bool> flags, vector<int> values = vector<int>());


private:
	UIHLContainer m_data;

	Player* m_owner;
//	EventConnection m_lvEvent;

	EventConnection m_handleDataEvent;
	EventConnection m_vipEvent;
	EventConnection m_moneyChangeEvent;
	EventConnection m_packageChangeEvent;
	EventConnection m_wearEquipEvent;
	EventConnection m_dropEquipEvent;
	EventConnection m_strengthEquipEvent;
	EventConnection m_inlayEquipEvent;
	EventConnection m_refineEquipEvent;
	EventConnection m_upgradeEquipEvent;
	EventConnection m_forceChangeEvent;
	EventConnection m_removeStoneEvent;

	EventConnection m_signInEvent;
	EventConnection m_submitTaskEvent;
	EventConnection m_enterInstEvent;
	EventConnection m_resetProtectKing;
	EventConnection m_moneyInstBuyCntEvent;
	EventConnection m_escortStartEvent;
	EventConnection m_passInstEvent;
	EventConnection m_onekeyQuest;

	EventConnection m_getMoneyShopEvent;
	EventConnection m_resetAllInstEvent;
	EventConnection m_startSweepEvent;
	EventConnection m_addArenaChallEvent;
	EventConnection m_passTargetEvent;
	EventConnection m_drawTargetReward;

	EventConnection m_skillUpEvent;

	EventConnection m_getSigninAward;
	EventConnection m_getOnlineAward;
	EventConnection m_getLevelAward;
	EventConnection m_getSevenLoginAward;
};

#endif /* CACTIVEHIGHLIGHTMGR_H_ */
