
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FCorpusUpperState_Firing : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusUpperState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusUpperState_Ironsight : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusUpperState_Reloading : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};
