
#include "Character/Warframe/Excalibur/RadialJavelin.h"
#include "Character/Warframe/Excalibur/Excalibur.h"

#include "Runtime/Engine/Classes/Engine/World.h"


FRadialJavelin::FRadialJavelin(AWarframe* Warframe) :
	FAbilityObject(Warframe, true, false, 1.0f, 2.5f)
{
	this->OnLevelChanged(0);
}

void FRadialJavelin::Tick(float DeltaTime)
{
	FAbilityObject::Tick(DeltaTime);
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

	Damage *= Warframe->GetAbilityStrength() * 0.01f;
	Range = static_cast<uint32>(Warframe->GetAbilityRange() * 0.01f * static_cast<float>(Range));

	return;
}

void FRadialJavelin::PreCast()
{
	
}

void FRadialJavelin::DoCast()
{
	/** Retrieve all warframe characters in specified sphere. */
	TArray<AWarframeCharacter*> OverlapActors;
	{
		TArray<FOverlapResult> Overlaps;

		FCollisionObjectQueryParams ObjectParams(ECollisionChannel::ECC_Pawn);

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(Warframe);

		Warframe->GetWorld()->OverlapMultiByObjectType(
			Overlaps,
			Warframe->GetActorLocation(),
			FQuat::Identity,
			ObjectParams,
			FCollisionShape::MakeSphere(this->Range),
			TraceParams
		);

		for (int32 OverlapIdx = 0; OverlapIdx < Overlaps.Num(); ++OverlapIdx)
		{
			FOverlapResult const& O = Overlaps[OverlapIdx];
			if (O.Actor.IsValid())
			{
				AWarframeCharacter* WarframeCharacter = ::Cast<AWarframeCharacter>(O.Actor.Get());
				if (WarframeCharacter)
				{
					OverlapActors.AddUnique(WarframeCharacter);
				}
			}
		}
	}

	/** Apply damage to characters that are not behind obstacles. */
	{
		uint32 JavelinUsed = 0;

		FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldStatic);

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;

		FCollisionResponseParams ReponseParams;

		for (AWarframeCharacter* Character : OverlapActors)
		{
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
