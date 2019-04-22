// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WarframeCharacter.h"
#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"
#include "Character/WarframeCharacterAIController.h"
#include "Gameplay/PickableObject/PickableObject.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Gameplay/WarframeGameMode.h"
#include "UI/CharacterWidget.h"
#include "UI/CharacterWidgetComponent.h"
#include "Weapon/RoundBase.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponFactory.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


FName AWarframeCharacter::StateMachineName("StateMachine");

FObjectPool<FStatusEffectData> AWarframeCharacter::StatusEffectDataPool;

// Sets default values
AWarframeCharacter::AWarframeCharacter(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UWarframeCharacterStateMachineComponent>(AWarframeCharacter::StateMachineName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// this->AIControllerClass = FWarframeConfigSingleton::Instance().FindResourceClass("BP_WarframeCharacterAIController");

	this->StateMachine = ObjectInitializer.CreateDefaultSubobject<UStateMachineComponent>(this, AWarframeCharacter::StateMachineName);
	{
		FStateMachineLayerInitializer LayerInitializer;
		this->StateMachine->Init(this, LayerInitializer);
	}

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** Character movement component settings. */
	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	CharacterMovement->JumpZVelocity = 600.0f;
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	CharacterMovement->bOrientRotationToMovement = true;

	/** Character widget component creation. */
	CharacterWidgetComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterWidgetComponent>(this, FName("CharacterWidget"));
	CharacterWidgetComponent->SetupAttachment(this->RootComponent);

	this->SetGenericTeamId(FGenericTeamId::NoTeam);
}

// Called when the game starts or when spawned
void AWarframeCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->StateMachine->ReInit();

	CharacterWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	
	LatestStatusEffectData.SetNum(CastToUnderlyingType(EDamageType::End) - CastToUnderlyingType(EDamageType::Begin) + 1);
}

void AWarframeCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	this->StateMachine->DisableAll();

	if (PrimaryWeapon != nullptr)
	{
		PrimaryWeapon->Destroy();
	}
	if (SecondaryWeapon != nullptr)
	{
		SecondaryWeapon->Destroy();
	}
	if (MeleeWeapon != nullptr)
	{
		MeleeWeapon->Destroy();
	}

	FStatusEffectData* Data;

	while (StatusEffectQueue.IsEmpty() == false)
	{
		StatusEffectQueue.Dequeue(Data);
		StatusEffectDataPool.Put(Data);
	}
	for (auto Iter = StatusEffectSet.CreateIterator(); Iter; ++Iter)
	{
		Data = *Iter;
		StatusEffectDataPool.Put(Data);
	}
	StatusEffectSet.Empty();
}

void AWarframeCharacter::Destroyed()
{
	Super::Destroyed();
}

// Called every frame
void AWarframeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	using StatusTickFuncType = void(AWarframeCharacter::*)(const FStatusEffectData*);
	static StatusTickFuncType StatusTickFunctions[] = {
		&AWarframeCharacter::NoneStatusTick,
		&AWarframeCharacter::SlashStatusTick,
		&AWarframeCharacter::ImpactStatusTick,
		&AWarframeCharacter::PunctureStatusTick,
		&AWarframeCharacter::HeatStatusTick,
		&AWarframeCharacter::ColdStatusTick,
		&AWarframeCharacter::ElectricityStatusTick,
		&AWarframeCharacter::ToxinStatusTick,
		&AWarframeCharacter::BlastStatusTick,
		&AWarframeCharacter::RadiationStatusTick,
		&AWarframeCharacter::NoneStatusTick,
		&AWarframeCharacter::MagneticStatusTick,
		&AWarframeCharacter::ViralStatusTick,
		&AWarframeCharacter::CorrosiveStatusTick,
		&AWarframeCharacter::NoneStatusTick,
		&AWarframeCharacter::VoidStatusTick,
		&AWarframeCharacter::NoneStatusTick
	};

	// Shield recharging.
	if (ShieldRechargeTimer > 3.0f * ShieldRechargeDelayMultiplier)
	{
		if (CurrentShield != MaxShield)
		{
			CurrentShield = FMath::Min(CurrentShield + (15.0f + 0.05f * MaxShield) * ShieldRechargeSpeedMultiplier * DeltaTime, MaxShield);
		}
	}
	else
	{
		ShieldRechargeTimer += DeltaTime;
	}

	InternalTime += DeltaTime;

	while (StatusEffectQueue.IsEmpty() == false)
	{
		FStatusEffectData *Data;
		StatusEffectQueue.Peek(Data);

		if (InternalTime > Data->TickTime)
		{
			StatusEffectQueue.Pop();

			// Determine how many times the status effect should tick.
			uint32 TickCount = FMath::Clamp<uint32>(FMath::TruncToInt(InternalTime - Data->TickTime)/* / 1.0f */, 0, Data->TickCount) + 1;

			for (uint32 TickIndex = 0; TickIndex < TickCount; ++TickIndex)
			{
				// Do Tick() here.
				(this->*StatusTickFunctions[CastToUnderlyingType(Data->Type)])(Data);
			}

			Data->TickCount -= TickCount;
			if (Data->TickCount != 0)
			{
				Data->TickTime += TickCount/* * 1.0f */;
				StatusEffectQueue.Enqueue(Data);
			}
			else
			{
				FStatusEffectData*& LastestData = LatestStatusEffectData[CastToUnderlyingType(Data->Type)];
				if (LastestData == Data)
				{
					LastestData = nullptr;
				}
				StatusEffectDataPool.Put(Data);
			}
		}
		else
		{
			break;
		}
	}

	for (auto Iter = StatusEffectSet.CreateIterator(); Iter; ++Iter)
	{
		if (InternalTime > (*Iter)->TickTime)
		{
			FStatusEffectData *Data = *Iter;

			// Do Tick() here.
			(this->*StatusTickFunctions[CastToUnderlyingType(Data->Type)])(Data);

			LatestStatusEffectData[CastToUnderlyingType(Data->Type)] = nullptr;
			StatusEffectDataPool.Put(Data);
			Iter.RemoveCurrent();
		}
	}
}

// Called to bind functionality to input
void AWarframeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarframeCharacter::InitPropertiesBP(int32 InCharacterID)
{
	this->Init(static_cast<ECharacterID>(InCharacterID));
}

void AWarframeCharacter::SetLevel(uint32 InLevel)
{
	this->Level = InLevel;

	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FCharacterInfo *CharacterInfo = GameInstance->GetCharacterInfo(CharacterID);

	if (CharacterInfo != nullptr)
	{
		this->MaxHealth = this->CurrentHealth = this->PropertyLevelScaling(CharacterInfo->Health, CharacterInfo->BaseLevel, 2.0f, 0.015f, Level);
		this->MaxShield = this->CurrentShield = this->PropertyLevelScaling(CharacterInfo->Shield, CharacterInfo->BaseLevel, 2.0f, 0.0075f, Level);
		this->Armor = this->PropertyLevelScaling(CharacterInfo->Armor, CharacterInfo->BaseLevel, 1.75f, 0.005f, Level);

		this->Affinity = this->PropertyLevelScaling(CharacterInfo->Affinity, CharacterInfo->BaseLevel, 0.5f, 0.1425f, Level);
	}
}

void AWarframeCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;

	AWarframeCharacterAIController* AIController = Cast<AWarframeCharacterAIController>(GetController());
	if (AIController != nullptr)
	{
		AIController->SetGenericTeamId(NewTeamID);
	}

	/** Registers owning actor as source of stimuli for senses */
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(this->GetWorld());
	if (PerceptionSystem)
	{
		PerceptionSystem->RegisterSourceForSenseClass(UAISense_Hearing::StaticClass(), *this);
		PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
	}
}

FGenericTeamId AWarframeCharacter::GetGenericTeamId()const
{
	return TeamID;
}

void AWarframeCharacter::OnSelected()
{
	Cast<UCharacterWidget>(this->CharacterWidgetComponent->GetUserWidgetObject())->OnSelected();
}

// Unselected by player.
void AWarframeCharacter::OnUnselected()
{
	Cast<UCharacterWidget>(this->CharacterWidgetComponent->GetUserWidgetObject())->OnUnselected();
}

const FHitResult &AWarframeCharacter::GetSelectedTarget()const
{
	return SelectedTarget;
}

void AWarframeCharacter::SetOrbDropChances(float HealthOrbChance, float EnergyOrbChance)
{
	this->OrbDropList.Add({ EPickableObjectID::HealthOrb, HealthOrbChance });
	this->OrbDropList.Add({ EPickableObjectID::EnergyOrb, HealthOrbChance + EnergyOrbChance });
}

void AWarframeCharacter::SetCommonDropItems(const TArray<EPickableObjectID>& IDs, const TArray<float>& Chances)
{
	float Base = 0.0f;

	for (int32 i = 0; i < IDs.Num(); ++i)
	{
		Base += Chances[i];

		this->CommonItemDropList.Add({ IDs[i], Base });
	}
}

void AWarframeCharacter::DropItem()
{
	{
		float Chance = FMath::FRandRange(0.0f, 1.0f);

		for (auto& Item : OrbDropList)
		{
			if (Chance < Item.Chance)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this->GetWorld()->GetAuthGameMode();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				APickableObject* PickableObjcet = this->GetWorld()->SpawnActor<APickableObject>(this->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				PickableObjcet->Init(Item.ID);
				break;
			}
		}
	}

	{
		float Chance = FMath::FRandRange(0.0f, 1.0f);

		for (auto& Item : CommonItemDropList)
		{
			if (Chance < Item.Chance)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this->GetWorld()->GetAuthGameMode();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				APickableObject* PickableObjcet = this->GetWorld()->SpawnActor<APickableObject>(this->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				PickableObjcet->Init(Item.ID);
				break;
			}
		}
	}
}

void AWarframeCharacter::SetWeapon(EWeaponSlotType WeaponSlotType, AWeaponBase* Weapon)
{
	switch (WeaponSlotType)
	{
	case EWeaponSlotType::Primary:
		PrimaryWeapon = Weapon;
		break;
	case EWeaponSlotType::Secondary:
		SecondaryWeapon = Weapon;
		break;
	case EWeaponSlotType::Melee:
		MeleeWeapon = Weapon;
		break;
	default:
		break;
	}
}

AWeaponBase *AWarframeCharacter::GetWeapon(EWeaponSlotType WeaponSlotType)
{
	switch (WeaponSlotType)
	{
	case EWeaponSlotType::Primary:
		return PrimaryWeapon;
	case EWeaponSlotType::Secondary:
		return SecondaryWeapon;
	case EWeaponSlotType::Melee:
		return MeleeWeapon;
	default:
		return nullptr;
	}
}

AWeaponBase *AWarframeCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AWarframeCharacter::SwitchRangedWeapon()
{
	if (CurrentRangedWeapon == PrimaryWeapon)
	{
		if (SecondaryWeapon != nullptr)
		{
			CurrentRangedWeapon = SecondaryWeapon;
		}
	}
	else // if (CurrentRangedWeapon == SecondaryWeapon)
	{
		if (PrimaryWeapon != nullptr)
		{
			CurrentRangedWeapon = PrimaryWeapon;
		}
	}
	CurrentWeapon = CurrentRangedWeapon;
}

void AWarframeCharacter::SwitchToRangedWeapon()
{
	if (CurrentRangedWeapon != nullptr)
	{
		CurrentWeapon = CurrentRangedWeapon;
	}
	else
	{
		if (PrimaryWeapon != nullptr)
		{
			CurrentWeapon = CurrentRangedWeapon = PrimaryWeapon;
		}
		else if (SecondaryWeapon != nullptr)
		{
			CurrentWeapon = CurrentRangedWeapon = SecondaryWeapon;
		}
	}
}

void AWarframeCharacter::SwitchToMeleeWeapon()
{
	if (MeleeWeapon != nullptr)
	{
		CurrentWeapon = MeleeWeapon;
	}
}

void AWarframeCharacter::Kill(AActor* Causer)
{
	this->DropItem();

	OnDied.Broadcast(Causer, this);

	this->Destroy();
}

void AWarframeCharacter::GainHealth(float Value)
{
	CurrentHealth += Value;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void AWarframeCharacter::ApplyDamageBP(AActor* DamageCauser, EDamageType Status, EDamageType DamageType, float Damage)
{
	this->ApplyStatusEffect(DamageCauser, this->GetActorLocation(), Status, Damage, 1.0f);

	this->ApplyDamage(DamageCauser, this->GetActorLocation(), DamageType, Damage);
}

void AWarframeCharacter::ApplyDamage(AActor* DamageCauser, const FVector& HitLocation, EDamageType DamageType, float Damage)
{
	TArray<FDamagePair> DamageArray;

	DamageArray.Add({ DamageType, Damage });

	this->ApplyDamage(DamageCauser, HitLocation, EDamageType::None, DamageArray, 1.0f, 0);
}

void AWarframeCharacter::ApplyDamage(AActor* DamageCauser, const FVector& HitLocation, EDamageType Status, TArray<FDamagePair> &DamageArray, float DamageScalar, uint32 CriticalTier)
{
	if (FGenericTeamId::GetAttitude(DamageCauser, this) == ETeamAttitude::Friendly)
	{
		if (Cast<AWarframeCharacter>(DamageCauser)->GetStatusTime(EDamageType::Radiation) == 0.0f)
		{
			return;
		}
	}

	AWarframeGameMode *GameMode = Cast<AWarframeGameMode>(this->GetWorld()->GetAuthGameMode());

	float Damage = 0.0f;
	float RemainedDamagePercentage = 1.0f;

	// Reset shield recharge timer to zero.
	ShieldRechargeTimer = 0.0f;

	// Body part multiplier.
	
	// Try apply damage to shield.
	bool HasRawDamage = false;

	if (!(DamageArray.Num() == 1 && DamageArray[0].Type == EDamageType::Raw) && CurrentShield > 0.0f)
	{
		for (auto &Pair : DamageArray)
		{
			if (Pair.Type == EDamageType::Raw)
			{
				HasRawDamage = true;
			}
			else
			{
				Damage += Pair.Value * this->GetShieldDamageModifier(Pair.Type);
			}
		}
		Damage *= DamageScalar;

		CurrentShield -= Damage;

		GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, Status, Damage, true, static_cast<int32>(CriticalTier));

		if (CurrentShield < 0.0f)
		{
			RemainedDamagePercentage = -CurrentShield / Damage;
			CurrentShield = 0.0f;
		}
		else if (HasRawDamage == false)
		{
			return;
		}
	}

	// Apply damage to health.
	Damage = 0.0f;

	for (auto &Pair : DamageArray)
	{
		Damage += Pair.Value * this->GetHealthDamageModifier(Pair.Type);
	}
	Damage *= DamageScalar;
	Damage *= RemainedDamagePercentage;

	CurrentHealth -= Damage;

	GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, Status, Damage, false, static_cast<int32>(CriticalTier));

	if (CurrentHealth < 0.0f)
	{
		this->Kill(DamageCauser);
	}
}

void AWarframeCharacter::ApplyStatusEffect(AActor* DamageCauser, const FVector& HitLocation, EDamageType Status, float BaseDamage, float DamageMultiplier)
{
	FStatusEffectData *Data;

	switch (Status)
	{
	case EDamageType::Slash:
		Data = StatusEffectDataPool.Get();

		Data->Type = EDamageType::Slash;
		Data->DamageCauser = DamageCauser;
		Data->HitLocationOffset = HitLocation - this->GetActorLocation();
		Data->Damage = BaseDamage * DamageMultiplier * 0.35f;
		Data->TickCount = 6;
		Data->TickTime = InternalTime + 1.0f;

		StatusEffectQueue.Enqueue(Data);
		LatestStatusEffectData[CastToUnderlyingType(EDamageType::Slash)] = Data;
		this->SlashStatusTick(Data);
		break;
	case EDamageType::Impact:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Impact)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 1.0f;
			this->ImpactStatusTick(Data);
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Impact;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 1.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Impact)] = Data;
			this->ImpactStatusTick(Data);
		}
		break;
	case EDamageType::Puncture:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Puncture)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 6.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Puncture;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 6.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Puncture)] = Data;
			this->PunctureStatusTick(Data);
		}
		break;
	case EDamageType::Heat:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Heat)];
		if (Data != nullptr)
		{
			Data->TickCount = 6;
			Data->TickTime = InternalTime + 1.0f;
			this->HeatStatusTick(Data);
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Heat;
			Data->DamageCauser = DamageCauser;
			Data->HitLocationOffset = HitLocation - this->GetActorLocation();
			Data->Damage = BaseDamage * DamageMultiplier * 0.5f;
			Data->TickCount = 6;
			Data->TickTime = InternalTime + 1.0f;

			StatusEffectQueue.Enqueue(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Heat)] = Data;
			this->HeatStatusTick(Data);
		}
		break;
	case EDamageType::Cold:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Cold)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 6.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Cold;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 1.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Cold)] = Data;
			this->ColdStatusTick(Data);
		}
		break;
	case EDamageType::Electricity:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Electricity)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 6.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Electricity;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 1.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Electricity)] = Data;
			this->ElectricityStatusTick(Data);
		}
		break;
	case EDamageType::Toxin:
		Data = StatusEffectDataPool.Get();

		Data->Type = EDamageType::Toxin;
		Data->DamageCauser = DamageCauser;
		Data->HitLocationOffset = HitLocation - this->GetActorLocation();
		Data->Damage = BaseDamage * DamageMultiplier * 0.5f;
		Data->TickCount = 8;
		Data->TickTime = InternalTime + 1.0f;

		StatusEffectQueue.Enqueue(Data);
		LatestStatusEffectData[CastToUnderlyingType(EDamageType::Toxin)] = Data;
		this->ToxinStatusTick(Data);
		break;
	case EDamageType::Blast:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Blast)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 3.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Blast;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 3.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Blast)] = Data;
			this->BlastStatusTick(Data);
		}
		break;
	case EDamageType::Radiation:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Radiation)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 12.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Radiation;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 12.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Radiation)] = Data;
			this->RadiationStatusTick(Data);
		}
		break;
	case EDamageType::Gas:
		this->ApplyDamage(DamageCauser, HitLocation, EDamageType::Toxin, BaseDamage * DamageMultiplier * 0.5);

		// Basically toxin status code.
		Data = StatusEffectDataPool.Get();

		Data->Type = EDamageType::Toxin;
		Data->DamageCauser = DamageCauser;
		Data->HitLocationOffset = HitLocation - this->GetActorLocation();
		Data->Damage = BaseDamage * DamageMultiplier * DamageMultiplier * 0.25f;
		Data->TickCount = 8;
		Data->TickTime = InternalTime + 1.0f;

		StatusEffectQueue.Enqueue(Data);
		LatestStatusEffectData[CastToUnderlyingType(EDamageType::Toxin)] = Data;
		this->ToxinStatusTick(Data);
		break;
	case EDamageType::Magnetic:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Magnetic)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 4.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Magnetic;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 4.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Magnetic)] = Data;
			this->MagneticStatusTick(Data);
		}
		break;
	case EDamageType::Viral:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Viral)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 6.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Viral;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 6.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Viral)] = Data;
			this->ViralStatusTick(Data);
		}
		break;
	case EDamageType::Corrosive:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Corrosive)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 999999.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Corrosive;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 999999.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Corrosive)] = Data;
			this->CorrosiveStatusTick(Data);
		}
		break;
	case EDamageType::Void:
		Data = LatestStatusEffectData[CastToUnderlyingType(EDamageType::Void)];
		if (Data != nullptr)
		{
			Data->TickTime = InternalTime + 3.0f;
		}
		else
		{
			Data = StatusEffectDataPool.Get();

			Data->Type = EDamageType::Void;
			Data->TickCount = 0;
			Data->TickTime = InternalTime + 3.0f;

			StatusEffectSet.Add(Data);
			LatestStatusEffectData[CastToUnderlyingType(EDamageType::Void)] = Data;
			this->VoidStatusTick(Data);
		}
		break;
	case EDamageType::Raw:
	case EDamageType::Tau:
	case EDamageType::None:
	default:
		break;
	}
}

float AWarframeCharacter::GetStatusTime(EDamageType Type)const
{
	FStatusEffectData* Data = this->LatestStatusEffectData[CastToUnderlyingType(Type)];
	if (Data != nullptr)
	{
		// Statuses with (TickCount > 0) have the same tick interval: 1.0s.
		// So we are fine here.
		return Data->TickTime - InternalTime + Data->TickCount /* * 1.0f */;
	}
	return 0.0f;
}

void AWarframeCharacter::Init(ECharacterID InCharacterID)
{
	CharacterID = InCharacterID;

	this->SetLevel(1);

	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FCharacterInfo *CharacterInfo = GameInstance->GetCharacterInfo(CharacterID);

	if (CharacterInfo != nullptr)
	{
		this->Name = CharacterInfo->Name;

		this->HealthType = CharacterInfo->HealthType;
		this->ShieldType = CharacterInfo->ShieldType;
		this->ArmorType = CharacterInfo->ArmorType;
	}
}

AWeaponBase* AWarframeCharacter::CreateWeapon(int32 WeaponID, const FTransform& Transform)
{
	return FWeaponFactory::Instance().SpawnWeapon<AWeaponBase>(this, static_cast<EWeaponID>(WeaponID), Transform);
}

float AWarframeCharacter::PropertyLevelScaling(float BaseValue, float BaseLevel, float Exponent, float Coefficient, float CurrentLevel)
{
	return BaseValue * (1.0f + FMath::Pow(CurrentLevel - BaseLevel, Exponent) * Coefficient);
}

void AWarframeCharacter::SlashStatusTick(const FStatusEffectData* Data)
{
	this->ApplyDamage(Data->DamageCauser, this->GetActorLocation() + Data->HitLocationOffset, EDamageType::Raw, Data->Damage);
}

void AWarframeCharacter::ImpactStatusTick(const FStatusEffectData* Data)
{
	// Try Knockback if not in knockback state.
}

void AWarframeCharacter::PunctureStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::HeatStatusTick(const FStatusEffectData* Data)
{
	this->ApplyDamage(Data->DamageCauser, this->GetActorLocation() + Data->HitLocationOffset, EDamageType::Heat, Data->Damage);
}

void AWarframeCharacter::ColdStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::ElectricityStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::ToxinStatusTick(const FStatusEffectData* Data)
{
	this->ApplyDamage(Data->DamageCauser, this->GetActorLocation() + Data->HitLocationOffset, EDamageType::Raw, Data->Damage * GetHealthDamageModifier(EDamageType::Toxin));
}

void AWarframeCharacter::BlastStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::RadiationStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::MagneticStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::ViralStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::CorrosiveStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::VoidStatusTick(const FStatusEffectData* Data)
{}

void AWarframeCharacter::NoneStatusTick(const FStatusEffectData* Data)
{
	// Do nothing.
}

float AWarframeCharacter::GetHealthDamageModifier(EDamageType DamageType)const
{
	static float HealthModifierArray
		[static_cast<uint32>(EHealthType::Object) - static_cast<uint32>(EHealthType::ClonedFlesh) + 1]
		[static_cast<uint32>(EDamageType::End) - static_cast<uint32>(EDamageType::Begin) + 1] = {
	/*ClonedFlesh*/
		{ 1.0f, 1.25f, 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.75f, 1.0f, 1.0f, 0.5f, 1.0f },
	/*Machinery*/
		{ 1.0f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.5f, 0.75f, 1.75f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 1.0f, 0.5f, 1.0f },
	/*Flesh*/
		{ 1.0f, 1.25f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.0f, 1.0f, 0.75f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Robotic*/
		{ 1.0f, 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.5f, 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Infested*/
		{ 1.0f, 1.25f, 1.0f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*InfestedFlesh*/
		{ 1.0f, 1.5f, 1.0f, 1.0f, 1.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Fossilized*/
		{ 1.15f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 0.5f, 1.5f, 0.25f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f },
	/*InfestedSinew*/
		{ 1.0f, 1.0f, 1.0f, 1.25f, 1.0f, 1.25f, 1.0f, 1.0f, 0.5f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Object*/
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	return HealthModifierArray[static_cast<uint32>(this->HealthType)][static_cast<uint32>(DamageType)];
}

float AWarframeCharacter::GetShieldDamageModifier(EDamageType DamageType)const
{
	static float ShieldModifierArray
		[static_cast<uint32>(EShieldType::ProtoShield) - static_cast<uint32>(EShieldType::Shield) + 1]
		[static_cast<uint32>(EDamageType::End) - static_cast<uint32>(EDamageType::Begin) + 1] = {
	/*Shield*/
		{ 1.0f, 1.0f, 1.5f, 0.8f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*ProtoShield*/
		{ 1.0f, 1.0f, 1.15f, 0.5f, 0.5f, 1.0f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f }
	};

	return ShieldModifierArray[static_cast<uint32>(this->ShieldType)][static_cast<uint32>(DamageType)];
}

float AWarframeCharacter::GetArmorDamageModifier(EDamageType DamageType)const
{
	static float ArmorModifierArray
		[static_cast<uint32>(EArmorType::AlloyArmor) - static_cast<uint32>(EArmorType::FerriteArmor) + 1]
		[static_cast<uint32>(EDamageType::End) - static_cast<uint32>(EDamageType::Begin) + 1] = {
	/*FerriteArmor*/
		{ 1.0f, 0.85f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.25f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f },
	/*AlloyArmor*/
		{ 1.0f, 0.5f, 1.0f, 1.15f, 1.0f, 1.25f, 0.5f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	return ArmorModifierArray[static_cast<uint32>(this->ArmorType)][static_cast<uint32>(DamageType)];
}
