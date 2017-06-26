/*				:   exchange.h
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */  
//				:	玩家间交易数据管理
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Exchang_define.h"
#include "Goods/PropData.h"
#include "ServerCommonDefine.h"
//-----------------------------------------------------------------------------
// 玩家间交易服务器端数据结构
//-----------------------------------------------------------------------------
struct tagExchangeData
{
	bool	bLock;			// 玩家是否确认交换
	bool	bVerify;		// 玩家是否再次确认交易
	uint	byTypeNum;		// 交易物品种类个数
	bool	bDummy;			// 对齐用	
	uint	nMoney;		// 交易的金钱
	int	nSerial[MAX_EXCHANGE_ITEM];	// 交易的物品，0表示没有交易物品
	short	n16ItemNum[MAX_EXCHANGE_ITEM];	// 待交易物品的个数
};

//-----------------------------------------------------------------------------
// 玩家间交易管理类
//-----------------------------------------------------------------------------
class ExchangeMgr
{
public:
	ExchangeMgr();
	~ExchangeMgr();

public:
	void CreateData();
	void DeleteData();

	uint	AddItem(uint nSerial, short n16Num);	// 返回 -- 成功:插入位置下标；失败:-1
	uint	DelItem();	// 返回错误码
	void	ResetMoney(uint nMoney);

	void	Lock();
	void	Unlock();
	void	Verify();

	void	SetTgtPlayerKey(const int dwTgtPlayerKey);

public:
	int	GetTgtPlayerKey() const;
	uint	GetMoney() const;
	uint	GetItemTypeNum() const;
	int*	GetSerialArray() const;
	short*	GetNumArray() const;
	bool	IsLock() const;
	bool	IsVerify() const;

private:
	tagExchangeData*	m_pExData;
	int				m_dwTgtPlayerKey;	// 目标玩家，GT_INVALID表示无交易目标
};



/*********************** 玩家间交易管理类中内联函数实现 *******************************/

inline void ExchangeMgr::DeleteData()	{ SAFE_DEL(m_pExData); }

//-----------------------------------------------------------------------------
// 设置交易目标
//-----------------------------------------------------------------------------
inline void	ExchangeMgr::SetTgtPlayerKey(const int dwTgtPlayerKey)
{
	m_dwTgtPlayerKey = dwTgtPlayerKey;
}

//-----------------------------------------------------------------------------
// 获取交易数据内容
//-----------------------------------------------------------------------------
inline int ExchangeMgr::GetTgtPlayerKey() const
{
	return m_dwTgtPlayerKey;
}

inline uint ExchangeMgr::GetMoney() const
{
	if( !m_pExData ) return 0;
	return m_pExData->nMoney;
}

inline uint ExchangeMgr::GetItemTypeNum() const
{
	if( !m_pExData ) return 0;
	return m_pExData->byTypeNum;
}

inline int* ExchangeMgr::GetSerialArray() const
{
	if( !m_pExData ) return NULL;
	return m_pExData->nSerial;
}

inline short* ExchangeMgr::GetNumArray() const
{
	if( !m_pExData ) return NULL;
	return m_pExData->n16ItemNum;
}

inline bool ExchangeMgr::IsLock() const
{ 
	if( !m_pExData ) return false;
	return m_pExData->bLock;
}

inline bool ExchangeMgr::IsVerify() const
{
	if( !m_pExData ) return false;
	return m_pExData->bVerify;
}

//-----------------------------------------------------------------------------
// 修改交易数据内容
//-----------------------------------------------------------------------------
inline void ExchangeMgr::ResetMoney(uint nMoney)
{ 
	if( !m_pExData ) return;
	m_pExData->nMoney = nMoney; 
}


inline void ExchangeMgr::Lock()
{ 
	if( !m_pExData ) return;
	m_pExData->bLock = true;
}

inline void ExchangeMgr::Unlock()
{ 
	if( !m_pExData ) return;
	m_pExData->bLock = false;
}

inline void	ExchangeMgr::Verify()
{
	if( !m_pExData ) return;
	m_pExData->bVerify = true;
}
