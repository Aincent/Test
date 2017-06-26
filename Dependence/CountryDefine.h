/*
 * CountryDefine.h
 *
 *  Created on: 2015年3月18日
 *      Author: root
 */

#ifndef COUNTRYDEFINE_H_
#define COUNTRYDEFINE_H_
#include <list>
#include <vector>
#include "Common.h"
#include "define.h"
#include "Timer/TimerInterface.h"
using namespace CommBaseOut;

#define notice_max_size 150
#define CAMP_LEVEL_MAX 10		//陈营最大等级

enum eCountryID
{
	eCountryID_null = 0,
	eCountryID_sui  = 1,
	eCountryID_tang = 2,
	eCountryID_max,
};

enum eCountryJob
{
	eCountryJob_null = 0,					//平民
	eCountryJob_leader = 1,				//霸主
	eCountryJob_counsellor = 2,	//军师
	eCountryJob_tongling = 3,   //统领
	eCountryJob_jiangjun = 4,   //将军
	eCountryJob_jinwei = 5,			 //禁卫
};

enum eCountryAttrType
{
	eCountryAttrType_contryid,
	eCountryAttrType_online,
	eCountryAttrType_level,
	eCountryAttrType_vip,
	eCountryAttrType_contribute,
	eCountryAttrType_power,
};

enum eCountryWarAttrID
{
	eCountryWarAttrID_sui=1,//隋营积分
	eCountryWarAttrID_tang,//唐营积分
	eCountryWarAttrID_contribute,//我的贡献度
	eCountryWarAttrID_sort,//我的排名
	eCountryWarAttrID_bekill,	//被杀次数
};

//捐献的物品信息
struct CountryItemInfo
{
    CountryItemInfo()
	{
	index=0;
	propID=0;
	count=0;
	}
   int index;
   int propID;
   int count;
};

//势力信息
struct CountryInfo
{
		CountryInfo()
		{
    	ID=0;
    	Level=1;
    	leaderID=0;
    	CounsellorID=0;
    	Money=0;
    	nums=0;
		win=0;
		lose=0;
		}
    unsigned char ID;
    unsigned char Level;
    int64 leaderID;
    int64 CounsellorID;
    int64 Money;
    int nums;
	int win;
	int lose;
    std::string leaderName;
    std::string CounsellorName;
    std::string notice;
	 std::vector<CountryItemInfo> itemInfo;
  };

	//陈营基本配置信息
	struct CountryBaseConfig
	{
			CountryBaseConfig(){balanceNum=0;jionLevel=0;moneyID=0;goldenID=0;goldenRate=0;}
			int balanceNum;//基础平衡人数限制
			int jionLevel;//加入陈营等级
			int moneyID;	 //铜钱id
			int goldenID; //元宝id
			int goldenRate;//元宝捐献倍率
	};

	//阵营boss时间
	struct CountryBossTime
	{
		BYTE weekDay;
		BYTE startHour;
		BYTE startMinutes;
		BYTE endHour;
		BYTE endMinutes;
		CountryBossTime():weekDay(0),startHour(0),startMinutes(0),endHour(0),endMinutes(0)
		{

		}
	};

	//
struct CountryPlayerData
{
		CountryPlayerData()
		{
				id = 0;
				lv = 0;
				contribute = 0;
				contributeWeek = 0;
				power = 0;
				profession = 0;
				job = 0;
				vip = 0;
				online = 0;
		}
		int64 id;//角色id
		short lv;//角色等级
		int contribute;//总贡献度
		int contributeWeek;//周贡献度
		int power;//战力
		BYTE profession;//职业
		BYTE job;//官职
		BYTE vip;//vip等级
		BYTE online;//是否在线
		std::string name;//角色名称
};

//陈营等级信息
struct CampGradeInfo
{
		CampGradeInfo()
		{
				lv = 1;
				money = 0;
				HP = 0;
				attack = 0;
				defense = 0;
				magicAttack = 0;
				magicDefense = 0;
				power = 0;
		}
		//阵营等级	铜钱	天机令	地煞令	玄真令	黄宇令	生命	物攻	法攻	物防	法防	战斗力
		int lv;
		int64 money;
		std::vector<std::pair<int,int> > props;
		int HP;
		int attack;
		int magicAttack;
		int defense;
		int magicDefense;
		int power;
};

//陈营官职奖励
struct CampJobPrize
{
	CampJobPrize()
	{
		jobID = 0;
		jobNums = 0;
		attriRate = 0;
		propID = 0;
	}
	unsigned char jobID;//官职ID
	unsigned char jobNums;//官职人数
	float attriRate;//属性加成
	int propID;//礼包id
};

//陈营商店信息
struct CampShopInfo
{
		CampShopInfo()
		{
				index = 0;
				propId = 0;
				nums = 0;
				contribute = 0;
				campLv = 0;
				nBindType = 0;
		}
		int index;
		int propId;
		int nums;
		int contribute;
		BYTE campLv;
		int nBindType;//绑定类型
};

//阵营胜负记录
struct CountryWarRecord 
{
	CountryWarRecord()
	{
		Win=0;
		Lose=0;
	}
	int Win;
	int Lose;
};

struct CountryWarSortData
{
	CountryWarSortData(int64 id,int con)
	{
		charID=id;
		contribute=con;
	}
	int64 charID;
	int contribute;
};

//阵营战数据
struct CountryWarData
{
	CountryWarData()
	{
		score_sui=0;
		score_tang=0;
		playerNum_sui=0;
		playerNum_tang=0;
	}
	int score_sui;	//隋营积分
	int score_tang;//唐营积分
	int playerNum_sui;//隋营人数
	int playerNum_tang;//唐营人数
};

//玩家占领五行柱参数
struct OccupyData
{
	OccupyData()
	{
		charID=0;
		objectKey=0;
	}
	int64 charID;//角色
	int64 objectKey;//五行柱key
};
struct OccupyDataEx : public OccupyData
{
	OccupyDataEx(){m_pMemory=NULL;}
	TimerConnection m_timer;
	OccupyData* m_pMemory;
};


//阵营战五行柱信息
struct CountryWarElementInfo
{
	CountryWarElementInfo()
	{
		objectKey=0;
		npcID=0;
		owner=0;
		time=0;
		state=true;
	}
	int objectKey;	//唯一id
	int npcID;				//npcID
	int owner;				//占领者陈营
	int64 time;				//当前开启的保护时间起点
	bool state;			//当前是否可采集
	TimerConnection m_protectedtimer;//保护时间定时器执行一次
	TimerConnection m_prizetimer;//积分奖励定时器(循环)
	std::vector<int64> m_occupys;//正在占领该对象的玩家
};

//阵营任务类型枚举
enum CountryWarTask_Type
{
	eCountryWarTask_kill=1,//杀人
	eCountryWarTask_monster,//杀怪
	eCountryWarTask_occup,//占领
	eCountryWarTask_subTagert,//子任务完成目标
};

//阵营战怪物信息
struct CountryWarMonsterConfig
{
	CountryWarMonsterConfig()
	{
		score=0;
	}
	int score;
};

//阵营战杂项配置信息
struct CountryWarConfig
{
	CountryWarConfig()
	{
		killScore=0;
		occupyScore=0;
		protectime=0;
		addscoreTime=0;
		addscore=0;
		scoreRate=0;
		firstPrize=0;
		secondPrize=0;
		thirdPrize=0;
		firstPrizeNum=0;
		secondPrizeNum=0;
		thirdPrizeNum=0;
		winTimesParam1=0;
		winTimesParam2=0;
		zonePrizeTime=0;
		prizeTime=0;
		bornthID=0;
		safeZonePrizeRate=0;
		noSafeZonePrizeRate=0;
		occupyAllNoticeID=0;
		continueWinNoticeID=0;
	}
	int killScore;//斩杀敌人积分
	int occupyScore;//占领积分
	int protectime;//占领后的保护时间
	int addscoreTime;//五行柱每隔多长时间增加一次积分
	int addscore;//五行柱每隔一定时间增加的积分
	int scoreRate;//全部占领的积分加成倍数

	int firstPrize;//第一名奖励
	int firstPrizeNum;//第一名奖励物品数量
	int secondPrize;//第二名奖励
	int secondPrizeNum;//第二名奖励物品数量
	int thirdPrize;//第三名奖励
	int thirdPrizeNum;//第三名奖励物品数量
	int winTimesParam1;//胜利达到多少场的整数倍奖励加成
	int winTimesParam2;//胜利达到多少场的整数倍奖励加成参数

	int occupyAllNoticeID;//全部占领五行柱公告id
	int continueWinNoticeID;//胜利多少场公告id

	int zonePrizeTime;//地图区域奖励时间
	int bornthID;				//出生复活点id
	int safeZonePrizeRate;	//安全区奖励倍数参数
	int noSafeZonePrizeRate;//非安全区奖励倍数参数

	int prizeTime;//每隔多长时间加一次阵营积分和玩家贡献度

	std::vector<std::pair<int,int> > countryWinprops;//阵营胜利奖励物品
	std::map<int,CountryWarMonsterConfig> monsterInfos;//任务怪
	std::vector<int> elementID;														//五行柱id
	std::vector<std::pair<int,int> > winPrizeProps; 	//胜利方玩家奖励
	std::vector<std::pair<int,int> > failPrizeProps;  //失败方玩家奖励
};

//阵营战任务配置信息
struct CountryWarTaskConfigInfo
{
	CountryWarTaskConfigInfo()
	{
		id=0;
		type=0;
		targetID=0;
		targetCount=0;
		score=0;
		contribute=0;
		propID=0;
		propCount=0;
	}
	int id;								//任务id
	int type;						//任务类型
	int targetID;			 //目标id
	int targetCount;	 //目标数量
	int score;					 //奖励阵营奖励积分
	int contribute;	 //贡献度
	int propID;				 //奖励道具id
	int propCount;		 //奖励道具数量
};

enum CountryTaskState
{
	CountryTaskState_no,//未领取
	CountryTaskState_ok,//可领取
	CountryTaskState_yes,//已经领取
};
//任务信息
struct CountryTaskInfo
{
	CountryTaskInfo()
	{
		id=0;
		targetCount=0;
		flag=0;
	}
	int id;//id
	int targetCount;//完成目标数
	int flag;//当前领取状态
};

//阵营战个人信息
struct PersonCountryWarInfo
{
	PersonCountryWarInfo()
	{
		contribute=0;
		beKilled=0;
		sort=0;
		isCanEnter = true;
	}
	int contribute;	//贡献
	int beKilled;			//被杀次数
	int sort;					//排名
	bool isCanEnter;	//能否进入副本
	std::map<int,CountryTaskInfo> taskInfo;//任务信息
};

struct CampRankData
{
	CampRankData()
	{
		id=0;
		level=0;
		job=0;
		contribute=0;
		weapon=0;
		cloth=0;
		shenbing=0;
		fashion=0;
		profession=0;
		sex = 0;
	}
	int64 id;//id
	std::string name;//姓名
	int level;//等级
	BYTE job;//官职
	BYTE profession;//职业
	BYTE sex;//性别
	int contribute;//周贡献
	int weapon;//武器
	int cloth;//衣服
	int shenbing;//神兵
	int fashion;//时装
};

struct CountryBossPos
{
	int nXPos;
	int nYPos;

	CountryBossPos():nXPos(0),nYPos(0)
	{

	}
};

struct CountryWarMapInfo
{
	CountryWarMapInfo()
	{
		nId=0;nXPos=0;nYPos=0;nXPos1=0;nYPos1=0;nXPos2=0;nYPos2=0;nRadius=0;nRate=0;
	}
		int nId;
		int nXPos;
		int nYPos;
		int nXPos1;
		int nYPos1;
		int nXPos2;
		int nYPos2;
		int nRadius;
		int nRate;
		std::vector<CountryBossPos> vPos;
};

struct BossMap
{
	int nId;
	int nXPos;
	int nYPos;
	int nXPos1;
	int nYPos1;
	int nXPos2;
	int nYPos2;
	int nRadius;
	int nBossId;
	int nRate;
	int nItemId;
	int nItemNum;
	std::vector<CountryBossPos> vPos;

	BossMap():nId(0),nXPos(0),nYPos(0),nXPos1(0),nYPos1(0),
			nXPos2(0),nYPos2(0),nRadius(0),nBossId(0),nRate(0),
			nItemId(0),nItemNum(0)
	{
		vPos.clear();
	}
};

struct BossPower
{
	int nItemId;
	int nNum;
	int nProbability;

	BossPower():nItemId(0),nNum(0),nProbability(0)
	{

	}
};

// 进入推荐阵营奖励
typedef struct sJoinRcndAward
{
	vector<DWORD> idList;
	vector<DWORD> numList;
	vector<WORD> isBind;

	sJoinRcndAward()
	{
		idList.clear();
		numList.clear();
		isBind.clear();
	}
}JoinRcndAward;

#endif /* COUNTRYDEFINE_H_ */
