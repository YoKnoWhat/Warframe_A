// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "WarframeCharacter.generated.h"


UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Impact,
	Puncture,
	Slash,
	Heat,
};

enum class EHealthType : uint32
{
	ClonedFlesh,
	Machinery,
	Flesh,
	Robotic,
	Infested,
	InfestedFlesh,
	Fossilized,
	InfestedSinew,
	Object,
};

enum class EShieldType : uint32
{
	Shield,
	ProtoShield,
};

enum class EArmorType : uint32
{
	FerriteArmor,
	AlloyArmor,
};

enum class EEnemyID : uint32
{
	// Light Grineer units
	Butcher,
	Flameblade,
	Guardsman,
	Powerfist,
	Scorpion,
	ShieldLancer,

	// Medium Grineer units
	Ballista,
	EliteLancer,
	Eviscerator,
	Hellion,
	Lancer,
	Scorch,
	Seeker,
	Trooper,

	// Heavy Grineer units
	Bailiff,
	Bombard,
	Commander,
	DrahkMaster,
	HeavyGunner,
	HyekkaMaster,
	// Manic,
	Napalm,
	Nox,

	// ...

	// Corpus crewmen
	Crewman,
	DetronCrewman,
	SniperCrewman,
	EliteCrewman,
	Tech,
	ProdCrewman,
	NullifierCrewman,
	Comba,
	Scrambus,

	// Corpus walkers
	Moa,
	ShockwaveMoa,
	RailgunMoa,
	FusionMoa,
	AntiMoa,
	IsolatorBursa,
	DroverBursa,
	DenialBursa,

	// Corpus ospreys
	LeechOsprey,
	MineOsprey,
	OxiumOsprey,
	ShieldOsprey,
	SappingOsprey,
	AttackDrone,
	ScavengerDrone,

	// ...
};

UCLASS()
class WARFRAME_A_API AWarframeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarframeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void InitProperties(int32 CharacterID, uint32 Level);

	UFUNCTION(BlueprintCallable)
		float GetArmor();

	UFUNCTION(BlueprintCallable)
		float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
		float GetMaxShield();

	UFUNCTION(BlueprintCallable)
		float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
		float GetCurrentShield();

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetWeapon(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetCurrentWeapon();

	float MyTakeDamage(float Damage, EDamageType DamageType);

protected:
	float Armor;
	float MaxHealth;
	float MaxShield;
	float CurrentHealth;
	float CurrentShield;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AWeaponBase *EquippedWeapon;

	float NoDamageTakenDuration;
	float ShieldRegenerationPerSecond;
};
