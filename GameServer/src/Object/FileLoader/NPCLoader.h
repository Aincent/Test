/*
 * NPCLoader.h
 *
 *  Created on: 2013��10��21��
 *      Author: helh
 */

#ifndef NPCLOADER_H_
#define NPCLOADER_H_

#include <map>
#include <vector>
#include "OtherDefine.h"
#include "define.h"


using namespace std;
using namespace CommBaseOut;

enum eNPCFunEnum
{
	eNPC_NOT_FUN =0,  //没有作用的NPC
	eNPC_FUN_SHOP =1,  //商店
	eNPC_FUN_DUP , //进入副本
	eNPC_FUN_ESORT , //截取黄冈
	eNPC_FUN_ITEMHOUSE , //仓库
	eNPC_FUN_MAX ,
};

typedef struct sNpcInfo
{
	struct uParam
	{
		vector<BYTE> signf; //关系（"<   ＝    >"）
		vector<int> param; //参数
	};

	int npcID;  // npc的标识id
	int mapid; //地图id
	int xpos; //x���
	int ypos; //y���
	int funID;  //功能 id
//	uParam fParam; //功能参数
//	int limitID[NPC_LIMIT_COUNT];  //限制id
//	uParam lParam[NPC_LIMIT_COUNT]; //限制参数
}NpcInfo;

typedef struct sSigFunInfo
{
	eNPCFunEnum fun;
	int npcID;
	sSigFunInfo():fun(eNPC_NOT_FUN),npcID(0)
	{}
}SigFunInfo;

typedef struct sNpcFunInfo
{
	int mapID; //地图ID
	vector<SigFunInfo> m_nFun; //此地图中 有功能的NPC信息
}NpcFunInfo;

class NPCInfo
{
public:
	~NPCInfo();

	static NPCInfo * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new NPCInfo();
		}

		return m_instance;
	}

	int Init(string &file);
	bool IsExistedNpc(int id);
	NpcInfo & GetNpcInfo(int id);
	const NpcInfo* GetInfo(int id);

	void GetAllMapNpc(int mapid, vector<NpcInfo> &info);

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	int ExamineDistance(int npcid, int xpos, int ypos);
	//获取 NPC功能枚举
	int GetNPCFunEnum(int npcid);
	//根据地图ID找到 相应有此功能的NPC
	int GetFunNpcIDByMapId(int mapid, eNPCFunEnum funenum);

private:
	NPCInfo();

private:
	static NPCInfo * m_instance;

	map<int, NpcInfo> m_npcInfo;   // npcID - info
	map<int , NpcFunInfo> m_mapFunNpc;  //地图有功能的 NPC
};


#endif /* NPCLOADER_H_ */
