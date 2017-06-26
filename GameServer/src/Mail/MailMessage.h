/*
 * MailMessage.h
 *
 *  Created on: 2014年6月4日
 *      Author: root
 */

#ifndef MAILMESSAGE_H_
#define MAILMESSAGE_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include <map>
#include "../QuickMessage.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "../StatisticMgr/StatisticMgr.h"
#include "StatisticBase/StatisticDefine.h"
#include "Singleton.h"
#include "MessageStruct/CharLogin/Mail.pb.h"
#include "PVPDefine.h"

using namespace std;
using namespace CommBaseOut;

//发送邮件的保存数据
//class SendMailDataAct : public NullBase
//{
//public:
//		SendMailDataAct(Safe_Smart_Ptr<CommBaseOut::Message> &message,const MailInfo::SendMail &req,const std::string& name):
//			mesReq(message),m_req(req),m_name(name)
//		{}
//		Safe_Smart_Ptr<CommBaseOut::Message> mesReq;
//		MailInfo::SendMail m_req;
//		std::string m_name;
//};


class MailMessageMgr:public Singleton<MailMessageMgr>, public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	MailMessageMgr();
public:
	~MailMessageMgr();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_MAILS);		//请求邮件基础信息列表
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_MAILINFO);		//请求邮件详细信息
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_DELETEMAIL);		//请求删除邮件
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GM_GETMAILGOODS);		//请求提取邮件物品

	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_MAILS);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_MAILINFO);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_DELETEMAIL);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GM2DB_GETMAILGOODS);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	//系统邮件 goods低32位 物品ID    高32数量 尽量用这个接口
	// goods可以接受多个,不用担心超过邮件所能存的物品   接口有做处理
	void SendSysMail(vector<int64> &goods, string &name, int64 charID=0,string substance = "", string theme = "系统邮件",	int money = 0, int exp = 0, int force = 0, int bindGolden = 0 );

	void SendMailByCharId(int64 charid,string substance = "", string theme = "系统邮件", MailInfo::SpecialMailItemList * specialItemInfo=NULL);

	void SendMail(int64 charid, const vector<PropItem>& items, string substance = "", string theme = "系统邮件" );

};


#endif /* MAILMESSAGE_H_ */
