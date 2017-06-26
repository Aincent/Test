/*
 * OrderMgr.h
 *
 *  Created on: 2014年6月27日
 *      Author: root
 */

#ifndef ORDERMGR_H_
#define ORDERMGR_H_
#include "Singleton.h"
#include "string.h"
#include "../MsgCommonClass.h"
#include "../Object/Player/Player.h"
using namespace CommBaseOut;
using namespace std;

class OrderMgr : public Singleton<OrderMgr>
{
public:
	OrderMgr();
	~OrderMgr();
	//命令
	bool OrderPlayer(Safe_Smart_Ptr<CommBaseOut::Message> &message, Smart_Ptr<Player> &player,string str);
};



#endif /* ORDERMGR_H_ */
