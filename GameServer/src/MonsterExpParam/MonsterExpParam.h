/*
 * MonsterExpParam.h
 *
 *  Created on: 2015年12月29日
 *      Author: root
 */

#ifndef MONSTEREXPPARAM_H_
#define MONSTEREXPPARAM_H_

#include "Singleton.h"
#include "define.h"
using namespace std;
using namespace CommBaseOut;

struct MonsterExpData
{
	MonsterExpData()
	{
		minlv = 0;
		maxlv = 0;
		expRate = 0;
	}

	int minlv;			//等级差下限
	int maxlv;			//等级差上限
	float expRate; 	//死亡时的经验比例
};

class MonsterExpParam : public Singleton<MonsterExpParam>
{
public:
	MonsterExpParam();
	~MonsterExpParam();

public:
	int GetExp(int monsterlv, int playerlv, int monsterExp);

private:
	void LoadFile();

private:
	std::vector<MonsterExpData> m_param_data;
};



#endif /* MONSTEREXPPARAM_H_ */
