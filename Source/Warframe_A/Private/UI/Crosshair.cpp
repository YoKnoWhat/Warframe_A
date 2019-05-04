
#include "UI/Crosshair.h"
#include "Character/TargetSelectionComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/Warframe/Warframe.h"

#define LINEAR_COLOR_BLUE	FLinearColor(0.0f, 0.673f, 1.0f, 1.0f)
#define LINEAR_COLOR_WHITE	FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
#define LINEAR_COLOR_RED	FLinearColor(0.737f, 0.018f, 0.018f, 1.0f)


UCrosshair::UCrosshair(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UCrosshair::Update(AWarframe* Character)
{
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(Character,
		Cast<AWarframeCharacter>(Cast<UTargetSelectionComponent>(Character->GetController()->GetComponentByClass(UTargetSelectionComponent::StaticClass()))->GetSelectedTarget().GetActor())
	);

	switch (Attitude)
	{
	case ETeamAttitude::Friendly:
		Focus_Dot->SetColorAndOpacity(LINEAR_COLOR_BLUE);
		Focus_Normal->SetColorAndOpacity(LINEAR_COLOR_BLUE);
		Focus_Left->SetColorAndOpacity(LINEAR_COLOR_BLUE);
		Focus_Right->SetColorAndOpacity(LINEAR_COLOR_BLUE);
		break;
	case ETeamAttitude::Neutral:
		Focus_Dot->SetColorAndOpacity(LINEAR_COLOR_WHITE);
		Focus_Normal->SetColorAndOpacity(LINEAR_COLOR_WHITE);
		Focus_Left->SetColorAndOpacity(LINEAR_COLOR_WHITE);
		Focus_Right->SetColorAndOpacity(LINEAR_COLOR_WHITE);
		break;
	case ETeamAttitude::Hostile:
		Focus_Dot->SetColorAndOpacity(LINEAR_COLOR_RED);
		Focus_Normal->SetColorAndOpacity(LINEAR_COLOR_RED);
		Focus_Left->SetColorAndOpacity(LINEAR_COLOR_RED);
		Focus_Right->SetColorAndOpacity(LINEAR_COLOR_RED);
		break;
	}

	if ((Character->GetStateMachine()->GetCurrentStateID(CastToUnderlyingType(EWarframeStateLayer::Aim)) == CastToUnderlyingType(EWarframeAimState::Aiming)) != bCacheIsAiming)
	{
		bCacheIsAiming = !bCacheIsAiming;
		if (bCacheIsAiming)
		{
			this->PlayAnimation(FocusAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		}
		else
		{
			this->PlayAnimation(FocusAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
		}
	}
}

void UCrosshair::OnApplyDamageToEnemy(bool IsHeadShot)
{
	if (IsHeadShot)
	{
		this->PlayAnimation(HeadShotHitAnimation, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);
	}
	else
	{
		this->PlayAnimation(NormalHitAnimation, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f);
	}
}
