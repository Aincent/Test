/*
 * HeroInfo.h
 *
 *  Created on: 2016年5月18日
 *      Author: root
 */

#ifndef HEROINFO_H_
#define HEROINFO_H_

#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/Map/HeroInstance.pb.h"
#include "define.h"
#include "CharDefine.h"

using namespace std;
using namespace CommBaseOut;

class Player;
class HeroInfo
{
public:
	HeroInfo(Player* player);
	~HeroInfo();

	void ReInit();
	void Release();

public:
	void InitHeroFromDB(const PlayerInfo::HeroInstanceData& info);
	void SetHeroInfo(PlayerInfo::HeroInstanceData* info);

	void onlineCalculateAtt();

	void GetHeroData(HeroPackage::AckHeroInfo* data);

	int FitchBox(int layer);

	int CreateChecke(int mapid, int layer);

	void PassLayer(int layer);

	int getLayer(){return m_layer;}

private:
	void	GetDataAttList(std::map<ECharAttrType,int64>& tempAttList);
	int CalculateFight();

	int Prize(int layer);

private:
	Player* m_player;
	int m_layer;
	std::vector<int> m_fitchs;
};



#endif /* HEROINFO_H_ */
