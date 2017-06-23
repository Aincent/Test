#ifndef _BEHAVIORHANDLE_H
#define _BEHAVIORHANDLE_H

#include "Common.h"
#include "define.h"
#include "Singleton.h"
#include "../StatistDataType.h"

using namespace CommBaseOut;
#define BEHAVIOR_DO(C_ID, H, O) BehaviorHandle::Handle(C_ID, H, O);
#define BEHAVIOR_BEGIN(C_ID, MAIN_TYPE, MINOR_TYPE, O, GOL, MON, FOR, BINDGOL)   \
		BehaviorHandle::Begin(C_ID, MAIN_TYPE, MINOR_TYPE, O, GOL, MON, FOR, BINDGOL);

#define BEHAVIOR_END(C_ID, MAIN_TYPE, MINOR_TYPE, O, GOL, MON, FOR, BINDGOL)   \
		BehaviorHandle::End(C_ID, MAIN_TYPE, MINOR_TYPE, O, GOL, MON, FOR, BINDGOL);

class StatBehavBase;

class BehaviorHandle
{
public:

	static void Begin(const int64& charID, const int& mainType, const int& minorType, StatBehavBase* base, const int& money, const int& golden, const int& force, const int& bindGolden);

	static void Handle(const int64& charID, const BehaviorHandType::HandleType& content, StatBehavBase* base);

	static void End(const int64& charID, const int& mainType, const int& minorType, StatBehavBase* base, const int& money, const int& golden, const int& force, const int& bindGolden);

private:
	BehaviorHandle() { }
	~BehaviorHandle() { }
};



#endif // _BEHAVIORHANDLE_H
