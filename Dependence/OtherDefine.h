/*
 * OtherDefine.h
 *
 *  Created on: 2013��10��21��
 *      Author: root
 */

#ifndef OTHERDEFINE_H_
#define OTHERDEFINE_H_

#define NPC_LIMIT_COUNT 4

//ai ����ʱ��
#define AI_CYCLE_TIME 100

//���������
#define REWARD_COUNT 4

//ս����ʽ����
//#define A_PARAM 20
//#define B_PARAM 80
//#define C_PARAM 40
//#define D_PARAM 33
//#define E_PARAM 80
//#define F_PARAM 40
const int max_submit_title_len = 48;

const int max_submit_text_len = 85;

const int max_submit_qq_len = 20;

enum eBugSubmitType
{
	//BUG
	eBugSubmitType_Bug,
	//投诉
	eBugSubmitType_Complain,
	//建议
	eBugSubmitType_Advise,
	//其它
	eBugSubmitType_Other,

	eBugSubmitType_Max,
};

enum eBugSubmitOp
{
	//未读
	eBugSubmitOp_NotRead,
	//已读
	eBugSubmitOp_Read,
	//已处理
	eBugSubmitOp_Done,
	//关闭
	eBugSubmitOp_Close,
	//所有
	eBugSubmitOp_All,

	eBugSubmitOp_Max,
};

#endif /* OTHERDEFINE_H_ */
