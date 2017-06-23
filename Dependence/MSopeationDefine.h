/*
 * MSopeationDefine.h
 *
 *  Created on: 2014年10月29日
 *      Author: root
 */

/*
 *
 * 此文件 为  数据管理服 操作 定义的 相关定义
 * */

const int Max_GetInfoCount_Num_Everytime = 15;   //以列表 形式 每次 获取 最大值



/*解封 帐号操作枚举*/
enum eMSLockOpenEnum
{
	e_openID_Lock = 1, //封号
	e_openID_Unlock ,  //解封帐号
	e_openID_Max
};


enum eMSTalkingEnum
{
	e_char_Shutup =1 , //禁言
	e_char_Talk, //解言
	e_char_Max
};

//以列表 形式  查看 玩家 信息
enum  eFindListPlayerEnum
{
	e_find_nearlyCreatChar = 0,  //最近 创建角色
	e_find_charByLv = 1,//按等级从高到底查找玩家
	e_find_charByName = 2,//根据玩家名模糊查询
	e_find_charByID = 3,//根据玩家ID查询
	e_find_charByOpenID = 4,//根据openid查询
	e_Max_findEnum,
};

//以列表 形式  查看 玩家 邮件
enum  eFindMailListEnum
{
	eFindMailList_All = 0,//全部
	eFindMailList_Read = 1,//已读
	eFindMailList_NotRead = 2,//未读
	eFindMailList_Deleted = 3,//已删除
	eFind_Max,
};

//未删除和已删除邮件分别放在不同的表里
enum eMailInfoFrom
{
	eMailInfoFrom_NotDelete,//未删除
	eMailInfoFrom_Deleted,//已删除
	eMailInfoFrom_Max,
};

