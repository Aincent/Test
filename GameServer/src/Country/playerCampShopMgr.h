/*
 * playerCampShopMgr.h
 *
 *  Created on: 2015年3月20日
 *      Author: root
 */

#ifndef PLAYERCAMPSHOPMGR_H_
#define PLAYERCAMPSHOPMGR_H_
#include "define.h"
#include <map>
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
using namespace std;
using namespace CommBaseOut;

class Player;
class PlayerCampShopMgr
{
public:
		PlayerCampShopMgr(Player *player);
		~PlayerCampShopMgr();

		void ReInit();
		void Release();
public:
		int getPropAttr(int id);
		void initShop(const PlayerInfo::CampShopInfo& data);
		void setPropAttr(int id, int count);
		void reSetAttr();

		void online(const PlayerInfo::CampShopInfo& data);
		void getShopData(PlayerInfo::CampShopInfo& data);

		// 是否有能买购买的商品
		bool IsHaveCanBuy();
private:
		bool isUpdate(DWORD64 time);
private:
		std::map<int,int> m_shopAttr;
		int64 m_updateTime;
		Player * m_player;
};


#endif /* PLAYERCAMPSHOPMGR_H_ */
