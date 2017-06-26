/*
 * MaskWordConfig.h
 *
 *  Created on: 2013��9��18��
 *      Author: root
 */

#ifndef MASKWORDCONFIG_H_
#define MASKWORDCONFIG_H_

#include <vector>
#include <string>
#include <regex.h>
#include "CSVLoader.h"

using namespace std;

class CMaskWordConf
{
public:
	~CMaskWordConf();

	static CMaskWordConf *GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new CMaskWordConf();
		}

		return m_instance;
	}

	void DestroyInstance()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}
	}

	int LoadFile(const string str);

	bool IsMaskWord(string str);

	void InitRegex();

	int CheckMailName(string str);

	int CheckMailTheme(string str);

	int CheckSubstance(string str);

	//bool CanFindMaskWord(string str);

private:
	CMaskWordConf();

private:
	static CMaskWordConf * m_instance;

	vector<string> m_maskWord;

	regex_t m_reg; //先声明一个正则表达式体。
};


#endif /* MASKWORDCONFIG_H_ */
