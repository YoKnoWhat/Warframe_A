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

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Begin = 0,
	None = Begin,

	// Physical damage.
	Slash,
	Impact,
	Puncture,

	// Primary elemental damage.
	Heat,
	Cold,
	Electricity,
	Toxin,

	// Secondary(combined) elemental damage.
	Blast,
	Radiation,
	Gas,
	Magnetic,
	Viral,
	Corrosive,

	// Unique damage.
	Raw, // True
	Void,
	Tau,

	End = Tau,
};

enum class EFactionType : uint8
{
	None,
	Tenno,
	Grineer,
	Corpus,
	Infested,
	Sentinent,
	Orokin,
	Syndicates,
	Wild,
};

enum class EModID : uint16
{};

enum class EPolarityID : uint8
{
	None,
	Madurai,
	Vazarin,
	Naramon,
	Zenurik,
	Unairu,
	Penjaga,
	Umbra,
};

enum class ECharacterID : uint16
{
	Begin = 0,

	None = Begin,

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

	EndEnemy = ScavengerDrone,

	// Warframes
	Ash,
	AshPrime,
	Atlas,
	Banshee,
	BansheePrime,
	Baruuk,
	Chroma,
	ChromaPrime,
	Ember,
	EmberPrime,
	Equinox,
	Excalibur,
	ExcaliburPrime,
	ExcaliburUmbra,
	Frost,
	FrostPrime,
	Gara,
	Garuda,
	Harrow,
	Hydroid,
	HydroidPrime,
	Inaros,
	Ivara,
	Khora,
	Limbo,
	LimboPrime,
	Loki,
	LokiPrime,
	Mag,
	MagPrime,
	Mesa,
	MesaPrime,
	Mirage,
	MiragePrime,
	Nekros,
	NekrosPrime,
	Nezha,
	Nidus,
	Nova,
	NovaPrime,
	Nyx,
	NyxPrime,
	Oberon,
	OberonPrime,
	Octavia,
	Revenant,
	Rhino,
	RhinoPrime,
	Saryn,
	SarynPrime,
	Titania,
	Trinity,
	TrinityPrime,
	Valkyr,
	ValkyrPrime,
	Vauban,
	VaubanPrime,
	Volt,
	VoltPrime,
	Wukong,
	Zephyr,
	ZephyrPrime,

	End = ZephyrPrime,
};

enum class EWeaponID : uint16
{
	Begin = 0,

	None = Begin, // No weapon equipped.

	// Primary weapons.
	Amprex,
	ArcaPlasmor,
	Argonak,
	Astilla,
	Attica,
	Battacor,
	Baza,
	Boar,
	BoarPrime,
	Boltor,
	BoltorPrime,
	Braton,
	BratonPrime,
	Burston,
	BurstonPrime,
	Buzlok,
	Cernos,
	CernosPrime,
	Convectrix,
	Corinth,
	Daikyu,
	Dera,
	DeraVandal,
	Drakgoon,
	Dread,
	Exergis,
	Ferrox,
	FluxRifle,
	Glaxion,
	Gorgon,

	// Secondary weapons.
	Acrid,
	Afuris,
	Akbolto,
	AkboltoPrime,
	Akbronco,
	AkbroncoPrime,
	Akjagara,
	Aklato,
	Aklex,
	Akmagnus,
	Aksomati,
	Akstiletto,

	// Melee weapons.
	AckBrunt,
	Amphis,
	Anku,
	Ankyros,
	AnkyrosPrime,
	ArcaTitron,
	Atterax,
	Bo,
	BoPrime,
	Boltace,
	BrokenScepter,
	BrokenWar,
	Cassowar,
	Caustacyst,
	CeramicDagger,
	Cerata,
	CobraCrane,
	Cronus,
	DakraPrime,

	End = DakraPrime,
}; // End enum class EWeaponID.

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
	Held,
	AutoBurst,
};

enum class EAmmoType : uint8
{
	Rifle,
	Shotgun,
	Sniper,
	Bow,
	Pistol,
};

enum class ENoiseType : uint8
{
	Alarming,
	Silent,
};

struct FMod
{
	FName	Name;
	uint32	Level;
};

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
	float	Accuracy;
	float	ChargeRate;
	uint32	Pellets;
	float	CriticalChance;
	float	CriticalMultiplier;
	uint32	FalloffStart;
	uint32	FalloffEnd;
	float	FalloffDamage;
	float	FireRate;
	uint32	PunchThrough;
	float	Reload;
	float	Status;
	float	Slash;
	float	Impact;
	float	Puncture;
	float	Heat;
	float	Cold;
	float	Electricity;
	float	Toxin;
	float	Blast;
	float	Radiation;
	float	Gas;
	float	Magnetic;
	float	Viral;
	float	Corrosive;
	// Struct alignment.
	ENoiseType			Noise;
	EWeaponTriggerType	Trigger;
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

struct FDamagePair
{
	EDamageType Type;
	float Value;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	PrimaryRifle,
	PrimaryShotgun,
	SniperRifle,
	Bow,
};


class ARoundBase;
class AWeaponBase;
class AWarframeCharacter;
class UCharacterWidgetComponent;
class UCharacterWidget;
class USpawnPoint;

template<class T>
constexpr std::underlying_type_t<T> CastToUnderlyingType(T value)
{
	return static_cast<std::underlying_type_t<T>>(value);
}
