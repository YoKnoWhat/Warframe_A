#pragma once

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "WarframeController.generated.h"


UCLASS()
class WARFRAME_A_API AWarframeController : public APlayerController
{
	GENERATED_BODY()

public:
	AWarframeController(const FObjectInitializer& ObjectInitializer);

	virtual void Possess(APawn* Pawn)override;
};