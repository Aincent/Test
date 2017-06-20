
#ifndef RANKINGMANAGER_H_
#define RANKINGMANAGER_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"

using namespace CommBaseOut;
class RankingMgr : public Message_Handler, public Request_Handler, public Ack_Handler
{
public:
	~RankingMgr();

	static RankingMgr *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new RankingMgr();
		}

		return m_instance;
	}

	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_RANKING);
	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_MERANKING);
//	DEF_MSG_REQUEST_DECLARE_FUN_H(MSG_REQ_GM2GT_GLORY);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_RANKING);
	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_MERANKING);
//	DEF_MSG_ACK_DECLARE_FUN_H(MSG_REQ_GT2WS_GLORY);
	virtual void Handle_Message(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Request(Safe_Smart_Ptr<Message> &message);
	virtual void Handle_Ack(Safe_Smart_Ptr<Message> &message);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

private:
	RankingMgr();

private:
	static RankingMgr * m_instance;

};


#endif /* QUESTMANAGER_H_ */
