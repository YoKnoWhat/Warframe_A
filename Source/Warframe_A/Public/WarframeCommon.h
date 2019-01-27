#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHealthType : uint8
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

UENUM(BlueprintType)
enum class EShieldType : uint8
{
	None,
	Shield,
	ProtoShield,
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	None,
	FerriteArmor,
	AlloyArmor,
};

enum class ECharacterID : uint32
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
	// SappingOsprey,
	AttackDrone,
	ScavengerDrone,

	// ...
};

struct FCharacterProperties
{
	uint32		BaseLevel;
	EHealthType	HealthType;
	uint32		Health;
	EShieldType	ShieldType;
	uint32		Shield;
	EArmorType	ArmorType;
	uint32		Armor;
	float		DamageReduction;
	float		Affinity;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	PrimaryRifle,
	PrimaryShotgun,
	SniperRifle,
	Bow,
};

UENUM(BlueprintType)
enum class EWeaponTriggerType : uint8
{
	Auto,
	SemiAuto,
	Burst,
	Hybrid,
	Continuous,
	Charge,
	Launcher,
	Speargun,
};

class AWeaponBase;
class AWarframeCharacter;
class UCharacterWidgetComponent;
class UCharacterWidget;
