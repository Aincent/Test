/*
 * ObjectFactory.h
 *
 *  Created on: 2013��10��15��
 *      Author: helh
 */

#ifndef OBJECTFACTORY_H_
#define OBJECTFACTORY_H_

#include "Object.h"
#include "define.h"
#include "Smart_Ptr.h"
#include "Common.h"
#include "./Player/Player.h"
#include "./NPC/NPC.h"
#include "./Monster/Monster.h"
#include "./Spring/Spring.h"
#include "./ItemDrop/DropItemAlloc.h"
#include "./GirlWar/GirlWar.h"
using namespace CommBaseOut;
using namespace std;

class ObjectFactory
{
public:
	~ObjectFactory();

	static ObjectFactory * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new ObjectFactory();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	enum
	{
		ePlayerInitCount = 500,
		eMonsterInitCount = 20000,
		ePetInitCount = 2000,
		eDropItemInitCount = 1000,
	};

	//得到一个对象
	Smart_Ptr<Object> AllocObject(eObjType eObjecType);

	//回收一个对象
	void Recycle(Smart_Ptr<Object>& object);

private:
	ObjectFactory();

	void CreateObiect(eObjType eObjecType, int size=1);
	Smart_Ptr<Object>	GetObject(eObjType eObjecType);

private:
	static ObjectFactory * m_instance;
	int m_onlyID;
	CSimLock m_idLock;

	std::queue<Smart_Ptr<Object> > m_players;
	std::queue<Smart_Ptr<Object> > m_monsters;
	std::queue<Smart_Ptr<Object> > m_girlwars;
	std::queue<Smart_Ptr<Object> > m_items;
	std::queue<Smart_Ptr<Object> > m_springs;
};


#endif /* OBJECTFACTORY_H_ */
