/*
 * GirlWarLoader.h
 *
 *  Created on: 2014年12月5日
 *      Author: root
 */

#ifndef GIRLWARLOADER_H_
#define GIRLWARLOADER_H_

#include <map>
#include <vector>
#include <string>
#include "OtherDefine.h"
#include "define.h"
#include "Lua/LuaInterface.h"
#include "CharDefine.h"


using namespace std;
using namespace CommBaseOut;

enum NeedType
{
	eSings = 1,  //签到需求次数
	eVip,	//vip等级领取
	eLv,	//Lv到达
	eProp, //道具到达
	eMicro,	//微端
	eMPVerification, //手机验证
	eInstancePass, // 副本通关
	eGuardInstancePass, // 守护秦王通关
};

struct GirlWarNeed
{
	int nId;
	int nSings;
	int nVip;
	int nLv;
	int nPropId;
	int nPropLv;
	bool bMicro;
	bool bMPVerification;
	int64 mapID; // 地图ID
	int guardNode; // 守护秦王节点

	GirlWarNeed():nId(0),nSings(0),nVip(0),nLv(0),nPropId(0),nPropLv(0),bMicro(false),bMPVerification(false),
			mapID(0), guardNode(0)
	{

	}

	void InsertData(NeedType etype,int value,int arg = 0, int64 arg1 = 0)
	{
		switch(etype)
		{
		case eSings:
			{
				nSings = value;
				break;
			}
		case eVip:
			{
				nVip = value;
				break;
			}
		case eLv:
			{
				nLv = value;
				break;
			}
		case eProp:
			{
				nPropId = value;
				nPropLv = arg;
				break;
			}
		case eMicro:
			{
				bMicro = value;
				break;
			}
		case eMPVerification:
			{
				bMPVerification = value;
				break;
			}
		case eInstancePass:
			{
				mapID = arg1;
				break;
			}
		case eGuardInstancePass:
			{
				guardNode = value;
				break;
			}
		}
	}

	bool IsEnoughCondition(NeedType etype,int value,int arg = 0, int64 arg1 = 0)
	{
		bool bResult = false;
		switch(etype)
		{
		case eSings:
			{
				if(nSings <= value)
				{
					bResult = true;
				}
				break;
			}
		case eVip:
			{
				if(nVip <= value)
				{
					bResult = true;
				}
				break;
			}
		case eLv:
			{
				if(nLv <= value)
				{
					bResult = true;
				}
				break;
			}
		case eProp:
			{
				if(nPropId == value && nPropLv <= arg)
				{
					bResult = true;
				}
				break;
			}
		case eMicro:
			{
				bResult = bMicro;
				break;
			}
		case eMPVerification:
			{
				bResult = bMPVerification;
				break;
			}
		case eInstancePass:
			{
				if (mapID == arg1)
				{
					bResult = true;
				}
				break;
			}
		case eGuardInstancePass:
			{
				if (guardNode == value)
				{
					bResult = true;
				}
				break;
			}
		}
		return bResult;
	}

	int GetPropId()
	{
		return nPropId;
	}
};


struct sGirlWarInfo
{
	int nId;	//ID
	string sName;	//名称
	BYTE nModle;	//模型
	int nMoveSpeed;
	int nBasehit;
	int nLuck;

	int nCommonSkill; //普通技能
	int nSkill1;	//主动技能
	int nSkill2; //被动技能
	int nSkill3; //被动技能

	int nExp1;	//经验系数A
	int nExp2;	//经验系数B
	int nExp3;	//经验系数C

	int	nMoveRange;	//多少距离位移
	int nAckRange;	//攻击距离
	int nAckInterval;	//攻出间隔

	int	nLimitLv;	//lv上限
	int nTriggerPro; //被动技能触发概率
	int  nFlushTime; //复活CD

	sGirlWarInfo():nId(0),nModle(0),nMoveSpeed(0),nBasehit(0),nLuck(0),nCommonSkill(0),nSkill1(0),nSkill2(0),nSkill3(0),nExp1(0),nExp2(0),nExp3(0),nMoveRange(0),nAckRange(0),nAckInterval(0),nLimitLv(0),nTriggerPro(0),nFlushTime(0)
	{
		sName.clear();
	}


};

struct sGirlWarStarrank
{
	int nId;	//ID
	int nStarrank; //星阶
	int nDebrisId; //碎片ID
	int nDebrisNum;//碎片数量
	int nResolveNum;//分解数量

	int nAddHp;		//增加生命
	int nAddPhyAck;	//增加物攻
	int nAddPhydef; //增加防御
	int nAddCrit;		//增加暴击
	int nAddDodge;		//增加闪避
	int nAddUng;		//增加抗暴

	int nAddOwerHp;		//增加主角生命
	int nAddOwerPhyAck;	//增加主角物攻
	int nAddOwerPhydef; //增加主角防御
	int nAddOwerCrit;		//增加主角暴击
	int nAddOwerDodge;		//增加主角闪避
	int nAddOwerUng;		//增加主角抗暴


	sGirlWarStarrank():nId(0),nStarrank(0),nDebrisId(0),nDebrisNum(0),nResolveNum(0),nAddHp(0),nAddPhyAck(0),nAddPhydef(0),
			nAddCrit(0),nAddDodge(0),nAddUng(0),nAddOwerHp(0),nAddOwerPhyAck(0),nAddOwerPhydef(0),nAddOwerCrit(0),nAddOwerDodge(0),nAddOwerUng(0)
	{

	}
};


struct sGirlWarQuality
{
	int nId;	//ID

	int nQuality; //品阶
	int nQualityLv; //品阶等级
	int m_nEquip1;//升阶道具1
	map<int,int> m_mAtt1;
	int m_nEquip2;//升阶道具1
	map<int,int> m_mAtt2;
	int m_nEquip3;//升阶道具1
	map<int,int> m_mAtt3;
	int m_nEquip4;//升阶道具1
	map<int,int> m_mAtt4;

	sGirlWarQuality():nId(0),nQuality(0),nQualityLv(0),m_nEquip1(0),m_nEquip2(0),m_nEquip3(0),m_nEquip4(0)
	{
		m_mAtt1.clear();
		m_mAtt2.clear();
		m_mAtt3.clear();
		m_mAtt4.clear();
	};
};

struct sGirlWarCollection
{
	int nId;
	int nStarrankDow;
	int nStarrankUp;
	int nAddHp;		//增加生命
	int nAddPhyAck;	//增加物攻
	int nAddPhydef; //增加防御
	int nAddCrit;		//增加暴击
	int nAddUng;		//增加抗暴
	int nAddDodge;		//增加闪避

	int nAddHp1;		//增加生命
	int nAddPhyAck1;	//增加物攻
	int nAddPhydef1; //增加防御
	int nAddCrit1;		//增加暴击
	int nAddUng1;		//增加抗暴
	int nAddDodge1;		//增加闪避


	int nAddHp2;		//增加生命
	int nAddPhyAck2;	//增加物攻
	int nAddPhydef2; //增加防御
	int nAddCrit2;		//增加暴击
	int nAddUng2;		//增加抗暴
	int nAddDodge2;		//增加闪避

	sGirlWarCollection():nId(0),nStarrankDow(0),nStarrankUp(0),nAddHp(0),nAddPhyAck(0),nAddPhydef(0),
			nAddCrit(0),nAddUng(0),nAddDodge(0),nAddHp1(0),nAddPhyAck1(0),nAddPhydef1(0),
			nAddCrit1(0),nAddUng1(0),nAddDodge1(0),nAddHp2(0),nAddPhyAck2(0),nAddPhydef2(0),
			nAddCrit2(0),nAddUng2(0),nAddDodge2(0)
	{

	}

	int GetAddHp(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddHp;break;}
			case eMagicHuman:{value = nAddHp1;break;}
			case eShooter:{value = nAddHp2;break;}
			default: {break;}
		}

		return value;
	}

	int GetAddPhyAck(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddPhyAck;break;}
			case eMagicHuman:{value = nAddPhyAck1;break;}
			case eShooter:{value = nAddPhyAck2;break;}
			default: {break;}
		}

		return value;
	}

	int GetAddPhydef(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddPhydef;break;}
			case eMagicHuman:{value = nAddPhydef1;break;}
			case eShooter:{value = nAddPhydef2;break;}
			default: {break;}
		}

		return value;
	}

	int GetAddCrit(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddCrit;break;}
			case eMagicHuman:{value = nAddCrit1;break;}
			case eShooter:{value = nAddCrit2;break;}
			default: {break;}
		}

		return value;
	}

	int GetAddUng(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddUng;break;}
			case eMagicHuman:{value = nAddUng1;break;}
			case eShooter:{value = nAddUng2;break;}
			default: {break;}
		}

		return value;
	}

	int GetAddDodge(int type) const
	{
		int value = 0;
		switch(type)
		{
			case eMuscleMan:{ value = nAddDodge;break;}
			case eMagicHuman:{value = nAddDodge1;break;}
			case eShooter:{value = nAddDodge2;break;}
			default: {break;}
		}

		return value;
	}

};

struct sGirlWarAttribute
{
	int nId;	//ID
	int nLv; //等级

	int nAddHp;		//增加生命
	int nAddPhyAck;	//增加物攻
	int nAddPhydef; //增加防御
	int nAddDodge;		//增加闪避
	int nAddCrit;		//增加暴击
	int nAddUng;		//增加抗暴

	sGirlWarAttribute():nId(0),nLv(0),nAddHp(0),nAddPhyAck(0),nAddPhydef(0),nAddDodge(0),nAddCrit(0),nAddUng(0)
	{

	}
};


class GirlWarLoader
{
public:
	~GirlWarLoader();

	static GirlWarLoader * GetInstance()
	{
		static GirlWarLoader m_instance;
		return &m_instance;
	}

	int Init(string &path);
	int InitLua(string& path);
	int InitGirlWarInfo(string &file);
	int InitGirlWarNeed(string &file);
	int InitGirlWarStarrank(string &file);
	int InitGirlWarQuality(string &file);
	int InitGirlWarCollection(string &file);
	int InitGirlWarAttribute(string &file);

	const sGirlWarInfo* GetGirlWarInfo(int nId);
	const sGirlWarStarrank* GetGirlWarStarrank(int nId, int nStar = 1);
	const sGirlWarQuality* GetGirlWarQuality(int nId, int nQuality = 1);
	const sGirlWarCollection* GetGirlWarCollection(int nStar);
	const sGirlWarCollection* GetGirlWarCollectionById(int nId);
	const GirlWarNeed* GetGirlWarNeed(int nId);
	const sGirlWarAttribute* GetGirlWarAttribute(int nId, int nLv = 1);

	void IsGirlWarNeed(NeedType etype,int value,int arg,vector<int>& vec, int64 arg1);

	int GetFlushTime(int nId);

	int GetMoveRange(int nId);

	int GetAckRange(int nId);

	int GetTriggerPro(int nId);

	int GetCommonSkill(int nId);

	int GetUpdateLv(int nId,int nLv);

	int GetLimitLv(int nId);

	void GetAllGirlWarInfo(map<int,sGirlWarInfo>& info)
	{
		info = m_mGirlWarInfo;
	}

	void GetGirlWarType(int nId, vector<int>& vec);

	int GetGirlWarNeedPropId(int nId);

//	int GetPrimaryExp()
//	{
//		return m_nPrimaryExp;
//	}
//
//	int GetMiddleExp()
//	{
//		return m_nMiddleExp;
//	}
//
//	int GetSeniorExp()
//	{
//		return m_nSeniorExp;
//	}

	int GetCommonExp()
	{
		return m_nCommonExp;
	}

	bool IsHavaEquip(int nId, int nQuality,int nEquipId);
	const map<int,int>* GetEquipAtt(int nId, int nQuality,int nEquipId);
private:
	GirlWarLoader();
	//2016-4-4 将多种经验丹合为一个
//	int m_nPrimaryExp;
//	int m_nMiddleExp;
//	int m_nSeniorExp;
	int m_nCommonExp;
	map<int,sGirlWarInfo> m_mGirlWarInfo;
	map<int,vector<int> > m_mType;
	map<int,GirlWarNeed> m_mGirlWarNeed;
	map<int,vector<int> > m_mGirlWarType;
	vector<sGirlWarStarrank> m_vGirlWarStarrank;
	vector<sGirlWarQuality> m_vGirlWarQuality;
	vector<sGirlWarCollection> m_vsGirlWarCollection;
	vector<sGirlWarAttribute> m_vGirlWarAttribute;
	CLuaInterface m_pLua;
};



#endif /* GIRLWARLOADER_H_ */
