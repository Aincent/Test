/*
 * Player.h
 *
 *  Created on: 2013��9��27��
 *      Author: helh
 */



#ifndef PLAYER_H_
#define PLAYER_H_
 
#include <map>
#include "../Creature.h"
#include "MessageStruct/Map/SynchPath.pb.h"
#include "MessageStruct/CharLogin/PlayerInfo.pb.h"
#include "MessageStruct/CharLogin/CharBaseInfo.pb.h"
#include "Smart_Ptr.h"
#include "util.h"
#include "MessageStruct/CharBattle/Battle.pb.h"
#include "MessageStruct/CharBase/CharAttribute.pb.h"
#include "MessageStruct/Quest/QuestMessage.pb.h"
#include "../../QuestNew/QuestDefine.h"
#include "CharDefine.h"
#include "../../CDTime/CDMgr.h"
#include "../../Skill/SkillManager.h"
#include "../FileLoader/BattleInfoLoader.h"
#include "../FileLoader/MakeTypeLoader.h"
#include "../../QuestNew/QuestPart.h"
#include "../../Container/ContainerBase.h"
#include "SvrConfig.h"
#include "./google/protobuf/message.h"
#include "FileLoader/GoodsLoader.h"
#include "MessageStruct/CharLogin/GoodsMessage.pb.h"
#include "../../Trade/Exchang.h"
#include "../../Map/ConfigLoader/MapConfig.h"
#include "../../MapInstance/InstanceMgr.h"
#include "../../Achievement/Achievement.h"
#include "../../Destination/Destination.h"
#include "../../Daily/Daily.h"
#include "../../Achievement/PlayerTitle.h"
#include "../../RewardHail/SignIn.h"
#include "../../Counter/Counter.h"
#include "../../RewardHail/OnlineReward.h"
#include "../../RewardHail/LevelReward.h"
#include "../../RewardHail/TreasonKingReward.h"
#include "../../VIP/PlayerVipPart.h"
#include "../../EventSet/EventDefine.h"
#include "../GirlWar/GirlWar.h"
#include "../../Chat/ChatMgr.h"
#include "../../Fashion/Fashion.h"
#include "../../Group/NewGroupMgr.h"
#include "../../Vitality/VitalityMgr.h"
#include "OpenApiV3.h"
#include "StatisticBase/StatisticDefine.h"
#include "../../ActiveUIHighLight/CActiveHighLightMgr.h"
#include "../../Trade/BaseShop.h"
#include "../../Activity/AnswerMgr.h"

#define check_prop_time 300000  //时限道具检测时间

using namespace std;
using namespace CommBaseOut;

class QuestPart;
class Monster;
class HorseMgr;
class MailMgr;
class DanLuMgr;
class MagicMgr;
class SMagicMgr;
class HeartMagicMgr;
class CJobLuckyMgr;
class LucklyTurntableMgr;
class AntiAddictionSystemMgr;
class ActivityMgr;
class GirlWarManager;
class EscortMgr;
class PlayerCampShopMgr;
class CountryRedEnvelopeMgr;
class ExtraAtributeMgr;
class Fighting;
class PlayerFestiveAttr;
class OnceReward;
class MarryRing;
class PlayerMeditation;
//class ArenaMgr;
class PlayerRegion;
class PlayerDistortion;
class ChristmasManage;
class NewYearActivity;
class ArenaMgr;
class PlayerChallenge;
class HeroInfo;

struct SaveDataInfo
{
	SaveDataInfo()
	{
		msgtype = 0;
		msgid = 0;
		count = 0;
		message = NULL;
		content = NULL;
		m_act   = NULL;
	}
	int msgtype;//消息类型
	int msgid;//消息ID
	Safe_Smart_Ptr<CommBaseOut::Message> message;//与通讯对方的信息
	Smart_Ptr<google::protobuf::Message> content;//消息内容
	Safe_Smart_Ptr<NullBase> m_act;
	int count;//次数
};

class Player : public CreatureObj
{
	friend class PlayerVipPart;
public:
	Player(eObjType type);
	~Player();

	void InitEvent();
	void ReInitPlayer();
	void ClearData();
	void ReleasePlayer();

	//取消打坐双修
	void CanccelMeditation();

	int AddExp(int exp);
	int64 AddExp(int exp, double ration);
	int64 DecreaseExp(int exp);
	//减少玩家经验：exp -= 当前等级升级所需要经验 ＊  ratio
	int DecreaseExp(float ratio);
	//战斗属性改变
	void ChangeBattleAttr(int endLv);
	//金钱改变，并且会自动同步
	void MoneyChange(int money, bool type = true);
	//金钱改变，不会同步到客户端，需要手动同步
	int ChangeMoney(int money, bool isAdd = true);
	int ChangeGolden(int value, bool isAdd = true);
	int ChangeGoldenCost(int value, bool isAdd = true);

	int ChangeBindGolden(int value, bool isAdd = true);

	void ChangeChallenge(int64 value, bool isAdd);

	//优先扣除绑定元宝
	bool CostGolden(int cost);
	//判断是否有足够的元宝和绑定元宝
	bool IsEnoughGolden(int cost);
	//扣除绑定元宝和元宝（优先扣除帮定元宝）,golden和bgolden为返回消耗的元宝和消耗的绑定元宝
	//调用之前必须先判断是否足够
	bool DecreaseGoldenCost(int cost, int &golden, int &bgolden);

	int ChangeCredit(int value, bool isAdd = true);

	bool GetSex() { 	return m_sex; }
	void SetSex(bool sex) { 	m_sex = sex; }

	int ChangeFContri(int value, bool isAdd = true);

	string & GetClanName() {	 return m_clanName; }
	void SetClanName(const string &name) { 	m_clanName = name; }

	int ChangeCountryContribution(int value, bool isAdd = true, bool sendtoWorld=true);

	//周贡献度
	int getWeekContrituion(){return m_cContribution_Week;}
	void setWeekContrituion(int value){ m_cContribution_Week = value;}

	BYTE getFitchFlag(){return m_FitchPrize;}
	void setFitchFlag(BYTE flag){m_FitchPrize=flag;}

	BYTE getKingRankFlag(){return m_kingRankFlag;}
	void setKingRankFlag(BYTE flag){ m_kingRankFlag=flag; }

	BYTE GetAccountType() { 	return m_accType; }
	void SetAccountType(BYTE type) { 	m_accType = type; }

	string & GetOpenID() { 	return m_openID; }
	void SetOpenID(string &openid) { 	m_openID = openid; }

	string& GetOpenKey() { return m_openKey;}
	void SetOpenKey(string &openkey) {m_openKey = openkey;}

	string& GetPf() {return m_pf;}
	void SetPf(string &pf){m_pf = pf;}

	string& GetPfKey() { return m_pfKey;}
	void SetPfKey(string& pfKey) {m_pfKey = pfKey;}

	string& GetCoupleName(){return m_sCoupleName;}
	void SetCoupleName(const string& coupleName){m_sCoupleName = coupleName;}

	virtual int CurHPChange(int hp, bool flag = true);
	virtual int ChangeHPMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual void SetCurHP(int hp);
	virtual void SetHPMax(int max);

	virtual int ChangeMPMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangeCurMP(int value, bool isAdd = true);
	virtual void SetMPMax(int max)
	{
		CreatureObj::SetMPMax(max);
		m_dirtyData[eBattleInfo] = true;
	}
	virtual void SetCurMP(int mp)
	{
		CreatureObj::SetCurMP(mp);
		m_dirtyData[eBattleInfo] = true;
	}

	int ChangeCurForce(int value, bool isAdd = true);

	virtual void AddStrength(int value);

	virtual void AddCleverPower(int value);

	virtual void AddCleverAir(int value);

	virtual void AddPhyPower(int value);
	virtual void AddPatience(int value);

	virtual void AddAgility(int value);

	int ChangePower(int value, bool isAdd = false);

	int ChangeFightPower(int value, bool isAdd = true);

	void SetOldPkType(int pkType)
	{
		m_oldPkType = pkType;
		m_dirtyData[eBaseInfo] = true;
	}

	int GetOldPkType()
	{
		return m_oldPkType;
	}

	int GetPkTypeByMap(int pkType);

	void SetPkTypeByMap(int pkType);

	void AddContinueBeheaded();
	//红名玩家一定几率掉落非绑定装备和物品
	bool RedNamePlayerDropEquipAndGoods(Goods::SimDropSomeGoods & dropGoods,std::vector<PropBase> &dropList);
	//罪恶值
	void AddSinValue(int sinValue)
	{
		AddSinValue((float)sinValue);
	}

	//罪恶值
	void AddSinValue(float sinValue);

	//杀死怪物或被其它玩家杀死降低罪恶值
	void ReduceSinValue(Smart_Ptr<CreatureObj> &obj);

	void ReduceSinValue(int sinValue)
	{
		ReduceSinValue((float)sinValue);
	}

	void ReduceSinValue(float sinValue);

	int GetSinState()
	{
		return this->m_sinState;
	}

	//经脉穴位
	int GetAcupoint()
	{
		return m_acupoint;
	}

	void AddAcupoint(int acupoint);
	//系统设置
	void SetSettings(int value , int type);
	int GetSettings(int type);
	void GetSettings(PlayerInfo::SettingInfo *info);

	void SetPrestige(uint value);

	void AddPrestige(int value, bool add = true);



	void UpdateSinValue(void * obj);

//	void UpateStastic();

	Smart_Ptr<QuestPart> & GetQuestMgr() { 	return m_quest; }

	void SetMyself(const Smart_Ptr<Player> &player)
	{
		if(!player)
		{
			RemoveTimerInterface();
			cout<<endl<<"Player Set Myself is NULL,Player is %lld"<<GetID();
		}

		m_myself = player;
	}

	Smart_Ptr<Player> &GetMyself() { 	return m_myself; }
	Smart_Ptr<CreatureObj> GetCreatureSelf() { 	return m_myself; }

	void SetChannelID(int channelID) {	 m_channelID=channelID; }
	int GetChannelID() {	 return m_channelID; }

	void SetDBChannelID(int channelID) {	 m_dbChannelID=channelID; }
	int GetDBChannelID() {	 return m_dbChannelID; }

	void SetDataFlag(int type) { 	m_dirtyData[type] = true; }

	Smart_Ptr<CDMgr> & GetCDMgr() { 	return m_cdMgr; }
	Smart_Ptr<SkillMgr> & GetSkillMgr() { 	return m_skillMgr; }

	Smart_Ptr<ArticleBaseMgr> & GetContainer(BYTE type)
	{
		return m_container[type];
	}

	Smart_Ptr<HorseMgr> & GetHorseManager()
	{
		return m_horseMgr;
	}

	Smart_Ptr<MagicMgr>& GetMagicWeapon();
	Smart_Ptr<SMagicMgr>& GetSMagic();
	Smart_Ptr<HeartMagicMgr>& GetHeartMagicWeapon();

	Smart_Ptr<CJobLuckyMgr> & GetCJobLucky();
	Smart_Ptr<ActivityMgr> & GetActivityMgr();
	Smart_Ptr<EscortMgr> & GetEscortMgr();

	void SetPath(PlayerSynch::PathToAllPlayer &path);

	void ClearPath();

	bool GetPath(PlayerSynch::PathToAllPlayer & path, int &index);

	void SetPathTime();

	void AddMonster(Smart_Ptr<Monster> &obj);

	void DeleteMonster(int key);
	void ClearMonter();

	void ClearMonsterPath();

	void UpdateEnemyList();


	void AddGirlWar(Smart_Ptr<GirlWar> &obj);
	void DeleteGirlWar(int key);
	void ClearGirlWar();


	//��·���㿪ʼ��ʱ�䵽Ŀǰ��ʱ��㣨���룩
	DWORD64 GetPathNowOffset()
	{
		return (CUtil::GetNowSecond() - m_synPathTime);
	}

	//ͬƽ��ʾ���������Ұͬ����
	void SendObjectSynch(Smart_Ptr<Object> & obj);
	//�뿪��Ұͬ��
	void SendObjLeave(Smart_Ptr<Object> & obj);

	//广播通用接口
	void SendSynchCommon(::google::protobuf::Message *from,int messageid);
	void SendSynchMessage(::google::protobuf::Message *from, int msgid);

	//九宫格广播消息
	void SendToAround(::google::protobuf::Message *content, int messageid,bool isMyself=true);

	//ͬ����ҵ����Ըı�
	void SynCharAttribute(BYTE type, int64 value, int state=eAttackSim);
	void SynCharAttribute(vector<int> &type);
	void SynCharAttribute();
	void BroadcastCharAttribute(vector<int> &type, bool isMyself = true);
	void BroadcastCharAttribute(int type, int value, int state, bool isMyself = true);
	void SendSynchAttr(CharBase::SynchObjectAttr &attr);
//	void SendSynchAttrState(CharBattle::ObjectState &attr);
	//同步属性到世界服
	void SynchCharAttributeToWS(BYTE type, int64 value, int state=eAttackSim);

	//ͬ������Ķ���
	void SendSynchAction(CharBattle::SynClientAttack &action);

	//ͬ���ƶ������Χ���
	void SendSynchPath(PlayerSynch::PathToAllPlayer & path);
	void SendSynchPath(PlayerSynch::MonsterPathToPlayer & path);
	void SendSynchPath(PlayerSynch::GirlWarPathToPlayer & path);

	//��ʼ����һ���Ϣ
	void InitBaseInfo(const PlayerInfo::BaseInfo &info);
	void SetBaseInfo(PlayerInfo::BaseInfo *info);

	//通知所有模块上线计算增加的永久属性,目前只用于玩家上线
	void onlineCalculateAtt();

	//等级增加的属性
	void onlineCalculateLevelAtt();
	//阵营增加的属性
	void onlineCalculateCampAtt();

	//上线计算经脉
	void onlineCalculateMeridianAtt();

	void calculateResultAddAtt(map<ECharAttrType, int64>& attList,eAttValueType valueType);

	//计算等级加的战斗力
	int calculateSomeLevelFight(int level);

	//计算阵营加的战斗力
	int calculateCampFight();

	//计算经脉战斗力
	int calculateMeridianFight();

	//��ʼ�����ս����Ϣ
	void InitBattleInfo(const PlayerInfo::BattleInfo &info);
	void SetBattleInfo(PlayerInfo::BattleInfo *info);

	void SetCharBaseInfo(CharLogin::CharBaseInfo *info);

	//��ʼ�����������Ϣ
	void InitQuestInfo(const PlayerInfo::QuestInfo &info);
	void SetQuestInfo(PlayerInfo::QuestInfo *info);

	//竞技场同步到世界服的信息
	void SynchPlayerCompeteInfoToWS(int ranks = 0, bool isSuccess = false);

	//初始化技能
	void InitSkillInfo(const PlayerInfo::SkillInfo &info);
	void SetSkillInfo(PlayerInfo::SkillInfo *info, bool isDel);
	//发送添加或者删除buff的消息
	void SendBuffState(CharBattle::BuffOperate * buf);

	static bool isBattleAtt(ECharAttrType att);

	//挂机信息
	void InitMonitorInfo(const PlayerInfo::MonitorInfo &info);

	void InitGirlWarInfo(const PlayerInfo::GirlWarInfo &info);

	void SetGirlWarInfo(PlayerInfo::GirlWarInfo *info);
	//ս������
	void SetBattleInfo(CharBase::CharAttr * info);

	//死亡后清除部分状态
	void clearState();

	void SetMeridiansVitality();

	void SetVitality();

	//��ɫ������
	virtual void Dead(Smart_Ptr<CreatureObj> &obj);
	virtual bool IsDead();
//	virtual bool IsInMap();
	virtual bool AddBuff(int id, Smart_Ptr<CreatureObj> &obj, int num=1);
	//移出buff不用删除buff
	virtual bool RemoveBuffByOnlyID(DWORD id);
	//移出buff并且结束buff
	virtual bool DeleteBuffByOnlyID(DWORD id);
	 //获得为参数指定的BUFF
	virtual void getBuffByEffectType(vector<DWORD>& onlyIDList,const BuffGetPublicData& data);
	virtual	void removeBuffByGroupID(DWORD groupID);
	bool RemoveBuffById(DWORD id);
	//被攻击了
	virtual void Attacked(Smart_Ptr<CreatureObj> &obj);
	//攻击别人
	virtual void Attack(Smart_Ptr<CreatureObj> &obj);
	//更新战斗状态
	void UpdateFightState(int64 now);

	//发送消息到gateserver
	void SendMessageToGateServer(int msgid, google::protobuf::Message *content);

	virtual int64 GetTypeValue(int type);

	void SetTypeValue(CharBase::CharAttChange& attr);

	void SetTypeValueChar(CharBase::CharAttChangeToGame& attr);
	//�ж��Ƿ��ڹ���Ƶ����
	int IsInAttackRate(int skillid = 0);

	//��ʱ�������
	void SaveAllPlayer(void * obj);
	void SaveAllPlayer(SaveDataInfo& sendData, bool flag = false, bool isDel = false);
	void SendToSaveData(PlayerInfo::SaveTypeInfo *info);
	void SendToSaveData(PlayerInfo::SaveTypeInfo *info, SaveDataInfo& sendData);

	int IsNpcExist(int npcId);
	int CheckDistanceWithNpc(int npcId);

	//发送相应容器里面的格子改变
	void SendSectorChange(vector<DWORD> &vec, int isFly=0);

	virtual int ChangeHPRate(int hp, bool flag = true);
	virtual int ChangeMoveSpeed(const int &value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangeHRate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangeARate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangePhyAttackMax(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangePDefence(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangeCRate(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual int ChangeBaserte(int value, bool isAdd = true,eAttValueType tempValueType = eAttValueBase);
	virtual void RelocatePos(int xpos, int ypos, bool isSyn = false);

	//获得背包管理器
	Smart_Ptr<ArticleBaseMgr> & GetPackage()
	{
		return m_container[ePackageType];
	}

	//获得仓库管理器
	Smart_Ptr<ArticleBaseMgr> & GetStoreHouse()
	{
		return m_container[eStoreHouseType];
	}

	//陈营商店
	Smart_Ptr<PlayerCampShopMgr> & getCampShopMgr();
	Smart_Ptr<BaseShopMgr> & GetBaseShopMgr();

	Smart_Ptr<PlayerFestiveAttr> getFestivalMgr();
	virtual void DecreaseExpRation(double rate)
	{
		if(rate <= 0)
			return;

		m_expRation -= rate;
		if(m_expRation < 0)
			m_expRation = 0;

//		LOG_ERROR(FILEINFO, "Player:%s DecreaseExpRation add of rate:%d, curent  expRate:%d", GetName().c_str(), rate, m_expRation);
		//m_dirtyData[eBaseInfo] = true;
	}

	int GetHorseModelID() { 	return m_horseModel; }
	void SetHorseModelID(const int &id) { 	m_horseModel = id; }

	int GetFashionID() { return m_fashionID; }
	void SetFashionID(const int& id) { m_fashionID = id; }

	Smart_Ptr<CHighLightMgr>& GetActiveHLMgr()
	{
		return m_highLightMgr;
	}

	//------------------------------------------------------------------------------
	// 玩家间交易相关
	//------------------------------------------------------------------------------


	ExchangeMgr& GetExchMgr() { return m_ExchMgr; }

	bool	IsExchanging()	{ return IsInCreatureState(ERS_Exchange); }
	bool	CanExchange()
	{
		return !IsInCreatureStateAny(ERS_Exchange | ERS_Shop | ERS_Stall)
				&& (GetExchMgr().GetTgtPlayerKey() == -1);
	}

	void	BeginExchange(DWORD dwTgtPlayerID)
	{
		SetCreatureState(ERS_Exchange);
		GetExchMgr().CreateData();
		GetExchMgr().SetTgtPlayerKey(dwTgtPlayerID);
	}

	void	EndExchange()
	{
		//交易冷却设置
		UnsetCreatureState(ERS_Exchange);
		GetExchMgr().DeleteData();
		GetExchMgr().SetTgtPlayerKey(GT_INVALID);
	}
//
	DWORD	ProcExchangeReq(Player* &pTarget, DWORD dwTgtPlayerID);
	DWORD	ProcExchangeReqRes(Player* &pApplicant, DWORD dwTgtPlayerID, DWORD dwErrorCode);
	DWORD	ProcExchangeAdd(Player* &pTarget, uint &pItem, int &nInsIndex);
	DWORD	ProcExchangeLock(Player* &pTarget);
	DWORD	ProcExchangeCancel(Player* &pTarget);
	DWORD	ProcExchangeVerify(Player* &pTarget, DWORD &dwFailedPlayerID);
//
	DWORD	VerifyExchangeData(PropBase* pItem[]);
	DWORD	ProcExchange();

	DWORD	ProcPrepareExchange(Player* &pTarget);

	bool IsInDistance(Smart_Ptr<CreatureObj> Obj, uint maxDistance);

	//是否可以走到这个点
	bool IsMoveToPosition(int xpos, int ypos);
	//判断是否在攻击范围内
	bool IsAttackDistance(int xpos, int ypos);

//	//组队函数
	bool isSameTeam(const Smart_Ptr<Player> &otherPLayer);

	//增加玩家罪恶值
	bool AddPlayerSinValue(const EventArgs & e);
	bool ReducePlayerSinValue(const EventArgs & e);
	//增加玩家仇恨值
	bool AddPlayerHatred(const EventArgs & e);

	bool PlayerChangeScene(const EventArgs & e);
	bool PlayerLeaveScene(const EventArgs & e);
	bool FightGirWar(const EventArgs & e);
	bool PlayerEnterGroup(const EventArgs & e);
	bool PlayerLeaveGroup(const EventArgs & e);
	//队长  更换 地图 更改 属性
	void ChangeAttrWhenGroupMemberChangeMap(const Smart_Ptr<GroupInfo> &group_ptr);
	void ChangeAttrWhenGroupAdd(const Smart_Ptr<GroupInfo> &group_ptr);
	void ChangeAttrWhenGroupReduce(const int & member_size);

	bool PlayerDead(const EventArgs & e);
	bool PlayerEnterMap(const EventArgs & e);

	void InitPrestigeInfo(const PlayerInfo::PrestigeInfoList &info);
	void InitDestinationInfo(const PlayerInfo::DestinationInfoList &info);

	void InitTitleInfo(const PlayerInfo::TitleInfoList &info);

	void InitFashionInfo(const FashionMsgProto::FashionShow& info);
	void InitVitalityInfo(const VitalityProto::ProtoVitalityInfo& info);

//	void InitWorshipInfo(WorshipMsgProto::ClientWorshipInfo& info);

	void InitOnceRewardInfo(const PlayerInfo::OnceRewardInfo& info);

	Smart_Ptr<OnceReward>& GetOnceReward()
	{
		return m_onceReward;
	}

	Smart_Ptr<Achievement> GetAchievement()
	{
		return m_Achievement;
	}

	Smart_Ptr<Destination>& GetDestination()
	{
		return m_Destination;
	}

	Smart_Ptr<Title> GetTitle()
	{
		return m_Title;
	}

	Smart_Ptr<PlayerVipPart> GetVipPart()
	{
		return m_VipPart;
	}

	void InitInstanceInfo(const PlayerInfo::InstanceInfoTwoDays &info);

	void InitMoneyInstance(const PlayerInfo::MoneyInstance &info);

	void InitGroupInstance(const PlayerInfo::GroupInstance &info);

	void InitEliteInstance(const PlayerInfo::EliteInstance &info);

	void InitCauchemarInstance(const PlayerInfo::CauchemarInstance &info);

	void InitAdvancedInstance(const PlayerInfo::AdvancedInstance& info);

	Smart_Ptr<InstanceMgr>&  GetInstanceMgr()
	{
		return m_instanceMgr;
	}
	Smart_Ptr<FashionShow> GetFashionMgr()
	{
		return m_fashionShow;
	}
	Smart_Ptr<VitalityMgr> GetVitalityMgr()
	{
		return m_vitalityMgr;
	}

	void InitSignInInfo(const PlayerInfo::SignInInfo &info);
	Smart_Ptr<SignIn> & GetSignIn()
	{
		return m_signIn;
	}

	void InitCounterInfo(const PlayerInfo::CounterServiceList &info);
	Smart_Ptr<CounterService> & GetCounterService()
	{
		return m_counterService;
	}

	void InitEscortInfo(const PlayerInfo::EscortInfo &info);

	void InitVipInfo(const PlayerInfo::PlayerVipInfo &info);

	void InitBaseShopInfo(const PlayerInfo::BaseShopInfo &info);

	void InitAnswerInfo(const PlayerInfo::AnswerInfo& info);

	Smart_Ptr<OnlineReward> &getOnlineReward()
	{
		return m_onlineReward;
	}

	Smart_Ptr<LevelReward> & GetLevelReward()
	{
		return m_levelReward;
	}

	Smart_Ptr<TreasonKingReward> & GetTreasonKingReward()
	{
		return m_treasonKingReward;
	}

//	void InitArenaInfo(const PlayerInfo::ArenaDBInfo &info);
//	Smart_Ptr<ArenaMgr>& GetArenaMgr()
//	{
//		return m_arenaMgr;
//	}

	void SetLoginTime(const int64 &time)
	{
		m_loginTime = time;
	}

	int64 GetLoginTime()
	{
		return m_loginTime;
	}

	void SetFightTime()
	{
		m_fightTime = CUtil::GetNowSecond();
	}

	virtual int IsAttacked(Smart_Ptr<CreatureObj> &obj, int type);

	void SetOffLineTime(const int64 &time)
	{
		m_offineTime = time;
	}

	int64 GetOffLineTime()
	{
		return m_offineTime;
	}

	void SetOnlineTime(const int64 &time)
	{
		m_onlineTime = time;
	}

	void SetSvrScore(int score)
	{
		m_serverScore = score;
	}
	int GetSvrScore()
	{
		return m_serverScore;
	}
	void AddSvrScore(int score)
	{
		m_serverScore += score;
		if(m_serverScore < 0)
			m_serverScore = 0;

		m_dirtyData[eBaseInfo] = true;
	}
	void DecreaseSvrScore(int score)
	{
		m_serverScore -= score;
		if(m_serverScore < 0)
			m_serverScore = 0;

		m_dirtyData[eBaseInfo] = true;
	}

	int64 GetOnlineTime()
	{
		return m_onlineTime;
	}

	void AddTaskMonster(Smart_Ptr<Monster> mon);

	Smart_Ptr<Monster> GetTaskMon();

	//检测时限物品定时器
	void beginCheckBagTimer();
	void endCheckBagTimer();
	void onCheckBag(void* args);

	Smart_Ptr<DanLuMgr> & GetDanLuManager();
	Smart_Ptr<LucklyTurntableMgr> & GetLucklyTableMgr();

	//
	void SetEscortNeedChangeMap(bool value) { m_bEscortNeedChangeMap = value;}
	bool GetEscortNeedChangeMap() { return m_bEscortNeedChangeMap;}

	//通知 要亮起的活动图标
	void SynchActiveHighLight();
	void UpdateUIHighLight(E_UIHL_HANDLE handleType, UIHLType type = UIHLType(eUIHLBig_Null, eUIHLSmall_Null),
			int count = 0,  bool flag = true, int value = 0);
	void UpdateUIHighLight(vector<E_UIHL_HANDLE> handleTypes, vector<E_UIHL_BIGTYPE> bigTypes, vector<E_UIHL_SMALLTYPE> smallTypes,
			vector<int> counts, vector<bool> flags, vector<int> values = vector<int>());
	void UpdateUIHighLight(E_UIHL_HANDLE handleType, HintValue value, UIHLType type = UIHLType(eUIHLBig_Null, eUIHLSmall_Null),
			 bool flag = true);
	//计算当前在线时长
	int64 CalOnlineTime();

	//同步信息到世界服 仅仅是同步了帮派奖励
	void SynchPlayerInfoToWorld();

	//设置初始化玩家部件数据标识
	void SetInitPartDataFlag(int type);
	//判断是否 玩家 所有数据已经初始完成
	bool GetInitPartDataFlag();

	//获得防沉迷收益系数(未认证或未成年在线两小时内1.0 两小时至5小时内0.5 5小时以上0.0)
	float GetAntiAddictionFactor(void);
	Smart_Ptr<AntiAddictionSystemMgr> & GetAntiAddictionSystemMgr(void)
	{
		return m_AntiAddictionMgr;
	}

	Smart_Ptr<DailyManager> & GetDailyManager(void)
	{
		return m_Daily;
	}
	std::vector<int> & GetCityRewardList(void)
	{
		return m_cityRewardList;
	}

	//说话相关
	void SetTalkThing(int64 tm){		m_nextTalkTime = tm;}
	int64 GetTalkingTm(){return m_nextTalkTime;	}
	//是否 现在能够说话
	bool IsCanTaling();

	void RemoveTimerInterface();

	bool KillMonster(const EventArgs & e);

	bool JoinCamp(const EventArgs & e);

	bool PlayerBeKilled(const EventArgs & e);

	Smart_Ptr<GirlWarManager>& GetGirlWarManager();

	/*发送 通知 结果  ----- (触发事件时  带不出 结果)*/
	void SendToPlayerResultCodeMessage(const int& retcode);

	//通知客户端每日信息更新
	void SendDayUpdate(const std::vector<int>& data);

	string &GetIP(){return m_ip;	}
	void SetIP(const string & ip) {  m_ip = ip; }

	int64 GetCurTotalOnlineTime();

	//统一  重置 在线 玩家需要信息
	void ResetPlayerSomethig();
	//重置刷新时间后才能重置的模块
	void ResetPLayerSomethingAferResetTime();
	//结束连斩BUFF
	void EndContinueBeheaded();
	//添加到仇恨列表
	void AddEnemy(Smart_Ptr<CreatureObj> &obj);
	//从仇恨列表中删除
	void DeleteTargetEnemy(Smart_Ptr<CreatureObj> & player);
	//删除整个仇恨列表
	void ClearEnemy();

	//---------------- 队伍相关------------------------------
	bool IsHaveGroup(){  return (bool)m_groupID; }
	void SetGroupID(const int64 &groupID){ m_groupID = groupID; }
	int64 GetGroupID() const {	return m_groupID; }
	uint GetGroupAttr()const {	return m_groupAddHp.size(); }

	//增减属性
	void addAttr(int oprate,const std::vector<pair<int,int> >& attrs,bool syn,eAttValueType tempValueType);
	void removeAttr(int oprate,eAttValueType tempValueType);
	void CountryJobAttrRemove();
	void CountryJobAttrAdd();
	void setFinishiInit(bool flag){m_isFinishInitAttr=flag;}
	bool isFinishInit(){return m_isFinishInitAttr;}
	//是否已经开放
	bool IsOpenSystem(int type);

	void setEnvelopCount(int envelopCount);

	Smart_Ptr<CountryRedEnvelopeMgr>& getEnvelopeMgr();

	int getMaxCanGetEnvelopeCount() const
	{
		return m_maxCanGetEnvelopeCount;
	}

	void setMaxCanGetEnvelopeCount(int maxCanGetEnvelopeCount)
	{
		m_maxCanGetEnvelopeCount = maxCanGetEnvelopeCount;
	}

	Smart_Ptr<ExtraAtributeMgr>& getExtraAtributeMgr();

	void SynchRechargeToWS(int golden);

	bool GetIsHaveCompensate(void)
	{
		return m_bIsHaveCompensate;
	}

	void SetIsHaveCompensate(bool isHaveCompensate)
	{
		m_bIsHaveCompensate = isHaveCompensate;
	}

	template<class T>
	void addAtt(ECharAttrType attType,T value,bool isAdd,eAttValueType tempValueType)
	{
		switch(attType)
		{
		case eCharHPMax:
			{
				ChangeHPMax(value,isAdd,tempValueType);
				break;
			}
		case eCharPhyAttackMax:
			{
				ChangePhyAttackMax(value,isAdd,tempValueType);
				break;
			}
		case eCharPhyDefence:
			{
				ChangePDefence(value,isAdd,tempValueType);
				break;
			}
		case eCharHitRate:
			{
				ChangeHRate(value,isAdd,tempValueType);
				break;
			}
		case eCharAvoidRate:
			{
				ChangeARate(value,isAdd,tempValueType);
				break;
			}
		case eCrackDownRate:
			{
				ChangeCRate(value,isAdd,tempValueType);
				break;
			}
		case eAvoidCrackRate:
			{
				ChangeBaserte(value,isAdd,tempValueType);
				break;
			}
		case eCharMPMax:
			{
				ChangeMPMax(value,isAdd,tempValueType);

				break;
			}
		case eCharMoveSpeed:
			{
				ChangeMoveSpeed(value,isAdd,tempValueType);
				break;
			}
		default:
			{
				break;
			}
		}
	}

	int GetYelloVip(){return m_YellowVip;}
	void SetYellowVip(int nYellowVip){m_YellowVip = nYellowVip;}

	int GetYellowYearVip(){return m_YellowYearVip;}
	void SetYellowYearVip(int nYellowYearVip){m_YellowYearVip = nYellowYearVip;}

	int GetYellowVipLevel(){return m_YellowVipLevel;}
	void SetYellowVipLevel(int nYellowVipLevel){m_YellowVipLevel = nYellowVipLevel;}

	int GetYellowHighVip(){return m_YellowHighVip;}
	void SetYellowHighVip(int nYellowHighVip){m_YellowHighVip = nYellowHighVip;}

	Smart_Ptr<Fighting> getFightingManage();

	Smart_Ptr<MarryRing> GetMarryRing();

	Smart_Ptr<PlayerRegion> GetRegion();

	Smart_Ptr<ChristmasManage> GetChristmas();

	Smart_Ptr<NewYearActivity> GetNewYear();

	Smart_Ptr<PlayerMeditation>& GetMeditationMgr()
	{
		return m_Meditation;
	}

	Smart_Ptr<PlayerDistortion>& GetDistortionMgr()
	{
		return m_Distortion;
	}

	void InitArenaInfo(const PlayerInfo::ArenaDataInfo &info);
	Smart_Ptr<ArenaMgr>& GetArenaMgr();

	Smart_Ptr<PlayerChallenge>& GetChallenge()
	{
		return m_challengeMgr;
	}

	Smart_Ptr<HeroInfo>& GetHeroMgr()
	{
		return m_heroMgr;
	}

	Smart_Ptr<AnswerMgr>& GetAnswerMgr()
	{
		return m_answerMgr;
	}

	//获取最佳的目标位置(当前生物以当前点和速度去获取)
	virtual void GetPerfectLocation(int moveSpeed, int xpos, int ypos, int &txpos, int &typos);

	// 客户端类型
	int GetLoginClientType();
	void SetLoginClientType(int loginClientType);
	void UpdateInfoByClientType();

	string Get58PlaySouce()
	{
		return m_58playSouce;
	}

	bool isBWillInInstance() const
	{
		return m_bWillInInstance;
	}

	void setBWillInInstance(bool bWillInInstance)
	{
		m_bWillInInstance = bWillInInstance;
	}

	//充值相关
	void SetRechargeTime(int64 tm){		m_rechargeTime = tm;}
	int64 GetRechargeTime(){return m_rechargeTime;	}

protected:
	//获取指定等级所加的属性
	void getDesignationLvAttList(int level,BYTE profession,map<ECharAttrType,int64>& attList);

private:
	//禁止 拷贝
	Player(const Player &);
	void operator = (const Player &);

private:

	PlayerSynch::PathToAllPlayer m_path;
	int64 m_synPathTime;
	int m_curIndex;
	bool m_sex; // 性别
	BYTE m_accType; // 帐号类型
	string m_openID; // openid
	string m_openKey;
	string m_pf;
	string m_pfKey;
	string m_sCoupleName;
	double m_expRation; //打野经验倍数
	int m_horseModel; //坐骑模型id
	int m_fashionID; // 时装模型ID 不代表穿戴上的时装
	int m_cContribution_Week; //陈营周贡献度
	BYTE m_FitchPrize;//本日是否领取官职奖励
	BYTE m_kingRankFlag;//争夺霸主标识

	int m_channelID;
	int m_dbChannelID;
	int64 m_deadTime;
	int m_oldPkType; //记录和平地图上的模式
	int m_sinState;//根据m_sinValue是否大于1000而来
	int m_acupoint; //经脉穴位
	int64 m_offineTime; //玩家下线时间
	int64 m_onlineTime; //玩家在线时长
	int64 m_totalOnlineTime;  //总在线时长
	int m_serverScore; //夸服战积分

	int64 m_loginTime; //玩家登录时间
	int64 m_nextTalkTime;   //下次可以说话时间
	int64 m_rechargeTime; //首充时间


	string m_ip;  //登录IP

	//----------重WS中维护的 数据表中获取的 相关信息  ---------------------//
	BYTE m_faJob; // 职位
	int m_fContribution; // 帮贡
	string m_clanName; //帮派名字
	int m_maxCanGetEnvelopeCount;//今天可获取的最大红包个数

	std::vector<int> m_cityRewardList;//昨天未领取奖励的帮派城池ID，从WS获取
	//----------------------------------------------------------------------//
	int64 m_groupID;  //所属队伍ID 由于 同一队伍 杀怪 计数 也计算 在内
	list<float> m_groupAddHp; ////通过 队伍 加的 血上限  -----------退出的 时候 用 来 减的

	EventConnection m_addHartedEvent;
	EventConnection m_addSinValueEvent;
	EventConnection m_reduceSinValueEvent;
	EventConnection m_enterMapEvent;
	EventConnection m_objDeadEvent;
	EventConnection m_killMonsterEvent;
	EventConnection m_joinCountry;
	EventConnection m_leaveMapEvent;
	EventConnection m_fightGirlWarEvent;

	Smart_Ptr<Player> m_myself; //����ָ��

	Smart_Ptr<QuestPart> m_quest;  //���������
	Smart_Ptr<CDMgr> m_cdMgr; //cd时间管理器
	Smart_Ptr<SkillMgr> m_skillMgr; //技能管理器
	Smart_Ptr<ArticleBaseMgr> m_container[eArticleMax]; //容器
	Smart_Ptr<HorseMgr> m_horseMgr; //坐骑管理器

	Smart_Ptr<MagicMgr> m_magicWeapon;  	//神兵
	Smart_Ptr<SMagicMgr> m_sMagic;  		//法器
	Smart_Ptr<HeartMagicMgr> m_heartMagic;  //心法

	Smart_Ptr<CJobLuckyMgr> m_cjobMgr; //官运
	Smart_Ptr<ActivityMgr> m_activity; //活动管理器
	Smart_Ptr<EscortMgr> m_escort; //皇纲押镖

	Smart_Ptr<DanLuMgr> m_danluMgr; //炼炉管理
	Smart_Ptr<LucklyTurntableMgr> m_lucktableMgr; //转盘管理
	Smart_Ptr<Monster> m_pTaskMon;

	bool m_dirtyData[eCharStructMax];
	bool m_initpartData[eCharStructMax];  //玩家组件信息初始化标识位 防止频繁上下线

	TimerConnection m_timerID; //��ʱ������ݶ�ʱ��
	TimerConnection m_sinValueTimer; //在线时常罪恶值将少定时器
	int64 m_fightTime; //战斗状态时间

	TimerConnection m_propcheckTimer;		//时限物品检测定时器

	PlayerInfo::MonitorInfo m_monitorInfo; //挂机信息
	Smart_Ptr<InstanceMgr> m_instanceMgr;//副本管理器
	Smart_Ptr<SignIn> m_signIn;  //签到管理器
	Smart_Ptr<CounterService> m_counterService;
	Smart_Ptr<OnlineReward> m_onlineReward; //在线奖励管理器
	Smart_Ptr<LevelReward> m_levelReward; //等级奖励管理器
	Smart_Ptr<TreasonKingReward> m_treasonKingReward; //反王管理器
	Smart_Ptr<Achievement>	m_Achievement;	//成就
	Smart_Ptr<Destination> m_Destination; //游戏目标
	Smart_Ptr<Title>	m_Title;			//称号
	Smart_Ptr<PlayerVipPart> m_VipPart;
	Smart_Ptr<AntiAddictionSystemMgr> m_AntiAddictionMgr;//防沉迷管理器
	Smart_Ptr<DailyManager> m_Daily;//每日必作管理器
	bool 	m_bEscortNeedChangeMap;		//镖车里玩家太远切换地图时，镖车不需要切换
	ExchangeMgr m_ExchMgr;

	Smart_Ptr<GirlWarManager> m_GirlWarMgr;
	map<int, Smart_Ptr<GirlWar> > m_targetGirlWar;
	map<int, Smart_Ptr<Monster> > m_targetMonster; //����Щ����ѡ����
	vector<Smart_Ptr<CreatureObj> > m_enemyList;	//仇恨列表

	Smart_Ptr<FashionShow> m_fashionShow;					//时装管理器
	Smart_Ptr<VitalityMgr> m_vitalityMgr;					//活跃度管理器

	Smart_Ptr<CountryRedEnvelopeMgr> m_envelopeMgr;					//阵营红包管理器

	Smart_Ptr<PlayerCampShopMgr> m_campShopMgr;//陈营商店

	Smart_Ptr<BaseShopMgr> m_baseShopMgr;//Base商店

	Smart_Ptr<PlayerFestiveAttr> m_festiveAttrMgr;//

	Smart_Ptr<ExtraAtributeMgr> m_extraAtributeMgr;//额外属性管理器

	std::map<int,std::vector<std::pair<int,int> > > m_attrData;	//临时属性数据(暂无时间详细设计)

	Smart_Ptr<CHighLightMgr> m_highLightMgr; // UI高亮管理器
	Smart_Ptr<Fighting> m_Fighting;			//战斗力
	Smart_Ptr<MarryRing> m_MarryRing;		//婚戒
	Smart_Ptr<PlayerMeditation> m_Meditation; //打坐双修
	Smart_Ptr<PlayerDistortion> m_Distortion; //变形

	Smart_Ptr<OnceReward> m_onceReward; // 一次性奖励
//	Smart_Ptr<ArenaMgr> m_arenaMgr; // 竞技场

	Smart_Ptr<PlayerRegion> m_Region;

	Smart_Ptr<ChristmasManage> m_Christmas;

	Smart_Ptr<NewYearActivity> m_NewYear;

	Smart_Ptr<ArenaMgr> m_arenaMgr;

	Smart_Ptr<PlayerChallenge> m_challengeMgr;

	Smart_Ptr<HeroInfo> m_heroMgr;

	Smart_Ptr<AnswerMgr> m_answerMgr;
protected:

private:
	int m_AttrTick;											//定时计数
	bool m_isFinishInitAttr;
	bool m_bIsHaveCompensate;//是否有补偿活动领取
	bool m_bWillInInstance;//是否处于已拥有一个副本，即将进入副本的状态

private:
	int m_YellowVip;
	int m_YellowYearVip; //是否为年费黄钻用户（0：不是； 1：是）
	int m_YellowVipLevel;	//黄钻等级。
	int m_YellowHighVip; //是否为豪华版黄钻用户（0：不是； 1：是）。
	int m_loginClientType; // 登录的客户端类型
	int m_accountType; // 帐号类型

private:
	string m_58playSouce;
};


#endif /* PLAYER_H_ */
