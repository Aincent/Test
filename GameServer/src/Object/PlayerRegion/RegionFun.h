/*
 * RegionFun.h
 *
 *  Created on: 01/12/2015
 *      Author: root
 */

#ifndef REGIONFUN_H_
#define REGIONFUN_H_

#include "Timer/TimerInterface.h"
#include "../../Skill/SkillManager.h"
#include "define.h"

class PlayerRegion;
class RegionData;
class Player;

using namespace CommBaseOut;

//----------区域功能基类---------------
class RegionBase
{
public:
	RegionBase(PlayerRegion* father,const RegionData* data,BYTE index) : m_Father(father),m_Data(data),m_Index(index){}

	virtual ~RegionBase(){}

public:
	Player* getOwner();

	virtual void revert() = 0;

	virtual void excute() = 0;

protected:
	PlayerRegion*     m_Father;
	const RegionData* m_Data;
	BYTE			  m_Index;
};

//--------------定时加经验-----------------
class RegionIntervalExp : public RegionBase
{
public:
	RegionIntervalExp(const RegionData* data,PlayerRegion* father,BYTE index);

	virtual ~RegionIntervalExp();

public:
	void time(void* p);

	virtual void revert();

	virtual void excute();

private:
	TimerConnection   m_Time;
};

//--------------修改属性-------------
class RegionAlterAtt : public RegionBase
{
public:
	RegionAlterAtt(const RegionData* data,PlayerRegion* father,BYTE index);

	virtual ~RegionAlterAtt(){}

public:
	virtual void revert();

	virtual void excute();
};

//--------------修改PK模式-------------
class RegionAlterPkMode : public RegionBase
{
public:
	RegionAlterPkMode(const RegionData* data,PlayerRegion* father,BYTE index);

	virtual ~RegionAlterPkMode(){}

public:
	virtual void revert();

	virtual void excute();

private:
	PkType m_OldPKMode;
};



#endif /* REGIONFUN_H_ */
