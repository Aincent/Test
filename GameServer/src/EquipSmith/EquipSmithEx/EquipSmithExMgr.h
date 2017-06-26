/*
 * EquipSmithExMgr.h
 *
 *  Created on: 2016年9月10日
 *      Author: root
 */

#ifndef EQUIPSMITHEXMGR_H_
#define EQUIPSMITHEXMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "../../QuickMessage.h"
#include "ServerMsgDefine.h"
#include "MessageStruct/CharLogin/Smith.pb.h"

using namespace CommBaseOut;

class Player;
class EquipSmithExMgr : public Singleton<EquipSmithExMgr>, public Message_Handler, public Request_Handler
{
public:
	EquipSmithExMgr();
	~EquipSmithExMgr();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_INALYSTONEINFO); // 宝石镶嵌的所有宝石信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_UPGRADEEQUIP);		//升级装备
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_INLAYQUICKSYNTH);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_STONEDAADEL);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_REFINE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_AFFIRMREFINE);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_STRENGTHEQUIP);


	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	// 快速升级宝石
	int InlayQuickUpStone(Smart_Ptr<Player>& player, EquipSmith::ClientReqUpgradeStone req);
	//强化
	int StrengthEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqStrengthEquip content, int& equipUpgrade, int& surplusProtect);
	//镶嵌和摘除宝石 sPos 宝石位置  ePos装备 位置  带出 宝石ID 用于  出发 镶嵌事件所需
	int64 AddDeleteStoneFromEquip(Smart_Ptr<Player> &player, EquipSmith::AddDelStone req, int* stoneID);
	//装备洗练
	int RefineEquip(Smart_Ptr<Player>& player, EquipSmith::ClientReqRefineEquip refineInfo, EquipSmith::ReturnNewAttr& ret);
	//替换洗练属性
	int AffirmRefine(Smart_Ptr<Player> &player, EquipSmith::ClintReqAffirmRefine content);
	//升级
	int UpgradeEquip(Smart_Ptr<Player> &player, EquipSmith::ClientReqUpgradeEquip req);
private:
	// 强化检查
	int StrengthEquipCheck(Smart_Ptr<Player> &player, int ePos, bool bmeterial ,bool bGolden);
	// 镶嵌检查
	int AddDeleteStoneCheck(Smart_Ptr<Player>& player, bool type, int equipPos, int stonePos,int inlayPos);
	// 洗练检查
	int RefineEquipCheck(Smart_Ptr<Player>& player, int pos, vector<int>& topLevelPos, bool isMustTopLv, bool isAutoBuy);
	// 替换检查
	int AffirmRefineCheck(Smart_Ptr<Player>& player, int pos, vector<int> lockPos, bool isAutoBuy);

	int InlayQuickUpCheck(Smart_Ptr<Player>& player, int curStoneID, int curStonePos, int equipPos);

	// 升级检查
	int UpgradeEquipCheck(Smart_Ptr<Player>& player, int ePos, bool bGolden, bool isProtect);

};

#endif /* EQUIPSMITHEXMGR_H_ */
