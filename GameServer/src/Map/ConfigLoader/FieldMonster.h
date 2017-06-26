/*
 * FieldMonster.h
 *
 *  Created on: 2013��10��22��
 *      Author: helh
 */

#ifndef FIELDMONSTER_H_
#define FIELDMONSTER_H_


#include <map>
#include <string>
#include <vector>

using namespace std;

typedef struct sPos
{
	int xpos;
	int ypos;
}Pos;

enum FieldMonsterType
{
	eRandomWayPos=0,//随机寻路坐标点
	ePointWayPos,//定点寻路坐标点(固定路径)
	eSingleWayPos,//定点寻路坐标点(固定路径),只走一回，不会来回巡逻
};

typedef struct sMapMonster
{
	int id; // ����id
	int count; // ����ˢ����
	FieldMonsterType type;
	int flushtime;	//刷新时间
	vector<Pos> randPos; // �����������
}MapMonster;

class FieldMonster
{
public:
	FieldMonster(string &path, int id);
	~FieldMonster();

	int Init();

	map<int, MapMonster> & GetMapMonster()
	{
		return m_mapMonster;
	}

private:

	string m_file;
	int m_mapID;

	map<int, MapMonster> m_mapMonster; //monsterid - monsterinfo
};



#endif /* FIELDMONSTER_H_ */
