/*
 * ExtraAtributeLoader.h
 *
 *  Created on: 2015年4月16日
 *      Author: root
 */

#ifndef EXTRAATRIBUTELOADER_H_
#define EXTRAATRIBUTELOADER_H_

#include "define.h"
#include <map>
#include <vector>
#include "Smart_Ptr.h"
#include "Singleton.h"

using namespace CommBaseOut;
using namespace std;

enum ExtraAtributeType
{
	ExtraAtributeType_Strength = 1,//强化
	ExtraAtributeType_Stone = 2,//宝石
	ExtraAtributeType_Refine = 3,//洗练
	ExtraAtributeType_EquipLv = 4,//装备等级
	ExtraAtributeType_Max,
};

struct ExtraAtribute
{
	DWORD extraAtriLv;	//档数
	DWORD type;			//类型
	DWORD num;			//数量
	DWORD basehp;		//基础生命
	DWORD baphyackmax;	//基础物攻max
	DWORD baphydefmax;	//基础物防max
	DWORD m_Dodge;		//闪避
	DWORD m_Hit;		//命中
	DWORD m_Crit;		//暴击
	DWORD m_ResistCrit; //抗暴

	ExtraAtribute():extraAtriLv(0),type(0),num(0),basehp(0),baphyackmax(0),baphydefmax(0),m_Dodge(0),m_Hit(0),
			m_Crit(0),m_ResistCrit(0)
	{

	}
};

class ExtraAtributeLoader : public Singleton<ExtraAtributeLoader>
{
public:
	ExtraAtributeLoader();
	~ExtraAtributeLoader();
public:
	int InitExtraAtribute(string &file);

	void GetExtraAtribute(DWORD type,DWORD extraLv,ExtraAtribute &extraAtributeInfo);

	void GetExtraAtributeList(DWORD type,std::map<DWORD,ExtraAtribute> &infoList);
private:
	//保存各种类型的档数
	std::map<DWORD,std::map<DWORD,ExtraAtribute> > m_extraAtributeMap;
};


#endif /* EXTRAATRIBUTELOADER_H_ */
