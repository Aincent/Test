/*
 * PropData.h
 *
 *  Created on: 2014年4月8日
 *      Author: helh
 */

#ifndef PROPDATA_H_
#define PROPDATA_H_

#include "define.h"
#include "PropDefine.h"
#include "../MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "../MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "../FileLoader/GoodsLoader.h"
#include "../MessageStruct/CharLogin/GoodsMessage.pb.h"

using namespace CommBaseOut;


// 道具基类
class PropBase
{
public:
	enum
	{
		Type = 1, // 类别
	};

	PropBase(int id, BYTE count = 1) : /*m_serialId(0),*/ m_id(id), m_flag(0), m_count(count)
	{

	}
	PropBase() : /*m_serialId(0),*/ m_id(-1), m_flag(0), m_count(0)
	{

	}

	virtual ~PropBase()
	{

	}

	virtual BYTE GetType() const
	{
		return PropBase::Type;
	}

	// 数量减少一个
	inline bool SubAmount()
	{
		if (m_count != 0)
		{
			if(--m_count <= 0)
			{
				//m_id = -1;
				SetEmpty();
			}

			return true;
		};

		return false;
	}

	// 数量减少一个
	inline bool SubAmount(int Num)
	{
		if(Num <=0)
			return false;

		if(Num > m_count)
		{
			return false;
		}
		else
		{
			m_count -= Num;
			if(m_count <= 0)
			{
				//m_id = -1;
				SetEmpty();
			}
			return true;
		}
	}

	// 设置数量
	inline void SetAmount(BYTE count)
	{
		m_count = count;
		if(m_count <= 0)
		{
			SetEmpty();
		}
	}

	// 得到数量
	inline BYTE GetAmount() const
	{
		return m_count;
	}

	// 标识操作
	inline bool IsFlag(ePropFlag fg) const
	{
		return (m_flag & fg) == fg;
	}

	WORD GetFlag()
	{
		return m_flag;
	}

	inline void SetFlag(WORD flag)
	{
		m_flag = flag;
	}

	inline void SetFlag(ePropFlag fg)
	{
		m_flag |= fg;
	}

	inline void ClearFlag(ePropFlag fg)
	{
		m_flag &= (~ fg); 
	}

	// 是否绑定
	inline bool IsBind() const
	{ 
		return IsFlag(ePropBind);
	}

	// 设置是否绑定
	inline void SetBind(bool value = true)
	{
		value ? SetFlag(ePropBind) : ClearFlag(ePropBind);
	}
	// 是否使用绑定
	inline bool IsUseBind() const
	{
		return IsFlag(ePropUseBind);
	}
	inline void SetUseBind(bool value)
	{
		value ? SetFlag(ePropUseBind) : ClearFlag(ePropUseBind);
	}
	inline bool IsSameBindType(ePropBindType emBindType) const
	{
		if ((IsBind() && emBindType == eGetBind) || (!IsBind() && emBindType == eNullBind))
			return true;
		else
			if (IsUseBind() && emBindType == eUseBind)
				return true;
		return false;
	}
	inline ePropBindType GetBindType() const
	{
		if ( IsBind() )
			return eGetBind;
		else if ( IsUseBind() )
			return eUseBind;
		else 
			return eNullBind;
	}

	// 设置为空
	inline void SetEmpty()
	{
		m_id    = -1;
		m_count = 0;
		m_flag  = 0;
	}

	//是否为无效
	inline bool IsValid()
	{
		return (m_id <= 0 ? true : false);
	}

	// 判断是否一至
	virtual bool IsSame(int id, int flag)
	{
		return ((m_id == id) && (flag == m_flag));
	}

	//设置当前的id
	inline void SetID(int id)
	{
		m_id = id;
	}

	// 得到当前的ID
	inline int GetID() const
	{
		return m_id;
	}

//	// 得到序列号ID
//	inline const DWORD GetSerialID() const
//	{
//		return m_serialId;
//	}

	inline int GetLeftPileNum()
	{
		const GoodsInfo* ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(m_id);
		if(ginfo==NULL)
			return 0;
		return ginfo->stack - m_count;
	}

	inline int GetMaxPileNum()
	{
			const GoodsInfo* ginfo = GoodsLoader::GetInstance()->GetItemDataByID(m_id);
			if(ginfo==NULL) return 0;
			return ginfo->stack;
	}

	inline int GetItemQuality()
	{
		const GoodsInfo *ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(m_id);
		if(ginfo==NULL) return 0;
		return ginfo->itemQuality;
	}

	inline BYTE GetItemType()
	{
		const GoodsInfo *ginfo =	GoodsLoader::GetInstance()->GetItemDataByID(m_id);
		if(ginfo==NULL) return 0;
		return ginfo->type;
	}

	virtual void SetClientInfo(CharLogin::ClientItemInfo *info)
	{
		info->set_id(m_id);
		if(m_id > 0 && m_count > 0)
		{
			info->set_flag(((m_flag & ePropBind) << 16) | (m_count & 0xff));
		}
		else
		{
			info->set_id(-1);
		}
	}


	virtual void SetClientInfo(CharLogin::ClientItemInfo *info, int count)
	{
		info->set_id(m_id);
		if(m_id > 0 && m_count > 0 && count > 0 && m_count >= count )
		{
			info->set_flag(((m_flag & ePropBind) << 16) | (count & 0xff));
		}
		else
		{
			info->set_id(-1);
		}
	}



	virtual void SetInfo(PlayerInfo::ItemInfo *info)
	{
		info->set_id(m_id);
		if(m_id > 0 && m_count > 0)
		{
			info->set_flag(((m_flag & ePropBind) << 16) | (m_count & 0xff));
		}
		else
		{
			info->set_id(-1);
		}
	}

	virtual void SetServerInfo(const PlayerInfo::ItemInfo *info)
	{
		int count = info->flag() & 0xff;

		if(m_id > 0 && count > 0)
		{
			m_count = info->flag() & 0xff;
			SetBind(info->flag() >> 16);
		}
		else
		{
			m_id = -1;
		}
	}

	virtual void CreateItem(const GoodsInfo &info)
	{
		if(eGetBind == info.bindTyle)
		{
			SetBind();
		}
	}

protected:

//	DWORD m_serialId; // 道具的序列号
	int m_id;     // 类型ID
	WORD m_flag;   // 物品的标识，是否绑定等布尔值变量
	BYTE m_count;  // 此物品的数量
};



#endif /* PROPDATA_H_ */
