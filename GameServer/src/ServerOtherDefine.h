/*
 * ServerOtherDefine.h
 *
 *  Created on: 2015年5月26日
 *      Author: root
 */

#ifndef SERVEROTHERDEFINE_H_
#define SERVEROTHERDEFINE_H_

#include <map>
#include "Smart_Ptr.h"
#include "CharDefine.h"
#include "hashMap/hash_map"

using namespace __gnu_cxx;
using namespace CommBaseOut;

struct sCDTime;
class Player;
struct GroupInfo;
struct CGameDuplicate;
struct CGameMarry;
struct sReqPathInfo;
struct Monster;
struct Object;
struct GirlWar;
struct NPC;
struct CGameZone;
struct PLayerCache;

typedef map<int, sCDTime> 	CDTIMEMAP;
typedef map<int64, Smart_Ptr<Player> > 	INT64PLAYERMAP;
typedef map<int64, int64> INT64INT64MAP;
typedef map<int64, Smart_Ptr<GroupInfo> > INT64GROUPINFOMAP;
typedef map< int64, Smart_Ptr<CGameDuplicate> > INT64GAMEDUPLICATEMAP; //存在的副本
typedef map< int64, Smart_Ptr<CGameMarry> > INT64GAMEDMARRY; //存在的副本
typedef map<int64, map<int, sReqPathInfo> > INT64MREQPATHINFOMAP;
typedef map<int, vector<Smart_Ptr<Monster> > > INTVMPNSTERMAP;
typedef map<int, Smart_Ptr<Object> > INTOBJECTMAP;
typedef map<int,Smart_Ptr<GirlWar> > INTGRILWARMAP;
typedef map<int, Smart_Ptr<NPC> > INTNPCMAP;
typedef map<int64, Smart_Ptr<CGameZone> > INT64CGAMEZONEMAP;   //�� Ϊ����id
typedef map<int, pair<int, int64> > WORLDBOSSZONEMAP; //世界BOSS出生所在的场景 boss的key -- CGameZone
typedef map<int64, PLayerCache > INT64PLAYERCACHEMAP;
typedef map<int64, map<int, int> > INT64MAPINTINT;
typedef map<int64, vector<int64> > INT64VINT64MAP;

#endif /* SERVEROTHERDEFINE_H_ */
