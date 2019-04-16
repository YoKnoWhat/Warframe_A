
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FCorpusLowerState_Crouching : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusLowerState_Falling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusLowerState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FCorpusLowerState_Jumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float Time;
};

class WARFRAME_A_API FCorpusLowerState_Sprinting : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float MaxWalkSpeedBerore;
};