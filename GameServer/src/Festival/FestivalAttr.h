/*
 * FestivalAttr.h
 *
 *  Created on: 2015年8月25日
 *      Author: root
 */

#ifndef FESTIVALATTR_H_
#define FESTIVALATTR_H_

#include "define.h"
#include <map>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
using namespace std;
using namespace CommBaseOut;

class Player;
class PlayerFestiveAttr
{
public:
		PlayerFestiveAttr(Player *player);
		~PlayerFestiveAttr();

		void ReInit();
		void Release();
public:
		int 	getAttr(int id);
		void attrAdd(int id);
		void initAttr(const PlayerInfo::FestivalInfo& data);
		void reSetAttr();

		void online(const PlayerInfo::FestivalInfo& data);
		void getFestiveAttrData(PlayerInfo::FestivalInfo& data);
private:
		bool isUpdate(DWORD64 time);
private:
		std::map<int,int> m_FestiveAttr;
		int64 m_updateTime;
		Player * m_player;
};



#endif /* FESTIVALATTR_H_ */
