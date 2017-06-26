#include "BehaviorHandle.h"
#include "../StatisticBase.h"

//void BehaviorHandle::Handle(int64 charID, E_Behavior_Handle H, BehaviorSaveType::INT64 T, int64 param)
//{
//	ServerReturn::ServerRetInt content;
//	content.set_ret(param);
//
//	BEHAVIOR_HANDLE_DO(H, T, &content, player);
//}
//
//void BehaviorHandle::Handle(int64 charID, E_Behavior_Handle H, BehaviorSaveType::TWO_DW64 T,
//		DWORD64 paramF, DWORD64 paramS)
//{
//	ServerReturn::ServerDoubleInt content;
//	content.set_retf(paramF);
//	content.set_rets(paramS);
//
//	BEHAVIOR_HANDLE_DO(H, T, &content, player);
//}

void BehaviorHandle::Begin(const int64& charID, const int& mainType, const int& minorType, StatBehavBase* base, const int& golden,
		const int& money, const int& force, const int& bindGolden)
{
	if (NULL == base)
	{
		return;
	}

	base->BehavMoniBegin(charID, mainType, minorType, golden, money , force, bindGolden);
}

void BehaviorHandle::Handle(const int64& charID, const BehaviorHandType::HandleType& content, StatBehavBase* base)
{
	if (NULL == base)
	{
		return;
	}

	base->BehaviorHandle(charID, content.GetHandleType(), content.Get());
//		StatisticMgr::GetInstance()->BehaviorHandle(charID, H, T->Get());
}

void BehaviorHandle::End(const int64& charID, const int& mainType, const int& minorType, StatBehavBase* base, const int& golden,
		const int& money, const int& force, const int& bindGolden)
{
	if (NULL == base)
	{
		return;
	}

	base->BehavMoniEnd(charID, mainType, minorType, golden, money, force, bindGolden);
}
