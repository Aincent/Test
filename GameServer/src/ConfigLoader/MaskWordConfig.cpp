/*
 * MaskWordConfig.cpp
 *
 *  Created on: 2013��9��18��
 *      Author: helh
 */

#include "MaskWordConfig.h"
#include "../Dependence/Ciconv/Ciconv.h"
#include "MessageCommonRet.h"
#include "CharDefine.h"

CMaskWordConf * CMaskWordConf::m_instance = 0;

CMaskWordConf::CMaskWordConf()
{
	InitRegex();
}

CMaskWordConf::~CMaskWordConf()
{
	regfree(&m_reg);
	m_maskWord.clear();
}

int CMaskWordConf::LoadFile(const string str)
{
	CSVLoader loader('	');

	if(!loader.OpenFromFile(str.c_str()))
	{
		return -1;
	}

	int index = 0;

	for(unsigned int i=0; i<loader.GetColCount(); ++i)
	{
		int flag = loader.GetInt(0, index);
		//表的第一行用来标识用什么语言，遇到不为零则读取此列的文字
		if(flag > 0)
			break;
		index++;
	}

	for(unsigned int i=1; i<loader.GetRowCount(); ++i)
	{
		string maskName = loader.GetString(i, index, "");
		if(maskName.compare(" ")==0 || maskName.compare("")==0)
			break;
		m_maskWord.push_back(maskName);
	}

	m_maskWord.push_back("/");
	m_maskWord.push_back("\\");

	return 0;
}

void CMaskWordConf::InitRegex()
{
	string str = "[\\]";//需要屏蔽的特殊字符
	regcomp(&m_reg,str.c_str(),REG_EXTENDED); //从字符串来编译这个表达式。
}

bool CMaskWordConf::IsMaskWord(string str)
{
	int ret = regexec(&m_reg,str.c_str(),0,NULL,0); //匹配字符串
	if(ret == 0)
	{
		return true;
	}

	if(m_maskWord.size() <= 0)
		return true;

	int i = 1;
	vector<string>::iterator it = m_maskWord.begin();
	for(; it!=m_maskWord.end()-1; ++it)
	{
		const char * result = strstr(str.c_str(),(*it).c_str());
		if(result != NULL)
		{
			return true;
		}
		i++;
	}

	return false;
}

int CMaskWordConf::CheckMailName(string str)
{
	int nameLen = CIconv::GetUTF8Count(str.c_str());
	if(nameLen <= 0)
	{
		return eMailNameNull;
	}

	if(nameLen > MAX_NAME_LEN)
	{
		// 名字不能太长
		return eNameTooLong;
	}

	//Table符
	size_t pos = str.find('	');
	if(pos != str.npos)
	{
		return eNameInvalid;
	}

	//空格符在名字的首位置或末尾
	pos = str.find(' ');
	if(0 == pos || (str.length() - 1) == pos)
	{
		return eNameInvalid;
	}

	return 0;
}

int CMaskWordConf::CheckMailTheme(string str)
{
	int nameLen = CIconv::GetUTF8Count(str.c_str());
	if(nameLen <= 0)
	{
		return eMailThemeNull;
	}

	if(nameLen > MAX_THEME_LEN)
	{
		// 不能太长
		return eMailThemeTooLong;
	}

	//Table符
	size_t pos = str.find('	');
	if(pos != str.npos)
	{
		return eMailThemeInvalid;
	}

	//首尾不能使用空格符
	pos = str.find(' ');
	if(0 == pos || (str.length() - 1) == pos)
	{
		return eMailThemeInvalid;
	}

	return 0;
}

int CMaskWordConf::CheckSubstance(string str)
{
	int nameLen = CIconv::GetUTF8Count(str.c_str());

	if(nameLen <= 0)
	{
		return eCantSendEmptyTextMail;
	}

	if(nameLen > MAX_SUBSTANCE_LEN)
	{
		// 不能太长
		return eMailSubstanceTooLong;
	}

	return 0;
}
