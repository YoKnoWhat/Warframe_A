
#include "UI/Minimap.h"
#include "Character/Warframe/Warframe.h"
#include "Gameplay/PickableObject/PickableObject.h"
#include "Utility/PhysicsFunction.h"


UMinimap::UMinimap(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UMinimap::Init(const FVector& InLevelOrigin, float InUnitScalar)
{
	LevelOrigin = InLevelOrigin;
	UnitScalar = InUnitScalar;

	EnemyMarkerArray.Init(this, "T_EnemyMarker", FVector2D(16.0f, 16.0f));
	ResourceMarkerArray.Init(this, "T_ResourceMarker", FVector2D(12.0f, 12.0f));
}

void UMinimap::Update(AWarframe* Character)
{
	FVector CharacterLocation = Character->GetActorLocation();

	PlayerMarker->SetRenderAngle(Character->GetActorRotation().Yaw);

	FVector2D CharacterToLevelOriginOffsetInUISpace = FVector2D(LevelOrigin.Y - CharacterLocation.Y, CharacterLocation.X - LevelOrigin.X);
	BackgroundPanel->SetRenderTranslation(CharacterToLevelOriginOffsetInUISpace * UnitScalar);
	BackgroundPanel->SetRenderAngle(-Character->GetControlRotation().Yaw);
	BackgroundPanel->SetRenderTransformPivot(FVector2D(0.5f, 0.5f) - CharacterToLevelOriginOffsetInUISpace * UnitScalar / BackgroundImage->Brush.ImageSize);

	TArray<ECollisionChannel> ObjectTypes;
	ObjectTypes.Add(ECollisionChannel::ECC_WorldStatic);
	ObjectTypes.Add(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypes.Add(ECollisionChannel::ECC_Pawn);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	TArray<AActor*> OutActors;

	if (WarframeUtil::SphereOverlapActors(
		Character,
		Character->GetActorLocation(),
		Character->GetEnemyRadar(),
		ObjectTypes,
		AActor::StaticClass(),
		ActorsToIgnore,
		OutActors) == true)
	{
		for (AActor* Actor : OutActors)
		{
			if (Actor->GetClass()->IsChildOf<AWarframeCharacter>())
			{
				if (FGenericTeamId::GetAttitude(Character, Actor) == ETeamAttitude::Hostile)
				{
					UImage* EnemyMarker = EnemyMarkerArray.Get();

					FVector ActorLocation = Actor->GetActorLocation();
					EnemyMarker->SetRenderTranslation(FVector2D(ActorLocation.Y - LevelOrigin.Y, LevelOrigin.X - ActorLocation.X) * UnitScalar);
					EnemyMarker->SetRenderAngle(Actor->GetActorRotation().Yaw);
				}
				else
				{
				}
			}
			if (Actor->GetClass()->IsChildOf<APickableObject>())
			{
				UImage* ResourceMarker = ResourceMarkerArray.Get();

				FVector ActorLocation = Actor->GetActorLocation();
				ResourceMarker->SetRenderTranslation(FVector2D(ActorLocation.Y - LevelOrigin.Y, LevelOrigin.X - ActorLocation.X) * UnitScalar);
				ResourceMarker->SetRenderAngle(Character->GetControlRotation().Yaw);
			}
		}
	}

	EnemyMarkerArray.Update();
	ResourceMarkerArray.Update();
}

void UMinimap::ClearCache()
{
	EnemyMarkerArray.Clear();
	ResourceMarkerArray.Clear();
}
