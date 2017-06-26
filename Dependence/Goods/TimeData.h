/*
 * TimeData.h
 *
 *  Created on: 2014年4月8日
 *      Author: helh
 */

#ifndef TIMEDATA_H_
#define TIMEDATA_H_

#include "define.h"
#include "PropData.h"
#include "util.h"
#include "../MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "../MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "../FileLoader/GoodsLoader.h"
#include "../MessageStruct/CharLogin/GoodsMessage.pb.h"

using namespace CommBaseOut;


// 时间类道具
class TimeProp : public PropBase
{
public:
	TimeProp(int id, int count = 1, int64 time=0) : PropBase(id, count), m_dwTime(time)
	{}

	enum
	{
		Type = 3,
	};

	virtual BYTE GetType() const
	{
		return TimeProp::Type;
	}

public:
	virtual void SetServerInfo(const PlayerInfo::ItemInfo *info)
	{
		int count = info->flag() & 0xff;
		if(m_id > 0 && count > 0)
		{
			m_count = info->flag() & 0xff;
			SetBind(info->flag() >> 16);
			m_dwTime = info->endtime();
		}
		else
		{
			m_id=-1;
		}
	}

	virtual void SetClientInfo(CharLogin::ClientItemInfo *info)
	{
		info->set_id(m_id);
		if(m_dwTime >= 0 && m_id > 0 && m_count > 0)
		{
			info->set_time(m_dwTime);
			info->set_flag(((m_flag & ePropBind) << 16) | (m_count & 0xff));
			int64 nowtime =(int64)(CUtil::GetNowSecond()*0.001);
			if(m_dwTime !=0 && nowtime>=m_dwTime)
			{
				info->set_id(-1);
				m_id = -1;
			}
		}
		else
		{
			info->set_id(-1);
		}
	}

	virtual void SetInfo(PlayerInfo::ItemInfo *info)
	{
		info->set_id(m_id);
		if(m_dwTime >= 0 && m_id > 0 && m_count > 0)
		{
			info->set_endtime(m_dwTime);
			info->set_flag(((m_flag & ePropBind) << 16) | (m_count & 0xff));
			int64 nowtime = (int64)(CUtil::GetNowSecond()*0.001);
			if(nowtime >= m_dwTime && m_dwTime > 0)
			{
				info->set_id(-1);
				m_id = -1;
			}
		}
		else
		{
			info->set_id(-1);
			m_id = -1;
		}
	}

	virtual void CreateItem(GoodsInfo &info)
	{
		if(eGetBind == info.bindTyle)
		{
			SetBind();
		}
	}

	inline int64 GetDwTime()
	{
		return m_dwTime;
	}

	inline void SetDwTime(int64 dwTime)
	{
		m_dwTime= dwTime;
	}

protected:
	int64 m_dwTime;	//0为永久道具; 否则为到期时间
};





#endif /* TIMEDATA_H_ */
