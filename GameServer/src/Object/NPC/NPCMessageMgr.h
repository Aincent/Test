/*
 * NPCMessageMgr.h
 *
 *  Created on: 2013��10��21��
 *      Author: helh
 */

#ifndef NPCMESSAGEMGR_H_
#define NPCMESSAGEMGR_H_

#include "Singleton.h"
#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../../QuickMessage.h"
#include "../Player/Player.h"

using namespace CommBaseOut;

/*
 * ������ҵ��npc���¼�
 * �ж�����Ƿ�����npc�Ĺ�������
 * ���û������ֱ�ӻظ���Ҳ����Ƿ�ɹ�
 */

	/*通过NPC 查看 角色仓库 或 商店*/

class NPCMessageMgr :  public Singleton<NPCMessageMgr>
{
public:
	~NPCMessageMgr();
	NPCMessageMgr();

	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REG_C2GM_NPCHOUSEINFO);

};


#endif /* NPCMESSAGEMGR_H_ */
