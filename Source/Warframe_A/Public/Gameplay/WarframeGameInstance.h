// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Engine/GameInstance.h"
#include "WarframeGameInstance.generated.h"


struct FCharacterInfo
{
	FName		Name;
	uint32		BaseLevel;
	uint32		Health;
	uint32		Shield;
	uint32		Armor;
	float		Affinity;
	// Struct alignment.
	EHealthType	HealthType;
	EShieldType	ShieldType;
	EArmorType	ArmorType;
};

struct FCharacterAppearance
{
	FName		Mesh;
	FName		AnimClass;
	FVector		RelativeLocation;
	FRotator	RelativeRotation;
	float		HalfHeight;
	float		Radius;
};

struct FEnemyInfo
{
	EFactionType	Faction;
	EWeaponID		Weapon1;
	EWeaponID		Weapon2;
};

struct FWarframeInfo
{
	uint32		Energy;
	float		SprintSpeed;
	EPolarityID AuraPolarity;
	EPolarityID ExilusPolarity;
	EPolarityID Polarity1;
	EPolarityID Polarity2;
	EPolarityID Polarity3;
	EPolarityID Polarity4;
	EPolarityID Polarity5;
	EPolarityID Polarity6;
	EPolarityID Polarity7;
	EPolarityID Polarity8;
};

struct FWeaponModeInfo
{
	float				Accuracy;
	float				ChargeRate;
	uint32				Pellets;
	float				CriticalChance;
	float				CriticalMultiplier;
	uint32				FalloffStart;
	uint32				FalloffEnd;
	float				FalloffDamage;
	float				FireRate;
	ENoiseType			Noise;
	uint32				PunchThrough;
	float				Reload;
	float				Status;
	EWeaponTriggerType	Trigger;
	/** Trigger specific begin. */
	uint32				MaxBurstCount;
	uint32				MaxChargeRounds;
	float				ChargeMultiplier;
	/** Trigger specific end. */
	float				Slash;
	float				Impact;
	float				Puncture;
	float				Heat;
	float				Cold;
	float				Electricity;
	float				Toxin;
	float				Blast;
	float				Radiation;
	float				Gas;
	float				Magnetic;
	float				Viral;
	float				Corrosive;
};

struct FWeaponInfo
{
	FName Name;
	uint32 Magazine;
	uint32 Ammo;
	TArray<FWeaponModeInfo> ModeInfoArray;
	// Struct alignment.
	EAmmoType AmmoType;
};

struct FWeaponAppearance
{
	FName		Mesh;
	FName		FireEmitter;
	FName		ReloadAnim;
};

struct FPickableObjectInfo
{
	FLinearColor	BeamColor;
	FName			Mesh;
};

/**
 * 
 */
UCLASS()
class WARFRAME_A_API UWarframeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWarframeGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init()override;

	virtual void Shutdown()override;

	const FCharacterInfo* GetCharacterInfo(ECharacterID CharacterID)const;

	const FCharacterAppearance* GetCharacterAppearance(ECharacterID CharacterID)const;

	const FEnemyInfo* GetEnemyInfo(ECharacterID CharacterID)const;

	const FWarframeInfo* GetWarframeInfo(ECharacterID CharacterID)const;

	const FWeaponInfo* GetWeaponInfo(EWeaponID WeaponID)const;

	const FWeaponAppearance* GetWeaponAppearance(EWeaponID WeaponID)const;

	const FPickableObjectInfo* GetPickableObjectInfo(EPickableObjectID PickableObjectID)const;

protected:
	void ReadInDataTables();

	void ReadInCharacterTable(const char* Begin, const char* End);

	void ReadInCharacterAppearanceTable(const char* Begin, const char* End);

	void ReadInEnemyTable(const char* Begin, const char* End);

	void ReadInWarframeTable(const char* Begin, const char* End);

	void ReadInWeaponTable(const char* Begin, const char* End);

	void ReadInWeaponAppearanceTable(const char* Begin, const char* End);

	void ReadInPickableObjectTable(const char* Begin, const char* End);

protected:
	TArray<FCharacterInfo> CharacterInfoArray;

	TArray<FCharacterAppearance> CharacterAppearanceArray;

	TArray<FEnemyInfo> EnemyInfoArray;

	TArray<FWarframeInfo> WarframeInfoArray;

	TArray<FWeaponInfo> WeaponInfoArray;

	TArray<FWeaponAppearance> WeaponAppearanceArray;

	TArray<FPickableObjectInfo> PickableObjectInfoArray;
};
