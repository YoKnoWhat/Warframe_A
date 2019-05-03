
#include "Character/Warframe/Excalibur/RadialJavelin.h"
#include "Character/Warframe/Excalibur/Excalibur.h"
#include "Utility/PhysicsFunction.h"

#include "Runtime/Engine/Classes/Engine/World.h"


FRadialJavelin::FRadialJavelin(AWarframe* Warframe) :
	FAbilityObject(Warframe, true, false, 1.0f, 2.5f)
{
	this->OnLevelChanged(0);
}

void FRadialJavelin::Tick(float DeltaTime)
{
	FAbilityObject::Tick(DeltaTime);

	EnergyCost = Warframe->GetAbilityEfficiency() * 75.0f;
	if (EnergyCost < Warframe->GetCurrentEnergy())
	{
		bIsCastable = true;
	}
	else
	{
		bIsCastable = false;
	}
}

void FRadialJavelin::OnLevelChanged(uint32 NewLevel)
{
	FAbilityObject::OnLevelChanged(NewLevel);

	switch (NewLevel)
	{
	case 0:
		Damage = 500.0f;
		Range = 1500;
		JavelinCount = 5;
		break;
	case 1:
		Damage = 650.0f;
		Range = 1800;
		JavelinCount = 7;
		break;
	case 2:
		Damage = 800.0f;
		Range = 2200;
		JavelinCount = 10;
		break;
	case 3:
		Damage = 1000.0f;
		Range = 2500;
		JavelinCount = 12;
		break;
	default:
		break;
	}

	Damage *= Warframe->GetAbilityStrength();
	Range = static_cast<uint32>(Warframe->GetAbilityRange() * static_cast<float>(Range));

	return;
}

void FRadialJavelin::PreCast()
{
	
}

void FRadialJavelin::DoCast()
{
	if (bIsCastable == false)
	{
		return;
	}

	Warframe->DrainEnergy(EnergyCost);

	/** Retrieve all warframe characters in specified sphere. */
	TArray<AActor*> OverlapActors;
	{
		TArray<ECollisionChannel> ObjectTypeArray = { ECollisionChannel::ECC_Pawn };
		TArray<AActor*> ActorsToIgnore = { Warframe };

		WarframeUtil::SphereOverlapActors(Warframe, Warframe->GetActorLocation(), this->Range, ObjectTypeArray, AWarframeCharacter::StaticClass(), ActorsToIgnore, OverlapActors);
	}

	/** Apply damage to characters that are not behind obstacles. */
	{
		uint32 JavelinUsed = 0;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;

		FCollisionResponseParams ReponseParams;

		for (AActor* Actor : OverlapActors)
		{
			AWarframeCharacter* Character = ::Cast<AWarframeCharacter>(Actor);

			FHitResult HitResult;
			// Warframe->GetWorld()->LineTraceSingleByChannel(
			// 	HitResult,
			// 	Warframe->GetActorLocation(),
			// 	Character->GetActorLocation(),
			// 	ECollisionChannel::ECC_WorldStatic,
			// 	QueryParams
			// );
			if (Warframe->GetWorld()->LineTraceSingleByObjectType(
				HitResult,
				Warframe->GetActorLocation(),
				Character->GetActorLocation(),
				ObjectQueryParams,
				QueryParams) == false)
			{
				TArray<FDamagePair> DamageArray = {
					{ EDamageType::Slash, this->Damage * 0.333333f },
					{ EDamageType::Impact, this->Damage * 0.333333f },
					{ EDamageType::Puncture, this->Damage * 0.333333f }
				};

				Character->ApplyDamage(Warframe, Character->GetActorLocation(), EDamageType::Impact, DamageArray, 1.0f, 0);

				++JavelinUsed;
			}

			if (JavelinUsed == JavelinCount)
			{
				break;
			}
		}
	}
}

void FRadialJavelin::PostCast()
{}
