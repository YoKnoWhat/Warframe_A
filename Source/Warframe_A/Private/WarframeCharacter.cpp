// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeCharacter.h"
#include "CharacterWidgetComponent.h"
#include "RoundBase.h"
#include "WarframeGameInstance.h"
#include "WarframeGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
AWarframeCharacter::AWarframeCharacter(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterWidget = ObjectInitializer.CreateDefaultSubobject<UCharacterWidgetComponent>(this, FName("CharacterWidget"));
	CharacterWidget->SetupAttachment(this->RootComponent);
}

// Called when the game starts or when spawned
void AWarframeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWarframeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// using StatusTickFuncType = void(AWarframeCharacter::*)(const FStatusEffectData&);
	typedef void(AWarframeCharacter::*StatusTickFuncType)(const FStatusEffectData&);
	static StatusTickFuncType StatusTickFunctions[] = {
		&AWarframeCharacter::SlashStatusTick,
		&AWarframeCharacter::ImpactStatusTick,
		&AWarframeCharacter::PunctureStatusTick,
		&AWarframeCharacter::HeatStatusTick,
		&AWarframeCharacter::ColdStatusTick,
		&AWarframeCharacter::ElectricityStatusTick,
		&AWarframeCharacter::ToxinStatusTick,
		&AWarframeCharacter::BlastStatusTick,
		&AWarframeCharacter::RadiationStatusTick,
		&AWarframeCharacter::GasStatusTick,
		&AWarframeCharacter::MagneticStatusTick,
		&AWarframeCharacter::ViralStatusTick,
		&AWarframeCharacter::CorrosiveStatusTick,
		&AWarframeCharacter::NoneStatusTick,
		&AWarframeCharacter::VoidStatusTick,
		&AWarframeCharacter::NoneStatusTick,
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
		FStatusEffectData Data;
		StatusEffectQueue.Peek(Data);

		if (InternalTime > Data.TickTime)
		{
			StatusEffectQueue.Pop();

			// Determine how many times the status effect should tick.
			uint32 TickCount = static_cast<uint32>(FMath::Max(FMath::TruncToInt(InternalTime - Data.TickTime)/* / 1.0f */, 0) + 1);

			for (uint32 TickIndex = 0; TickIndex < TickCount; ++TickIndex)
			{
				// Do Tick() here.
				(this->*StatusTickFunctions[CastToUnderlyingType(Data.Type)])(Data);
			}

			Data.TickCount -= TickCount;
			if (Data.TickCount != 0)
			{
				Data.TickTime += TickCount/* * 1.0f */;
				StatusEffectQueue.Enqueue(std::move(Data));
			}
		}
		else
		{
			break;
		}
	}

	for (auto Iter = StatusEffectSet.CreateIterator(); Iter; ++Iter)
	{
		if (InternalTime > Iter->TickTime)
		{
			// Do Tick() here.
			(this->*StatusTickFunctions[CastToUnderlyingType(Iter->Type)])(*Iter);

			Iter.RemoveCurrent();
		}
	}
}

void AWarframeCharacter::SlashStatusTick(const FStatusEffectData &Data)
{
	this->ApplyDamage(Data.DamageCauser, Data.HitLocation, EDamageType::Raw, Data.Damage);
}

void AWarframeCharacter::ImpactStatusTick(const FStatusEffectData &Data)
{
	// Try Knockback if not in knockback state.
}

void AWarframeCharacter::PunctureStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::HeatStatusTick(const FStatusEffectData &Data)
{
	this->ApplyDamage(Data.DamageCauser, Data.HitLocation, EDamageType::Heat, Data.Damage);
}

void AWarframeCharacter::ColdStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::ElectricityStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::ToxinStatusTick(const FStatusEffectData &Data)
{
	this->ApplyDamage(Data.DamageCauser, Data.HitLocation, EDamageType::Toxin, Data.Damage);
}

void AWarframeCharacter::BlastStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::RadiationStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::GasStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::MagneticStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::ViralStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::CorrosiveStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::VoidStatusTick(const FStatusEffectData &Data)
{}

void AWarframeCharacter::NoneStatusTick(const FStatusEffectData &Data)
{
	// Do nothing.
}

// Called to bind functionality to input
void AWarframeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarframeCharacter::InitPropertiesBP(int32 CharacterID, int32 Level)
{
	this->Init(static_cast<ECharacterID>(CharacterID), static_cast<uint32>(Level));
}

void AWarframeCharacter::Init(ECharacterID CharacterID, uint32 Level)
{
	this->Level = Level;

	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FCharacterInfo *CharacterInfo = GameInstance->GetCharacterInfo(CharacterID);
	
	if (CharacterInfo != nullptr)
	{
		this->Name = CharacterInfo->Name;

		this->HealthType = CharacterInfo->HealthType;
		this->MaxHealth = this->CurrentHealth = this->PropertyLevelScaling(CharacterInfo->Health, CharacterInfo->BaseLevel, 2.0f, 0.015f, Level);

		this->ShieldType = CharacterInfo->ShieldType;
		this->MaxShield = this->CurrentShield = this->PropertyLevelScaling(CharacterInfo->Shield, CharacterInfo->BaseLevel, 2.0f, 0.0075f, Level);

		this->ArmorType = CharacterInfo->ArmorType;
		this->Armor = this->PropertyLevelScaling(CharacterInfo->Armor, CharacterInfo->BaseLevel, 1.75f, 0.005f, Level);

		this->Affinity = this->PropertyLevelScaling(CharacterInfo->Affinity, CharacterInfo->BaseLevel, 0.5f, 0.1425f, Level);
	}
}

const FHitResult &AWarframeCharacter::GetTarget()const
{
	return CurrentTarget;
}

AWeaponBase *AWarframeCharacter::GetWeapon(EWeaponType WeaponType)
{
	return EquippedWeapon;
}

AWeaponBase *AWarframeCharacter::GetCurrentWeapon()
{
	return EquippedWeapon;
}

void AWarframeCharacter::ApplyDamageBP(AActor *DamageCauser, EDamageType Status, EDamageType DamageType, float Damage)
{
	float BaseDamagePhys;
	if (DamageType == EDamageType::Slash || DamageType == EDamageType::Impact || DamageType == EDamageType::Puncture)
	{
		BaseDamagePhys = Damage;
	}
	else
	{
		BaseDamagePhys = 0.0f;
	}

	this->ApplyStatusEffect(DamageCauser, this->GetActorLocation(), Status, Damage, BaseDamagePhys);

	this->ApplyDamage(DamageCauser, this->GetActorLocation(), DamageType, Damage);
}

void AWarframeCharacter::ApplyDamage(AActor *DamageCauser, const FVector &HitLocation, EDamageType DamageType, float Damage)
{
	AWarframeGameMode *GameMode = Cast<AWarframeGameMode>(this->GetWorld()->GetAuthGameMode());

	float RemainedDamagePercentage = 1.0f;

	// Reset shield recharge timer to zero.
	ShieldRechargeTimer = 0.0f;

	// Body part multiplier.

	// Try apply damage to shield.
	if (DamageType != EDamageType::Raw && CurrentShield > 0.0f)
	{
		CurrentShield -= Damage * this->GetShieldDamageModifier(DamageType);

		GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, EDamageType::None, Damage, true, 0);

		if (CurrentShield < 0.0f)
		{
			RemainedDamagePercentage = -CurrentShield / Damage;
			CurrentShield = 0.0f;
		}
		else
		{
			return;
		}
	}

	// Apply damage to health.
	Damage *= this->GetHealthDamageModifier(DamageType);
	Damage *= RemainedDamagePercentage;

	CurrentHealth -= Damage;

	GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, EDamageType::None, Damage, false, 0);

	if (CurrentHealth < 0.0f)
	{
		OnDied.Broadcast(DamageCauser, this);
	}
}

void AWarframeCharacter::ApplyDamage(AActor *DamageCauser, const FVector &HitLocation, ARoundBase *Round, float DamageScalar)
{
	AWarframeGameMode *GameMode = Cast<AWarframeGameMode>(this->GetWorld()->GetAuthGameMode());
	const TArray<FDamagePair> &DamageArray = Round->GetDamageArray();
	EDamageType Status = Round->GetStatusEffect();

	float Damage = 0.0f;
	float RemainedDamagePercentage = 1.0f;

	// Reset shield recharge timer to zero.
	ShieldRechargeTimer = 0.0f;

	// Body part multiplier.
	
	// Apply status effect first.
	this->ApplyStatusEffect(DamageCauser, HitLocation, Status, Round->GetBaseDamage(), Round->GetBaseDamagePhys());
	
	// Try apply damage to shield.
	if (CurrentShield > 0.0f)
	{
		for (auto &Pair : DamageArray)
		{
			Damage += Pair.Value * this->GetShieldDamageModifier(Pair.Type);
		}
		Damage *= DamageScalar;

		CurrentShield -= Damage;

		GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, Status, Damage, true, Round->GetCriticalTier());

		if (CurrentShield < 0.0f)
		{
			RemainedDamagePercentage = -CurrentShield / Damage;
			CurrentShield = 0.0f;
		}
		else
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

	GameMode->OnCharacterDamaged(DamageCauser, this, HitLocation, Status, Damage, false, Round->GetCriticalTier());

	if (CurrentHealth < 0.0f)
	{
		OnDied.Broadcast(DamageCauser, this);
	}
}

float AWarframeCharacter::PropertyLevelScaling(float BaseValue, float BaseLevel, float Exponent, float Coefficient, float CurrentLevel)
{
	return BaseValue * (1.0f + FMath::Pow(CurrentLevel - BaseLevel, Exponent) * Coefficient);
}

void AWarframeCharacter::ApplyStatusEffect(AActor *DamageCauser, const FVector &HitLocation, EDamageType Status, float BaseDamage, float BaseDamagePhys)
{
	FStatusEffectData Data;

	switch (Status)
	{
	case EDamageType::Slash:
		Data.Type = EDamageType::Slash;
		Data.DamageCauser = DamageCauser;
		Data.HitLocation = HitLocation;
		Data.Damage = BaseDamagePhys * 0.35f;
		Data.TickCount = 6;
		Data.TickTime = InternalTime + 1.0f;
		StatusEffectQueue.Enqueue(Data);
		this->SlashStatusTick(Data);
		break;
	case EDamageType::Impact:
		for (auto &Data : StatusEffectSet)
		{
			if (Data.Type == EDamageType::Impact)
			{
				Data.TickTime = InternalTime + 1.0f;
				this->ImpactStatusTick(Data);
				break;
			}
		}
		Data.Type = EDamageType::Impact;
		Data.TickTime = InternalTime + 1.0f;
		StatusEffectSet.Add(Data);
		this->ImpactStatusTick(Data);
		break;
	case EDamageType::Puncture:
		for (auto &Data : StatusEffectSet)
		{
			if (Data.Type == EDamageType::Puncture)
			{
				Data.TickTime = InternalTime + 6.0f;
				break;
			}
		}
		Data.Type = EDamageType::Puncture;
		Data.TickTime = InternalTime + 6.0f;
		StatusEffectSet.Add(Data);
		this->PunctureStatusTick(Data);
		break;
	case EDamageType::Heat:
		for (auto &Data : StatusEffectSet)
		{
			if (Data.Type == EDamageType::Heat)
			{
				Data.TickTime = InternalTime + 6.0f;
				this->HeatStatusTick(Data);
				break;
			}
		}
		Data.Type = EDamageType::Heat;
		Data.DamageCauser = DamageCauser;
		Data.HitLocation = HitLocation;
		Data.Damage = BaseDamage * 0.5f;
		Data.TickCount = 6;
		Data.TickTime = InternalTime + 1.0f;
		StatusEffectQueue.Enqueue(Data);
		this->HeatStatusTick(Data);
		break;
	case EDamageType::Cold:
		for (auto &Data : StatusEffectSet)
		{
			if (Data.Type == EDamageType::Cold)
			{
				Data.TickTime = InternalTime + 6.0f;
				break;
			}
		}
		Data.Type = EDamageType::Cold;
		Data.TickTime = InternalTime + 1.0f;
		StatusEffectQueue.Enqueue(Data);
		this->ColdStatusTick(Data);
		break;
	case EDamageType::Electricity:
		break;
	case EDamageType::Toxin:
		break;
	case EDamageType::Blast:
		break;
	case EDamageType::Radiation:
		break;
	case EDamageType::Gas:
		break;
	case EDamageType::Magnetic:
		break;
	case EDamageType::Viral:
		break;
	case EDamageType::Corrosive:
		break;
	case EDamageType::Void:
		break;
	case EDamageType::Raw:
	case EDamageType::Tau:
	case EDamageType::None:
	default:
		break;
	}
}

float AWarframeCharacter::GetHealthDamageModifier(EDamageType DamageType)const
{
	static float HealthModifierArray
		[static_cast<uint32>(EHealthType::Object) - static_cast<uint32>(EHealthType::ClonedFlesh) + 1]
		[static_cast<uint32>(EDamageType::Tau) - static_cast<uint32>(EDamageType::Slash) + 1] = {
	/*ClonedFlesh*/
		{ 1.25f, 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 1.75f, 1.0f, 1.0f, 0.5f, 1.0f },
	/*Machinery*/
		{ 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.5f, 0.75f, 1.75f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 1.0f, 0.5f, 1.0f },
	/*Flesh*/
		{ 1.25f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.0f, 1.0f, 0.75f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Robotic*/
		{ 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.5f, 0.75f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Infested*/
		{ 1.25f, 1.0f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*InfestedFlesh*/
		{ 1.5f, 1.0f, 1.0f, 1.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Fossilized*/
		{ 1.15f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 0.5f, 1.5f, 0.25f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f },
	/*InfestedSinew*/
		{ 1.0f, 1.0f, 1.25f, 1.0f, 1.25f, 1.0f, 1.0f, 0.5f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*Object*/
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	return HealthModifierArray[static_cast<uint32>(this->HealthType)][static_cast<uint32>(DamageType)];
}

float AWarframeCharacter::GetShieldDamageModifier(EDamageType DamageType)const
{
	static float ShieldModifierArray
		[static_cast<uint32>(EShieldType::ProtoShield) - static_cast<uint32>(EShieldType::Shield) + 1]
		[static_cast<uint32>(EDamageType::Tau) - static_cast<uint32>(EDamageType::Slash) + 1] = {
	/*Shield*/
		{ 1.0f, 1.5f, 0.8f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 0.75f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	/*ProtoShield*/
		{ 1.0f, 1.15f, 0.5f, 0.5f, 1.0f, 1.0f, 1.25f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f }
	};

	return ShieldModifierArray[static_cast<uint32>(this->ShieldType)][static_cast<uint32>(DamageType)];
}

float AWarframeCharacter::GetArmorDamageModifier(EDamageType DamageType)const
{
	static float ArmorModifierArray
		[static_cast<uint32>(EArmorType::AlloyArmor) - static_cast<uint32>(EArmorType::FerriteArmor) + 1]
		[static_cast<uint32>(EDamageType::Tau) - static_cast<uint32>(EDamageType::Slash) + 1] = {
	/*FerriteArmor*/
		{ 0.85f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.25f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f },
	/*AlloyArmor*/
		{ 0.5f, 1.0f, 1.15f, 1.0f, 1.25f, 0.5f, 1.0f, 1.0f, 1.75f, 1.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	return ArmorModifierArray[static_cast<uint32>(this->ArmorType)][static_cast<uint32>(DamageType)];
}
