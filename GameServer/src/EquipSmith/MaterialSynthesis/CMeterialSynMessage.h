/*
 * CMeterialSynMessage.h
 *
 *  Created on: 2015年9月14日
 *      Author: root
 */

#ifndef CMETERIALSYNMESSAGE_H_
#define CMETERIALSYNMESSAGE_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "../../QuickMessage.h"
#include "ServerMsgDefine.h"
#include "MessageStruct/CharLogin/Smith.pb.h"

using namespace CommBaseOut;

class Player;

class CMeterialSynMessage : public Singleton<CMeterialSynMessage>,  public Message_Handler, public Request_Handler
{
public:
	CMeterialSynMessage();
	~CMeterialSynMessage();

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GT2GM_STONECREATE);
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_METERIALSYNTHESIS);

	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);

	// 宝石合成
	int CreateStone(Smart_Ptr<Player> &player, EquipSmith::StoneCreate *StoneCreate,int & stoneLv);
	int MeterialSynthesis(Smart_Ptr<Player>& player, EquipSmith::ClientReqMeterialSyn& meterialSyn);

	int GetParcelCanSynth(Smart_Ptr<Player>& player);
private:
	// 是否达到合成条件
	int IsSynthesisCon(Smart_Ptr<Player>& player, int destMeterialID, int num);
	// 材料合成
	int MeterialSynthesis(Smart_Ptr<Player>& player, int destMeterialID, int num);
};

#endif /* CMETERIALSYNMESSAGE_H_ */
