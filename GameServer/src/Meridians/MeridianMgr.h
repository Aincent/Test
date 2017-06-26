/*
 * MeridianMgr.h
 *
 *  Created on: 2014年6月5日
 *      Author: root
 */

#ifndef MERIDIANMGR_H_
#define MERIDIANMGR_H_

#include "Network/MessageHandler.h"
#include "MsgDefineMacro.h"
#include "ServerMsgDefine.h"
#include "../QuickMessage.h"
#include "../MessageBuild.h"
#include "Smart_Ptr.h"
#include "../Dependence/MessageStruct/Meridians/meridians.pb.h"

using namespace std;
using namespace CommBaseOut;

#define MAX_OPEN_MERIDIAN_LV 18

enum eMeridianType
{
	eMeridianTypeOne 		  = 0,
	eMeridianTypeGoldOne  		  = 1,
	eMeridianTypeNormalAuto   = 2,
	eMeridianTypeGoldenAuto = 3,
};

class MeridianMgr : public Singleton<MeridianMgr>
{
public:
	MeridianMgr();

	~MeridianMgr();
public:
	//请求打开经脉
	DEF_MSG_QUICK_REQUEST_DECLARE_FUN_H(MSG_REQ_C2GT_OPENMERIDIANS);

	//接口的count变成冲穴的类型
	int OpenMeridians(Smart_Ptr<Player> player, int acupoint,  int isGolden, eMeridianType count,  Meridians::RetOpenMeridians& ret);

	void getBforeIDAllAtt(map<ECharAttrType,int64>& attList,int id,ECharProf profession);

	// 当前人物等级的经脉是否是最大等级
	bool IsAcupointMaxByLv(Smart_Ptr<Player> player);

	bool IsCanAdvance(Smart_Ptr<Player> player);
protected:
	int CheckAdvanceCond(Smart_Ptr<Player> player);
};



#endif /* MERIDIANMGR_H_ */
