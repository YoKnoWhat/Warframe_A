
#include "StateObject.h"


int32 UStateObject::GetID_Implementation()const
{
	return -1;
}

int32 UStateObject::OnUpdate_Implementation(float DeltaTime)
{
	return -1;
}

void UStateObject::OnEnter_Implementation(AActor *Owner, int32 StateFromID)
{}

void UStateObject::OnExit_Implementation()
{}

int32 UStateObject::OnCustomEvent_Implementation(int32 EventID)
{
	return -1;
}
