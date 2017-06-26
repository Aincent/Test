/*
 * FiledInfo.h
 *
 *  Created on: 2013��10��10��
 *      Author: helh
 */

#ifndef FILEDINFO_H_
#define FILEDINFO_H_

#include <vector>
#include "Smart_Ptr.h"
#include "define.h"
#include "FieldMonster.h"
#include "EffectMath/EffectVector.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

class FieldInfo;
class CGameZone;


enum eRegionType
{
	eRegionNULL    = 0,
	eRegionRound   = 1,	//圆
	eRegionPolygon = 2, //多边形
};

enum eRegionFunType
{
	eRegionFunNULL 	     = 0,
	eRegionAlterPkType   = 1,	//切换PK模式
	eRegionAlterAtt      = 2,	//修改属性
	eRegionIntervalExp   = 3,   //间隔时间内加经验
	eRegionIntervalForce = 4,   //间隔时间内加真气

};

struct RegionFun
{
	RegionFun() : m_FunType(eRegionFunNULL){}
	eRegionFunType m_FunType;
	vector<double> m_FunPama;
};

struct RegionData
{
	RegionData() : m_ID(0),m_Type(eRegionNULL),m_Radius(0)
	{
		resetData();
	}

	void resetData()
	{
		m_ID 		   = 0;
		m_Type 		   = eRegionNULL;
		m_Radius 	   = 0;
		m_PosList.clear();
		m_RegionFun.clear();
		m_StateLimit.clear();
		m_DoingLimit.clear();
	}

	int 				 m_ID;
	eRegionType 		 m_Type;
	int					 m_Radius;
	vector<EffectVector> m_PosList;
	vector<RegionFun>	 m_RegionFun;
	vector<EAttackState> m_StateLimit;
	vector<eObjectDoing> m_DoingLimit;
};

class SectorInfo
{
public:
	SectorInfo(int index);
	SectorInfo();
	~SectorInfo();

	void AddNearbySector(SectorInfo * pInfo);
	void SetIndexFlag(BYTE flag)
	{
		m_Flag = flag;
	}

	//保留该接口兼容以前版本
	bool GetIndexFlag();

	//得到格子真正的标识
	BYTE GetSectorFlag()
	{
		return m_Flag;
	}

	int GetSectorIndex();
	void SetSectorIndex(int index);

//	//获取旁边的格子
//	void GetSectorIndex(CGameZone *zone, vector<int> &vec, vector<int> &useVec, int count);

private:
	int m_sectorIndex;
	BYTE m_Flag;
	vector< SectorInfo * > m_nearbySector;
};

class FieldInfo
{
public:
	FieldInfo(string &path, int id);
	~FieldInfo();

	int Init(string &path,string& str);

	void initRegion(string& str);

	void initRegionType(string str,RegionData& data);

	int CutSector(string &path);

	int GetTotalSize()
	{
		return m_totalSector;
	}

	int GetNineXSector()
	{
		return m_nineXSector;
	}

	int GetNineYSector()
	{
		return m_nineYSector;
	}

	int GetXMaxNum()
	{
		return m_xMaxNum;
	}

	int GetYMaxNum()
	{
		return m_yMaxNum;
	}

	int GetNineXSize() { 	return m_nineXSize; }
	int GetNineYSize() { 	return m_nineYSize; }

	int GetNineTotalSize() { 	return m_nineXSize*m_nineYSize; }

	//获取总得格子个数
	int GetAllSectorSize()
	{
		return m_totalSector;
	}

	//格子的标识
	int GetPosFlag(int xpos, int ypos);

	//获取格子旁边的格子
//	void GetAroundSector(CGameZone *zone, int index, int count, vector<int> &vec, vector<int> &useVec);
	void GetAroundSector(CGameZone *zone, int distance, int xpos, int ypos, int count, vector<int> &vec);
	void GetAroundSector(CGameZone *zone, int xpos, int ypos, int expos, int eypos, vector<Pos> &vec);

	//判断格子是否可以走
	bool IsValidSector(int index);
	bool HaveValidPos(CGameZone *zone, int xpos, int ypos, int expos, int eypos);
	//	随机获得一个旁边可以走的点
	bool RandGetValidPos(CGameZone *zone, int xpos, int ypos, int &expos, int &eypos);

	bool RandGetPetValidPos(CGameZone *zone, int xpos, int ypos, int &expos, int &eypos);

	int GetSectorSize()
	{
		return m_dwSectorSize;
	}

	const map<int,RegionData>& getRegionList() {return m_RegionList;}

private:

	int m_dwFieldCode; // ��ͼ���
	string m_file;

	int m_dwSectorSize;	  // m_dwSectorSize表示格子的横宽度,m_dwSectorSize/2表示格子的纵宽度
	int m_xMaxNum;	  // 表示地图横向格子最大个数
	int m_yMaxNum;     // 表示地图纵向格子最大个数

	int    m_iShiftMaxX; // 地图的横坐标宽度
	int    m_iShiftMaxY; // 地图的纵坐标宽度

	int m_nineXSector; //每个九宫格横坐标所占用的格子个数
	int m_nineYSector; //每个九宫格纵坐标所占用的格子个数

	int m_nineXSize; //地图横坐标九宫格个数
	int m_nineYSize; // 地图纵坐标九宫格个数

	int m_totalSector;    //表示地图格子总数
	SectorInfo *m_allSector;   //地图的所有格子信息
	map<int,RegionData> m_RegionList;
};


#endif /* FILEDINFO_H_ */
