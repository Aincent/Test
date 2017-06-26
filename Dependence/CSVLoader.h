/*
 * CSVLoader.h
 *
 *  Created on: 2013��9��18��
 *      Author: helh
 */

#ifndef CSVLOADER_H_
#define CSVLOADER_H_


#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"


class CSVLoader
{
protected:
	std::vector< std::vector< std::string > >		m_vecRecordList;	// ��¼�б?������ע���кͿ��У�
	char											m_cSeparator;		// �ֶμ�ķָ��
	std::string										m_szFileName;		// ��Ҫ�����ļ����

public:
	CSVLoader( const char p_cSeparator = ',' )
	: m_cSeparator( p_cSeparator )
	{
	}

	virtual ~CSVLoader()
	{
		__Close();
	}

public:

	//�򿪶�ȡһ���ļ�
	bool OpenFromFile( const char* p_szFileName )
	{
		FILE* pFile = fopen( p_szFileName, "rb" );
		if ( NULL ==  pFile )
		{
			return false;
		}

		fseek( pFile, 0, SEEK_END );
		long lFileSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );

		char* szBuffer = new char[lFileSize];
		fread( szBuffer, 1, lFileSize, pFile );
		fclose( pFile );

		std::string szTemp;
		char cCurrentChar;
		int nStartPos = 0;
		int nCurrentRow = 0;

		bool bPreDel = false;		// ���ǵ�˫б��ע�ͣ��Ӹñ���
		bool bDel = false;

		bool bDQM = false;			// �Ƿ���ǰ��˫���

		// Ԥ����һ���ռ�
		if( lFileSize > 0 )
		{
			std::vector< std::string > newStringVec;
			m_vecRecordList.push_back( newStringVec );
		}

		for( long i = 0; i < lFileSize; ++i )
		{
			cCurrentChar = *( szBuffer + i );

			// ���⴦��"//"����ע�ʹ���
			if( cCurrentChar == '/' )
			{
				if( bPreDel )
				{
					// ȷ�����н���ע��
					bDel = true;
					// �ָ���־
					bPreDel = false;
				}
				else
				{
					bPreDel = true;
				}
			}

			// ���⴦��˫��Ŵ���
			if( cCurrentChar == '"' )
			{
				if( !bDQM )
				{
					nStartPos += 1;
					bDQM = true;
				}
			}


			// �ָ��
			if( cCurrentChar == m_cSeparator )
			{
				// ע�ʹ���
				if( !bDel )
				{
					if( bDQM )
					{
						for( int j = 0; j < i - nStartPos - 1; ++j )
						{
							szTemp.append( szBuffer + nStartPos + j, 1 );
						}
					}
					else
					{
						for( int j = 0; j < i - nStartPos; ++j )
						{
							szTemp.append( szBuffer + nStartPos + j, 1 );
						}
					}
					m_vecRecordList[nCurrentRow].push_back( szTemp );
					szTemp.clear();
				}

				nStartPos = i + 1;
				bDQM = false;
			}
			// ���з�
			else
				if ( cCurrentChar == 10 )
				{
					// ע�ʹ���
					if( !bDel )
					{
						if( bDQM )
						{
							for( int j = 0; j < i - nStartPos -2; ++j )
							{
								szTemp.append( szBuffer + nStartPos + j, 1 );
							}
						}
						else
						{
							for( int j = 0; j < i - nStartPos; ++j )
							{
								szTemp.append( szBuffer + nStartPos + j, 1 );
							}
						}


						// �������һ�еĻ��з�
						szTemp.erase( szTemp.find_last_not_of('\t'), (size_t)(szTemp.find_last_not_of('\t') + 1) );


						m_vecRecordList[nCurrentRow].push_back( szTemp );
						szTemp.clear();
					}

					nStartPos = i + 1;

					// ע�ʹ���
					if( !bDel )
					{
						nCurrentRow++;

						if ( i + 1 < lFileSize )
						{
							// �����µ�һ��
							std::vector< std::string > newStringVec;
							m_vecRecordList.push_back( newStringVec );
						}
					}

					bDel = false;
					bDQM = false;
				}
		}

		delete [] szBuffer;
		return true;
	}

private:
	//��ȡ��Ϻ�ر�һ���ļ�
	void __Close()
	{
		m_vecRecordList.clear();


	}

public:
	bool GetBool( unsigned int p_nRow, unsigned int p_nCol, bool p_bDefineValue = false ) const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return p_bDefineValue;
		}

		std::string szOut = (m_vecRecordList[p_nRow])[p_nCol];

		if( ( strcasecmp( szOut.c_str(), "false" ) == 0 ) || ( strcasecmp( szOut.c_str(), "0" ) == 0 ) )
		{
			return false;
		}
		if( ( strcasecmp( szOut.c_str(), "true" ) == 0 ) || ( strcasecmp( szOut.c_str(), "1" ) == 0 ) )
		{
			return true;
		}
		return p_bDefineValue;
	}

	CommBaseOut::int64	GetInt64( unsigned int p_nRow, unsigned int p_nCol, int p_nDefineValue = 0 )const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return p_nDefineValue;
		}

		const std::string& szOut = (m_vecRecordList[p_nRow])[p_nCol];
		if (szOut.size() == 0)
			return p_nDefineValue;

		return atoll( szOut.c_str() );
	}

	int	GetInt( unsigned int p_nRow, unsigned int p_nCol, int p_nDefineValue = 0 )const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return p_nDefineValue;
		}

		const std::string& szOut = (m_vecRecordList[p_nRow])[p_nCol];
		if (szOut.size() == 0)
			return p_nDefineValue;

		return atoi( szOut.c_str() );
	}

	float GetFloat( unsigned int p_nRow, unsigned int p_nCol, float p_fDefineValue = 0.0f ) const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return p_fDefineValue;
		}

		const std::string& szOut = (m_vecRecordList[p_nRow])[p_nCol];
		if (szOut.size() == 0)
			return p_fDefineValue;

		return (float)atof( szOut.c_str() );
	}

	const std::string& GetString( unsigned int p_nRow, unsigned int p_nCol, const std::string& p_szDefineValue = "" )const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return p_szDefineValue;
		}

		return (m_vecRecordList[p_nRow])[p_nCol];
	}

	void  GetChar(unsigned int p_nRow, unsigned int p_nCol, char* pBuff, int buffLen) const
	{
		if (isValidPos(p_nRow, p_nCol) == false)
		{
			return ;
		}

		const std::string& szOut = (m_vecRecordList[p_nRow])[p_nCol];
		if (szOut.size() != 0)
		{
			strncpy(pBuff,m_vecRecordList[p_nRow][p_nCol].c_str(),buffLen);
		}
		else
		{
			pBuff[0] = 0;
		}
	}

	//�����ļ��ָ��
	void SetSeparator( char p_cSeparator ){ m_cSeparator = p_cSeparator; }

	//�������
	unsigned int GetRowCount() const { return (unsigned int)(m_vecRecordList.size()); }

	//�������
	unsigned int GetColCount() const { return (unsigned int)(m_vecRecordList[0].size());}

protected:
	bool isValidPos(unsigned int p_nRow, unsigned int p_nCol) const
	{
		if (( p_nRow < 0 ) || ( p_nRow >= m_vecRecordList.size() )
			|| ( p_nCol < 0 ) || ( p_nCol >= m_vecRecordList[p_nRow].size() ))
		{
			return false;
		}

		return true;
	}
};


#endif /* CSVLOADER_H_ */
