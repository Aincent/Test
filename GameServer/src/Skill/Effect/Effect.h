/*
 * Effect.h
 *
 *  Created on: Oct 15, 2015
 *      Author: root
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include "EffectCommonData.h"
#include "define.h"
#include "../BuffBase.h"
#include "../SkillLoader/SkillLoader.h"
#include "CharDefine.h"
#include "../../Object/Creature.h"

using namespace CommBaseOut;

struct BuffInfo;
struct EffectOneParam;

struct EffectParam
{
	EffectParam() : m_Self(NULL),m_Target(NULL){}
	CreatureObj* m_Self;
	CreatureObj* m_Target;
};

class Effect
{
public:
	Effect(BuffBase& father);

	virtual ~Effect();

public:
	virtual void enter(const BuffInfo& info,const EffectCommonData& data) = 0;

	//用于清空数据,虚函数不要在析构函数中调用
	virtual void release(){};

public:
	static Effect* createEffect(const BuffInfo& info,BuffBase& buff);

	static float getEffectValue(const EffectOneParam& data,EffectParam& param);

	//客户端状态转成服务器状态
	static ECreatureState switchEnumToServer(EAttackState type);

	//服务器状态转成客户端状态
	static EAttackState switchEnumToClient(ECreatureState type);

	//根据状态获取对应增强属性
	static ECharAttrType stateToStrengAtt(EAttackState state); 

	//根据状态获取对应抵抗属性
	static ECharAttrType stateToResistAtt(EAttackState state);

	//判断该buff是否要做状态概率控制
	static EAttackState isAddStateControl(const BuffInfo& info);

protected:
	BuffBase& 		m_Father;
};

#endif /* EFFECT_H_ */
