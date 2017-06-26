#ifndef GODTREEFILE_H_ 
#define GODTREEFILE_H_

#include <vector>
#include <map>
#include "define.h"
#include "Singleton.h"


using namespace std;
using namespace CommBaseOut;

struct GodCollectReward
{
	GodCollectReward() : m_ItemID(0),m_Counts(0),m_Bind(0){}
	int m_ItemID;
	int m_Counts;
	int m_Bind;
};

struct GodFallReward
{
	GodFallReward() : m_EveryCounts(0),m_Range(0){}
	GodCollectReward m_Item;
	int m_EveryCounts;
	int m_Range;
};

struct CollectData
{
	CollectData() : m_Condition(0){}
	int m_Condition;
	GodCollectReward m_CollectReward;
};

struct GodTreeData
{
	GodTreeData() : m_Level(0),m_Exp(0),m_Range(0),m_NPCID(0){}
	int m_Level;
	int m_Exp;
	map<int,CollectData> m_CollectItems;
	vector<GodFallReward> m_FallReward;
	int m_Range;
	int m_NPCID;
};

struct GodTreeStaticData
{
	GodTreeStaticData() : m_AddExp(0),m_CountdownTime(0),m_InternationalID(0),m_PromptTextID(0),m_PromptTextValue(0){}
	int m_AddExp;
	int m_CountdownTime;
	int m_InternationalID;
	int m_PromptTextID;
	int m_PromptTextValue;
};

class GodTreeFile : public Singleton<GodTreeFile> 
{
public:
	GodTreeFile();

	~GodTreeFile();

public:
	int init(const string& direct);

	int initFile(const string& file);

	int initStatic(const string& file);

	const GodTreeData* getData(int level);

	const map<int,GodTreeData>& getDatas(){return m_Datas;}

	void getLevelAndCurExp(int allExp,int& level,int& exp); 

	void getCanBuckle(map<int,int>& canBuckle,const map<int,int>& haveBuckle,int level);

	void getSubmitReward(int level,map<int,int>& submits,vector<DWORD>& items,vector<DWORD>& nums,vector<WORD>& flags); 

	const GodTreeStaticData& getStaticData(){return m_StaticData;}

	const map<int,CollectData>* getCollectData(int level);	
	
private:
	map<int,GodTreeData> m_Datas;
	GodTreeStaticData	 m_StaticData;
};
#endif
