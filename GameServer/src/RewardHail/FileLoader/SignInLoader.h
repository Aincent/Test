/*
 * SignInLoader.h
 *
 *  Created on: 2014年6月17日
 *      Author: root
 */

#ifndef SIGNINLOADER_H_
#define SIGNINLOADER_H_

#include <map>
#include "CSVLoader.h"

using namespace CommBaseOut;
using namespace std;

typedef struct sSigninStruct
{
	int i_zhenqi;
	int i_money;
	int i_bglod;
	vector<DWORD> m_nAward;			    //奖励物品ID
	vector<DWORD> m_nAwardNum;			//对应奖励物品数量
	int i_chenckInNum;
}SigninStruct;

typedef struct sSigninDailyStruct
{
	DWORD nId;
	DWORD nDay;
	DWORD nAward;
	DWORD nAwardNum;
	DWORD nVip;
	DWORD nBind;

	sSigninDailyStruct():nId(0),nDay(0),nAward(0),nAwardNum(0),nVip(0),nBind(0)
	{

	}

}SigninDailyStruct;

class SignInLoader
{
public:

	~SignInLoader();

	static SignInLoader * GetInstance()
	{
		if(m_instance == 0)
		{
			m_instance = new SignInLoader();
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


	int Init(string & path);
	int InitSignInLoader(string & file);
	int InitSigninDailyInLoader(string & file);

	SigninStruct * GetSignInLoader(int id)
	{
		map<int , SigninStruct>::iterator it = m_signinStruct.find(id);
		if(it == m_signinStruct.end())
		{
			return 0;
		}
		return &it->second;
	}

	const map<int, SigninStruct>& getSigninStruct() const
	{
		return m_signinStruct;
	}

	void GetSigninDailyStruct(int nDaily,SigninDailyStruct*& psigninDailyStruct);

private:
	SignInLoader();

private:

	static SignInLoader * m_instance;
	map<int, SigninStruct> m_signinStruct;
	vector<SigninDailyStruct> m_signinDailyStruct;
};



#endif /* SIGNINLOADER_H_ */
