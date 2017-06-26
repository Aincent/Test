//-----------------------------------------------------------------------------
/*				:   msg_exchange.cpp
 *
 *  Created on: 2014.5.8
 *      Author: cz
 */  
//				:	玩家间交易消息
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum
{
	E_Exchange_Accept				= 0,	// 对方同意交易
	E_Exchange_Refuse				= 1,	// 对方拒绝		
	E_Exchange_NotEnough_BagSpace	= 25,	// 背包空间不足
	E_Exchange_RoleBusy				= 53,	// 被申请方忙
	E_Exchange_WithMore				= 54,	// 不能和多人同时交易
	E_Exchange_OutOfRange			= 55,	// 交易距离范围之外
	E_Exchange_NotInSame_Map		= 56,	// 不在同一区域
	E_Exchange_ItemCanNot_Exchange	= 43,	// 物品不能交易
	E_Exchange_ItemCanNot_Find		= 8,	// 没有找到指定数量的物品
	E_Exchange_NotEnough_Money		= 11,	// 交易的金钱不足
	E_Exchange_ApplicantTgt_Change	= 58,	// 交易申请的交易目标发生变化
	E_Exchange_NotAnswer			= 59,	// 对方没有响应确认
	E_Exchange_SelfBusy				= 60,	// 玩家自己忙
	E_Exchange_PlayerBusy			= 61,
	E_Exchange_PlayeDead				= -17,
	E_Exchange_PlayeLogOut			= 51,

};


//----------------------------------------------------------------------------
#pragma pack(pop)
