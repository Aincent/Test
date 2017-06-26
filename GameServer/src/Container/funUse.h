/*
 * GoodsManager.h
 *
 *  Created on: 2014年4月25日
 *      Author: root
 */

#ifndef FUNUSE_H_
#define FUNUSE_H_

using namespace std;

class Player;

struct BaseArgs
{
	int GoodsID;
	int Num;
	int Pos;
	BaseArgs():GoodsID(-1),Num(0),Pos(-1)
	{}
};

//检查物品使用
#define CHECK_USE_GOODS(type, Player, CheckArgs) global_##type(Player, CheckArgs);

//使用物品
#define USE_GOODS(type, Player, CheckArgs) global_##type(Player, CheckArgs);


int global_check_glod(Player *player, BaseArgs *useArgs);
int global_check_double_exp(Player *player, BaseArgs *useArgs);
int global_check_double_mry(Player *player, BaseArgs *useArgs);
int global_check_cottage_prop(Player *player, BaseArgs *useArgs);
int global_check_send_prop(Player *player, BaseArgs *useArgs);
int global_check_megaphone_prop(Player *player, BaseArgs *useArgs);
int global_check_strengthen_prop(Player *player, BaseArgs *useArgs);
int global_check_succinct_prop(Player *player, BaseArgs *useArgs);
int global_check_protect_prop(Player *player, BaseArgs *useArgs);
int global_check_prestige_prop(Player *player, BaseArgs *useArgs);
int global_check_buff_prop(Player *player, BaseArgs *useArgs);
int global_check_horse_prop(Player *player, BaseArgs *useArgs);
int global_check_gift_prop(Player *player, BaseArgs *useArgs);
int global_check_task_prop(Player *player, BaseArgs *useArgs);
int global_check_girl_prop(Player *player, BaseArgs *useArgs);
int global_check_skin_prop(Player *player, BaseArgs *useArgs);
int global_check_gifts_prop(Player *player, BaseArgs *useArgs);
//int global_check_fashions_prop(Player *player, BaseArgs *useArgs);
//int global_check_quantity_prop(Player *player, BaseArgs *useArgs);
int global_check_sin_prop(Player *player, BaseArgs *useArgs);
int global_check_vip_card_prop(Player *player, BaseArgs *useArgs);
int global_check_box_prop(Player *player, BaseArgs *useArgs);
int global_check_countryBossBox_prop(Player *player, BaseArgs *useArgs);
int global_check_title_prop(Player *player, BaseArgs *useArgs);

int global_use_glod(Player *player, BaseArgs *useArgs);
int global_use_double_exp(Player *player, BaseArgs *useArgs);
int global_use_double_mry(Player *player, BaseArgs *useArgs);
int global_use_cottage_prop(Player *player, BaseArgs *useArgs);
int global_use_send_prop(Player *player, BaseArgs *useArgs);
int global_use_megaphone_prop(Player *player, BaseArgs *useArgs);
int global_use_strengthen_prop(Player *player, BaseArgs *useArgs);
int global_use_succinct_prop(Player *player, BaseArgs *useArgs);
int global_use_protect_prop(Player *player, BaseArgs *useArgs);
int global_use_prestige_prop(Player *player, BaseArgs *useArgs);
int global_use_buff_prop(Player *player, BaseArgs *useArgs);
int global_use_horse_prop(Player *player, BaseArgs *useArgs);
int global_use_gift_prop(Player *player, BaseArgs *useArgs);
int global_use_task_prop(Player *player, BaseArgs *useArgs);
int global_use_decrease_sin_prop(Player *player, BaseArgs *useArgs);
int global_use_girl_prop(Player *player, BaseArgs *useArgs);
int global_use_skin_prop(Player *player, BaseArgs *useArgs);
int global_use_gifts_prop(Player *player, BaseArgs *useArgs);
//int global_use_fashions_prop(Player *player, BaseArgs *useArgs);
//int global_use_quantity_prop(Player *player, BaseArgs *useArgs);
int global_use_exp_prop(Player *player, BaseArgs *useArgs);
int global_use_force_prop(Player *player, BaseArgs *useArgs);
int global_use_golden_prop(Player *player, BaseArgs *useArgs);
int global_use_vip_card_prop(Player *player, BaseArgs *useArgs);
int global_use_box_prop(Player *player, BaseArgs *useArgs);
int global_use_countryBossBox_prop(Player *player, BaseArgs *useArgs);
int global_use_title_prop(Player *player, BaseArgs *useArgs);

#endif /* FUNUSE_H_ */
