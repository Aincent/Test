/*
 * SignInLoader.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#include "SignInLoader.h"
#include "StrArgPkg.h"

SignInLoader * SignInLoader::m_instance = 0;

SignInLoader::SignInLoader()
{

}

SignInLoader::~SignInLoader()
{

}

int SignInLoader::Init(string &path)
{
	string fileParam = path + "CheckinGiftTypeData.csv";
	string signinDaily = path + "EverydayCheckinGift.csv";

	if(InitSignInLoader(fileParam) || InitSigninDailyInLoader(signinDaily))
	{
		return -1;
	}

	return 0;
}

int SignInLoader::InitSignInLoader(string & file)
{

	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		SigninStruct info;
		int count = 0;

		int id = loader.GetInt(i, count++);
		info.i_zhenqi = loader.GetInt(i, count++);
		info.i_money = loader.GetInt(i, count++);
		info.i_bglod = loader.GetInt(i, count++);

		//奖励物品ID
		string funStrItem = loader.GetString(i, count++, "");
		StrArgPkg funPkgItem("|", funStrItem);
		funPkgItem.setArgs(funStrItem);

		for(uint itemSize=0; itemSize<funPkgItem.size(); ++itemSize)
		{
			StrArgPkg limitPkg(":", getArg(funPkgItem, itemSize).c_str());
			for(unsigned int i=0; i<limitPkg.size(); ++i)
			{
				info.m_nAward.push_back(atoi(getArg(limitPkg, i++).c_str()));
				info.m_nAwardNum.push_back(atoi(getArg(limitPkg, i).c_str()));
			}
		}

		info.i_chenckInNum = loader.GetInt(i, count++);
		if(info.i_chenckInNum <=0)
			continue;

		m_signinStruct[id] = info;
	}
	return 0;
}

int SignInLoader::InitSigninDailyInLoader(string & file)
{
	CSVLoader loader;

	if(!loader.OpenFromFile(file.c_str()))
	{
		return -1;
	}

	for(unsigned int i=0; i<loader.GetRowCount(); ++i)
	{
		SigninDailyStruct info;
		int count = 0;

		count++;
		info.nId = loader.GetInt(i, count++);
		info.nDay = loader.GetInt(i, count++);
		info.nAward = loader.GetInt(i, count++);
		info.nAwardNum = loader.GetInt(i, count++);
		info.nVip = loader.GetInt(i, count++);
		info.nBind = loader.GetInt(i, count++);

		m_signinDailyStruct.push_back(info);
	}

	return 0;
}

void SignInLoader::GetSigninDailyStruct(int nDaily,SigninDailyStruct*& psigninDailyStruct)
{
	time_t t = 0;
	struct tm local;
	time(&t);
	localtime_r(&t,&local);

	DWORD type = 0;

	if((local.tm_mon+1) % 2 == 0)
	{
		type = 2;
	}
	else
	{
		type = 1;
	}

	vector<SigninDailyStruct>::iterator itr =  m_signinDailyStruct.begin();
	for(; itr != m_signinDailyStruct.end(); ++itr)
	{
		if(itr->nId == type && itr->nDay == (DWORD)nDaily)
		{
			psigninDailyStruct = &(*itr);
			break;
		}
	}
}


