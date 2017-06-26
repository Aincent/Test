/*
 * ObjectFactory.cpp
 *
 *  Created on: 2013��10��15��
 *      Author: helh
 */

#include "ObjectFactory.h"
#include "Log/Logger.h"

ObjectFactory * ObjectFactory::m_instance = 0;

ObjectFactory::ObjectFactory():m_onlyID(1)
{
	//GUARD(CSimLock, obj, &m_idLock);

	//CreateObiect(ePlayer, ePlayerInitCount);
	//CreateObiect(eMonster, eMonsterInitCount);
	//CreateObiect(ePet, ePetInitCount);
	//CreateObiect(eDropItem, eDropItemInitCount);
	//CreateObiect(eSpring, 500);
}

ObjectFactory::~ObjectFactory()
{

}

Smart_Ptr<Object> ObjectFactory::AllocObject(eObjType eObjecType)
{
	int key = 0;

	GUARD(CSimLock, obj, &m_idLock);
	key = m_onlyID++;
	if(m_onlyID <= 0)
	{
		m_onlyID = 1;
	}

	Smart_Ptr<Object>	object = GetObject(eObjecType);
	if(object.Get() != NULL)
	{
		object->SetKey(key);
	}

	return object;
}

void ObjectFactory::Recycle(Smart_Ptr<Object>& object)
{
	GUARD(CSimLock, obj, &m_idLock);

	switch(object->GetType())
	{
		case ePlayer:
		{
			m_players.push(object);
		}
		break;

		case eMonster:
		{
			m_monsters.push(object);
		}
		break;

		case ePet:
		{
			m_girlwars.push(object);
		}
		break;

		case eDropItem:
		{
			m_items.push(object);

			//printf("\n 回收一个对象,当前拥有: %d \n", m_items.size());
		}
		break;

		case eSpring:
		{
			//m_springs.push(object);
		}
		break;

		default:
			break;
	}
}

void ObjectFactory::CreateObiect(eObjType eObjecType, int size)
{
	switch(eObjecType)
	{
		case ePlayer:
		{
			for(int i=0; i<size; ++i)
			{
				Player* pObject = new Player(ePlayer);
				Smart_Ptr<Player> obj = pObject;
				m_players.push(obj);
			}
		}
		break;

	 case eNpc:
		{
			//pObject = new NPC(eNpc);
		}
		break;

		case eMonster:
		{
			for(int i=0; i<size; ++i)
			{
				Monster* pObject = new Monster(eMonster);
				Smart_Ptr<Monster> obj = pObject;
				m_monsters.push(obj);
			}
		}
		break;

		case eDropItem:
		{
			for(int i=0; i<size; ++i)
			{
				DropItemsAllot* pObject = new DropItemsAllot(eDropItem);
				Smart_Ptr<DropItemsAllot> obj = pObject;
				m_items.push(obj);
			}
		}
		break;

		case eSpring:
		{
			//for(int i=0; i<size; ++i)
			//{
			//	Spring* pObject = new Spring(eSpring);
			//	Smart_Ptr<Spring> obj = pObject;
			//	m_springs.push(obj);
			//}
		}
		break;

		case ePet:
		{
			for(int i=0; i<size; ++i)
			{
				GirlWar* pObject = new GirlWar(ePet);
				Smart_Ptr<GirlWar> obj = pObject;
				m_girlwars.push(obj);
			}
		}
		break;

		default:
		{
			LOG_ERROR(FILEINFO, "create unknown object");
		}
	}

}

Smart_Ptr<Object>	ObjectFactory::GetObject(eObjType eObjecType)
{
	switch(eObjecType)
	{
		case ePlayer:
		{
			//if(m_players.size() == 0)
			//{
			//	CreateObiect(ePlayer, ePlayerInitCount/2);
			//printf("\n 创建%d个对象ePlayer  \n", ePlayerInitCount/2 );
			//}

			//Smart_Ptr<Object> obj = m_players.front();
			//m_players.pop();

			Smart_Ptr<Object> obj = new Player(ePlayer);

			return obj;
		}
		break;

		case eMonster:
		{
			//if(m_monsters.size() == 0)
			//{
			//	CreateObiect(eMonster, eMonsterInitCount/2);
			//printf("\n 创建%d个对象eMonster  \n", eMonsterInitCount/2 );
			//}

			//Smart_Ptr<Object> obj = m_monsters.front();
			//m_monsters.pop();

			Smart_Ptr<Object> obj = new Monster(eMonster);

			return obj;
		}
		break;

		case eDropItem:
		{
			//if(m_items.size() == 0)
			//{
			//	CreateObiect(eDropItem, eDropItemInitCount/2);
				//printf("\n 创建500个对象eDropItem  \n");
			//}

			//Smart_Ptr<Object> obj = m_items.front();
			//m_items.pop();

			Smart_Ptr<Object> obj = new DropItemsAllot(eDropItem);
			return obj;
		}
		break;

		case ePet:
		{
			//if(m_girlwars.size() == 0)
			//{
			//	CreateObiect(ePet, ePetInitCount/2);
			//}

			//Smart_Ptr<Object> obj = m_girlwars.front();
			//m_girlwars.pop();

			Smart_Ptr<Object> obj = new GirlWar(ePet);
			return obj;
		}
		break;

		case eNpc:
		{
			Smart_Ptr<Object> obj = new NPC(eNpc);
			return obj;
		}
		break;

		case eSpring:
		{
			//if(m_springs.size() == 0)
			//{
			//	CreateObiect(eSpring, 100);
			//}

			//Smart_Ptr<Object> obj = m_springs.front();
			//m_springs.pop();
			Smart_Ptr<Object> obj = new Spring(eSpring);
			return obj;
		}
		break;

		default:
		{
			return NULL;
		}
	}

	return NULL;
}





