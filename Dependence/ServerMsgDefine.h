/*
 * ServerMsgDefine.h
 *
 *  Created on: 2013-9-20
 *      Author: helh
 */

#ifndef SERVERMSGDEFINE_H_
#define SERVERMSGDEFINE_H_
////////////////////////////  1  -1000  内为服务器直接通信消息id/////////////////////////////////////
//13，14为gameserver通过gateserver转发到client的中继简单消息id（客户端只需要接受14的消息id）
//15，16为client通过gateserver转发到gameserver的中继简单消息id（客户端只需要发送15的消息id）
//17，18为client通过gateserver转发到gameserver的中间请求消息id（客户端只需要发送17的消息id）
//22，14为worldserver通过gateserver转发到client的中继简单消息id（客户端只需要接受14的消息id）
// 23，24为client通过gateserver转发到worldserver的中继简单消息id（客户端只需要发送23的消息id）
//25，26为client通过gateserver转发到worldserver的中继请求消息id（客户端只需要发送25的消息id）
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const int MSG_SIM_GT2LS_PUSHCONNINFO = 1;    //gateserver���͸�longinserver�ͻ�����Ҫ���ӵ�ip��port
//const int MSG_SIM_DB2LS_PUSHSERVERID = 2;    //DBserver���͸�longinserver������ID
//const int MSG_SIM_LS2GT_PUSHSERVERID = 3;    //longinserver���͸�gateserver������ID
const int MSG_SIM_GT2GM_PUSHSERVERID = 4;    //gateserver���͸�gameserver������ID
const int MSG_SIM_DB2GM_PUSHSERVERID = 5;    //dbserver���͸�gameserver������ID
const int MSG_SIM_GM2GT_PUSHALLMAP   = 6;    //gameserver���͸�0�3gateserver���еĳ����͸���
const int MSG_SIM_GT2LS_PUSHGSMAP    = 7;    //gateserver���͸�0�3loginserver�����͸���
const int MSG_SIM_GT2LS_DELETEGSMAP  = 8;    //gateserver���͸�0�3loginserversɾ��͸���
const int MSG_SIM_GT2WS_PUSHCONNINFO = 9;    //gateserver向worldserver发送连接信息
const int MSG_SIM_GT2WS_PUSHGSMAP    = 10;    //gateserver向worldserver发送所有场景
const int MSG_SIM_GT2WS_DELETEGSMAP  = 11;    //gateserver向worldserver发送删除场景
const int MSG_SIM_GT2LS_ADDSCENE     = 12;    //gateserver通知loginserver创建了一个场景
const int MSG_SIM_GM2GT_QUICKMESSAGE = 13;    //发送快捷消息(给客户端发送的简单消息都可用)
const int MSG_SIM_GT2C_QUICKMESSAGE  = 14;    //发送快捷消息(给客户端发送的简单消息都可用)
const int MSG_SIM_C2GT_QUICKMESSAGE  = 15;    //发送转发消息(转发给逻辑服的简单消息都可用)
const int MSG_SIM_GT2GM_QUICKMESSAGE = 16;    //发送转发消息
const int MSG_REQ_C2GT_QUICKMESSAGE  = 17;    //发送转发请求消息(转发给逻辑服的请求消息都可用)
const int MSG_REQ_GT2GM_QUICKMESSAGE = 18;    //发送转发请求消息
const int MSG_SIM_GM2PS_FINDPATH     = 19;    //寻路
const int MSG_SIM_PS2GM_PATHPOS      = 20;    //寻路路径点
const int MSG_REQ_GM2WS_PUSHSERVERID = 21;    //gmserver请求worldserver服组id
const int MSG_SIM_WS2C_QUICKMESSAGE  = 22;    //发送快捷消息(给客户端发送的简单消息都可用)
const int MSG_SIM_C2WS_QUICKMESSAGE  = 23;    //发送快捷消息(从客户端发送的简单消息都可用)
const int MSG_SIM_GT2WS_QUICKMESSAGE = 24;    //发送快捷消息(从客户端发送到gt转发到ws简单消息都可用)
const int MSG_REQ_C2WS_QUICKMESSAGE  = 25;    //发送转发请求消息(转发给ws服的请求消息都可用)
const int MSG_REQ_GT2WS_QUICKMESSAGE = 26;    //发送转发请求消息
const int MSG_REQ_GM2MS_PUSHSERVERID = 27;    //gmserver请求managerServer服组id


/////////////////////////通用消息id////////////////////////////////////////////////

const int MSG_SIM_GT2GT_SYNCHMESSAGE     = 101;    //广播消息




const int MSG_REQ_HTML2LS_WILLLOGIN = 1000;    //--通过网站--请求 LS将要要 登录
const int MSG_REQ_C2LS_GETROLEID = 1001;    //�ͻ�������longinserver���ؽ�ɫid
const int MSG_REQ_LS2DB_GETROLEID = 1002;    //loginserver����dbserver���ؽ�ɫid
const int MSG_REQ_C2LS_GETRANDNAME = 1003;    //�ͻ�������longinserver���ؽ�ɫ�漴����
const int MSG_REQ_LS2DB_GETRANDNAME = 1004;    //loginserver����dbserver���ؽ�ɫ�漴����
const int MSG_REQ_C2LS_CREATEROLE = 1005;    //�ͻ�������longinserver�½���ɫ
const int MSG_REQ_LS2DB_CREATEROLE = 1006;    //loginserver����dbserver�½���ɫ
const int MSG_SIM_LS2GT_WILLLOGIN = 1007;    //longinserver����gateserver��ɫ����������
const int MSG_SIM_GT2GM_CLIENTWILLIN = 1008;   //gateserver������Ϣ��gameservers��ȡ�����Ϣ������
//const int MSG_REQ_LS2DB_CLIENTWILLIN = 1009;   //gameserver������Ϣ��bdservers��ȡ�����Ϣ������
const int MSG_REQ_C2GT_PLAYERINFO = 1010;   //client����Gateservers�����Ϣ
const int MSG_REQ_GT2GM_PLAYERINFO = 1011;   //Gateservers����gameserver�����Ϣ
const int MSG_REQ_GM2DB_PLAYERINFO = 1012;   //Gameservers����DBserver�����Ϣ
const int MSG_SIM_C2GT_SYNCH = 1013;   //client����Gateserversͬ�������Ϣ
const int MSG_SIM_GT2LS_PLAYERLOGIN = 1014;   //Gateservers����Loginservers����Ѿ���¼����Ϣ
//const int MSG_SIM_GT2PS_PLAYERLOGIN = 1015;   //Gateservers����Pathservers����Ѿ���¼����Ϣ
const int MSG_SIM_GT2PS_PLAYEREXIT = 1016;   //Gateservers����Pathservers����Ѿ���¼����Ϣ
const int MSG_SIM_GT2LS_CLIENTBREAK  = 1017;    //gateserver���͸�longinserver�ͻ��˶Ͽ�
const int MSG_REQ_GT2PS_SYNCH = 1018;   //Gateservers����pathserver��֤�����Ϣ
const int MSG_REQ_C2GT_FINDPATH = 1019;   //client����gateserver��֤��·��Ϣ
const int MSG_SIM_GT2C_SENDPOS = 1020;   //gateserver��client�������¶�λ�������
const int MSG_REQ_GT2GM_SYNCHPOS = 1021;   //gateserver��gameserverͬ����������
const int MSG_REQ_GT2GM_PLAYEREXIT = 1022;   //gateserver��gameserver֪ͨ�������
const int MSG_SIM_C2GT_HEARTBEAT = 1023;   //client��gateserver���������
const int MSG_SIM_GT2GM_SYNPATH = 1024;   //gateserver��gameserverͬ��·����
//const int MSG_SIM_GM2GT_SYNPATH = 1025;   //gameserver��gateserver�㲥·����
const int MSG_SIM_GT2C_SYNPATH = 1026;   //gateserver��client�㲥·����
//const int MSG_SIM_GM2GT_SYNPLAYERIN = 1027;   //gameserver��gateserver�㲥��ҽ���
const int MSG_SIM_GT2C_SYNPLAYERIN = 1028;   //gateserver��client�㲥��ҽ���
//const int MSG_SIM_GM2GT_SYNPLAYERLEAVE = 1029;   //gameserver��gateserver�㲥����뿪��Ұ
const int MSG_SIM_GT2C_SYNPLAYERLEAVE = 1030;   //gateserver��client�㲥����뿪��Ұ
const int MSG_REQ_C2GT_CLIENTIN = 1031;   //client��gateserver������ҽ��볡��
const int MSG_REQ_GT2GM_CLIENTIN = 1032;   //gateserver��gameserver������ҽ��볡��
//const int MSG_SIM_GM2GT_SYNMONSTERIN = 1033;   //gameserver��gateserver�㲥���������Ұ
const int MSG_SIM_GT2C_SYNMONSTERIN = 1034;   //gateserver��client�㲥���������Ұ
//const int MSG_SIM_GM2GT_SYNMONSTERLEAVE = 1035;   //gameserver��gateserver�㲥�����뿪��Ұ
const int MSG_SIM_GT2C_SYNMONSTERLEAVE = 1036;   //gateserver��client�㲥�����뿪��Ұ
const int MSG_SIM_GM2GT_SYNMONSTERPATH = 1037;   //gameserver��gateserver�㲥����·����
const int MSG_SIM_GT2C_SYNMONSTERPATH = 1038;   //gateserver��client�㲥����·����
const int MSG_REQ_C2GT_CHARATTR = 1039;   //client��gateserver�����ɫս������
const int MSG_REQ_GT2GM_CHARATTR = 1040;   //gateserver��gameserver�����ɫս������
//const int MSG_REQ_C2GT_QUESTINFO = 1041;   //请求任务列表
const int MSG_REQ_GT2GM_QUESTINFO = 1042;   //gateserver��gameserver�����ɫ������Ϣ
//const int MSG_REQ_C2GT_ACCEPTQUEST = 1043;   //请求领取任务
const int MSG_REQ_GT2GM_ACCEPTQUEST = 1044;   //gateserver��gameserver�����ɫ��������
const int MSG_SIM_GM2GT_SYNCHATTR = 1045;   //gameserver��gateserverͬ��������Եĸı�
const int MSG_SIM_GT2C_SYNCHATTR = 1046;   //gateserver��clientͬ��������Եĸı�
const int MSG_REQ_C2GM_UPDATEQUEST = 1047;   //提交任务
//const int MSG_REQ_GT2GM_UPDATEQUEST = 1048;   //gateserver��gameserver�����ɫ�ύ����
const int MSG_SIM_GT2C_UPDATEQUESTTARGET = 1049;   //更新任务
const int MSG_SIM_GT2GM_SYNCHOBJECTATTR = 1050;   //gameserver��gateserverͬ�����Ըı�
const int MSG_SIM_GT2C_SYNCHOBJECTATTR = 1051;   //gateserver��clientͬ�����Ըı�
//const int MSG_SIM_GT2GM_SYNCHOBJECTACTION = 1052;   //广播玩家攻击动作
const int MSG_SIM_GT2C_SYNCHOBJECTACTION = 1053;   //广播玩家攻击动作
const int MSG_SIM_GT2GM_SYNCHPOS = 1054;   //gateserver2gameserver同步坐标
//const int MSG_SIM_GT2GM_SYNCHPLAYERDEAD = 1054;   //gameserver��gateserverͬ���������
const int MSG_SIM_C2GT_ATTACK = 1055;   //client��gateserver���󹥻�
const int MSG_SIM_GT2GM_ATTACK = 1056;   //gateserver��gameserver���󹥻�
//const int MSG_SIM_C2GT_PLAYER_EXIT = 1057;  //玩家退出游戏
//const int MSG_REQ_GT2GM_PLAYER_EXIT = 1058;  //玩家退出游戏
const int MSG_SIM_GT2GM_SAVE_PLAYERINFO = 1057;  //定时保存玩家信息
const int MSG_SIM_GM2GT_UPDATE_QUESTTARGET = 1058;  //更新任务进度
const int MSG_SIM_GT2C_PLAYER_EXIT = 1059;  //被下线了
const int MSG_REQ_LG2GT_PLAYER_EXIT = 1060;  //T下线

const int MSG_SIM_GM2C_SYNCH_COMPETE_LEVEAL		= 1061;	//竞技场怪物离开视野
//const int MSG_REQ_C2GT_ACCEPTABLE_QUEST = 1061;  //请求获取可接任务
const int MSG_REQ_GT2GM_ACCEPTABLE_QUEST = 1062;  //请求获取可接任务
const int MSG_REQ_C2GT_UPGRADE_SKILL = 1063;  //请求升级技能
const int MSG_REQ_GT2GM_UPGRADE_SKILL = 1064;  //请求升级技能
const int MSG_SIM_GM2GT_BUFFOPERATE = 1065;  //buff的添加和移出
const int MSG_SIM_GT2C_BUFFOPERATE = 1066;  //buff添加和移出
const int MSG_SIM_GT2C_CHANGE_SERVER = 1067;  //换服
const int MSG_REQ_C2GT_CHANGEMAP    = 1068;    //请求改变场景
const int MSG_REQ_GT2GM_CHANGEMAP    = 1069;    //请求改变场景
const int MSG_REQ_GT2WS_CLIENTIN  = 1070;      //gateserver向worldserver发送玩家join in scene
const int MSG_REQ_GM2GT_CHANGEMAP    = 1071;    //gameserver向gateserver发送改变场景
const int MSG_REQ_GT2WS_CHANGEMAP    = 1072;    //gateserver向worldserver发送改变场景
const int MSG_REQ_WS2GT_CHANGEMAP    = 1073;    //worldserver向gateserver发送改变场景
const int MSG_SIM_GT2WS_PLAYEREXIT   = 1074;    //gateserver向worldserver发送玩家下线
const int MSG_REQ_GT2GM_CHANGESCENE  = 1075;    //请求改变场景
//const int MSG_SIM_GT2PS_CHANGESCENE  = 1076;    //改变场景
const int MSG_SIM_GT2LS_CHANGESCENE  = 1077;    //改变场景
const int MSG_SIM_GT2WS_CHANGESCENE  = 1078;    //改变场景
const int MSG_REQ_GT2WS_WILLLOGIN    = 1079;    //玩家将要登录
const int MSG_REQ_WS2GT_WILLLOGIN    = 1080;    //玩家将要登录
const int MSG_REQ_WS2GT_CREATESCENE    = 1081;    //创建场景
const int MSG_REQ_GT2GM_CREATESCENE    = 1082;    //创建场景
const int MSG_SIM_GM2GT_SYNQUEST     = 1083;    //gameserver向gateserver同步玩家任务信息
//const int MSG_REQ_C2GT_QUICKUPDATEQUEST     = 1084;    //client向gateserver请求快速完成任务
const int MSG_REQ_GT2GM_QUICKUPDATEQUEST    = 1085;    //gateway向gameserver请求快速完成任务
const int MSG_REQ_GM2DB_HORSEINFO       = 1086;     //请求坐骑等信息

const int MSG_REQ_WS2DB_WPLAYERINFO         = 1087;     //ws向db请求玩家世界节点数据
const int MSG_REQ_GM2GT_PLAYER_ONLINE         = 1088;     //请求玩家上线
const int MSG_REQ_GT2GM_PLAYER_ONLINE         = 1089;     //请求玩家上线
//const int MSG_REQ_GT2GM_SYNCH                 = 1090;     //请求玩家验证路点
const int MSG_SIM_GM2GT_ACCEPTABLE_QUEST      = 1090; //游戏服向网关服同步更新玩家的可接任务列表
//const int MSG_REQ_C2GT_GIVEUP_QUEST      = 1091; //游戏服向网关服同步更新玩家的可接任务列表

//revival复活
const int MSG_SIM_C2GT_REVIVAL = 1093;   //client 向 gateserver请求复活
const int MSG_SIM_GT2GM_REVIVAL = 1094;   //gateserver向 gameserver请求复活
const int MSG_SIM_GM2GT_REVIVAL = 1095;   //gameserver向gateserver回复复活消息
const int MSG_SIM_GT2C_REVIVAL = 1096;    //gateserver向client 回复复活消息

const int MSG_REQ_C2GT_CHANGEPKTYPE= 1097; //切换PK模式
const int MSG_REQ_GM2DB_INSTANCEINFO = 1098; //请求副本信息
const int MSG_REQ_GM2DB_PRESTIGEINFO = 1099; //请求成就信息
const int MSG_REQ_GM2DB_TITLEINFO	 = 1100;	//请求称号

const int MSG_SIM_GT2C_ADDCOLLECT = 1101; //击杀怪物收集物品返回客户端
const int MSG_REQ_C2GM_FIYING_SHOES = 1102;	//飞鞋

const int MSG_SIM_GT2WS_UPDATECHANGESCENE = 1103; //更新
const int MSG_SIM_GM2GT_UPDATECHANGESCENE = 1104;

//const int MSG_REQ_C2GT_MAPLINEINFO			 = 1105;	//请求分线信息
//const int MSG_REQ_GT2WS_MAPLINEINFO		 = 1106;	//向世界服请求分线消息

//const int MSG_SIM_GM2MS_STATISTICDATA		= 1107;	//发送统计数据
const int MSG_SIM_GM2C_FORCE_CHANGEMAP		= 1108;	//强制玩家切换地图
const int MSG_SIM_GM2C_SYNCH_COMPETE_MONSTER		= 1109;	//竞技场怪物进入视野

const int MSG_SIM_GM2C_UPDATEMAINTASK = 1110;	//更新主线任务
const int MSG_SIM_GM2C_UPDATEDAILYTASK = 1111; //更新日常任务
const int MSG_SIM_GM2C_UPDATECLANTASK = 1112;	//更新帮派任务
const int MSG_REQ_C2GM_FRESHSTAR = 1113;					//日常和帮派任务刷星
const int MSG_REQ_C2GM_ONEKEYCOM = 1114;					//一键完成

const int MSG_REQ_GM2DB_LESSINFO = 1115;					//细碎信息集合
const int MSG_SIM_GM2C_PACKINFO  = 1116;   // 作任务的 时候 通知 客户断 背包满了

const int MSG_SIM_GM2C_MAPLINEINFO			= 1117;			//推送分线信息
//const int MSG_SIM_C2GM_CHANGEMAPLINE	= 1118;			//请求换线

const int MSG_SIM_GT2GM_FILLCLIENTLOGINIP = 1119;  //设置 GM玩家中  登录IP信息(为了 后台 获取 分线 玩家信息)
//const int MSG_SIM_DB2MS_STATISTICDATA = 1200; // 发送 DBSever的统计数据
//const int MSG_SIM_LS2MS_STATISTICDATA = 1201; // 发送 LoginServer的统计数据

const int MSG_SIM_GM2GT_PLAYER_EXIT = 1202;  //GameServer踢玩家下线

const int MSG_SIM_WS2GM_SYNCHATTR = 1203;  //WorldServer 同步玩家数到 GameServer

//const int MSG_SIM_WS2MS_STATISTICDATA = 1204;//发送 WorldServer玩家行为日志

const int MSG_SIM_GM2C_UPDATECAMPASK  = 1205;//更新阵营任务

const int MSG_SIM_GM2C_UPDATEGUIDE_TASK = 1206;//更新引导任务

const int MSG_SIM_GT2LS_KICKOFF_PLAYERS = 1207;//网关服通知登录服踢出部分玩家

const int MSG_SIM_GT2GM_UPDATE_CHANNEL = 1208;//网关服通知逻辑服更新频道信息

const int MSG_REQ_GS2DB_SAVE_PLAYERDATA = 1209;//游戏服向db请求保存数据

const int MSG_SIM_WS2GM_SYNCHATTRCHAR = 1210;  //WorldServer 同步玩家数到 GameServer

const int MSG_REQ_LS2DB_VERIFICATIONISROBOT = 1211; // 请求DB  玩家是否是机器人

const int MSG_SIM_GM2MS_STATISTDATA = 1212; // GM 同步 统计行为数据 到 MS
const int MSG_SIM_DB2MS_STATISTDATA = 1213; // DB 同步统计数据到MS
const int MSG_SIM_LS2MS_STATISTDATA = 1214; // LS 同步统计数据到MS
const int MSG_SIM_WS2MS_STATISTDATA = 1215; // WS同步统计数据到MS

///////////////////////////////////////////////////////物品      3000 －4000   //////////////////////////////////////



const int MSG_SIM_GM2GT_CONTAINERINFO     = 3000;    //同步玩家容器信息
const int MSG_SIM_GT2C_CONTAINERINFO      = 3001;    //同步玩家容器信息
const int MSG_REQ_GM2DB_CONTAINERINFO     = 3002;    //请求玩家容器信息

const int MSG_REQ_C2CT_MOVEGOODS          = 3003;    //移动物品
const int MSG_SIM_C2CT_SORTBACKPACK       = 3004;    //整理背包
const int MSG_REQ_C2CT_USEGGOODS          = 3005;    //使用物品
const int MSG_REQ_C2CT_DISCARDGOODS       = 3006;    //丢弃物品
const int MSG_REQ_C2CT_ADDBACKPACKSIZE    = 3007;    //开启格子
const int MSG_SIM_GT2C_SAVEGOODS          = 3008;    //添加物品

const int MSG_REQ_GT2GM_USEGOODS          = 3009;     //使用物品
const int MSG_REQ_GT2GM_MOVEGOODS         = 3010;     //移动物品
const int MSG_REQ_GT2GM_ADDBACKPACKSIZE   = 3011;     //开启包裹
const int MSG_REQ_GT2GM_DISCARDGOODS      = 3012;     //丢弃物品
const int MSG_REQ_GT2GM_SELLGOODS         = 3013;     //出售物品
const int MSG_REQ_GT2GM_SPLITGOODS        = 3014;     //拆分物品
const int MSG_SIM_GT2GM_SORTBACKPACK      = 3015;     //整理背包

const int MSG_SIM_GM2GT_DROPITEM          = 3016;     //掉落物品
const int MSG_SIM_GT2C_DROPITEM           = 3017;     //掉落物品
const int MSG_REQ_C2GT_PICKITEM           = 3018;     //拾取物品
const int MSG_REQ_GT2GM_PICKITEM          = 3019;     //拾取物品
//const int MSG_SIM_GM2GT_DELETEDROP        = 3020;     //删除尸体盒
const int MSG_SIM_GT2C_DELETEDROP         = 3021;     //删除尸体盒
const int MSG_SIM_GT2C_DELETEITEM         = 3022;     //删除物品
const int MSG_SIM_GM2GT_DELETEITEM        = 3023;     //删除物品

const int MSG_REQ_C2CT_MOVEQUICKSECTOR    = 3024;     //快捷栏移动
const int MSG_REQ_GT2GM_MOVEQUICKSECTOR   = 3025;     //快捷栏移动
const int MSG_SIM_GM2GT_SAVEGOODS         = 3026;    //添加物品
//const int MSG_SIM_GT2C_SAVEGOODS          = 3027;    //添加物品

const int MSG_REQ_C2CT_REFINE           = 3028;	  //物品洗练
const int MSG_REQ_GT2GM_REFINE		  = 3029;	  //物品洗练
const int MSG_REQ_C2GT_WEAREQUIP		  = 3030;	  //穿戴装备
const int MSG_REQ_GT2GM_WEAREQUIP		  = 3031;	  //穿戴装备
//const int MSG_REQ_C2CT_RECOVEREQUIP		  = 3032;	  //丢弃装备
const int MSG_REQ_GT2GM_RECOVEREQUIP		  = 3033;	  //丢弃装备
const int MSG_REQ_C2CT_AFFIRMREFINE				=  3034;			//确认洗练
const int MSG_REQ_GT2GM_AFFIRMREFINE			=  3035;    //确认洗练
const int MSG_SIM_GT2GM_ADDBACKPACKSIZE = 3036;

const int MSG_REQ_C2GT_SPLITGOODS = 3037;

const int MSG_SIM_GM2C_UPDATEQUICKSECTOR = 3038;

const int MSG_SIM_ADDBACKPACKSIZEBYUPVIP = 3039;   //自动开启了 仓库格子(推送消息）

//const int MSG_SIM_DB2GM_CONTAINERINFO     = 3040;    //数据库返回玩家容器信息

//--------------------------------------------------战斗相关(4000  -  5000)----------------------------------

const int MSG_SIM_C2GT_MONITOR         = 4001;     //保存挂机系统
const int MSG_SIM_GT2GM_MONITOR        = 4002;     //保存挂机系统



//---------------------------------------坐骑相关 5000 － 5500------------------------------

const int MSG_REQ_C2GT_HORSEINFO        = 5001;     //请求坐骑信息
const int MSG_REQ_GT2GM_HORSEINFO       = 5002;     //请求坐骑信息
const int MSG_REQ_C2GT_HANDLEHORSE      = 5003;     //请求上下坐骑
const int MSG_REQ_GT2GM_HANDLEHORSE     = 5004;     //请求上下坐骑
const int MSG_REQ_C2GT_HORSEADD         = 5005;     //请求坐骑放养
const int MSG_REQ_GT2GM_HORSEADD        = 5006;     //请求坐骑放养
const int MSG_SIM_GT2C_HANDLEHORSE      = 5007;     //广播上下坐骑
const int MSG_SIM_GM2GT_GETHORSE        = 5008;     //获得一个坐骑
const int MSG_SIM_GT2C_GETHORSE         = 5009;     //获得一个坐骑
const int MSG_C2GM_BUYHORSESKIN         = 5010;     //购买 坐骑 皮肤
const int MSG_SIM_GM2C_GETSKIN						 = 5011;				//获得
const int MSG_SIM_GM2C_SKINEND						 = 5012;				//模型时间结束
const int MSG_SIM_C2GM_ALTERHORSESKINUIHL = 5013; // 改变(关闭)坐骑皮肤UI高亮状态




//－－－－－－－－－－－－－－－－－－－－－－锻造系统5500 － 6000--------------------------------

const int MSG_REQ_C2GT_STONEDAADEL         = 5501;     //镶嵌，摘除
const int MSG_REQ_GT2GM_STONEDAADEL        = 5502;     //镶嵌，摘除
const int MSG_REQ_C2GT_STONECREATE         = 5503;     //合成
const int MSG_REQ_GT2GM_STONECREATE        = 5504;     //合成
const int MSG_REQ_C2GT_STRENGTHEQUIP       = 5505;     //装备强化
const int MSG_REQ_GT2GM_STRENGTHEQUIP      = 5506;     //装备强化
const int MSG_REQ_C2GM_UPGRADEEQUIP 				 = 5507;				//装备升级
const int MSG_REQ_C2GM_SUCCEEDSTRENGTH     = 5508;				//继承强化
const int MSG_REQ_C2GM_INALYSTONEINFO				= 5509; 		// 宝石镶嵌的所有位置宝石信息
const int MSG_REQ_C2GM_METERIALSYNTHESIS = 5510; // 材料合成
const int MSG_REQ_C2GM_INLAYQUICKSYNTH = 5511; // 镶嵌快速升级宝石
const int MSG_SIM_GM2C_NOTICESMITHUIHL = 5512; // 通知客户端锻造UI高亮

//－－－－－－－－－－－－－－－－－－－－－－好友系统6000 － 6500--------------------------------
const int MSG_REQ_C2GT_FINDFRIEND          = 6000;     //查找好友
const int MSG_REQ_C2GT_OPENFRIEND        		= 6001;     //打开好友面板
const int MSG_REQ_C2GT_ADDFRIEND         		= 6002;     //添加好友
const int MSG_REQ_C2GT_DELETEFRIEND        = 6003;     //删除好友
const int MSG_SIM_GT2C_BLESSFRIEND       		= 6004;     //祝福好友
const int MSG_SIM_C2GT_REQBLESSFRIEND       		= 6005;     //请求祝福好友
const int MSG_SIM_CT2C_SYNADDFRIEND        = 6006;      //通知被添加的好友
const int MSG_SIM_CT2C_SYNUPDATE           = 6007;     //上下线通知好友
const int MSG_REQ_CT2WS_OPENFRIEND        	 = 6008;     //打开好友面板
const int MSG_REQ_CT2WS_FINDFRIEND        	 = 6009;     //打开好友面板
const int MSG_REQ_CT2WS_ADDFRIEND         	 = 6010;     //添加好友
const int MSG_REQ_CT2WS_DELETEFRIEND        = 6011;     //删除好友
const int MSG_SIM_WS2CT_SYNUPDATE           = 6012;     //上下线通知好友
const int MSG_SIM_WS2C_SYNADDFRIEND        = 6013;      //通知被添加的好友

//const int MSG_SIM_GM2GT_SYNWCHATTR           = 6014;    //游戏向网关同步世界服角色信息
//const int MSG_SIM_GT2WS_SYNWCHATTR           = 6015;    //网关向世界同步世界服角色信息

const int MSG_SIM_WS2GT_BLESSFRIEND       		= 6016;     //祝福好友
const int MSG_SIM_WS2GM_REQBLESSFRIEND        = 6018;
const int MSG_SIM_WS2GM_SAVE_PLAYERINFO      = 6019; //定时保存玩家在世界服的数据
const int MSG_REQ_C2GT_SEEOTHERPLAYERINFO    = 6020; //查看其他玩家的信息
const int MSG_REQ_GT2GM_SEEOTHERPLAYERINFO    = 6021; //查看其他玩家的信息
const int MSG_REQ_GM2DB_SEEOTHERPLAYERINFO    = 6022; //查看其他玩家的信息
const int MSG_SIM_GM2GT_ADDHATRED             = 6023; //被击杀后将击杀者添加为仇人并且增加其对方仇恨值
const int MSG_REQ_GT2WS_ADDHATRED             = 6024; //被击杀后将击杀者添加为仇人并且增加其对方仇恨值
const int MSG_SIM_GT2C_ADDHATRED             = 6025; //被击杀后将击杀者添加为仇人并且增加其对方仇恨值
const int MSG_REQ_C2GT_RECOMMEND_FRIEND        = 6026; //请求推荐好友
const int MSG_REQ_GT2WS_RECOMMEND_FRIEND        = 6027; //请求推荐好友
const int MSG_SIM_WS2DB_DELETE_MEINWHO        = 6028; //删除我在谁好友的连表
const int MSG_SIM_WS2C_RECOMMEND_FRIEND 		= 6029;	//服务器推送好友

const int MSG_SIM_GM2C_PLAYER_DEADINFO	 		= 6030;	//玩家死亡信息

//－－－－－－－－－－－－－－－－－－－－－－系统6100 - 6200--------------------------------
const int MSG_SIM_GM2WS_SYNWCHATTR          = 6100;    //游戏向世界同步世界服角色信息
const int MSG_REQ_GM2WS_BUYLIMITGOODS		= 6101;			//购买限时商品
const int MSG_REQ_WS2GM_BUYLIMITGOODS		= 6102;			//购买限时商品
const int MSG_REQ_WS2GM_CREATESCENE             = 6110;			//请求他建场景

//－－－－－－－－－－－－－－－－－－－－－－排名系统6500 - 6600--------------------------------

const int MSG_REQ_C2GT_RANKING		= 6501;			//请求排行榜
const int MSG_REQ_C2GT_MERANKING	= 6502;			//请求自己的排名信息
const int MSG_REQ_GT2WS_RANKING		= 6503;			//请求排行榜信息
const int MSG_REQ_GT2WS_MERANKING		= 6504;		//请求角色排名
//const int MSG_REQ_GM2GT_GLORY	= 6513;			//请求自己的排行榜称号
//const int MSG_REQ_GT2WS_GLORY				= 6514;			//请求自己的排行榜称号
const int MSG_REQ_GM2WS_GLORY = 6505;//请求自己的排行榜称号

//－－－－－－－－－－－－－－－－－－－－－－聊天系统6600 - 6700--------------------------------
const int MSG_REQ_C2GT_CHAT			= 6601;			//玩家发言
const int MSG_REQ_GT2WS_CHAT		= 6602;			//玩家发言
const int MSG_REQ_GT2GM_CHAT		= 6603;			//玩家发言
const int MSG_SIM_WS2GT_CHAT		= 6604;			//广播聊天信息
const int MSG_SIM_WS2GM_CHAT		= 6605;			//广播聊天信息
const int MSG_SIM_GT2C_CHAT			= 6606;			//广播聊天信息
const int MSG_SIM_GM2GT_MARQUEE		= 6607;			//跑马灯字幕
const int MSG_SIM_GM2WS_MARQUEE		= 6608;			//跑马灯字幕
const int MSG_SIM_WS2GT_MARQUEE		= 6609;			//跑马灯字幕
const int MSG_SIM_GT2C_MARQUEE		= 6610;			//跑马灯字幕
const int MSG_SIM_WS2GM_WORLDCHATVITALITY = 6611; // 通知GM完成世界聊天活跃度  (特殊)

//－－－－－－－－－－－－－－－－－－－－－－邮件系统6700 - 6800--------------------------------
const int MSG_REQ_C2GM_MAILS			= 6701;			//请求邮件信息
const int MSG_REQ_C2GM_MAILINFO			= 6702;			//请求邮件详细信息
//const int MSG_REQ_C2GT_MAILCONTACTS		= 6703;			//请求最近联系人列表（限5人）
const int MSG_REQ_C2GM_DELETEMAIL		= 6704;			//删除邮件
const int MSG_REQ_C2GM_GETMAILGOODS		= 6705;			//提取物品
//const int MSG_REQ_C2GM_SENDMAIL			= 6706;			//发送邮件
const int MSG_SIM_GT2C_NEWMAIL			= 6707;			//收到新邮件
//const int MSG_REQ_GM2DB_SENDMAIL		= 6708;			//发送邮件
const int MSG_SIM_WS2GT_SENDMAIL		= 6709;			//发送邮件
const int MSG_SIM_DB2WS_NEWMAIL			= 6710;			//发送新邮件
const int MSG_REQ_GM2DB_MAILS			= 6711;			//请求邮件信息
const int MSG_REQ_GM2DB_MAILINFO		= 6712;			//请求邮件详细信息
const int MSG_REQ_GM2DB_DELETEMAIL		= 6713;			//删除邮件
const int MSG_REQ_GM2DB_GETMAILGOODS	= 6714;			//提取物品
const int MSG_SIM_GM2GT_MAILS			= 6715;			//请求邮件信息
const int MSG_SIM_GM2DB_SENDMAIL		= 6716;			//发送邮件

//－－－－－－－－－－－－－－－－－－－－－－商城系统6800 - 6900--------------------------------
//const int MSG_REQ_C2GT_MALLINFO				= 6801;			//请求商城限时商品信息
const int MSG_REQ_C2GT_BUYGOODS				= 6802;			//购买商品
const int MSG_SIM_GT2C_LIMITGOODS			= 6803;			//广播限量商品被购信息
//const int MSG_SIM_WS2GT_LIMITGOODS			= 6804;			//广播限量商品被购信息
//const int MSG_REQ_GM2GT_BUYLIMITGOODS		= 6805;			//购买限时商品
//const int MSG_REQ_GT2WS_BUYLIMITGOODS		= 6806;			//购买限时商品
//const int MSG_SIM_GT2GM_BUYLIMITGOODS		= 6807;			//购买限时商品
//const int MSG_REQ_GT2WS_MALLINFO			= 6808;			//请求商城限时商品信息
const int MSG_REQ_WEB2MS_MALL_LIMIT_OP = 6809;					//网页后台向管理服增删改查
const int MSG_REQ_MS2WS_MALL_LIMIT_OP = 6810;					//管理服向世界服增删改查
const int MSG_REQ_C2WS_MALL_LIMIT_INFO = 6811;				//客户端获取活动概况和自己的购买情况
const int MSG_REQ_C2WS_MALL_LIMIT_BUY = 6812;					//客户端请求购买
const int MSG_REQ_WS2GM_MALL_LIMIT_BUY = 6813;					//逻辑服验证购买以及购买操作
const int MSG_REQ_C2WS_MALL_LIMIT_TYPE_INFO = 6814;			//客户端获取自己的购买情况

//－－－－－－－－－－－－－－－－－－－－－－日常系统6900 - 6999--------------------------------
//const int MSG_REQ_C2GT_DAILYINFO			= 6901;			//日常任务信息
//const int MSG_REQ_C2GT_HAVETODOAWARD		= 6902;			//领取每日必做奖励
//const int MSG_REQ_C2GT_ACTIVITIESAWARD		= 6903;			//领取每日活动奖励
//const int MSG_SIM_GT2C_HAVETODOINFO			= 6904;			//每日必做广播
//const int MSG_SIM_GT2C_ACTIVITIESINFO		= 6905;			//每日活动广播

//－－－－－－－－－－－－－－－－－－－－－－系统设置 7000--------------------------------
const int MSG_SIM_GM2WS_SETTINGS		= 6999;			//系统设置
const int MSG_REQ_C2GM_SETTINGS			= 7000;			//系统设置

//－－－－－－－－－－－－－－－－－－－－－－与NPC交易7000 - 75000--------------------------------

const int MSG_REQ_C2GT_BUYNPCITEM	 = 7001;			//请求购买NPC物品
const int MSG_REQ_C2GT_SELLITEM		 = 7002;			//将物品卖给NPC
const int MSG_REQ_C2GT_BUYBACK		 = 7003;			//回购卖出的物品

const int MSG_REQ_GT2GM_BUYNPCITEM	 = 7004;			//请求购买NPC物品
const int MSG_REQ_GT2GM_SELLITEM		 = 7005;			//将物品卖给NPC
const int MSG_REQ_GT2GM_BUYBACK		 = 7006;			//回购卖出的物品

//－－－－－－－－－－－－－－－－－－－－baseShop－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_C2GM_BASESHOP_BUY = 7051; 			  //请求商店购物
const int MSG_REQ_C2GM_BASESHOP_MYSTER = 7052; 		  //玩家请求神秘商店数据
const int MSG_REQ_C2GM_BASESHOP_FLUSH_MYSTER = 7053; //玩家请求刷新神秘商店数据
const int MSG_REQ_C2GM_BASESHOP_INFO = 7054; //玩家请求商店数据

//------------------------------ 通过NPC 查看 角色仓库 或 商店 ----------------------------------------------------
const int MSG_REG_C2GM_NPCHOUSEINFO        = 7101; //获取NPC 角色仓库仓库  商店信息 //快捷消息

//－－－－－－－－－－－－－－－－－－－－－－玩家交易7501 - 8000--------------------------------
const int MSG_REQ_C2GT_INITEXCHANGE	 		 = 7501;			//请求发起交易
const int MSG_REQ_C2GT_OTHERSTATUS			 = 7502;			//交易者发送请求状态0为拒绝，1为同意
const int MSG_REQ_C2GT_CANCELEXCHANGE		 = 7503;			//客户端请求取消交易
const int MSG_REQ_C2GT_EXCHANGELOCK			 = 7504;			//请求交易锁定
const int MSG_REQ_C2GT_VERIFYEXCHANGE		 = 7505;			//确认交易

const int MSG_SIM_GM2GT_EXCHANGETOOTHER    = 7506;       //将请求人的ID发给对方
const int MSG_SIM_GM2GT_LOCKINFOTOOTHER    = 7507;       //锁定后给对方发的消息
const int MSG_SIM_GT2C_BUILDEXCHANGE		 = 7508;			//接受交易后 给申请者发送消息
const int MSG_SIM_GM2GT_CANCELTOOTHER	 	 = 7509;			//取消给另外一个交易者的消息
const int MSG_SIM_GM2GT_VERIFYTOOTHER	 	 = 7515;			//成功发给另外一个交易者的消息

const int MSG_REQ_GT2GM_INITEXCHANGE	 	 = 7510;			//请求发起交易
const int MSG_REQ_GT2GM_OTHERSTATUS			 = 7511;			//交易者发送请求状态1为拒绝，0为同意
const int MSG_REQ_GT2GM_CANCELEXCHANGE		 = 7512;			//客户端请求取消交易
const int MSG_REQ_GT2GM_EXCHANGELOCK		 = 7513;			//请求交易锁定
const int MSG_REQ_GT2GM_VERIFYEXCHANGE		 = 7514;			//确认交易

//－－－－－－－－－－－－－－－－－－－－－－玩家队伍8001 - 8500--------------------------------
const int MSG_REQ_C2GM_CREATETEAM	 		 = 8001;			//请求创建队伍
const int MSG_REQ_C2GM_INVITETOTEAM			 = 8002;			//邀请加入队伍
const int MSG_REQ_C2GM_REPLYINVITE 			 = 8003;			//邀请回复请求状态1为拒绝，0为同意
const int MSG_REQ_C2GM_KICKMEMBER			 = 8004;			//踢出队员
const int MSG_REQ_C2GM_CHANGELEADER			 = 8005;			//更换队长
const int MSG_REQ_C2GM_APPLICATETOTEAM		 = 8006;			//申请入队
const int MSG_REQ_C2GM_GETTEAMID				 = 8007;			//请求附近队伍ID
const int MSG_REQ_C2GM_GETPLAYERS			 = 8008;			//请求附近玩家
const int MSG_REQ_C2GM_LEAVETEAM				 = 8009;			//离开队伍
const int MSG_SIM_GM2C_APPTOLEADER			 = 8010;			//申请信息转给队长
const int MSG_REQ_C2GM_LEADERRETAPP			 = 8011;			//队长回复申请者
const int MSG_REQ_C2GM_LOOKFORTEAMINFO		 = 8012;			//查看队伍信息
const int MSG_REQ_C2GM_UPDATETEAMINFO		 = 8013;			//更新队伍信息
const int MSG_SIM_GM2C_CREATETETEAM_NEWS = 8014;       //当玩家创建队伍时，在世界频道和队伍频道都发出带超链接的文本消息

const int MSG_SIM_BROADCASTLEAVEPLAYER		 = 8015;			//玩家离开队伍广播给其他玩家
const int MSG_SIM_BROADCASTKICKOUT			 = 8016;			//提出玩家广播
const int MSG_SIM_BROADCASTCHANGELEADER	 = 8017;			//更换队长
const int MSG_SIM_BROADCASTPLAYERSINFO		 = 8018;		   //广播其他队友信息
const int MSG_SIM_TOINVITOR					 = 8019;			//发送邀请给玩家
const int MSG_SIM_APPGETPLAYERSINFO			 = 8020;			//申请者获得队友信息

const int MSG_SIM_INSTANCEGROUPCREATE		 = 8021;			//队长创建了副本，询问队员是否愿意进入
const int MSG_REQ_MEMBERAPPLYTOINSTANCE	 = 8022;			//队员是否同意进入副本
const int MSG_REQ_SYSTEAMMEMBERMAP = 8023;							//同步队员位置信息

const int MSG_SIM_GM2C_KICKMEMBER = 8024;  //通知 提出队员
//const int MSG_SIM_GM2C_APPTOLEADER = 8025; // 通知 队长 有人 申请加入 队伍
const int MSG_SIM_GM2C_LEAVEGROUP = 8026;  //通知 有 人 退出 队伍
const int MSG_REQ_C2GM_FUZZYSELECTPLAYERINFO  = 8027;  //模糊查询 在线玩家

//－－－－－－－－－－－－－－－－－－－－－－玩家副本8501 - 8600--------------------------------
const int MSG_REQ_C2GT_CREATEINSTANCE	 		 = 8501;			//请求创建副本
const int MSG_REQ_C2GT_LEAVEINSTANCE	 		 = 8502;			//请求退出副本
const int MSG_REQ_C2GT_UPDATEINSTANCE	 		 = 8503;			//更新副本信息
const int MSG_SIM_GT2C_ENDINSTANCE	 		 	 = 8504;			//副本结束
const int MSG_REQ_C2GT_SWEEPINSTANCE	 		 = 8505;			//请求扫荡副本
const int MSG_REQ_C2GT_SWEEPSPEEDUP		 		 = 8506;			//请求加速扫荡副本
const int MSG_REQ_C2GT_SWEEPINSTANCEEND	 	 = 8507;			//扫荡副本结束
const int MSG_REQ_C2GT_RESETINSTANCE	 		 = 8508;			//请求重置副本
const int MSG_REQ_C2GT_RESETALLINSTANCE	 	 = 8509;			//请求重置所有副本
const int MSG_REQ_C2GT_OPENINSTANCESWEEP	 	 = 8510;			//请求打开扫荡面板
const int MSG_REQ_C2GT_INSTANCEGROUPREADY		 = 8511;			//副本组队准备申请
const int MSG_REQ_C2GT_INSTANCEGROUPENTER		 = 8512;			//组队进入副本
const int MSG_REQ_C2GT_MEMBERFLOPINFO			 = 8513;			//队员翻牌消息
const int MSG_REQ_GM2GT_ENDINSTANCE				 = 8514;			//
const int MSG_REQ_C2GT_GETREWARDS				 = 8515;			//领取奖励
const int MSG_REQ_C2GT_ENTERINSTANCE			 = 8516;			//进入副本检测
const int MSG_REQ_C2GT_INSTANCETEAMINFO		 = 8517;			//获取副本队伍信息
const int MSG_REQ_C2GM_UPDATEINSTANCEMONSTERPOS = 8518;		//将副本怪物所有的坐标点同步给客户端（副本的怪物空闲状态不寻路）
//const int MSG_SIM_C2GM_TEAMINSTANCENOTREADY = 8519; //踢除未准备的玩家并进入副本

const int MSG_REQ_GT2WS_CREATEINSTANCE	 		 = 8520;			//向世界服请求创建副本
const int MSG_REQ_WS2GT_CREATEINSTANCE	 		 = 8521;			//向网关服请求创建副本
const int MSG_REQ_GT2GM_CREATEINSTANCE	 		 = 8522;			//向场景服请求创建副本
const int MSG_SIM_WS2GT_CREATEDSCENE			 = 8524;			//告诉玩家所在场景副本创建完毕
const int MSG_SIM_GT2GM_CREATEDSCENE			 = 8525;			//告诉玩家所在场景副本创建完毕
const int MSG_REQ_GM2GT_DELETEINSTANCE			 = 8526;			//GM请求GT删除副本
const int MSG_REQ_GT2WS_DELETEINSTANCE			 = 8527;			//GT请求世界服删除副本
const int MSG_REQ_GT2LG_DELETEINSTANCE			 = 8528;			//GT请求登入服删除副本
const int MSG_SIM_GM2C_INSTANCEGROUPREADY		 = 8529;			//广播队员准备信息
const int MSG_SIM_GM2C_MEMBERFLOPINFO			 = 8530;			//广播队员翻牌信息
const int MSG_SIM_GM2C_LEAVEINSTANCE			 = 8531;			//通知玩家退出副本
//-------------------------------------------副本new------------------------------------

const int MSG_REQ_C2GM_CREATEINSTANCE	 		 = 8532;			//请求创建副本
const int MSG_REQ_GM2WS_CREATEINSTANCE	 		 = 8533;			//请求创建副本
const int MSG_REQ_GM2DB_ARENAINFO	 		       = 8534;			//请求竞技场等信息
const int MSG_REQ_C2WS_ARENAINFO					 = 8535;			//请求竞技场信息
const int MSG_REQ_C2WS_WARREPORT 				 = 8536;			//请求竞技场玩家战报
const int MSG_REQ_C2GM_ARENAINFO 				       = 8537;			//请求竞技场玩家数据
const int MSG_REQ_WS2C_WARREPORT 				 = 8538;			//推送竞技场玩家战报

const int MSG_REQ_C2GM_CHANCECDTIME 			 = 8539;			//取消冷脚时间
const int MSG_REQ_C2GM_ADDCHALLENGETIMES	 	 = 8540;			//增加挑战次数

const int MSG_REQ_C2GM_RANKINGREWARD			 =  8541;			//领取排名奖励


const int MSG_REQ_GM2WS_SYNCARENA				 = 8542;			// 同步竞技场信息
const int MSG_SIM_GM2C_COMPETE_ISOVER			 = 8543;			//通知玩家竞技场结束了
const int MSG_REG_GM2WS_SYNCRANKINGREWARD     = 8544;			//同步竞技场领奖信息
const int MSG_REG_WS2GM_SYNCRANKINGREWARD     = 8545;			//同步竞技场领奖信息
const int MSG_REG_C2WS_RANKINGNAME      	 		= 8546;			//请求奖励界面玩家排名名字

const int MSG_REQ_WS2GM_GROUP_LIMIT						= 8547;		//请求组队副本剩余次数
const int MSG_REQ_C2WS_ALL_TEAM_INFO					= 8548;		//请求组队副本所有队伍信息
const int MSG_REQ_C2WS_JION_TEAM 						= 8549;		 //请求加入副本队伍
const int MSG_REQ_C2WS_QUICK_JOIN_TEAM			= 8550;		//请求快速加入队伍
const int MSG_REQ_C2WS_CREATE_TEAM					= 8551;		//请求创建队组队副本队伍
const int MSG_SIM_WS2C_ADD_TEAM_MEMBER			= 8552;		//推送添加成员信息
const int MSG_REQ_C2WS_TEAM_READY 					= 8553;		//队员请求准备
const int MSG_SIM_WS2C_TEAM_READY						= 8554;		//通知队员已准备
const int MSG_SIM_C2WS_FIND_TEAMMEMBER			= 8555;		//请求队友信息
const int MSG_SIM_WS2C_TEAMMEMBER						= 8556;		//告诉客户端请求队友信息
const int MSG_REQ_C2WS_KICKMEMBER						= 8557;		//踢出队员
const int MSG_SIM_WS2C_KICKMEMBER						= 8558;		//通知队员已被踢出
const int MSG_REQ_C2WS_ENTER_TEAM_MAP			= 8559;     //请求进入组队副本
const int MSG_SIM_WS2C_ENTER_TEAM_MAP				= 8560;		//通知队员进入组队副本
const int MSG_SIM_GM2C_HARM_RANK						= 8561;		//同步伤害排名
const int MSG_SIM_WS2C_OUT_LINE 							= 8562;		//同步队员下线
const int MSG_REQ_C2WS_EXIT_TEAM							= 8563;		//请求退出队伍
const int MSG_SIM_WS2C_EXIT_TEAM							= 8564;		//同步退出队伍信息
const int MSG_SIM_GM2WS_DEL_TEAM						= 8565;		//组队副本结束，销毁队伍
const int MSG_REQ_C2GM_LOTTERY								= 8566;		//VIP副本抽奖
const int MSG_SIM_WS2GM_ARENABECHALL				= 8567; 		// 同步竞技场被挑战者排名
const int MSG_SIM_GM2C_ARENABECHALL = 8568; // 同步竞技场被挑战者 被挑战了
const int MSG_SIM_WS2C_EXITGROUP						= 8569;		//通知队员离开队伍
const int MSG_SIM_GM2WS_QUITE_TEAM       = 8570; //通知世界服退出副本
const int MSG_REQ_C2GM_SWEEPALLINSTANCE  = 8571; //一键扫荡副本
const int MSG_SIM_GM2C_CURRSWEEPINSTANCE = 8572; //通知正在扫荡的副本
const int MSG_REQ_C2GM_STOPSWEEPALLINSTANCE  = 8573; //停止一键扫荡副本
const int MSG_SIM_GM2C_OVERSWEEPALLINSTANCE = 8574; //通知一键扫荡副本结速
const int MSG_SIM_GM2C_OVERSWEEPINSTANCE = 8575; //副本扫荡结束
const int MSG_SIM_GM2WS_SYSINSTANCERANK = 8576;  //同步副本排名
const int MSG_REQ_C2WS_ASKINSTANCERANK = 8577; //请求副本排名
const int MSG_REQ_C2GM_ASKINSTANCERANK = 8578; //请求副本排名
const int MSG_REQ_C2GM_FINISH_GUIDE    = 8579; //完成指引副本

const int MSG_REQ_C2GM_GROUPINSTANCE_INFO = 8580;  //请求组队副本信息
const int MSG_REQ_C2GM_SHOPPING_GROUPINSTANCE = 8581; //请求购买组队副本

const int MSG_REQ_GM2C_FLUSH_GUIDER		  = 8582; //新手副本通知客户端刷出NPC指引者
const int MSG_REQ_C2GM_ELITE_INFO = 8583; //获取精英副本信息
const int MSG_REQ_C2GM_CAUCHEMAR_INFO	= 8584; //获取恶梦副本信息

const int MSG_REQ_C2WS_INVITEMEMBER_LIST = 8585; //请求邀请列表
const int MSG_REQ_C2WS_INVITEMEMBER = 8586; //请求邀请具体好友
const int MSG_SIM_WS2C_INVITEDMEMBER = 8587; //通知好友被邀请
const int MSG_REQ_WS2GM_INVITLIMIT = 8588; //邀请限制
const int MSG_REQ_C2GM_INSTANCE_INFO = 8589; //获取普通副本信息
const int MSG_REQ_C2GS_CHALLENGEINFO = 8590; //请求挑战副本信息
const int MSG_REQ_C2WS_LAYERRANK = 8591; 			//请求查询某层的排行信息
const int MSG_REQ_SWEEP_CHALLENGE = 8592; 		//请求开始扫荡
const int MSG_REQ_END_CHALLENGE = 8593; 				//请求结束扫荡
const int MSG_REQ_EXCHANGE_CHALLENGE = 8594; //请求兑换物品
const int MSG_GS2C_SWEEP_OVER = 8595; 					//服务器推送扫荡结束
const int MSG_GS2WS_CHALLENGE_RESULT = 8596;	//挑战副本结果
const int MSG_C2GS_GATHER_BOX = 8597;						//采集挑战宝箱
const int MSG_REQ_C2GM_ADVANCED_INFO = 8598; //获取进进阶副本信息
const int MSG_REQ_C2WS_TEAM_CHANCEREADY = 8599;		//队员请求取消准备
const int MSG_SIM_WS2C_TEAM_CHANCEREADY = 8600;		//通知队员取消准备
//-------------------------------------------副本new------------------------------------


//－－－－－－－－－－－－－－－－－－－－－－经脉系统8601 - 8620--------------------------------
const int MSG_REQ_C2GT_OPENMERIDIANS	 		 = 8601;			//请求打开经脉
const int MSG_REQ_C2GT_BROCASTOPENMERIDIANS	 		 = 8602;			//广播


//-------------------------------------活动次数统计--------------------------------------------
const int MSG_REQ_GM2DB_COUNTERSERVICEINFO  = 8603; //获取玩家活动次数统计服务信息

const int MSG_REQ_C2GS_HEROINFO  = 8604; 					//请求英雄副本信息
const int MSG_REQ_C2GS_FITCH_HERO_BOX  = 8605; 	//请求领取宝箱额外奖励
const int MSG_SIM_GS2C_TIME_INFO  = 8606; 				//进入副本后服务器推送开始战斗倒计时时间及结束时间
const int MSG_SIM_GS2C_HERO_RESULT  = 8607;			 //服务器推送胜利失败结果


//－－－－－－－－－－－－－－－－－－－－－－奖励大厅系统8621 - 8700--------------------------------
const int MSG_REQ_GM2DB_SIGNININFO	 		 = 8621;			//获取每日签到信息
const int MSG_REQ_C2GT_SIGNIN          = 8622;     //
const int MSG_REQ_C2GT_GETREWARD = 8623;
const int MSG_REQ_C2GT_OPENSIGNIN      = 8624;
const int MSG_REQ_C2GT_ONLINEREWARDINFO = 8625; //获取玩家在线奖励信息
const int MSG_REQ_C2GT_LEVELREWARDINFO = 8627; //获取玩家等级奖励信息
const int MSG_REQ_C2GT_TOTALLOGINREWARDINFO = 8628; //获取玩累计奖励信息
const int MSG_SIM_GM2C_SYNONCEREWARD = 8629; // 同步一次性奖励信息

//－－－－－－－－－－－－－－－－－－－－－－世界BOSS8701 - 8750--------------------------------
const int MSG_REQ_C2GT_WORLDBOSS = 8701;
const int MSG_SIM_C2GT_UPDATEWORLDBOSSSTATUS = 8703;

//－－－－－－－－－－－－－－－－－－－－－－-副本new－－－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_C2GM_EXP_INFO = 8751; //获取经验副本信息
const int MSG_REQ_C2GM_MONEY_INFO = 8752; //获取铜钱副本信息

//－－－－－－－－－－－－－－－－－－－－－－成就系统9000 - 9100--------------------------------
const int MSG_REQ_GETRECENTPRESTIGE				 = 9001;			//成就总揽
const int MSG_REQ_GETTITLEINOF					 = 9002;			//称号列表
const int MSG_REQ_GETPRESTIGETYPE				 = 9003;			//获取某类成就信息
const int MSG_REQ_GETPRESTIGEPRIZE				 = 9004;			//获取成就奖励
const int MSG_REQ_ACTIVETITLE						 = 9005;			//激活称号
const int MSG_REQ_GETACHSHOPINFO				= 9006;			//获取成就商店信息
const int MSG_REQ_BUGACHSHOPGOODS			= 9007;			//购买成就商店物品
const int MSG_SIM_WS2GM_UPDATATITLEID					= 9008;			//世界服主动要求GM更新称号
const int MSG_SIM_PRESTIGECOMPLETE				 = 9020;			//某成就完成
//－－－－－－－－－－－－－－－－－－－－－－采集 9100 －9200 --------------------------------
const int MSG_REQ_C2GM_GATHER					= 9101;			//开始采集
//const int MSG_SIM_GM2C_ENDGATHER				= 9102;			//结束采集

//－－－－－－－－－－－－－－－－－－－－－－护送 9500 －9600 --------------------------------
const int MSG_REQ_C2GM_ESCORTGETQUALITY		= 9501;			//领取皇纲品质
const int MSG_REQ_C2GM_ESCORTFRESH				= 9502;			//刷新皇纲
const int MSG_REQ_C2GM_ESCORTSTART				= 9503;			//开始运镖
const int MSG_REQ_C2GM_ESCORTIMEDCOMPLETE		= 9504;			//立即完成
const int MSG_REQ_C2GM_ESCORTSPEEDUP			= 9505;			//加速
const int MSG_REQ_C2GM_ESCORTABANDON			= 9506;			//放弃
const int MSG_REQ_C2GM_ESCORTGETRPIZE			= 9507;			//领取奖励
const int MSG_REQ_C2GM_OPENESCORT				= 9508;			//
const int MSG_REQ_C2GM_ESCORTROB				= 9509;				//劫持

const int MSG_SIM_GM2C_UPDATEESCORTINFO		= 9510;			//更新运镖状态信息
const int MSG_SIM_GM2C_ROBESCORTINFO			=	9511;			//劫镖信息
const int MSG_SIM_GM2C_ESCORT_RESULT			=	9512;			//押镖结果

const int MSG_REQ_GM2DB_ESCORTINFO				= 9530;
const int MSG_SIM_WS2GM_DOUBLEESCORTBEGIN = 9531; // 通知GM双倍活动开启

//－－－－－－－－－－－－－－－－－－－－－－护送 9600 －9650--------------------------------
const int MSG_REQ_C2GM_MEDITATION			  = 9601;			//请求打坐，或取消打坐，0表示请求打坐，1取消打坐

const int MSG_SIM_GM2C_MEDITATIONINFO			= 9602;			//通知玩家打坐经验获得
const int MSG_SIM_GM2C_SINGLE_MEDITATION = 9603;			//服务器广播通知单修信息
const int MSG_REQ_C2GM_DOUBLE_MEDITATION = 9604;			//邀请双修
const int MSG_GM2C_RECEIVE_MEDITATION_INVIT = 9605;		//服务器通知玩家收到邀请双修信息
const int MSG_REQ_REPLY_MEDITATION_INVIT = 9606; 			//玩家同意或拒绝双修
const int MSG_REQ_CANCCEL_DOUBLE_MEDITATION = 9607;		//玩家请求取消双修,自己单修
const int MSG_GM2C_BROADCAST_DOUBLE_MEDITATINO = 9608;//服务器广播九宫格内有玩家双修了
const int MSG_GM2WS_PLAYER_NEXUS = 9609;								//逻辑服请求世界服玩家的关系
const int MSG_GM2WS_ADD_CLOSENESS = 9610;							//逻辑服通知世界服增加亲密度
const int MSG_WS2GS_NEXUS_CHANGE = 9611;								//关系改变
const int MSG_GS2C_NEXUS_CHANGE = 9612;									//关系改变

//－－－－－－－－－－－－－－－－－－－－－－VIP与充值 9650 －9700 --------------------------------
const int MSG_REQ_C2GM_GETVIPGIFT			  = 9650;			//领取VIP奖励
const int MSG_REQ_C2GM_GETGIFTINFO			  = 9651;			//VIP礼物领取状态

const int MSG_REQ_GM2DB_PLAYERVIPINFO			= 9660;				//请求VIP信息

const int MSG_REQ_GM2DB_PLAYERRECHARGEINFO  	= 9661;		//请求充值信息
const int MSG_REQ_C2GM_PLAYERRECHARGEINFO 	  = 9662;			//请求充值信息
const int MSG_REQ_C2GM_GETRECHARGEGIFT			   = 9663;			//领取充值奖励

/////-----------------------------------神兵 9700 － 9800－－－－－－－－－－－－－－－－－－－－－－－－

const int MSG_SIM_GM2C_MAGICWEAPON_INFO			= 9701;			//发送神兵信息
const int MSG_REQ_C2GM_ADVANCE_MAGIC   			= 9702;			//请求进阶神兵信息
const int MSG_REQ_C2GM_FLUSH_MAGIC_SKILL   		= 9703;			//请求刷新神兵技能信息
const int MSG_SIM_C2GM_REPLACE_MAGIC_SKILL		= 9704;			//替换神兵技能信息
const int MSG_SIM_GM2C_SYNCH_MAGIC      	    = 9705;			//广播神兵信息


const int MSG_SIM_GM2C_MAGIC_INFO				= 9710;			//发送法器信息
const int MSG_REQ_C2GM_IMPOSE_MAGIC				= 9711;			//法器进阶
const int MSG_SIM_GM2C_HEART_MAGIC_INFO			= 9712;			//发送心法信息
const int MSG_REQ_C2GM_IMPOSE_HEART_MAGIC		= 9713;			//心法进阶

/////-----------------------------------官运 9800 － 9900－－－－－－－－－－－－－－－－－－－－－－－－

const int MSG_REQ_C2GM_CJOB_INFO   			= 9801;			//请求官运信息
const int MSG_REQ_C2GM_RAND_CJOB   			= 9802;			//请求随即官印
const int MSG_REQ_C2GM_SALE_CJOB   			= 9803;			//请求卖出/拾取官印
const int MSG_REQ_C2GM_EXCHANGE_CJOB   			= 9804;			//请求兑换官印
const int MSG_REQ_C2GM_EAT_CJOB   			= 9805;			//请求吞噬官印
const int MSG_REQ_C2GM_WEAR_CJOB   			= 9806;			//请求穿戴官印
const int MSG_REQ_C2GM_OPEN_CJOB   			= 9807;			//请求开启官印格子
const int MSG_REQ_C2GM_CHANGE_CJOB   			= 9808;			//请求交换身上的官印
const int MSG_REQ_C2GM_UNWEAR_CJOB   			= 9809;			//请求脱下身上的官印
const int MSG_SIM_GM2C_OPEN_PLAYER_CJOB   			= 9810;			//开启身上的官印格子



//------------------------------  丹道 ----------------------------------------------------

const int MSG_REQ_C2GT_DANLUINFO				  = 10001; //获取丹道信息
const int MSG_REG_GT2GM_DANLUINFO       =  10002;

const int MSG_REG_C2GT_EATDANLU         = 10003;  //丹道 练炉
const int MSG_REG_GT2GM_EATDANLU        = 10004;

const int MSG_REG_C2GT_DANLUEQUIP      	= 10005;  //练炉出物品

const int MSG_REG_C2GT_GETDANLUEQUIP      	= 10006;  //获取练炉出物品

//------------------------------  钱庄 ----------------------------------------------------

const int MSG_REG_C2GT_MONSYSHOPINFO        = 10010; //获取钱庄信息
const int MSG_REG_C2GT_EXCHANGEMONEY        = 10011;  //钱庄操作

//------------------------------  幸运转盘 ----------------------------------------------------

const int MSG_REG_C2GT_LUCKLYTURNTABLEINFO        = 10015; //获取转盘 信息
const int MSG_REG_C2GT_REFULSH_LUCKLYTURNTABL_EREAW        = 10016;  //转盘 操作
const int MSG_REG_C2GT_ROLL_LUCKLYTURNTABL        = 10017; //获取转盘 信息
const int MSG_REG_C2GT_GET_LUCKLYTURNTABL_EREAW        = 10018;  //转盘 操作

//------------------------------  答题活动10050－10100 ----------------------------------------------------
const int MSG_REQ_C2GM_ENTERANSWERACTIVITY		= 10051;	//申请进入答题活动
const int MSG_REQ_C2GM_LEAVEANSWERACTIVITY		= 10052;	//请求离开答题活动

const int MSG_SIM_GM2C_ANSWERINSTANCEID		   = 10060;
const int MSG_SIM_GM2C_PUBLISHANSWER				= 10061;	//公布答案
const int MSG_SIM_GM2C_STARTCOUNTDOWN				= 10062;	//开始倒计时
const int MSG_SIM_GM2C_SYNCANSWERINFO				= 10063;	//同步进入答题场景信息
const int MSG_REQ_GM2GT_GENERATEQUESTIONS			= 10064;	//GM请求创建题目
const int MSG_REQ_GT2WS_GENERATEQUESTIONS			= 10065;	//向WS请求创建题目
const int MSG_REQ_GM2GT_ACTIVITYEND					= 10066;	//GM通知GT活动结束
//const int MSG_REQ_GM2WS_ACTIVITYEND					= 10067;	//GM通知WS活动结束
const int MSG_REQ_GM2WS_SYNCRANK					= 10068;	//排名同步信息GM2WS
const int MSG_SIM_WS2GM_TOCLIENTRANK				= 10070;	//
const int MSG_SIM_GM2C_TOCLIENTRANK					= 10071;	//将整个服的排名发给客户端

const int MSG_REQ_C2GM_ANSWER_INFO              = 10080; //请求个人答题数据
const int MSG_REQ_C2GM_CHOICE_ANSWER				= 10081; //答题

//－－－－－－－－－－－－－－－－－－－－－－监控 10100--------------------------------
const int MSG_REG_C2GT_MAPINFO 									= 10100;							//获取地图信息表
const int MSG_REG_C2GT_MAPPLAYERINFO					= 10101;							//获取地图里面玩家信息表
const int MSG_REQ_C2GT_GETALLGAMESVRINFO      			= 10102; // 获取所有分线的信息
//－－－－－－－－－－－－－－－－－－－－－－开始进入主界面 所有活动要显示 效果 10200--------------------------------
//const int MSG_SIM_HIGHLIGHT_ACTIVEINFO 									= 10200; // 通知前端ui高亮  已经不用了

//const int MSG_SIM_C2GT_UPDATEDAILY 													= 10201;  //通知每日任务刷新

const int MSG_SIM_C2GT_UPDATEDAYCOUNTER              = 10202;//通知  每日次数刷新
const int MSG_SIM_NOTICE_UIHIGHLIGHT								= 10203; // 通知前端ui高亮
const int MSG_SIM_NOTICE_TASKPANEL = 10204; // 通知前端任务面板


//-------------------------------帮派     10300----------------------------------------------

const int MSG_REQ_C2WS_CREATE_CLAN				       = 10300;							//创建帮派
const int MSG_REQ_C2WS_CLAN_BASEINFO				    = 10301;							//请求帮派基本信息
const int MSG_REQ_C2WS_CLAN_LIST					       = 10302;							//请求帮派列表信息
const int MSG_REQ_C2WS_CLAN_MEMBER_LIST			 		= 10303;							//请求帮派成员列表信息
const int MSG_REQ_C2WS_CLAN_EVENT					      = 10304;						//请求帮派事件
const int MSG_REQ_C2WS_CLAN_REQUEST_LIST					= 10305;						//请求玩家申请加入本帮派的成员列表
const int MSG_REQ_C2WS_CLAN_CONTRI_LIST					= 10306;							//请求帮派贡献列表
const int MSG_REQ_C2WS_JOIN_CLAN		        			= 10307;							//请求加入帮派
const int MSG_SIM_C2WS_CLAN_TITLE		        		= 10308;							//帮派公告
const int MSG_REQ_C2WS_MEMBER_CONTRIT		        = 10309;							//成员请求贡献金钱或者金币
const int MSG_REQ_C2WS_UPDATE_CLAN    		      = 10310;							//帮派升级
const int MSG_REQ_C2WS_CHANGE_ICON		        		= 10311;							//更换兵符图标
const int MSG_REQ_C2WS_CHANGE_JOB     		      = 10312;							//请求任职
const int MSG_REQ_C2WS_OUT_JOB        		      = 10313;							//请求禅让
const int MSG_REQ_C2WS_KICK_MEMBER      		     = 10314;							//请求开除
const int MSG_SIM_C2WS_LEAVE_CLAN      		     = 10315;							//退出帮派／解散帮派
const int MSG_SIM_C2WS_CHANGE_JOIN_CON 		     = 10316;							//改变自动加入申请
const int MSG_REQ_C2WS_JOIN_CONTROL      		    = 10317;							//审核入帮操作
const int MSG_REQ_C2WS_REQUEST_JOIN      		    = 10318;							//请求 邀请别人入帮
const int MSG_SIM_WS2C_REQUESTED_JOIN      		   = 10319;							//通知被邀请人有人加他入帮
const int MSG_REQ_C2WS_REFUSE_OR_JOIN      		   = 10320;							//请求拒绝或者同意邀请加入帮派
const int MSG_REQ_WS2GM_TACK_OFF          		   = 10321;							//请求扣除创建帮派费用
const int MSG_REQ_WS2GM_CHANGE_ATTR          		= 10322;							//请求加入帮派属性修改
const int MSG_SIM_GM2WS_SYN_ATTR          		   = 10323;							//帮派属性同步
const int MSG_SIM_GM2WS_SYN_CLANEVENT          	= 10324;							//帮派事件同步
const int MSG_REQ_WS2GM_COST_CONTRIBUTION      	= 10325;							//帮派捐献扣钱
const int MSG_SIM_GM2C_SYN_CLANNAME            	= 10326;							//帮派名字改变同步
const int MSG_SIM_WS2GM_SYN_ATTR          		   = 10327;							//帮派属性同步

const int MSG_REQ_C2WS_CLAN_SINGUP			   				= 10328;						//请求报名帮派霸业战
const int MSG_REQ_C2WS_SINGUP_INFO			        	= 10329;						//查看报名信息
const int MSG_REQ_C2WS_CLAN_BATTER_INFO		   	= 10330;						//查看对战列表信息
const int MSG_REQ_C2WS_ENTER_BATTLE_INSTANCE		= 10331;						//请求进入副本
const int MSG_REQ_C2GS_OCCUPY_FLAG							  	= 10332;						//请求占领旗子
const int MSG_SIM_GS2C_CLAN_BATTLE_BASE	   		= 10333;						//进入副本，推送战场基本数据
const int MSG_SIM_GS2C_CLAN_BATTLE_UPDATE   		= 10334;						//服务器推送战场积分数据属性改变
const int MSG_SIM_GS2C_FLAG_STATE_CHANGE	   		= 10335;						//服务器广播通知某个玩家占领了旗子   推送xxx帮派xxx玩家占领xxx旗子成功
const int MSG_SIM_GS2C_FLAG_TIMEOUT				   		= 10336;						//服务器推送旗子的保护时间到了
const int MSG_SIM_GS2C_CLAN_BATTLE_RESULT   		= 10337;						//服务器推送胜利失败结果

const int MSG_SIM_GS2C_OCCUPY_FAIL									= 10338;							//通知采集占领旗子失败

const int MSG_SIM_GM2WS_CLAN_BATTLE_RESULT   	= 10340;							//通知世界服胜利失败结果
const int MSG_REQ_GS2WS_FACTION_NAME				   	= 10341;							//逻辑服向世界服请求某个帮派的名称
const int MSG_SIM_WS2GM_LEAVE_CLANWAR				   	= 10342;							//通知帮派解散，帮派撤出战斗

const int MSG_REQ_C2WS_BATTLE_NUMBER         	= 10343;							//请求战场人数
const int MSG_SIM_WS2C_GANG_CHANNEL								= 10344;								//帮派频道每隔3分钟发送一次
const int MSG_REQ_C2GM_MAP_ALL_CREATURE					= 10345;							//M图中，所有角色坐标
const int MSG_SIM_GM2C_BATTLE_RANK 								= 10346; 							//帮派战场中本帮战况排名
const int MSG_SIM_GM2WS_OFFLINE_REWARD 					= 10347;							//帮派战下线奖励

const int MSG_REQ_C2WS_LIMIT_SHOP 								= 10348;							//帮派商品剩余数量
const int MSG_REQ_C2GM_SHOP_GOODS							= 10349;							//帮贡兑换物品
const int MSG_REQ_GM2WS_SHOP_SUCCESS 					= 10350;							//帮派购减物品
const int MSG_REQ_C2WS_SKILL_UPDATE							= 10351;							//帮派技能升级
const int MSG_SIM_WS2C_SKILL_UPDATE							= 10352;							//通知其它帮派成员升级成功
const int MSG_SIM_WS2GM_AD_SKILLBUFF						= 10353;							//增删技能buff
const int MSG_SIM_WS2GM_CLANWARBEGIN 					= 10354; 						// 通知GM帮派战开始
const int MSG_REQ_WS2GM_SYNWILLJOIN = 10355; //  同步玩家加入
const int MSG_REQ_C2WS_CLANWELFAREINFO = 10356; // 请求帮派福利信息
const int MSG_REQ_C2WS_SENDCLANWELFARE = 10357; // 请求发送帮派福利
const int MSG_REQ_WS2GM_CLANWELFARECOST = 10358; // WS请求GM 扣除帮派福利花费
const int MSG_REQ_C2WS_TAKECLANWELFARE = 10359; // 请求领取帮派福利
const int MSG_REQ_WS2GM_TAKECLANWELFARE = 10360; // WS请求GM 领取帮派福利
const int MSG_REQ_C2WS_BACK_CLAN_MAP = 10365; 			//请求返回帮派住地
const int MSG_REQ_C2WS_CLAN_SKILLS = 10366;  				//请求个人的帮派技能信息
const int MSG_REQ_C2WS_UPGRADE_CLAN_SKILL = 10367; 	//请求升级帮派技能技能
const int MSG_SIM_WS2GS_ACTIVE_START_END = 10368; 	//通知帮派强盗活动开启或结束
const int MSG_SIM_WS2GS_DESTROY_CLAN_MAP = 10369; 	//通知销毁帮派场景
const int MSG_SIM_GS2WS_ROBBER_RESULT = 10370; 			//通知强盗活动结果
const int MSG_SIM_GS2WS_CLAN_CONTRIBUTE_ALTER = 10371;	//通知帮派资金增加
const int MSG_SIM_GS2WS_ROBBER_XXXX = 10375; 			//通知销毁帮派场景
const int MSG_REQ_C2WS_INTEGRAL_INFO = 10376; //积分战信息
const int MSG_REQ_C2WS_INTEGRAL_SINGUP = 10377; //积分战报名
const int MSG_REQ_C2WS_INTEGRAL_RESINGUP = 10378; //积分战补报
const int MSG_REQ_C2WS_ATTCLAN_INFO = 10379; //查看对方帮派信息
const int MSG_REQ_C2WS_INTEGRAL_NOTICEWAR = 10380; //通知在线成员参战
const int MSG_REQ_C2GM_INTEGRAL_KILLRANK_INFO = 10381; //查看击杀排名
const int MSG_SIM_GM2C_INTEGRAL_SINGLEKILL = 10382; //副本面板信息
const int MSG_SIM_GM2C_INTEGRAL_REWARD = 10383; //副本奖励信息
const int MSG_SIM_WS2C_INTEGRALWINREWARD = 10384; //帮派胜利奖励信息
const int MSG_REQ_WS2GM_INTEGRAL_SYSRESINGUP = 10385; //积分战补报扣元宝
const int MSG_SIM_WS2C_INTEGRAL_NOTICEONLYID = 10386; //通知帮派积分战副本ID
const int MSG_SIM_GM2WS_INTEGRAL_SYSRESULT = 10387; //同步积分战结果
const int MSG_SIM_WS2C_INTEGRAL_NOTICEWAROTHER = 10388; //同步通知结果

const int MSG_REQ_C2WS_CHANCE_JOIN_CLAN = 10389; //取消加入帮派

//--------------------------数据管理服  操作-----------------------------------------------------
//const int MSG_REQ_MS2WS_ONLINECOUNT				       = 10400;							//获取在线人数

const int MSG_SIM_MS2GM_SENDMAIL                = 10401;     				//发送系统邮件
const int MSG_REQ_C2MS_SENDMAIL                 =  10402;   				//管理系统  通知发送 系统邮件
const int MSG_REQ_MS2DB_SENDMAIL                = 10403;

const int MSG_REQ_C2MS_CHARINFO                 = 10404;            //获取角色信息
const int MSG_REQ_MS_DB_CHARINFO                = 10405;

const int MSG_REQ_C2MS_RANKINGINFO              = 10406;           //获取 排行信息
const int MSG_REQ_MS2WS_RANKINGINFO             = 10407;

const int MSG_REQ_C2MS_OPENIDDOSOMETHING        = 10408;      //对账号进行解/禁
const int MSG_REQ_MS2DB_OPENIDDOSOMETHING       = 10409;
const int MSG_REQ_C2MS_LOCKOPENIDINFO           = 10410;     //获取 服中被封帐号
const int MSG_REQ_MS2DB_LOCKOPENIDINFO          = 10411;

const int MSG_REQ_C2MS_ABOUTTALKING             = 10412;   //解 禁言
const int MSG_REQ_MS2DB_ABOUTTALKING            = 10413;

const int MSG_REQ_C2MS_SHUTUPINFO               = 10414;  //查询 所有被进言的信息
const int MSG_REQ_MS2DB_SHUTUPINFO              = 10415;

//---------------------通知  解/禁 说话----------------------------------------//
const int MSG_SIM_DB2WS_ABOUTTALKING            = 10416;   //DB通知 说话相关
const int MSG_SIM_WS2GT_ABOUTTALKING            = 10417;
const int MSG_SIM_GT2C_ABOUTTALKING             = 10418;  //通知客户段有解/禁言
const int MSG_SIM_GT2GM_ABOUTTALKING            = 10419;

//-------------------获取当行  显示 信息------------------//
const int MSG_REQ_C2MS_GETOUTPUTLINECHARINFO 				= 10420;
const int MSG_REQ_MS2DB_GETOUTPUTLINECHARINFO 				= 10421;

//-----------------通过网站获取 登录服组的 相关信息------------------------//
const int MSG_REQ_C2MS_GETLOGINSERVERINFO     = 10422;
const int MSG_REQ_MS2DB_GETLOGINSERVERINFO     = 10423;

const int MSG_SIM_REPORT_LOGINPLAYERCOUNT      = 10424;  //登录服报告在线人数 获取状态

const int MSG_REQ_WB2MS_BRANCHONLINECOUNT     = 10425;  //获取 分线 在线人数
const int MSG_REQ_MS2GM_BRANCHONLINECOUNT     = 10426;  //获取 分线 在线人数

const int MSG_REQ_WB2MS_BRANCHONLINEPLAYERINFO     = 10427; //获取 分线 在线玩家信息
const int MSG_REQ_MS2GM_BRANCHONLINEPLAYERINFO     = 10428; //获取 分线 在线玩家信息

const int MSG_SIM_GM2MS_REPORTONLINECOUNT 						=  10429;    //分线 向 管理服报告 当前在线人数

const int MSG_REQ_C2MS_SYSTEMNOTICE 				 =  10430;//系统公告操作（增加 删除 修改 查看）
const int MSG_REQ_MS2WS_SYSTEMNOTICE             = 10431;
//----------------------------获取 以列表形式  获取玩家 列表相关 信息---------------------------------
const int MSG_REQ_WEB2MS_LIST_PLAYERINFO           = 10432;
const int MSG_REQ_MS2DB_LIST_PLAYERINFO           = 10433;

// --------------- 网站获取帮派信息 的 相关信息
const int MSG_REQ_C2MS_GETALLCLANINFO										= 10434; // 客户端向管理服获取所有帮派信息
const int MSG_REQ_MS2WS_GETALLCLANINFO 								= 10435; // 管理服向世界服获取所有帮派信息
const int MSG_REQ_C2MS_GETSOMECLANINFO									= 10436; // 客户端向管理服获取某个帮派详细信息
const int MSG_REQ_MS2WS_GETSOMECLANINFO									= 10437; // 管理服向世界服获取某个帮派详细信息


const int MSG_REQ_C2MS_EXCHANGE_GIFT            = 10438;//网页->MS 兑换礼品
const int MSG_SIM_MS2WS_EXCHANGE_GIFT            = 10439;//兑换礼品
const int MSG_SIM_GM2C_EXCHANGE_GIFT            = 10440;//兑换礼品

const int MSG_SIM_C2GM_EXCHANGE_GIFT            = 10441;//兑换礼品
const int MSG_SIM_GM2MS_EXCHANGE_GIFT            = 10442;//兑换礼品
const int MSG_SIM_MS2C_EXCHANGE_GIFT            = 10443;//MS->网页 兑换礼品
//-------------------------------邮件列表－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_WEB2MS_MAILLIST               = 10444;
const int MSG_REQ_MS2DB_MAILLIST                = 10445;
//------------------邮件详情－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_WEB2MS_MAILINFO               = 10446;
const int MSG_REQ_MS2DB_MAILINFO                = 10447;
//-------------------------获取同一个OpendID下的所有角色部分信息----------------------
const int MSG_REQ_WEB2MS_PLAYER_GET_ROLEINFO_BY_OPENID = 10448;
const int MSG_REQ_MS2DB_PLAYER_GET_ROLEINFO_BY_OPENID = 10449;
//-------------------------防沉迷系统------------------------------------------------------------
const int MSG_REQ_C2GM_BIND_ANTI_ADDICTION_ID   = 10500;                   //绑定身份证号码
const int MSG_SIM_GM2DB_BIND_ANTI_ADDICTION_ID  = 10501;			          //让DB保存

const int MSG_REQ_C2MS_FORBIDDEN  = 10502;//解除与禁止
const int MSG_REQ_MS2WS_FORBIDDEN  = 10503;
const int MSG_REQ_WS2GM_FORBIDDEN  = 10504;
const int MSG_SIM_WS2DB_FORBIDDEN  = 10505;
//const int MSG_SIM_GM2MS_REPORTPLAYERONLINETIME  = 10510; // 分线 向 管理服报告 所有玩家在线时长
//-----------------------------------充值--------------------------------------------
const int MSG_REQ_WEB2MS_RECHARGE = 10506;
const int MSG_REQ_MS2WS_RECHARGE = 10507;
const int MSG_REQ_WS2DB_RECHARGE = 10508;
const int MSG_REQ_WS2GM_RECHARGE = 10509;

//客户端提交反馈
const int MSG_REQ_C2GM_BUG_SUBMIT = 10510;
const int MSG_REQ_GM2DB_BUG_SUBMIT = 10511;
//网站后台获取反馈基本信息列表
const int MSG_REQ_WEB2MS_BUG_SUBMIT_LIST = 10512;
const int MSG_REQ_MS2DB_BUG_SUBMIT_LIST = 10513;
//网站后台处理反馈
const int MSG_REQ_WEB2MS_BUG_SUBMIT_OP = 10514;
const int MSG_REQ_MS2DB_BUG_SUBMIT_OP = 10515;

//-------------------------获取一个OpendID下的qzoneID角色的基本信息----------------------
const int MSG_REQ_WEB2MS_PLAYER_GET_ROLEINFO_BY_OPENID_QZONEID = 10516;
const int MSG_REQ_MS2DB_PLAYER_GET_ROLEINFO_BY_OPENID_QZONEID = 10517;

//-------------------------获取背包或仓库详情------------------------------------------
const int MSG_REQ_WEB2MS_PLAYER_GET_CONTAINERINFO = 10518;
const int MSG_REQ_MS2DB_PLAYER_GET_CONTAINERINFO = 10519;

const int MSG_SIM_GM2C_DROP_GOODS_OR_QEUIP	 = 10600;	//强制玩家掉落几件物品包括装备

const int MSG_REQ_WEB2MS_RANKING = 10601;//网页向管理服请求排行榜信息
const int MSG_REQ_MS2WS_RANKING = 10602;//管理服向世界服请求排行榜信息
//------------------------获取各等级区间人数-------------------------------
const int MSG_REQ_WEB2MS_PLAYER_LV_COUNT = 10603;//网页向管理服请求各等级区间人数
const int MSG_REQ_MS2DB_PLAYER_LV_COUNT = 10604;//管理服向DB请求各等级区间人数
//----------------------------------守护秦王－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

const int MSG_REQ_C2GM_PROTECTED_KING_INFO   = 10650;                   //获取守护秦王信息
const int MSG_REQ_C2GM_REFLUSH_COUNT         = 10651;                   //请求重置
const int MSG_REQ_C2GM_DRIRECTED_FINISH      = 10652;                   //请求扫荡
const int MSG_REQ_C2GM_GET_REWARD            = 10653;                   //请求获取奖励
const int MSG_SIM_C2GM_GET_NODES_REWARD      = 10654;                   //获取闯关奖励
const int MSG_SIM_GM2C_PROTECTE_RESULT       = 10655;                   //同步一关的结果
const int MSG_SIM_WS2GM_DOUBLE_PROTECTEDKING = 10656; // 双倍守护秦王


//----------------------------------剿灭反王－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

const int MSG_REQ_C2GM_TREASONKING_INFO = 10700;    	//获取反王通关信息
const int MSG_REQ_C2GM_TREASONKING_SOMETHING= 10701;    	//进行挖宝操作
const int MSG_REQ_C2GM_TREASONKING_REWARD = 10702;	//获取奖励
const int MSG_REQ_C2GM_TREASONKING_RESET = 10703; //重置


//------------------------------黄钻特权－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_C2GM_YELLOWVIP_INFO = 10750;    //获取黄钻信息
const int MSG_REQ_C2GM_YELLOWVIP_NOVICEPACKS = 10751;    //新手礼包
const int MSG_REQ_C2GM_YELLOWVIP_GROWPACKS = 10752;    //成长礼包
const int MSG_REQ_C2GM_YELLOWVIP_DAILYPACKS = 10753;    //每日礼包
const int MSG_REQ_C2GM_YELLOWVIP_YEARVIPPACKS = 10754;    //年费黄钻
const int MSG_REQ_C2GM_YELLOWVIP_GIRLWAR = 10755;    //年费黄钻

//---------------------------------------------------游戏目标-------------------------------
const int MSG_REQ_GETDESTINATIONTYPE				 = 10800;			//获取某阶段游戏目标信息
const int MSG_REQ_GETDESTINATIONPRIZE				 = 10801;			//获取游戏目标奖励
const int MSG_SIM_DESTINATIONCOMPLETE				 = 10802;			//某游戏目标完成
const int MSG_REQ_GETHAVEREWARDDESTINATION         =  10803;        //获取有奖励可领的游戏目标

//----------------------------------夸服竞技－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

const int MSG_REQ_C2GM_BEYOND_SERVER_INFO				 = 10850;			//获取夸服竞技信息
const int MSG_REQ_C2WS_BEYOND_SERVER_RANK				 = 10851;     //获取夸服竞技 排行
const int MSG_REQ_C2WS_BEYOND_RANK_PLAYER				 = 10852;     //获取夸服竞技 排行的玩家信息
const int MSG_REQ_GM2WS_MATCH_PLAYER      				 = 10853;     //匹配对手
const int MSG_SIM_WS2GM_MATCH_PLAYER      				= 10854;     //通知gm匹配对手
const int MSG_SIM_GM2WS_MATCHED_PLAYER      			= 10855;     //通知ws已经匹配到对手
const int MSG_SIM_WS2GM_ALREADY_MATCH_PLAYER    = 10856;     //通知gm已经匹配到对手，删除角色信息
const int MSG_SIM_WS2C_MATCH_PLAYER_SUCCESS     = 10857;     //通知客户端匹配到对手
const int MSG_SIM_C2WS_CANCEL_MATCH_PLAYER      = 10858;     //取消匹配对手
const int MSG_SIM_GM2WS_PK_RESULT           			= 10859;     //通知ws夸服战结果
const int MSG_SIM_GM2C_BEYOND_RESULT            = 10860;     //通知客户端战斗结果
const int MSG_SIM_WS2GM_CANCEL_MATCH_PLAYER     = 10861;     //通知gm玩家取消匹配了
const int MSG_REQ_C2GM_GET_BEYOND_REWARD 				 = 10862;			//获取奖励
const int MSG_REQ_C2GM_MATCH_PLAYER									= 10863;			// 匹配对手


//----------------------------------夸服竞技－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

//--------------------------------------------------每日必做---------------------------------
const int MSG_REQ_C2GM_GETDAILYPRACTICEINFO        = 10900;         //获取闭关修炼信息
const int MSG_REQ_C2GM_GETDAILYPRACTICEREWARD      = 10901;         //获取闭关修炼奖励
const int MSG_REQ_C2GM_GETYESTERDAYCOUNTERINFO     = 10902;         //获取昨天活动的完成次数
const int MSG_REQ_C2GM_RECOVERRESOURCE             = 10903;         //资源追回
const int MSG_REQ_GM2WS_GETCLANCOVERSOURCEINFO     = 10904;         //获取帮派奖励的领取情况
const int MSG_REQ_C2GM_GETDAILYINFO                = 10905;         //获取每日必做信息

//----------------------------------战姬---------------------------------------------

const int MSG_SIM_GM2C_SYNGIRLWARPATH = 11000; 		//同步战姬路径
const int MSG_REQ_C2GM_SYNGIRLWARINFO = 11001;		//请求战姬信息
const int MSG_REQ_C2GM_ASKGIRLWARUPLV = 11002;	//请求战姬升级
const int MSG_REQ_C2GM_ASKGIRLWARTOFIGHT = 11003;		//请求出战或休战
const int MSG_SIM_GM2C_SYNGIRLWAR = 11004;  	//同步战姬
const int MSG_SIM_GM2C_SYNGIRLWARLEAVE = 11005;	//同步战姬离开
const int MSG_REQ_C2GM_ASKGIRLWARREVIVE = 11006;	//请求战姬复活
const int MSG_SIM_GM2C_GIRLWARACTIVATE = 11007;	//激活战姬
//const int MSG_SIM_GM2C_GETGIRLWAR = 11008;	//激活限时战姬
//const int MSG_SIM_GM2C_DELETEGIRL = 11009;			//删除限时战姬
const int MSG_SIM_C2GM_UPDATEGIRLWAR_UISTATE = 11010; // 更改战姬UI高亮状态
const int MSG_REQ_C2GM_ASKGIRLWAR_WAREEQUIP = 11011; //请求战姬加成
const int MSG_REQ_C2GM_ASKGIRLWAR_ONEKEYWAREEQUIP = 11012; //请求战姬一键加成
const int MSG_REQ_C2GM_ASKGIRLWAR_UPQUALITY = 11013; //请求战姬进阶
const int MSG_REQ_C2GM_ASKGIRLWAR_UPSKILL = 11014; //请求战姬技能升级
const int MSG_REQ_C2GM_GIRLWARUPSTAR = 11015;	//请求战姬升星
const int MSG_REQ_C2GM_GETGIRLWAR = 11016; //被动获取战姬

// ------------------------------ 时装 -------------------------------------------
const int MSG_REQ_C2GM_FASHIONINFO = 11100; // 请求时装信息
const int MSG_REQ_C2GM_ACTIVATEFASHION = 11101; // 请求激活时装
const int MSG_REQ_C2GM_WEARFASHION = 11102; // 请求穿戴或卸下时装
const int MSG_REQ_C2GM_STRENTHFASHION = 11103; // 强化时装
// const int MSG_SIM_GM2C_SYNFASHION = 11103; // 同步时装
const int MSG_SIM_GM2C_HANDLEFASHION = 11104; // 广播穿卸时装
const int MSG_REQ_C2GM_SHOWHIDEFASHION = 11105; // 时装显示/隐藏


//------------------------------市场---------------------------------------------
const int MSG_REQ_C2WS_MARKET_INFO = 11200;	//请求市场信息
const int MSG_REQ_C2WS_MARKET_SHOPPING = 11201; //请求市场购物
const int MSG_REQ_C2WS_SHOP_INFO = 11202;	//请求店铺信息
const int MSG_REQ_C2WS_UNDER_CARRIAGE = 11203;	//请求物品下架
const int MSG_REQ_C2WS_UP_CARRIAGE = 11204;	//请求物品上架
const int MSG_REQ_C2WS_EARNINGS = 11205;	//请求店铺收益
const int MSG_REQ_WS2GM_MAKERT_INFO = 11206; //请求市场信息
const int MSG_SIM_WS2GM_CHANGE_EARNINGS = 11207;//增加收益
const int MSG_REQ_WS2GM_UP_CARRIAGES = 11208;	//商品上架
const int MSG_SIM_C2GM_UPDATEMARKET_UISTATE = 11209; // 更改市场UI状态
const int MSG_SIM_WS2GM_NOTICESELLERUI = 11210; // 通知出售者UI状态

//------------------------------结婚-----------------------------------------------
const int MSG_REQ_C2WS_PROPOSE = 11300;	//求婚
const int MSG_SIM_WS2C_NOTE_PROPOSED = 11301;	//通知被求婚
const int MSG_REQ_C2WS_PROPOSE_ANSER = 11302;		//回复求婚信息
const int MSG_SIM_WS2C_PROPOSED_RESULT = 11303;	//求婚结果
const int MSG_REQ_C2WS_MARRY_INFO = 11304; 	//请求结婚信息
const int MSG_REQ_C2WS_ORDER_WEDDING = 11305;	//请求婚宴预定
const int MSG_REQ_C2WS_INVITATION_ISSUED_INFO = 11306;	//喜贴发放信息
const int MSG_REQ_C2WS_INVITAITON_ISSUED = 11307;	//发放喜帖
const int MSG_REQ_C2WS_DELETE_INVITAITION = 11308; //删除喜贴
const int MSG_SIM_WS2C_INVITAITON_SEND = 11309;	//服务器发送喜贴
const int MSG_SIM_WS2C_INVITATION_INSTANCE = 11310; //副本推送
const int MSG_REQ_C2GM_INVITAITON_CELEBRATION = 11311; //庆祝婚礼方式
const int MSG_REQ_WS2GM_CHECK_MARRY = 11312;	//检查结婚条件
const int MSG_REQ_WS2GM_BUY_RING = 11313; //购买戒指
const int MSG_SIM_GM2C_INVITAITON_CELEBRATION = 11314; //推送播放特效类型
const int MSG_SIM_WS2C_ORDER_WEDDING = 11315;	//返回客户端婚宴预定信息
const int MSG_REQ_C2GM_CELE_STATUS = 11316;	//请求婚礼进行状态
 const int MSG_SIM_GM2C_CELE_READY = 11317;	//推送对方准备信息
 const int MSG_SIM_WS2C_DELETE_INVITE = 11318; //推送被删除邀请信息
 const int MSG_SIM_WS2C_WDDING_OVER = 11319; //推送婚宴结束信息
//－－－－－－－－－－－－－－－－－－－－－－活跃度－－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_C2GM_GETVITALITYINFO = 11500;		//获取活跃度信息
const int MSG_REQ_C2GM_GETVITALITYGIFT = 11501;		//领取活跃度奖励
const int MSG_SIM_GM2C_VITALITYINFO	= 11502;				//同步活跃度信息

// ------------------------------------ 膜拜 －－－－－－－－－－－－－－－－－－－－－－－－
const int MSG_REQ_C2WS_WORSHIP = 11600; // 请求膜拜
const int MSG_REQ_WS2GM_WORSHIP = 11601; // WS向GM请求膜拜
const int MSG_REQ_WS2DB_WORSHIP = 11602; // WS向DB请求膜拜
const int MSG_REQ_WS2GB_ADDBYWORSHIP = 11603; // WS2GM增加被膜拜者
const int MSG_REQ_WS2GM_CHECKWORSHIP = 11604; // 检查是否能膜拜
//---------------------------------陈营-------------------------------------------------
const int MSG_REQ_C2WS_COUNTRYTOTAL = 12000; 		 //加入陈营前请求陈营总体信息
const int MSG_REQ_C2WS_COUNTRYINFO = 12001; 		  //请求本陈营信息
const int MSG_REQ_C2WS_JIONCOUNTRY = 12002; 		  //请求加入陈营
const int MSG_REQ_C2WS_CHANGECOUNTRY = 12003;   //请求更换陈营
const int MSG_REQ_C2WS_COUNTRY_LIVEUP = 12004;   //升级陈营
const int MSG_REQ_C2GS_COUNTRY_JUANXIAN = 12005; //陈营物品捐献
const int MSG_REQ_C2WS_COUNTRY_NOTICE = 12006;   //修改陈营公告
const int MSG_REQ_C2WS_COUNTRY_MEMBERS = 12007;	 //客户端请求某页陈营成员列表
const int MSG_REQ_C2WS_COUNTRY_JOB_LASTWEEK = 12008;	 //客户端请求上周陈营官职成员信息
const int MSG_REQ_C2WS_COUNTRY_KING_RANK = 12009;	 //客户端请求霸主排行
const int MSG_REQ_C2GS_COUNTRY_JOB_DAYPRIZE = 12010;	 //客户端请求领取每日官职奖励
const int MSG_REQ_C2GS_COUNTRY_SHOPINFO = 12011;	 //客户端请求陈营商店物品信息
const int MSG_REQ_C2GS_COUNTRY_BUY = 12012;	 				 //客户端请求购买陈营商店物品
const int MSG_REQ_C2WS_COUNTRY_STORE = 12013;  		 //请求陈营仓库信息
const int MSG_REQ_GS2WS_COUNTRY_JUANXIAN = 12014;  //逻辑服向世界服请求捐献
const int MSG_WS2GS_COUNTRY_LIVEUP = 12015;  			 //世界服通知逻辑服陈营升级
const int MSG_WS2GS_COUNTRY_BASEINFO = 12016;  		 //世界服通知逻辑服陈营基本信息
const int MSG_WS2GS_COUNTRY_RANK_RESET = 12017;  	 //世界服通知逻辑服周排行重置
const int MSG_WS2GS_COUNTRY_ATTR_ALTER = 12018;  	 //世界服通知逻辑服陈营属性改变
const int MSG_SIM_WS2GM_EXCHANGE_GIFT            = 12019;//兑换礼物
const int MSG_REQ_WS2GM_GETJOINAWARD = 12020; // 获取加入推荐阵营奖励


//------------------------------------阵营红包id:12100~12199----------------------------------
const int MSG_REQ_C2WS_GET_REDENVELOPE_LIST = 12100;//获取可领取的红包列表
const int MSG_REQ_C2GM_GET_REDENVELOPE  = 12101;//获取红包
const int MSG_REQ_GM2WS_GET_REDENVELOPE = 12102;//获取红包
const int MSG_REQ_C2GM_SEND_REDENVELOPE  = 12103;//红包派发
const int MSG_REQ_GM2WS_SEND_REDENVELOPE  = 12104;//红包派发
const int MSG_REQ_C2WS_GET_REDENVELOPE_RECORD_LIST  = 12105;//红包领取记录
const int MSG_REQ_C2WS_GET_REDENVELOPE_REWARD  = 12106;//获取红包被其他玩家领取后的奖励
const int MSG_REQ_WS2GM_GET_REWARD = 12107;//世界服通知逻辑服领取奖励
const int MSG_REQ_GET_ATTR = 12108;//客户端获取某个或某几个属性
//const int MSG_SIM_WS2GM_ISHAVEENVELOPETOGET = 12109;//世界服向逻辑服同步玩家能领取的红包
const int MSG_SIM_WS2GM_AllGMADDENVELOPE = 12110; // 世界服通知所有逻辑服的所有玩家增加领取的红包
const int MSG_SIM_WS2GM_PLAYERADDENVELOPE = 12111; // 世界服通知逻辑服的所有玩家增加领取的红包
const int MSG_SIM_WS2GM_ENVSENDERBYGET = 12112; // 世界服通知逻辑服的红包发送者的

//----------------------------------阵营boss----------------------------------------------------------
const int MSG_REQ_C2WS_ENTER_COUNTRY_MAP = 12200;	//请求进入副本
const int MSG_SIM_GM2C_COUNTRY_INTEGRAL = 12201; //推送阵营积分
const int MSG_SIM_GM2C_COUNTRY_HARM_RANK = 12202; //阵营伤害排行
const int MSG_SIM_GM2C_COUNTRY_HARM_SETTLE = 12203;//阵营伤害结算
const int MSG_REQ_WS2C_COUNTRY_BEFORE_START = 12204;//副本开始提前通知时间
const int MSG_SIM_GM2C_COUNRTY_REWARD = 12205; //阵营奖励
const int MSG_SIM_GM2WS_SYSWIN_REWARD = 12206; //同步阵营胜利奖励


//---------------------------------------------------陈营战---------------------------------------
const int MSG_REQ_COUNTRY_ELEMENTS_INFO 	= 12300;	//请求五行柱占领信息（被谁占领或没有占领，保护时间。。。等）
const int MSG_REQ_COUNTRY_WAR_DATA 				= 12301;	//请求争夺战绩信息（积分贡献排名。。。等等
const int MSG_REQ_COUNTRY_FITCH_TASK_REWARD = 12302;	//请求领取争夺活动中的任务奖励
const int MSG_REQ_COUNTRY_WAR_HISTORY = 12303;	//请求陈营历史胜负战绩
const int MSG_REQ_COUNTRY_OCCUPY_ELEMENTS = 12304;	//请求占领五行柱

const int MSG_SIM_GS2C_COUNTRY_WAR_REWARD = 12305;	//通知活动结算奖励
const int MSG_SIM_GS2C_COUNTRY_WAR_ELEMENT_STATE = 12306;	//每根五行柱的状态改变（谁占领了）
const int MSG_SIM_GS2C_COUNTRY_WAR_ATTR_ALTER = 12307;	//战绩属性改变通知(id+值)
const int MSG_SIM_GS2C_COUNTRY_WAR_TASK_UPDATE = 12308;	//阵营战任务目标更新
const int MSG_REQ_COUNTRY_WAR_MAP = 12309;									//客户端请求陈营战副本地图id
const int MSG_SIM_WS2GS_COUNTRY_WAR_MAP = 12310;					//世界服通知逻辑服阵营副本id
const int MSG_SIM_GS2C_ELEMENT_PROTECTED_TIME = 12311;	//通知五行柱保护时间到了
const int MSG_SIM_GS2C_OCCUPY_SUCCESS = 12312;							//通知玩家五行柱占领成功
const int MSG_SIM_WS2GS_WAR_RECORD = 12313;							//世界服同步阵营历史战绩到逻辑服
const int MSG_SIM_GS2WS_WAR_RESULT = 12314;							//逻辑服同步阵营战绩到世界服

const int MSG_SIM_WS2GM_COUNTRYBATTLEOPEN = 12315; // 世界服通知逻辑服阵营的战争开启

//----------------------------------------------阵营霸主争夺战---------------------------
const int MSG_REQ_C2WS_COUNTRY_OVERLORD_MAP = 12320;  //客户端请求阵营霸主争夺战副本地图id
const int MSG_REQ_C2WS_COUNTRY_OVERLORD_PQ = 12321;  //客户端请求阵营霸主争夺战参与资格排行
const int MSG_SIM_GM2C_COUNTRY_OVERLOAD_WARDATA = 12322; //客户端请求阵营霸主争夺战排行


//----------------------------------节日杀怪活动-----------------------------------
const int MSG_SIM_WS2GS_START_END_ACTIVE = 12350;				//世界服通知逻辑服活动开启结束
const int MSG_SIM_GS2C_START_END_ACTIVE = 12351;				//逻辑服通知客户端活动开启结束
const int MSG_REQ_C2GS_FESTIVAL_ACTIVE_PRIZE = 12352;	//客户端请求兑换节日活动奖励
const int MSG_REQ_GS2C_DAYUPDATE = 12353;									//通知客户端每日更新信息
const int MSG_REQ_C2GS_FESTIVAL_EXCHAGE_INFO = 12354;	//


//------------------------------------------运营活动12400~12499--------------------------------
const int MSG_REQ_WEB2MS_ACTIVITIESINFO_OP = 12400;//网页往管理服发送运营活动操作（增删改查）
const int MSG_REQ_MS2WS_ACTIVITIESINFO_OP = 12401;//管理服往世界服发送运营活动操作（增删改查）
const int MSG_REQ_C2WS_GETACTIVITYINFO = 12402;//客户端获取活动概况和自己的领取情况
const int MSG_REQ_C2WS_GETACTIVITYREWARD = 12403;//客户端请求获取奖励
const int MSG_REQ_C2WS_ACTIVITYINVEST = 12404;//客户端请求投资
const int MSG_REQ_C2WS_ACTIVEACTIVITIES = 12405;//当前开启的活动ID列表
const int MSG_REQ_C2WS_ACTIVEFRANKING = 12406; //客户端请求开服冲榜活动
const int MSG_REQ_WS2GM_SYSREWARD = 12407; //同步奖励
const int MSG_SIM_GM2WS_RECHARGE = 12408;//告诉世界服充值,此条消息只为了测试用,实际充值都是从后台来冲的
const int MSG_REQ_WS2GM_ACTIVITY_INVESTCOST = 12409; //扣除投资金钱
//const int MSG_SIM_WS2GM_ISHAVECOMPENSATETOGET = 12410;//通知GM服务器有补偿活动领取
const int MSG_SIM_WS2C_SYSACTIVITYOVER = 12411; // 通知活动结束
const int MSG_SIM_WS2GM_NOTICEACTIVITYSTATE = 12412; // 通知GM活动状态

//-----------------------------------------支付接入------------------------------------
const int MSG_REQ_C2WS_ASKPAYINFO = 12500; //请求支付
const int MSG_REQ_C2MS_CALLBACKINFO = 12501; //回调参数信息
const int MSG_REQ_MS2WS_CALLBACKINFO = 12502; //回调参数信息
const int MSG_REQ_WS2GM_SYSPAYINFO = 12503; //玩家充值返回
const int MSG_SIM_C2WS_ASKAPPTASK = 12504; //请求处理任务
const int MSG_REQ_C2MS_MARKETCALLBACKINFO = 12505; //集市回调信息
const int MSG_REQ_MS2WS_MARKETCALLBACKINFO = 12506; //集市回调信息
const int MSG_SIM_WS2GM_SYSMARKETINFO = 12507; //玩家集市奖励发放

//--------------------------------------IEINFO-------------------------------------------------------------------------------
const int MSG_SIM_C2WS_ASKSAVEIEINFO = 12601;

//客户端获取系统公告
const int MSG_REQ_C2WS_SYSTEMNOTICE             = 12700;

//----------------------界面答题-------------------------
const int MSG_REQ_C2WS_GET_ALL_ANSWER_DATA	    = 12701;	//取答题数据
const int MSG_REQ_WS2C_SEND_ANSWER_STAGE_DATA	= 12702;	//世界服发送答题各阶段数据
const int MSG_REQ_C2WS_CHOICE_ANSWER_DATA	    = 12703;	//客户端发送答选择的答案
const int MSG_REQ_C2WS_SHOW_ANSWER       	    = 12704;	//客户端请求排除一个错误答客
const int MSG_REQ_C2WS_GET_ANSWER_REWARD     	= 12705;	//领取奖励
const int MSG_REQ_WS2GM_BUCKLE_ATT		     	= 12706;	//扣属性
const int MSG_REQ_WS2GM_GIVE_ANSWER_ITEM     	= 12707;	//给物品
const int MSG_SIM_WS2GM_BEGINANSWER = 12708; // 通知所有玩家开始答题
//const int MSG_SIM_WS2GM_PLAYERBEGINANSWER = 12709; // 通知单个玩家开始答题 用于进入游戏时

//---------------------取玩家信息走一次世界服-----------------
const int MSG_REQ_GM2WS_GET_WS_DATA	     		= 12708;	//逻辑服取数据走一次世界服
const int MSG_REQ_WS2GM_PLAYER_ANSWER     		= 12709;	//玩家参与一次答题

//------------------------好友祝福------------------------
const int MSG_REQ_C2GM_RECEIVE_GOURD_EXP		= 12710;
const int MSG_REQ_C2GM_GET_GOURD_EXP			= 12711;
const int MSG_REQ_C2WS_ADD_FRIEND_LIST			= 12712;
const int MSG_REQ_C2WS_RECEIVE_EXP   			= 12713;
const int MSG_REQ_C2WS_RECOMMEND_FRIEND			= 12714;
const int MSG_SIM_WS2C_PROMPT_RECEIVE_BLESS		= 12715;
const int MSG_REQ_C2WS_REPLY_ADDFRIEND = 12716;	//拒绝或同意添加好友
const int MSG_SIM_WS2C_ADDFRIEND = 12717;				//添加好友
const int MSG_SIM_WS2C_DELETEFRIEND = 12718;			//删除好友

//------------------------------斗战重楼----------------------------------------------
const int MSG_REQ_C2WS_PVP_MAP					 = 12720;//客户端请求斗战重楼活动副本id
const int MSG_REQ_C2GS_PVP_GATHER	    = 12721;//客户端请求采集火种
const int MSG_SIM_GS2C_GATHER			    = 12722;//服务器推送玩家采集成功(只通知采集的那个人)
const int MSG_REQ_C2WS_PVP_KILL_RANK  = 12723;//玩家请求查看斗战重楼击杀排行榜数据
const int MSG_SIM_WS2C_PVP_PRIZE  	   = 12724;//活动结束时服务器推送奖励信息
const int MSG_REQ_C2WS_PVP_FITCH_PRIZE = 12725;//请求领取斗战重楼奖励
const int MSG_SIM_GS2C_PVP_DATA	   	  = 12726;//玩家进入副本时，服务器推送本层数据
const int MSG_SIM_GS2C_PVP_BOSSDAMAGE_RANK  = 12727;//服务器推送boss伤害排名数据
const int MSG_SIM_GS2C_PVP_BOSS_HP  		= 12728;//服务器推送boss hp数据
const int MSG_SIM_GS2C_PVP_FIRST_WAR		= 12729;//服务器推送首战提示
const int MSG_SIM_GS2C_PVP_INTEGRAL_RANK  = 12730;//服务器推积分排名数据
const int MSG_SIM_GS2C_PVP_BOSSDDEAD_RANK  = 12731;//服务器推送boss被杀排名数据

const int MSG_SIM_WS2GS_PVP_MAPS  = 12735;//世界服通知逻辑服所有得副本信息
const int MSG_SIM_GS2WS_PVP_PLAYER_DATA  = 12736;//逻辑服通知世界服玩家战斗信息(层数、击杀总数、获得经验总数)
const int MSG_REQ_GS2WS_PVP_PLAYER_DATA  = 12737;//进入副本请求世界服玩家的击杀排行 击杀总数 总经验
const int MSG_SIM_GS2WS_PVP_BOSS_DEAD    = 12738;//boss死亡
const int MSG_REQ_WS2GS_PVP_BOSS_Damage_Rank    = 12739;//世界服向逻辑服请求Boss伤害数据
const int MSG_SIM_WS2GS_PVP_END    			 = 12740;//世界服通知逻辑服活动结束
const int MSG_SIM_WS2GS_ADD_ITEM		 = 12741;//世界服通知逻辑服加物品
const int MSG_SIM_GS2WS_LEAVE_LAYER = 12742;//逻辑服通知世界服离开或者换层
const int MSG_SIM_WS2GS_PVP_NOOPERATE = 12743;//世界服通知逻辑服禁止副本操作

const int MSG_SIM_C2WS_NOT_MARRY_REQUEST_FRIEND_INFO 	   = 12744;	//未结婚打开结婚面版
const int MSG_SIM_C2WS_HAVE_MARRY_REQUEST_FRIEND_INFO 	   = 12745;	//已结婚打开结婚面版
const int MSG_SIM_C2WS_REQUEST_MARRY					   = 12746;	//请求结婚
const int MSG_SIM_C2WS_GIVE_FLOWER						   = 12747;	//送花
const int MSG_SIM_WS2C_RECEIVE_FLOWER					   = 12748;	//收到送花
const int MSG_SIM_C2GM_RING_UP_STEP			   			   = 12749;	//婚戒升阶
const int MSG_SIM_C2WS_DIVORCE						   	   = 12750;	//离婚
const int MSG_SIM_C2WS_FORCE_DIVORCE					   = 12751;	//强制离婚
const int MSG_SIM_WS2C_FORCE_DIVORCE_NOTICE_OTHER		   = 12752;	//强制离婚通知对方
const int MSG_REQ_C2GM_REQUEST_RING_INFO				   = 12753;	//请求婚戒信息
const int MSG_SIM_C2GM_MARRY_CHANGE_MAP			   		   = 12754;	//夫婚换图
const int MSG_SIM_WS2C_UPDATE_MARRY_OTHER_INFO			   = 12755;	//更新夫妻对方信息
const int MSG_SIM_C2GM_RECEIVE_REWARD					   = 12756;	//领取奖励
const int MSG_SIM_WS2C_NOTICE_REQUEST_MARRY				   = 12757;	//通知有人向自己求婚
const int MSG_SIM_WS2C_NOTICE_BEING_DIVORCE				   = 12758;	//通知被离婚方对方请求离婚
const int MSG_SIM_C2WS_REQUEST_MARRY_RESULT				   = 12759;	//被求婚发送是否同意结婚
const int MSG_SIM_WS2C_MARRY_RESULT						   = 12760; //求婚结果
const int MSG_SIM_C2WS_DIVORCE_RESULT					   = 12761; //离婚结果
const int MSG_SIM_WS2C_MARRY_OTHER_INFO_UPDATE			   = 12762; //结婚对方信息变更
const int MSG_REQ_WS2GM_GET_MARRY_REWARD				   = 12763; //取结婚奖励
const int MSG_REQ_WS2GM_BUCKLE_ITEM						   = 12764; //世界服向逻辑服扣除物品
const int MSG_SIM_WS2GM_MARRY							   = 12764; //通知客户端结婚
const int MSG_REQ_WS2GM_FORCE_DIVORCE_BUCKLE_GLODEN		   = 12765; //强制离婚扣钱
const int MSG_SIM_WS2GM_FORCE_DIVORCE_GIVE_GLODEN		   = 12766; //强制离婚给钱
const int MSG_SIM_WS2GM_DIVORCE_SUCCESS					   = 12767; //成功离婚，清除逻辑服数据
const int MSG_REQ_GM2WS_MARRY_CHANGE_MAP				   = 12768; //不在一条线上请求世界服换图
const int MSG_REQ_WS_GM_MARRY_GET_OTHER_MAP_INFO		   = 12769; //不在一条线上请求结婚对方地图信息
const int MSG_WSC_DIVORCE_TRANSMIT_RESULT			   	   = 12770; //转发离婚结果

const int MSG_SIM_C2WS_VOCATIVE_PARTNER = 12771; //呼唤伴侣
const int MSG_SIM_WS2C_ANOTHER_VOCATIVE_PARTNER = 12772; //通知呼唤伴侣
const int MSG_REQ_C2WS_READY_CHALLENGE = 12773; //准备挑战
const int MSG_SIM_WS2C_ANOTHER_READY_CHALLENGE = 12774; //通知对方准备挑战
const int MSG_REQ_C2WS_STOP_CHALLENGE = 12775; //停止挑战
const int MSG_SIM_WS2C_ANOTHER_STOP_CHALLENGE = 12776; //通知对方停止挑战
const int MSG_SIM_WS2C_MARRY_CARBON_INFO = 12777; //	通知副本信息
const int MSG_REQ_WS2GM_MARRY_CARBON_LIMIT = 12778; //结婚副本限制
const int MSG_SIM_WS2C_START_CHALLENGE = 12779; //开始挑战
const int MSG_REQ_C2GM_JUMP_POINT = 12780; //请求跳点
const int MSG_SIM_GM2C_JUMP_MAP = 12781; //跳转地图
const int MSG_REQ_C2GM_DOWN_POINT = 12782; //请求下落
const int MSG_SIM_GM2C_DOWN_MAP = 12783; //下落地图
const int MSG_REQ_C2GM_SUPPORT_ANOTHER = 12784; //支援对方
const int MSG_SIM_GM2WS_DEL_MARRY_CARBON	= 12785;		//销毁结婚副本

//------------------铜钱副本------------------------
const int MSG_REQ_C2GS_MONEYINSTANCE_BUY_ITEM	   = 12800;	//请求购买物品使用
const int MSG_REQ_C2GS_MONEYINSTANCE_USESKILL  		= 12801;  //请求使用技能
const int MSG_REQ_C2GS_MONEYINSTANCE_BUY_ATTR  		= 12802;  //使用元宝购买属性
const int MSG_REQ_C2GS_MONEYINSTANCE_FITCHPRIZE		= 12803;  //请求领取铜钱奖励
const int MSG_REQ_C2GS_MONEYINSTANCE_RANK			  		= 12804;	//请求铜钱副本排行榜

const int MSG_REQ_GS2C_MONEYINSTANCE_NOTICE	  		= 12805;	//通知公告
const int MSG_REQ_GS2C_MONEYINSTANCE_FLUSH		  		= 12806;	//通知怪物波次刷新(包含怪物等级及基础战斗属性)
const int MSG_REQ_GS2C_MONEYINSTANCE_TOWER_ATTR		= 12807;	//通知修罗塔属性改变
const int MSG_REQ_GS2C_MONEYINSTANCE_ATTR			  		= 12808;	//通知玩家铜钱、杀怪数、Boss数
const int MSG_REQ_C2GS_MONEYINSTANCE_EXIT			  		= 12809;	//请求退出铜钱副本
const int MSG_REQ_GS2C_MONEYINSTANCE_PRIZE			  	= 12810;	//铜钱副本结算
const int MSG_REQ_C2GS_MONEYINSTANCE_BUY_COUNT	  	= 12811;	//铜钱副本购买次数
const int MSG_SIM_GS2WS_MONEYINSTANCE_RESULT      = 12812; //通知世界服副本结束时的战绩排行
const int MSG_REQ_GS2WS_MONEYINSTANCE_RANK        = 12813; //请求世界服副本战绩排行

//世界BOSS副本
const int MSG_REQ_C2WS_WORLDBOSS_INFOS 		= 12815; 					//请求查询所有世界BOSS信息
const int MSG_REQ_C2WS_WORLDBOSS_DUP_ID 	= 12816;						//请求副本的id
const int MSG_REQ_C2GS_WORLDBOSS_FITCH  	= 12817;						//请求领取奖励
const int MSG_SIM_GS2C_WORLDBOSS_DAMAGE 	= 12818;						//服务器推送boss伤害排名
const int MSG_SIM_GS2C_WORLDBOSS_RESULT 	= 12819;						//服务器推送结算结果
const int MSG_SIM_GS2C_WORLDBOSS_TIME		 	= 12820;						//服务器推送剩余时间
const int MSG_SIM_GS2WS_WORLDBOSS_JIONORLEAVE	= 12821;			//逻辑服通知世界服玩家进入或离开副本
const int MSG_SIM_GS2WS_WORLDBOSS_BOSSDEAD	= 12822;					//逻辑服通知世界服BOSS死亡
const int MSG_SIM_WS2GM_WORLDBOSS_UIHLSTATE = 12823; // 世界服通知逻辑服UI高亮状态
const int MSG_SIM_WS2GM_NOTICEBOSS_OPENCNT = 12824; // 世界服通知逻辑服 世界Boss开启数量
const int MSG_REQ_C2GM_WORLDBOSS_DAMAGE_LIST	= 12825; //客户端请求boss伤害排名

//玫瑰沙滩
const int MSG_REQ_C2WS_SEABEACH_INFO	= 12830;								//请求沙滩副本信息
const int MSG_REQ_C2GS_SEABEACH_JUMP	= 12831;								//请求跳跃
const int MSG_REQ_C2GS_SEABEACH_GATHER	= 12832;							//请求采集
const int MSG_GS2C_SEABEACH_GATHER_RESULT	= 12833;					//服务器通知玩家采集成功或者失败
const int MSG_REQ_SEABEACH_LEAVE_OBJECT			= 12834;					//玩家请求离开物件对象
const int MSG_GS2C_SEABEACH_INCOME_CHANGE	= 12835;					//服务器推送经验真气(进去时为当前总的，后面为每次更新)
const int MSG_GS2C_SEABEACH_LEAVE_GATHER_OBJECT	= 12836;		//服务器广播玩家离开采集对象状态还原
const int MSG_GS2C_SEABEACH_LEAVE_GATHER_FAIL		= 12837;		//服务器通知自己采集失败
const int MSG_GS2C_SEABEACH_JUMP_INFOL		= 12838;						//服务器广播玩家跳水信息

//-------------------------------------------------
//----------------------天降宝箱-----------------------------------
const int MSG_REQ_C2GM_ASKBOX = 12901; //请求采集宝箱
const int MSG_SIM_GM2C_BOXTIME = 12902;

const int MSG_REQ_C2WS_SEE_CLAN_MENBER = 12903;	//查看帮派玩家列表
const int MSG_SIM_GM2C_CD_TIME_CHANGE  = 12904;	//CD改变
const int MSG_REQ_FINISH_ONE_QUEST	   = 12905; //立刻完成某任备

// --------------------- 竞技场 ------------------------------
const int MSG_REQ_C2GM_REFRESHARENA = 12951; // 刷新竞技场排行
const int MSG_REQ_GM2WS_ARENAOTHERPLAYERDATA = 12952; // 请求获取竞技场其他玩家数据
const int MSG_REQ_GM2WS_ARENAINFO = 12953; //
const int MSG_REQ_GM2WS_ARENAROBOTINFO = 12954; // 请求竞技场机器人信息
const int MSG_REQ_GM2WS_ARENAROBOTDATA = 12955; // 请求竞技场机器人信息 与前者一样的功能,但是发出意义不同
const int MSG_REQ_GM2WS_GETARENANEWRANK = 12956; // 请求竞技场最新排名 用于在玩家达到等级
const int MSG_REQ_GM2WS_REFRESHARENA = 12957; //

//-----------------------58play充值-------------------------------

const int MSG_REQ_C2MS_58PLAYASKPAY = 13001; //请求支付信息
const int MSG_REQ_MS2WS_58PLAYASKPAY = 13002; //请求支付信息
const int MSG_REQ_WS2GM_SYS58PLAYPAYINFO = 13003; //同步58Play支付信息
const int MSG_REQ_C2WS_58PLAY_INVITEFRIEND = 13004; //邀请好友
const int MSG_REQ_C2MS_58PLAY_FBLIKE = 13005; //点赞
const int MSG_REQ_C2WS_58PLAY_FBINFO = 13006;  //请求点赞面板信息
const int MSG_SIM_WS2C_58PLAY_FBINFO = 13007; //同步FB信息
const int MSG_REQ_C2WS_58PLAY_GETREWARD = 13008; //领取奖励
const int MSG_REQ_WS2GM_58PLAY_SYSREWARD = 13009; //同步奖励信息
const int MSG_REQ_C2WS_58PLAY_FBLIKE = 13010; //请求支付信息
const int MSG_REQ_MS2WS_58PLAY_FBLIKE = 13011; //点赞
const int MSG_REQ_WS2DB_SYS58PLAYPAYINFO = 13012; //同步58Play支付信息

const int MSG_REQ_C2MS_CONNECTINGASKPAY = 13013; //请求联运支付信息
const int MSG_REQ_MS2WS_CONNECTINGASKPAY = 13014; //请求联运支付信息
const int MSG_REQ_WS2GM_SYSCONNECTINGINFO = 13015; //同步联运支付信息

//------------------------圣诞节活动------------------
const int MSG_REQ_C2G_GET_BREAK_EGG_INFO		 = 13052;	//获取砸蛋信息
const int MSG_REQ_C2G_GET_CHRISTMAS_LOGIN_INFO   = 13053;   //获取登录信息
const int MSG_REQ_C2G_GET_LIMIT_BUY_INFO		 = 13054;   //获取限购信息
const int MSG_REQ_C2G_EGG_RESLUSH				 = 13055;	//砸蛋刷新
const int MSG_REQ_C2G_BREAK_EGG					 = 13056;	//砸蛋
const int MSG_REQ_C2G_RECEIVE_REWARD			 = 13057;	//登录奖励领取
const int MSG_REQ_C2G_LIMIT_BUY_REFLUSH			 = 13058;	//限购刷新
const int MSG_REQ_C2G_LIMIT_BUY_BUY				 = 13059;   //购买限购物品
const int MSG_REQ_C2G_CHRISTMAS_EXCHANGE		 = 13060;	//兑换物品

//-----------------------------360充值-----------------------------
const int MSG_REQ_C2MS_360PLAYASKPAY = 13101; //请求支付信息
const int MSG_REQ_MS2WS_360PLAYASKPAY = 13102; //请求支付信息
const int MSG_REQ_WS2GM_SYS360PLAYPAYINFO = 13103; //同步360Play支付信息
const int MSG_REQ_WS2DB_SYS360PLAYPAYINFO = 13104; //同步360Play支付信息

//-------------点击头像请求数据-------------------
const int MSG_REQ_C2GM_CLICK_PLAYER_HEAD		= 13150;
const int MSG_REQ_GM2C_RESPONSES_INVITE_GROUP	= 13151;

//---------------春节活动---------------------
const int MSG_REQ_C2W_GET_NEW_YEAR_DATA	   = 13200;		//获取春节限购数据
const int MSG_REQ_C2W_BUY_NEW_YEAR_ITEM    = 13201;		//购买春节物品
const int MSG_REQ_C2GM_NEW_YEAR_LOGIN_DATA = 13202;		//获取春登录数据	
const int MSG_REQ_C2GM_RECEIVE_NEW_YEAR	   = 13203;		//领取春节登录奖励
const int MSG_REQ_C2W_BUY_HORSE_SKIN	   = 13204;		//购买坐骑皮肤
const int MSG_REQ_W2GM_CHECK_PACKAGE	   = 13205;		//检测包裹
const int MSG_W2GM_GET_NEW_YEAR_DATA	   = 13206;		//世界服获取春节数据

//-----------------准点在线活动------------------------------
const int MSG_REQ_C2GM_GET_ON_LINE_AT_TIME = 13300;//获取准点在线活动

//-----------------周任务--------------------
const int MSG_SIM_GM2C_UPDATE_WEEK_TASK  = 13310;		//周任务
const int MSG_REQ_C2GM_REFLUSH_WEEK_TASK = 13311;		//更新周任务

//----------------远程玩家装备信息-------------
const int MSG_SIM_GM2C_REMOTE_PLAYER_EQUIP = 13312;		//远程玩家换装备

//-----------------帮派神树---------------------
const int MSG_REQ_C2W_GET_GODTREE_DATA = 13315;		//获取神树数据
const int MSG_SIM_W2C_GODTREE_UPGRADE  = 13316;		//神树升级
const int MSG_REQ_C2W_SUBMIT_ITEM	   = 13317;		//提交物品
const int MSG_SIM_MAKE_GODTREE_ITEM		   = 13319; //生成掉落物
const int MSG_REQ_W2GM_BUCKLE_ITEM		   = 13320; //扣除物品
const int MSG_SIM_GM2C_FALL_EFFECT		   = 13321; //洒落特效


//-----------------世界Boss更新通知客户端-----------------
const int MSG_SIM_GM2C_WORLDBOSS_UPDATE = 13350; //世界BOSS刷新通知
const int MSG_SIM_W2GM_WORLDBOSS_OVER   = 13351; //通知世界BOSS结束

const int MSG_SIM_C2GM_CHANGE_TASK_MAP  = 13360;
#endif /* SERVERMSGDEFINE_H_ */








