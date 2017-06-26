/*
 * FieldInfo.cpp
 *
 *  Created on: 2013��10��14��
 *      Author: helh
 */

#include "FieldInfo.h"
#include "Lua/LuaInterface.h"
#include <math.h>
#include "Log/Logger.h"
#include "../GameZone.h"
#include "Memory/MemAllocator.h"
#include "CSVLoader.h"
#include "StrArgPkg.h"

SectorInfo::SectorInfo(int index):m_sectorIndex(index),m_Flag(1)
{

}

SectorInfo::SectorInfo():m_sectorIndex(-1),m_Flag(1)
{

}

SectorInfo::~SectorInfo()
{
	m_nearbySector.clear();
}

bool SectorInfo::GetIndexFlag()
{
	return m_Flag != 1;
}

void SectorInfo::AddNearbySector(SectorInfo * pInfo)
{
	m_nearbySector.push_back(pInfo);
}

int SectorInfo::GetSectorIndex()
{
	return m_sectorIndex;
}

void SectorInfo::SetSectorIndex(int index)
{
	m_sectorIndex = index;
}

FieldInfo::FieldInfo(string &path, int id):m_dwFieldCode(id),m_dwSectorSize(0),m_xMaxNum(0),m_yMaxNum(0),m_iShiftMaxX(0),\
m_iShiftMaxY(0),m_nineXSector(0),m_nineYSector(0),m_nineXSize(0),m_nineYSize(0),m_totalSector(0),m_allSector(0)
{
	m_file = path + "FieldInfo.lua";
}

FieldInfo::~FieldInfo()
{
	if(m_allSector)
	{
		delete[] m_allSector;
		m_allSector = 0;
	}
}

int FieldInfo::Init(string &path,string& str)
{
	Smart_Ptr<CLuaInterface> lua = NEW CLuaInterface();

	lua->OpenLua();

	if(lua->LoadFile(m_file) != 0 || lua->Call() != 0)
	{
		return -1;
	}

	lua->SetLuaFunction("GetSectorSize");

	if(lua->Call(0, 1))
	{
		return -1;
	}

	m_dwSectorSize = lua->GetInt();

	lua->SetLuaFunction("GetMapSize");

	if(lua->Call(0, 2))
	{
		return -1;
	}

	m_iShiftMaxY = lua->GetInt();
	m_iShiftMaxX = lua->GetInt();

	lua->SetLuaFunction("GetNineSector");

	if(lua->Call(0, 2))
	{
		return -1;
	}

	m_nineYSector = lua->GetInt();
	m_nineXSector = lua->GetInt();


	//test
	/*lua->SetLuaFunction("GetRegen");
	if(lua->Call(0, 1))
	{
			return -1;
	}

	int tableCount = lua->GetTableFieldCount();
	for(int i=1; i<=tableCount; ++i)
	{
			if(lua->GetFieldTable(i) != 0)
			{
				return -1;
			}

			int a1 = lua->GetFieldInt("regenid");

			char szbuffer[256] = {0};
			lua->GetFieldString(szbuffer, 256, "params");

			if(lua->GetFieldTable(1) != 0)
			{
				return -1;
			}

			int twCount = lua->GetTableFieldCount();
			for(int j=1; j<=twCount; ++j)
			{
					if (lua->GetFieldTable(j) != 0)
					{
						return -1;
					}

					int a = lua->GetFieldInt("fun");
					int b = lua->GetFieldInt("funId");
					char szbuffer[256] = {0};
					lua->GetFieldString(szbuffer, 256, "param");

					lua->CloseTable();
			}

			lua->CloseTable();
			lua->CloseTable();
		}

	lua->CloseTable();*/
	lua->CloseLua();

	//���ָ���
	int res = CutSector(path);
	if(res)
	{
		return res;
	}

	initRegion(str);

	return 0;
}

void FieldInfo::initRegion(string& str)
{
	string tStr = str + "RegionData.csv";
	CSVLoader loader;
	if (!loader.OpenFromFile(tStr.c_str()))
	{
		return ;
	}

	RegionData tempData;
	int tempRow = 0;
	unsigned int tempCounts = loader.GetRowCount();

	for (unsigned int i = 0;i < tempCounts; ++i)
	{
		tempRow 		  = 0;
		tempData.resetData();

		tempData.m_ID     = loader.GetInt(i,tempRow++);

		++tempRow;

		StrArgPkg tRegionTypePkg(">",loader.GetString(i,tempRow++));
		tempData.m_Type   = static_cast<eRegionType>(atoi(getArg(tRegionTypePkg,0).c_str()));

		initRegionType(getArg(tRegionTypePkg,1),tempData);

		StrArgPkg tFunPkg("|>:",loader.GetString(i,tempRow++));
		for(size_t j = 0;j < tFunPkg.size();++j)
		{
			RegionFun tFunData;
			tFunData.m_FunType = static_cast<eRegionFunType>(atoi(getArg(tFunPkg[j],0).c_str()));

			if(tFunPkg[j].size() >= 2)
			{
				for(size_t z = 0;z < tFunPkg[j][1].size();++z)
				{
					string tFunStr = getArg(tFunPkg[j][1],z);
					tFunData.m_FunPama.push_back(atof(tFunStr.c_str()));
				}
			}

			tempData.m_RegionFun.push_back(tFunData);
		}

		StrArgPkg tStagePkg(":",loader.GetString(i,tempRow++));
		for(size_t j = 0;j < tStagePkg.size();++j)
		{
			EAttackState tState = static_cast<EAttackState>(atoi(getArg(tStagePkg,j).c_str()));
			tempData.m_StateLimit.push_back(tState);
		}

		StrArgPkg tDoingPkg(":",loader.GetString(i,tempRow++));
		for(size_t j = 0;j < tDoingPkg.size();++j)
		{
			eObjectDoing tDoing = static_cast<eObjectDoing>(atoi(getArg(tDoingPkg,j).c_str()));
			tempData.m_DoingLimit.push_back(tDoing);
		}

		m_RegionList[tempData.m_ID] = tempData;
	}

	return ;
}

void FieldInfo::initRegionType(string str,RegionData& data)
{
	switch(data.m_Type)
	{
	case eRegionRound:
		{
			StrArgPkg limitPkg(":;",str);
			data.m_Radius = atoi(getArg(limitPkg,0).c_str());

			EffectVector tempVector;
			tempVector.x = atoi(getArg(limitPkg[1],0).c_str());
			tempVector.y = atoi(getArg(limitPkg[1],1).c_str());

			data.m_PosList.push_back(tempVector);

			break;
		}
	case eRegionPolygon:
		{
			StrArgPkg limitPkg(":;",str);
			for(size_t i = 0;i < limitPkg.size();++i)
			{
				EffectVector tempVector;
				tempVector.x = atoi(getArg(limitPkg[i],0).c_str());
				tempVector.y = atoi(getArg(limitPkg[i],1).c_str());

				data.m_PosList.push_back(tempVector);
			}

			break;
		}
	default:
		{
			break;
		}
	}

	return ;
}

int FieldInfo::CutSector(string &path)
{
	CLuaInterface lua;
	string file = path + "Data/Map/MapPath.lua";

	lua.OpenLua();

	if(lua.LoadFile(file) != 0 || lua.Call() != 0)
	{
		return -1;
	}

	int xMaxOffset = (m_iShiftMaxX % m_dwSectorSize) ? 1 : 0;
	int yMaxOffset = (m_iShiftMaxY % m_dwSectorSize) ? 1 : 0;

	m_xMaxNum = m_iShiftMaxX / m_dwSectorSize + xMaxOffset;
	m_yMaxNum = m_iShiftMaxY / m_dwSectorSize + yMaxOffset;

	m_totalSector = m_yMaxNum * m_xMaxNum;

//	m_allSector = (SectorInfo *)new char[m_totalSector * sizeof(SectorInfo)];
//	for(int i=0; i<m_totalSector; ++i)
//	{
//		new(m_allSector + i) SectorInfo(i);
//	}

	m_allSector = new SectorInfo[m_totalSector];
	for(int i=0; i<m_totalSector; ++i)
	{
		m_allSector[i].SetSectorIndex(i);
	}

	for(int j=0; j<m_totalSector; ++j)
	{
		SectorInfo * pInfo = &m_allSector[j];
		int xpos = j % m_xMaxNum;
		int ypos = j / m_xMaxNum;

		lua.SetLuaFunction("IsCanGo");
		lua.SetInt(m_dwFieldCode);
		lua.SetInt(xpos);
		lua.SetInt(ypos);

		if(lua.Call(3, 1))
		{
			return -1;
		}

		pInfo->SetIndexFlag(lua.GetInt());

		//���������ǰ��
		pInfo->AddNearbySector(pInfo);

		for( int tx=xpos-1; tx<=xpos+1; ++tx )
		{
			if( ( tx >= m_xMaxNum ) || ( tx < 0 ) )
				continue;

			for( int ty=ypos-1; ty<=ypos+1; ++ty )
			{
				if( ( ty >= m_yMaxNum ) || ( ty < 0 ) || (tx == xpos && ty == ypos ) )
				{
					continue;
				}

				pInfo->AddNearbySector( &m_allSector[ ty * m_xMaxNum + tx ] );
			}
		}
	}

	int xOffset = (m_xMaxNum % m_nineXSector) ? 1 : 0;
	int yOffset = (m_yMaxNum % m_nineYSector) ? 1 : 0;

	m_nineXSize = m_xMaxNum / m_nineXSector + xOffset;
	m_nineYSize = m_yMaxNum / m_nineYSector + yOffset;

	lua.CloseLua();

	return 0;
}

void FieldInfo::GetAroundSector(CGameZone *zone, int xpos, int ypos, int expos, int eypos, vector<Pos> &vec)
{
	vector<Pos> ldeq;
	vector<Pos> rdeq;
	bool islvalid = false;
	bool isrvalid = false;
	bool lSuc = false;
	Pos fpos;

	fpos.xpos = xpos;
	fpos.ypos = ypos;
	ldeq.push_back(fpos);
	rdeq.push_back(fpos);

	if(xpos == expos && ypos != eypos)
	{
		//远程攻击最长是8格,超过8格就不动了
		for(int i=1; i<9; ++i)
		{
			if(islvalid && isrvalid)
				break;

			int lbxpos = xpos - i;
			int rbxpos = xpos + i;

			if(lbxpos > 0 && !islvalid)
			{
				int bindex = m_xMaxNum * ypos + lbxpos;
				if(bindex < m_totalSector)
				{
					SectorInfo * binfo = &m_allSector[bindex];
					if(binfo->GetIndexFlag() && !zone->IsHasMonster(bindex))
					{
						int llbxpos = lbxpos - 1;
						if(lbxpos > 0)
						{
							int lbindex = m_xMaxNum * eypos + llbxpos;
							if(lbindex < m_totalSector)
							{
								SectorInfo * lbinfo = &m_allSector[lbindex];
								if(lbinfo->GetIndexFlag() && !zone->IsHasMonster(lbindex))
								{
									Pos tpos;

									tpos.xpos = llbxpos;
									tpos.ypos = eypos;
									ldeq.push_back(tpos);
									lSuc = true;

									break;
								}
							}
							else
							{
								islvalid = true;
							}
						}
						else
						{
							islvalid = true;
						}

						Pos tpos;

						tpos.xpos = lbxpos;
						tpos.ypos = ypos;
						rdeq.push_back(tpos);
					}
					else
					{
						islvalid = true;
					}
				}
				else
				{
					islvalid = true;
				}
			}
			else
			{
				islvalid = true;
			}

			if(lbxpos == rbxpos)
				continue;

			if(rbxpos <= m_xMaxNum && !isrvalid)
			{
				int bindex = m_xMaxNum * ypos + rbxpos;
				if(bindex < m_totalSector)
				{
					SectorInfo * binfo = &m_allSector[bindex];
					if(binfo->GetIndexFlag() && !zone->IsHasMonster(bindex))
					{
						int rrbxpos = rbxpos - 1;
						if(rrbxpos > 0)
						{
							int rbindex = m_xMaxNum * eypos + rrbxpos;
							if(rbindex < m_totalSector)
							{
								SectorInfo * rbinfo = &m_allSector[rbindex];
								if(rbinfo->GetIndexFlag() && !zone->IsHasMonster(rbindex))
								{
									Pos tpos;

									tpos.xpos = rrbxpos;
									tpos.ypos = eypos;
									rdeq.push_back(tpos);

									break;
								}
							}
							else
							{
								islvalid = true;
							}
						}
						else
						{
							islvalid = true;
						}

						Pos tpos;

						tpos.xpos = rbxpos;
						tpos.ypos = ypos;
						rdeq.push_back(tpos);
					}
					else
					{
						islvalid = true;
					}
				}
				else
				{
					islvalid = true;
				}
			}
			else
			{
				islvalid = true;
			}
		}

		if(lSuc)
		{
			vec = ldeq;
		}
		else
		{
			vec = rdeq;
		}
	}
	else if(xpos != expos && ypos == eypos)
	{
		//远程攻击最长是8格,超过8格就不动了
		for(int i=1; i<9; ++i)
		{
			if(islvalid && isrvalid)
				break;

			int lbypos = ypos - i;
			int rbypos = ypos + i;

			if(lbypos > 0 && !islvalid)
			{
				int bindex = m_xMaxNum * lbypos + xpos;
				if(bindex < m_totalSector)
				{
					SectorInfo * binfo = &m_allSector[bindex];
					if(binfo->GetIndexFlag() && !zone->IsHasMonster(bindex))
					{
						int llbypos = lbypos - 1;
						if(lbypos > 0)
						{
							int lbindex = m_xMaxNum * llbypos + expos;
							if(lbindex < m_totalSector)
							{
								SectorInfo * lbinfo = &m_allSector[lbindex];
								if(lbinfo->GetIndexFlag() && !zone->IsHasMonster(lbindex))
								{
									Pos tpos;

									tpos.xpos = expos;
									tpos.ypos = llbypos;
									ldeq.push_back(tpos);
									lSuc = true;

									break;
								}
							}
							else
							{
								islvalid = true;
							}
						}
						else
						{
							islvalid = true;
						}

						Pos tpos;

						tpos.xpos = xpos;
						tpos.ypos = lbypos;
						rdeq.push_back(tpos);
					}
					else
					{
						islvalid = true;
					}
				}
				else
				{
					islvalid = true;
				}
			}
			else
			{
				islvalid = true;
			}

			if(lbypos == rbypos)
				continue;

			if(rbypos <= m_yMaxNum && !isrvalid)
			{
				int bindex = m_xMaxNum * rbypos + xpos;
				if(bindex < m_totalSector)
				{
					SectorInfo * binfo = &m_allSector[bindex];
					if(binfo->GetIndexFlag() && !zone->IsHasMonster(bindex))
					{
						int rrbypos = rbypos - 1;
						if(rrbypos > 0)
						{
							int rbindex = m_xMaxNum * rrbypos + expos;
							if(rbindex < m_totalSector)
							{
								SectorInfo * rbinfo = &m_allSector[rbindex];
								if(rbinfo->GetIndexFlag() && !zone->IsHasMonster(rbindex))
								{
									Pos tpos;

									tpos.xpos = expos;
									tpos.ypos = rrbypos;
									rdeq.push_back(tpos);

									break;
								}
							}
							else
							{
								islvalid = true;
							}
						}
						else
						{
							islvalid = true;
						}

						Pos tpos;

						tpos.xpos = xpos;
						tpos.ypos = rbypos;
						rdeq.push_back(tpos);
					}
					else
					{
						islvalid = true;
					}
				}
				else
				{
					islvalid = true;
				}
			}
			else
			{
				islvalid = true;
			}
		}

		if(lSuc)
		{
			vec = ldeq;
		}
		else
		{
			vec = rdeq;
		}
	}
	else
	{
		if(RandGetValidPos(zone, xpos, ypos, fpos.xpos, fpos.ypos))
		{
			vec.push_back(fpos);
		}
	}
}

void FieldInfo::GetAroundSector(CGameZone *zone, int distance, int xpos, int ypos, int count, vector<int> &vec)
{
	int tminxpos = xpos - distance;
	if(tminxpos < 0)
		tminxpos = 0;
	int tmaxxpos = xpos + distance;
	if(tmaxxpos >= m_xMaxNum)
		tmaxxpos = m_xMaxNum - 1;
	int tminypos = ypos - distance;
	if(tminypos < 0)
		tminypos = 0;
	int tmaxypos = ypos + distance;
	if(tmaxypos >= m_yMaxNum)
		tmaxypos = m_yMaxNum - 1;

	for(int i=tminxpos; i<=tmaxxpos; ++i)
	{
		if(tminypos == 0 || tmaxypos == m_yMaxNum)
		{
			break;
		}

		int minindex = m_xMaxNum * tminypos + i;
		int maxindex = m_xMaxNum * tmaxypos + i;
		if(minindex >= m_totalSector || maxindex >= m_totalSector)
		{
			continue;
		}

		SectorInfo * mininfo = &m_allSector[minindex];
		SectorInfo * maxinfo = &m_allSector[maxindex];

		if(mininfo->GetIndexFlag())
		{
			if(zone->IsAllocDrop(minindex))
			{
				vec.push_back(minindex);
			}
		}

		if(mininfo == maxinfo)
			continue;

		if(maxinfo->GetIndexFlag())
		{
			if(zone->IsAllocDrop(maxindex))
			{
				vec.push_back(maxindex);
			}
		}
	}

	for(int j=tminypos+1; j<tmaxypos; ++j)
	{
		if(tminxpos == 0 || tmaxxpos == m_yMaxNum)
		{
			break;
		}

		int minindex = m_xMaxNum * j + tminxpos;
		int maxindex = m_xMaxNum * j + tmaxxpos;
		if(minindex >= m_totalSector || maxindex >= m_totalSector)
		{
			continue;
		}

		SectorInfo * mininfo = &m_allSector[minindex];
		SectorInfo * maxinfo = &m_allSector[maxindex];

		if(mininfo->GetIndexFlag())
		{
			if(zone->IsAllocDrop(minindex))
			{
				vec.push_back(minindex);
			}
		}

		if(maxinfo->GetIndexFlag())
		{
			if(zone->IsAllocDrop(maxindex))
			{
				vec.push_back(maxindex);
			}
		}
	}
}

int FieldInfo::GetPosFlag(int xpos, int ypos)
{
	int index = m_xMaxNum * ypos + xpos;
	if(index < m_totalSector)
	{
		SectorInfo * info = &m_allSector[index];
		return info->GetSectorFlag();
	}
	return 1;
}



bool FieldInfo::IsValidSector(int index)
{
	if(index >= m_totalSector)
	{
		return false;
	}

	SectorInfo * info = &m_allSector[index];

	return info->GetIndexFlag();
}

bool FieldInfo::RandGetValidPos(CGameZone *zone, int xpos, int ypos, int &expos, int &eypos)
{
	vector<Pos> tVec;

	int tminxpos = xpos - 1;
	if(tminxpos < 0)
		tminxpos = 0;
	int tmaxxpos = xpos + 1;
	if(tmaxxpos >= m_xMaxNum)
		tmaxxpos = m_xMaxNum - 1;
	int tminypos = ypos - 1;
	if(tminypos < 0)
		tminypos = 0;
	int tmaxypos = ypos + 1;
	if(tmaxypos >= m_yMaxNum)
		tmaxypos = m_yMaxNum - 1;

	for(int i=tminxpos; i<=tmaxxpos; ++i)
	{
		for(int j=tminypos; j<tmaxypos; ++j)
		{
			if(i == xpos && j == ypos)
				continue;

			int index = m_xMaxNum * j + i;
			if(index >= m_totalSector)
			{
				continue;
			}

			SectorInfo * info = &m_allSector[index];
			if(info->GetIndexFlag() && !zone->IsHasMonster(index))
			{
				Pos tPos;

				tPos.xpos = i;
				tPos.ypos = j;
				tVec.push_back(tPos);
			}
		}
	}

	if(tVec.size() <= 0)
		return false;

	int randCount = rand() % tVec.size();

	expos = tVec[randCount].xpos;
	eypos = tVec[randCount].ypos;

	return true;
}


bool FieldInfo::RandGetPetValidPos(CGameZone *zone, int xpos, int ypos, int &expos, int &eypos)
{
	vector<Pos> tVec;

	int tminxpos = xpos - 1;
	if(tminxpos < 0)
		tminxpos = 0;
	int tmaxxpos = xpos + 1;
	if(tmaxxpos >= m_xMaxNum)
		tmaxxpos = m_xMaxNum - 1;
	int tminypos = ypos - 1;
	if(tminypos < 0)
		tminypos = 0;
	int tmaxypos = ypos + 1;
	if(tmaxypos >= m_yMaxNum)
		tmaxypos = m_yMaxNum - 1;

	for(int i=tminxpos; i<=tmaxxpos; ++i)
	{
		for(int j=tminypos; j<tmaxypos; ++j)
		{
			if(i == xpos && j == ypos)
				continue;

			int index = m_xMaxNum * j + i;
			if(index >= m_totalSector)
			{
				continue;
			}

			SectorInfo * info = &m_allSector[index];
			if(info->GetIndexFlag())
			{
				Pos tPos;

				tPos.xpos = i;
				tPos.ypos = j;
				tVec.push_back(tPos);
			}
		}
	}

	if(tVec.size() <= 0)
		return false;

	int randCount = rand() % tVec.size();

	expos = tVec[randCount].xpos;
	eypos = tVec[randCount].ypos;

	return true;
}



bool FieldInfo::HaveValidPos(CGameZone *zone, int xpos, int ypos, int expos, int eypos)
{
	if(expos > xpos)
	{
		for(int i=0; i<=1; ++i)
		{
			int txpos = xpos + i;
			if(txpos >= m_xMaxNum)
				continue;

			for(int j=-1; j<=1; ++j)
			{
				int typos = ypos + j;
				if(typos >= m_yMaxNum || typos < 0 || (txpos == xpos && typos == ypos))
				{
					continue;
				}

				int index = m_xMaxNum * typos + txpos;
				if(index < m_totalSector)
				{
					SectorInfo * info = &m_allSector[index];
					if(info->GetIndexFlag() && !zone->IsHasMonster(index))
					{
						return true;
					}
				}
			}
		}
	}
	else if(expos < xpos)
	{
		for(int i=-1; i<=0; ++i)
		{
			int txpos = xpos + i;
			if(txpos < 0)
				continue;

			for(int j=-1; j<=1; ++j)
			{
				int typos = ypos + j;
				if(typos >= m_yMaxNum || typos < 0 || (txpos == xpos && typos == ypos))
				{
					continue;
				}

				int index = m_xMaxNum * typos + txpos;
				if(index < m_totalSector)
				{
					SectorInfo * info = &m_allSector[index];
					if(info->GetIndexFlag() && !zone->IsHasMonster(index))
					{
						return true;
					}
				}
			}
		}
	}
	else if(eypos > ypos)
	{
		for(int i=0; i<=1; ++i)
		{
			int typos = ypos + i;
			if(typos >= m_yMaxNum)
				continue;

			for(int j=-1; j<=1; ++j)
			{
				int txpos = xpos + j;
				if(txpos >= m_xMaxNum || txpos < 0 || (txpos == xpos && typos == ypos))
				{
					continue;
				}

				int index = m_xMaxNum * typos + txpos;
				if(index < m_totalSector)
				{
					SectorInfo * info = &m_allSector[index];
					if(info->GetIndexFlag() && !zone->IsHasMonster(index))
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		for(int i=-1; i<=0; ++i)
		{
			int typos = ypos + i;
			if(typos < 0)
				continue;

			for(int j=-1; j<=1; ++j)
			{
				int txpos = xpos + j;
				if(txpos >= m_xMaxNum || txpos < 0 || (txpos == xpos && typos == ypos))
				{
					continue;
				}

				int index = m_xMaxNum * typos + txpos;
				if(index < m_totalSector)
				{
					SectorInfo * info = &m_allSector[index];
					if(info->GetIndexFlag() && !zone->IsHasMonster(index))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}



