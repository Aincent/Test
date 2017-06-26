/*
 * SettingsMgr.cpp
 *
 *  Created on: 2014年6月13日
 *      Author: root
 */

#include <cstdlib>
#include "SettingsMgr.h"
#include "../Object/Player/PlayerMessage.h"
#include "MessageStruct/ServerReturnInt.pb.h"
#include "MessageStruct/ServerReturn2Int.pb.h"
#include "MessageStruct/MSProtoc/MS_BugSubmit.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "../Map/SceneManager.h"
#include "MessageCommonRet.h"
#include "Log/Logger.h"
#include "OtherDefine.h"
#include "../ConfigLoader/MaskWordConfig.h"
#include "../Dependence/Ciconv/Ciconv.h"
#include "../MsgCommonClass.h"

SettingsMgr::SettingsMgr()
{
	DEF_MSG_QUICK_REQUEST_REG_FUN(SettingsMgr, MSG_REQ_C2GM_SETTINGS);		//修改系统设置
	DEF_MSG_QUICK_REQUEST_REG_FUN(SettingsMgr, MSG_REQ_C2GM_BUG_SUBMIT);

	DEF_MSG_ACK_REG_FUN(eGameServer, MSG_REQ_GM2DB_BUG_SUBMIT);
}

SettingsMgr::~SettingsMgr()
{

}

void SettingsMgr::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_MSG_ACK_DISPATCH_FUN(MSG_REQ_GM2DB_BUG_SUBMIT);

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SettingsMgr,MSG_REQ_C2GM_SETTINGS)
{
	LOG_DEBUG(FILEINFO, "client req settings");
//	ServerReturn::ServerRetInt req;
	PlayerInfo::SettingInfo req;
	ServerReturn::ServerRetInt ret;
	ServerReturn::ServerDoubleInt sim;
//	ServerMessage::ClientSingleMessage ack;
	vector<int> vecType;

	req.ParseFromString(content);
	if(!player)
	{
		return;
	}

	for(int i = 0; i < req.type_size(); i++)
	{
		player->SetSettings(req.value(i),req.type(i));
	}

	player->SetDataFlag(eBaseInfo);
	player->SynCharAttribute(vecType);


	ret.set_ret(player->getAttManage()->getValue<int>(eSettings));

	REQ_MSG_QUICK_ACK_FUN(message, &req, MSG_REQ_C2GM_SETTINGS);

	sim.set_retf(player->GetID());
	sim.set_rets(player->getAttManage()->getValue<int>(eSettings));

	Smart_Ptr<SvrItem> worldSvr;
	ServerConHandler::GetInstance()->GetWorldServerBySvrID(GET_SERVER_CHARID(player->GetID()),worldSvr);
	if(!worldSvr)
		return;

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_SIM_GM2WS_SETTINGS, &sim, worldSvr.Get(), SimpleMessage);
	Message_Facade::Send(messRet);
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(SettingsMgr,MSG_REQ_C2GM_BUG_SUBMIT)
{
	int error = 0;
	MS_BugSubmit::Admin_C2GMBugSubmit req;
	MS_BugSubmit::Admin_GM2DBBugSubmit toDB;
	ServerReturn::ServerRetInt ret;

	req.ParseFromString(content);

	if(!player)
	{
		return;
	}

	int type = req.type();
	if(type < eBugSubmitType_Bug || type > eBugSubmitType_Other)
	{
		error = eBugSubmitTypeError;
	}

	string title = req.title();
	int len = CIconv::GetUTF8Count(title.c_str());
	if(0 == error && title.length() <= 0)
	{
		error = eThemeNull;
	}

	if(0 == error && len > max_submit_title_len)
	{
		error = eThemeTooLong;
	}

	string text = req.text();
	len = CIconv::GetUTF8Count(text.c_str());
	if(0 == error && text.length() <= 0)
	{
		error = eTextNull;
	}

	if(0 == error && len > max_submit_text_len)
	{
		error = eTextTooLong;
	}

	string qq = req.qq();
	len = qq.length();
	if(0 == error && len > max_submit_qq_len)
	{
		error = eLinkTooLong;
	}

	for(int i = 0;i < len;++i)
	{
		if(qq[i] < '0' || qq[i] > '9')
		{
			error = eLinkError;
			break;
		}
	}

	if(0 == error)
	{
		//验证屏蔽字
		if(CMaskWordConf::GetInstance()->IsMaskWord(title) ||
				CMaskWordConf::GetInstance()->IsMaskWord(text))
		{
			error = eMaskWord;
		}
	}

	if(0 != error)
	{
		ret.set_ret(error);
		REQ_MSG_QUICK_ACK_FUN(message, &ret, MSG_REQ_C2GM_BUG_SUBMIT);
		return;
	}

	toDB.set_charid(GET_PLAYER_CHARID(player->GetID()));
	toDB.set_state(0);
	toDB.set_type(req.type());
	toDB.set_title(req.title());
	toDB.set_text(req.text());
	toDB.set_qq(req.qq());

	Smart_Ptr<SvrItem> dbSvr;
	ServerConHandler::GetInstance()->GetDBServerBySvrID(GET_SERVER_CHARID(player->GetID()),dbSvr);
	if(!dbSvr)
		return;

	Safe_Smart_Ptr<CommBaseOut::Message> messRet = build_message(MSG_REQ_GM2DB_BUG_SUBMIT, &toDB, dbSvr.Get(), Request);
	messRet->SetAct(new requestAct(message));
	Message_Facade::Send(messRet);
}

DEF_MSG_ACK_DEFINE_FUN(SettingsMgr, MSG_REQ_GM2DB_BUG_SUBMIT)
{
	//请求超时
	if(message->GetErrno() == eReqTimeOut)
	{
		return;
	}
	ServerReturn::ServerRetInt req;

	int len = 0;

	char * content = message->GetBuffer(len);
	req.ParseFromArray(content, len);

	REQ_MSG_QUICK_ACK_FUN(static_cast<requestAct *>(act.Get())->mesReq, &req, MSG_REQ_C2GM_BUG_SUBMIT);
}
