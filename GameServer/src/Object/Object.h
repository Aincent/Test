/*
 * Object.h
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "define.h"

using namespace CommBaseOut;

enum eObjType
{
	eObject = ( 1<<1 ),
	ePlayer = ( eObject | ( 1<<2 ) ), //���
	eNpc = ( eObject | ( 1<<3 ) ),	//NPC
	eMonster = ( eObject | ( 1<<4 ) ),	//����
	ePet = ( eObject | ( 1<<5 ) ),	//����
	eDropItem = ( eObject | ( 1<<6) ),	//掉落尸体盒
	eSpring = ( eObject | ( 1<<7) ), //温泉
	eMax
};

class Object
{
public:
	Object(eObjType type):m_type(type),m_mapid(0),m_xpos(0),m_ypos(0),m_key(0),m_sectorGroup(-1)
	{	}
	virtual ~Object()
	{	}

	void Release()
	{
		m_mapid = 0;
		m_xpos = 0;
		m_ypos = 0;
		m_key = 0;
		m_sectorGroup = -1;
	}

	void SetKey(int key) { 	m_key = key; }
	int GetKey() { 	return m_key; }
	int GetType() { 	return m_type; }

	void SetGroup(int group){m_sectorGroup = group;}

	int GetGroup() { 	return m_sectorGroup; }

	WORD GetXPos() { 	return m_xpos; }
	void SetXPos(WORD pos) { 	m_xpos = pos; }

	WORD GetYPos() { 	return m_ypos; }
	void SetYPos(WORD pos) { 	m_ypos = pos; }

	int64 GetMapID() { 	return m_mapid; }
	void SetMapID(int64 mapid) { 	m_mapid = mapid; }

private:

	int m_type; //eObjType
	int64 m_mapid; // ��ͼid
	WORD m_xpos; // x���
	WORD m_ypos; // y���
	int m_key; // ������ object Ψһ��ʶ
	int m_sectorGroup; // ���ڵľŹ����ӱ��(ֻ������һ�εľŹ���,���ͬ���󣬼������¾Ź���Ÿ�����)
};


#endif /* OBJECT_H_ */
