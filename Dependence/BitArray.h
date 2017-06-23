/*
 * BitArray.h
 *
 *  Created on: 2013��10��30��
 *      Author: helh
 */

#ifndef BITARRAY_H_
#define BITARRAY_H_

#include "define.h"

using namespace CommBaseOut;

class CBitArray
{
public:
	CBitArray();

	~CBitArray();

	void Init();
	void Release();

	//���ô�С
	void SetSize(int nSize);

	//����ĳ��λ��ֵ
	void SetBit(DWORD nIndex, bool bViald);

	//��ȡĳ��λ��ֵ
	bool GetBit(DWORD nIndex);

	//�������
	void SetData(const unsigned char *pData, int nDataSize);

	//��ȡ���
	const unsigned char* GetData(int &nDataSize);

	bool operator[](DWORD index);

private:
	BYTE*	m_pBitArray;			//�洢�ռ�
	DWORD	m_nSize;				//�����С
	DWORD	m_nDataDize;			//��ݴ�С
};


#endif /* BITARRAY_H_ */
