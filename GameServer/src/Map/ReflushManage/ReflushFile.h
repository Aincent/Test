#ifndef REFLUSHFILE_H_
#define REFLUSHFILE_H_

#include "define.h"
#include "../ConfigLoader/FieldMonster.h"
#include "Singleton.h"
#include "FileLoader/FestivalUIFile.h"

using namespace std;
using namespace CommBaseOut;

struct ReflushOne
{
	ReflushOne() : m_MonsterID(0),m_Counts(0){}
	int   		m_MonsterID;
	vector<Pos> m_PosList;
	short 		m_Counts;
};

struct ReflushData
{
	ReflushData() : m_MapID(0),m_IntervalTime(0){}
	int 			   m_MapID;
	vector<ReflushOne> m_OneList;
	int				   m_IntervalTime;
	DateFormat		   m_StartDate;
	DateFormat		   m_EndData;
};

class ReflushFile : public Singleton<ReflushFile>
{
public:
	ReflushFile();

	~ReflushFile();

public:
	int init(string& str);

	const vector<ReflushData>* getFileData(int mapID);

	bool isInTime(const DateFormat& beginTime,const DateFormat& endTime);

public:
	map<int,vector<ReflushData> > m_FlieList;
};

#endif 
