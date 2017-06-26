/*
 * FieldSpring.h
 *
 *  Created on: 2014年10月24日
 *      Author: helh
 */

#ifndef FIELDSPRING_H_
#define FIELDSPRING_H_



#include <map>
#include <string>
#include <vector>

using namespace std;

typedef struct sMapSpring
{
	int hprate; //hp血量比率
	int range; //加血范围
	int xpos;
	int ypos;
}MapSpring;

class FieldSpring
{
public:
	FieldSpring(string &path, int id);
	~FieldSpring();

	int Init();

	vector<MapSpring> & GetSpring()
	{
		return m_vecSpring;
	}

private:

	string m_file;
	int m_mapID;

	vector<MapSpring> m_vecSpring;
};





#endif /* FIELDSPRING_H_ */
