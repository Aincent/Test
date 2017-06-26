#include "FriendManage.h"
#include "util.h"
#include "Network/Message_Facade.h"
#include "PlayerMessage.h"
#include "../../Attribute/AttributeCreature.h"

FriendManage* FriendManage::m_instance = NULL;

FriendManage::FriendManage()
{
	//祝福好友加经验
	DEF_MSG_SIMPLE_REG_FUN(eGameServer, MSG_SIM_WS2GM_REQBLESSFRIEND);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FriendManage, MSG_REQ_C2GM_RECEIVE_GOURD_EXP);

	DEF_MSG_QUICK_REQUEST_REG_FUN(FriendManage, MSG_REQ_C2GM_GET_GOURD_EXP);

	m_Lua.OpenLua();

	loadLuaFile();
}

FriendManage::~FriendManage()
{
	m_Lua.CloseLua();
}

FriendManage* FriendManage::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new FriendManage();
	}

	return m_instance;
}

void FriendManage::DestroyInstance()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = 0;
	}
}

bool FriendManage::loadLuaFile()
{
	std::string tempPath;
	CUtil::GetModulePath(tempPath);
	tempPath += "Data/WorldFriend.lua";

	if(m_Lua.LoadFile(tempPath) != 0 || m_Lua.Call() != 0)
		return false;
	return true;
}

int FriendManage::getLevel()
{
	m_Lua.SetLuaFunction("getGourdReceiveLevel");
	if(m_Lua.Call(0,1) != 0)
		return 0;

	int tempLv = m_Lua.GetInt();
	return tempLv;
}

int FriendManage::getExpValueByLevel(short level)
{
	m_Lua.SetLuaFunction("GainExpCalculation");
	m_Lua.SetInt(level);
	if(m_Lua.Call(1,1) != 0)
		return 0;
	return m_Lua.GetInt();
}

int FriendManage::getMaxGourdExp()
{
	m_Lua.SetLuaFunction("getAllGourdExp");

	if(m_Lua.Call(0,1) != 0)
		return 0;
	return m_Lua.GetInt();
}

void FriendManage::Handle_Message(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN
	DEF_MSG_SIMPLE_DISPATCH_FUN(MSG_SIM_WS2GM_REQBLESSFRIEND);
	DEF_SWITCH_TRY_DISPATCH_END
}

void FriendManage::Handle_Request(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN


	DEF_SWITCH_TRY_DISPATCH_END
}

void FriendManage::Handle_Ack(Safe_Smart_Ptr<Message> &message)
{
	DEF_SWITCH_TRY_DISPATCH_BEGIN

	DEF_SWITCH_TRY_DISPATCH_END
}

DEF_MSG_SIMPLE_DEFINE_FUN(FriendManage, MSG_SIM_WS2GM_REQBLESSFRIEND)
{
	LOG_DEBUG(FILEINFO, "Player bless friend at GameServer");
	ServerReturn::ServerDoubleIntArray ret;
	Smart_Ptr<Player> player;

	char *con;
	int len = 0;
	con = message->GetBuffer(len);
	ret.ParseFromArray(con, len);
	PlayerMessage::GetInstance()->GetPlayerByCharid(ret.retf(), player);

	if(!player)
		return ;

	int tempSize = ret.rets_size();
	for(int i = 0;i < tempSize;++i)
	{
		//等级取祝福时对应的等级
		int tempExp = FriendManage::GetInstance()->getExpValueByLevel(ret.rets(i));
		if(tempExp == 0)
			continue;

		if(player->getAttManage()->getValue<int>(eGourdExp) != INVALID_GOURD_VALUE)
		{
			int tempGourdExp    = player->getAttManage()->getValue<int>(eGourdExp);
			int tempMaxGourdExp = getMaxGourdExp();
			if(tempGourdExp >= tempMaxGourdExp)
				return ;
			if (FriendManage::GetInstance()->getLevel() <= player->getAttManage()->getValue<int>(eCharLv) && 0 == tempGourdExp && 0 < tempExp)
			{
				player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_ExpCalabash, eUIHLSmall_ExpCalabash_CanGetReward), 1, true);
			}

			player->getAttManage()->addValue(eGourdExp,tempExp);
		}
		else
		{
			player->AddExp(tempExp);
		}
	}

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FriendManage,MSG_REQ_C2GM_RECEIVE_GOURD_EXP)
{
	ServerReturn::ServerRetInt tempAck;
	if(player->getAttManage()->getValue<int>(eGourdExp) == INVALID_GOURD_VALUE)
	{
		tempAck.set_ret(eReceived);
		REQ_MSG_QUICK_ACK_FUN(message,&tempAck,MSG_REQ_C2GM_RECEIVE_GOURD_EXP);

		return ;
	}

	if(getLevel() > player->getAttManage()->getValue<int>(eCharLv))
	{
		tempAck.set_ret(eLVNotEnough);
		REQ_MSG_QUICK_ACK_FUN(message,&tempAck,MSG_REQ_C2GM_RECEIVE_GOURD_EXP);

		return ;
	}

	int tempGroudExp = player->getAttManage()->getValue<int>(eGourdExp);
	player->AddExp(tempGroudExp);

	player->getAttManage()->setValue(eGourdExp,INVALID_GOURD_VALUE);

	int64 tempValue = tempGroudExp;
	tempValue = tempValue << 32 | eRetSuccess;
	tempAck.set_ret(tempValue);
	REQ_MSG_QUICK_ACK_FUN(message,&tempAck,MSG_REQ_C2GM_RECEIVE_GOURD_EXP);

	if(FriendManage::GetInstance()->getLevel() <= player->getAttManage()->getValue<int>(eCharLv) && player->getAttManage()->getValue<int>(eGourdExp) == INVALID_GOURD_VALUE)
	{
		player->UpdateUIHighLight(eUIHLHandle_UpAndSyn, UIHLType(eUIHLBig_ExpCalabash, eUIHLSmall_ExpCalabash_CanGetReward), 1, false);
	}

	return ;
}

DEF_MSG_QUICK_REQUEST_DEFINE_FUN(FriendManage,MSG_REQ_C2GM_GET_GOURD_EXP)
{
	int64 tempValue = player->getAttManage()->getValue<int>(eGourdExp);

	tempValue = tempValue << 32;
	ServerReturn::ServerRetInt tempAck;
	tempAck.set_ret(tempValue);

	REQ_MSG_QUICK_ACK_FUN(message,&tempAck,MSG_REQ_C2GM_GET_GOURD_EXP);

	return ;
}



