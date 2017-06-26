/*				:   exchange.cpp
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */
//				:	玩家间交易数据管理
//////////////////////////////////////////////////////////////////////////

#include "Exchang.h"
#include "msg_Exchang.h"

//-----------------------------------------------------------------------------
// 构造&析构函数
//-----------------------------------------------------------------------------
ExchangeMgr::ExchangeMgr()
{
	m_pExData		= NULL;
	m_dwTgtPlayerKey	= -1;
}

ExchangeMgr::~ExchangeMgr()
{
	SAFE_DEL(m_pExData);
}

//-----------------------------------------------------------------------------
// 创建交易空间 -- 如果已存在，则重新初始化
//-----------------------------------------------------------------------------
void ExchangeMgr::CreateData()
{
	if(NULL == m_pExData)
	{
		m_pExData = new tagExchangeData;
	}

	if( !m_pExData ) return;

	m_pExData		= NULL;
	m_dwTgtPlayerKey	= -1;

	for(size_t i = 0; i < MAX_EXCHANGE_ITEM; i++)
	{
		m_pExData->nSerial[i] = -1;
	}
}

//-----------------------------------------------------------------------------
// 向交易数据结构中添加交易物品 -- 返回 -- 成功:插入位置下标；失败:-1
//-----------------------------------------------------------------------------
uint ExchangeMgr::AddItem(uint nSerial, short n16Num)
{
	if( !m_pExData ) return 0;

	int nInsIndex = -1;
	for(uint i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		if(nSerial < 0)
		{
			return -1;
		}
		else if(m_pExData->nSerial[i] < 0 && -1 == nInsIndex)
		{
			nInsIndex = i;
			break;
		}
	}

	if(nInsIndex != -1)
	{
		m_pExData->nSerial[nInsIndex] = nSerial;
		m_pExData->n16ItemNum[nInsIndex] = n16Num;

		++m_pExData->byTypeNum;
	}

	return nInsIndex;
}

//-----------------------------------------------------------------------------
// 从交易物品中取出指定物品 -- 成功返回0，失败返回相应错误码
//-----------------------------------------------------------------------------
uint ExchangeMgr::DelItem()
{
	if( !m_pExData ) return 0;

	for(uint i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		m_pExData->nSerial[i] = -1;
		--m_pExData->byTypeNum;
		return 0;		//sucess
	}

	return E_Exchange_ItemCanNot_Find;
}
