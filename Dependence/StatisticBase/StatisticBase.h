#ifndef _STATISTICBASE_H
#define _STATISTICBASE_H

#include "define.h"
#include "Common.h"
#include "google/protobuf/message.h"
//#include "../StatisticDefine.h"
#include "StatisticDefine.h"
#include "StatistDataType.h"

using namespace CommBaseOut;

/*
 * 各Server的服统计类需要统计行为则继承StatBehavBase, 并且 将BehaviorMonitor作为组件插在管理服类
 * 需要统计全局则继承StatGlobalBase
 *
 *
 *
	（行为、全局）Handle类提供接口调用  Handle类静态函数调用管理服类的相关Handle接口(例如：行为操作、全局统计)
	或者 为了各自服统计类的美观及易懂性，可在各Server的利用宏替换机制，但是必须调用（行为、全局）Handle类提供的接口
	例子：如GameServer\src\StatisticMgr\StatisticHandle.h

	remark:
	行为监视：监视 玩家某个主动行为操作包括这一个行为背后产生的一些操作，为监视行为；
						例如 商城购买物品 行为，属于行为，代码中要体现出从购买物品这个消息开始时 就要开始监视，
						在消息结束之前结束监视，中间产生操作：消耗元宝/消耗铜钱/增加物品，视为一个Handle（操作）；
						如果有新增模块并且有行为操作（被动行为操作不能被监视，特殊情况则可以，但是尽量不要有这种情况），
						在StatistDefine.h 文件新增行为类型，如果需要添加Handle（操作），而这个操作的类型在StatistDataType.h 里面不存在，
						则增加操作类型；在代码中调用行为监视组件相关API即可。
	全局统计：为某个统计点进行统计，不为某个玩家 服务/统计，为全局统计；例如 统计 玩家 官印 科举花费的金钱，
						代码体现 要在官印 科举花费的金钱数值处 统计数值。如需增加统计数据类型，则在StatistDataType.h里面新增；
						在代码中调用行为统计组件相关API即可。
	不懂看例子

	如有新增模块，则模块负责人自行添加 行为监视/全局统计 新增模块。

*/

//class StatisticBase
//{
//public:
//	virtual void BehavMoniBegin(const int64& charID, const int& mainType, const int& minorType, const int& money, const int& golden, const int& force) = 0;
//	virtual void BehaviorHandle(const int64& charID, const E_Behavior_Handle& H, const ::google::protobuf::Message* data) = 0;
//	virtual void BehavMoniEnd(const int64& charID, const int& mainType, const int& minorType, const int& money, const int& golden, const int& force) = 0;
//
//	virtual void GlobalStatist(const Global::MainType::E_TYPE mainT, const Global::MinorType::E_TYPE minorT, const ::google::protobuf::Message* data) = 0;
//
//
//	virtual ~StatisticBase() { }
//
//private:
//	StatisticBase() { }
//};

class StatBehavBase
{
public:
	virtual void BehavMoniBegin(const int64& charID, const int& mainType, const int& minorType, const int64& golden,
			const int64& money, const int64& force, const int64& bindGolden) = 0;
	virtual void BehaviorHandle(const int64& charID, const BehaviorHandType::E_Type& H, const ::google::protobuf::Message* data) = 0;
	virtual void BehavMoniEnd(const int64& charID, const int& mainType, const int& minorType, const int64& golden,
			const int64& money, const int64& force, const int64& bindGolden) = 0;

	StatBehavBase() { }
	virtual ~StatBehavBase() { }

};

class StatGlobalBase
{
public:
	virtual void GlobalStatist(const GlobalType::Main::E_TYPE& mainT, const GlobalType::Minor::E_TYPE& minorT, const StatistSaveType::E_TYPE& type, const ::google::protobuf::Message* data) = 0;

	StatGlobalBase() { }
	virtual ~StatGlobalBase() { }

};



#define BEHAVMONI_BEGIN_DECLARE virtual void BehavMoniBegin   \
	(const int64&, const int&, const int&, const int64&, const int64&, const int64&, const int64&);
#define BEHAVMONI_BEGIN_DEFINE(T) void T::BehavMoniBegin  \
		(const int64& charID, const int& mainType, const int& minorType, const int64& golden, const int64& money,  \
				const int64& force, const int64& bindGolden)

#define BEHAVMONI_HANDLE_DECLARE  virtual void BehaviorHandle  \
	(const int64&, const BehaviorHandType::E_Type&, const ::google::protobuf::Message*);
#define BEHAVMONI_HANDLE_DEFINE(T) void T::BehaviorHandle  \
		(const int64& charID, const BehaviorHandType::E_Type& H, const ::google::protobuf::Message* data)

#define BEHAVMONI_END_DECLARE  virtual void BehavMoniEnd  \
	(const int64&, const int&, const int&, const int64&, const int64&, const int64&, const int64&);
#define BEHAVMONI_END_DEFINE(T) void T::BehavMoniEnd  \
		(const int64& charID, const int& mainType, const int& minorType, const int64& golden, const int64& money,  \
				const int64& force, const int64& bindGolden)


#define GLOBAL_STATIS_DECLARE  virtual void GlobalStatist  \
	(const GlobalType::Main::E_TYPE&, const GlobalType::Minor::E_TYPE&, const StatistSaveType::E_TYPE&, const ::google::protobuf::Message*);
#define GLOBAL_STATIS_DEFINE(T)  void T::GlobalStatist  \
	(const GlobalType::Main::E_TYPE& mainT, const GlobalType::Minor::E_TYPE& minorT, const StatistSaveType::E_TYPE& saveT, const ::google::protobuf::Message* data)


struct GlobalKey
{
	GlobalType::Main::E_TYPE mainT;
	GlobalType::Minor::E_TYPE minorT;
	StatistSaveType::E_TYPE saveT;

    bool operator<(const GlobalKey& __dest) const
    {
    	return mainT < __dest.mainT && minorT < __dest.minorT && saveT < __dest.saveT;
    }
};



#endif
