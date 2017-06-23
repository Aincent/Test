/*
 * BitArray.cpp
 *
 *  Created on: 2013��10��30��
 *      Author: helh
 */

#include "BitArray.h"
#include "util.h"

unsigned char g_BitArray[8] =
{
	1,1<<1,1<<2,1<<3,
		1<<4,1<<5,1<<6,1<<7
};

CBitArray::CBitArray():m_pBitArray(0),m_nSize(0),m_nDataDize(0)
{
	m_pBitArray = new BYTE[1];
	m_nSize = 8;
}

CBitArray::~CBitArray()
{
	Release();
}

void CBitArray::Init()
{
	if(m_pBitArray != NULL)
	{
		Release();
	}

	m_pBitArray = new BYTE[1];
	m_nDataDize = 0;
	m_nSize = 8;
}

void CBitArray::Release()
{
	if(m_pBitArray != NULL)
	{
		delete[] m_pBitArray;
		m_pBitArray = NULL;
	}
}

void CBitArray::SetSize(int nSize)
{
	if (nSize < 8)
	{
		return;
	}

	if(m_pBitArray)
	{
		delete[] m_pBitArray;
	}

	if (nSize % 8 != 0)
	{
		m_nDataDize = (nSize >> 3) + 1;
	}
	else
	{
		m_nDataDize = nSize >> 3;
	}

	//m_pBitArray = new unsigned char[m_nDataDize];
	m_pBitArray = new BYTE[m_nDataDize];
	memset(m_pBitArray, 0, m_nDataDize);
	m_nSize = nSize;
}

void CBitArray::SetBit(DWORD nIndex, bool bViald)
{
	if (nIndex >= m_nSize)
	{
		return;
	}

	unsigned int nArrayIndex = nIndex >> 3;
	unsigned int nMod = nIndex % 8;

	if (bViald)
	{
		m_pBitArray[nArrayIndex] |= g_BitArray[nMod];
	}
	else
	{
		m_pBitArray[nArrayIndex] &= ~g_BitArray[nMod];
	}
}

bool CBitArray::GetBit(DWORD nIndex)
{
	if (nIndex >= m_nSize)
	{
		return 0;
	}

	unsigned int nArrayIndex = nIndex >> 3;
	unsigned int nMod = nIndex % 8;

	return ((m_pBitArray[nArrayIndex] & g_BitArray[nMod]) != 0);
}

void CBitArray::SetData(const unsigned char *pData, int nDataSize)
{
	SetSize(nDataSize << 3);
	CUtil::SafeMemmove(m_pBitArray, m_nDataDize, pData, nDataSize);
}

const unsigned char* CBitArray::GetData(int &nDataSize)
{
	nDataSize = m_nDataDize;
	return m_pBitArray;
}

bool CBitArray::operator[](DWORD index)
{
	if(index > m_nSize)
		return false;
	int dataIndex = index >> 3;
	int bitIndex	= index % 8;

	return ((m_pBitArray[dataIndex] & g_BitArray[bitIndex]) != 0);
}


