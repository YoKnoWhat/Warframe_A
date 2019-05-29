#pragma once

#include "Runtime/CoreUObject/Public/UObject/ObjectMacros.h"


/** Custom Trace Channels. */
#define ECC_Weapon	ECC_GameTraceChannel1

/** Custom Object Channels. */
#define ECC_Round	ECC_GameTraceChannel2

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

enum class EWarframeTeamID : uint8
{
	Player,
	Neutral,
	Enemy1,
	Enemy2,
	Enemy3,
	Enemy4,
	Enemy5,
	Enemy6,
	Enemy7,
	Enemy8,
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

	// Grineer begin
	Grineer_Begin,

	// Light Grineer units
	Butcher = Grineer_Begin,
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

	Grineer_End = Nox,

	// Grineer end

	// ...

	// Corpus begin
	Corpus_Begin,

	// Corpus crewmen
	Crewman = Corpus_Begin,
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

	// Corpus end
	Corpus_End = ScavengerDrone,

	Enemy_End = Corpus_End,

	// Warframe begin
	Warframe_Begin,

	Ash = Warframe_Begin,
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

	// Warframe end
	Warframe_End = ZephyrPrime,

	End = Warframe_End,
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
	Grakata,
	Grinlok,
	Harpak,
	Hek,
	Hema,
	Hind,
	Ignis,
	IgnisWraith,

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

	Staticor,

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

enum class EPickableObjectID : uint8
{
	Begin = 0,

	None = Begin,
	HealthOrb,
	EnergyOrb,
	RifleAmmo,
	ShotgunAmmo,
	SniperAmmo,
	BowAmmo,
	PistolAmmo,
	Credit,
	AlloyPlate,
	Ferrite,
	NanoSpores,
	Salvage,
	Circuits,
	Cryotic,
	Oxium,
	Plastids,
	PolymerBundle,
	Rubedo,
	ArgonCrystal,
	ControlModule,
	Gallium,
	Morphics,
	NeuralSensors,
	Neurodes,
	NitainExtract,
	OrokinCell,
	Tellurium,

	End = Tellurium,
};

enum class EPredefinedColorID : uint8
{
	Begin = 0,

	White = Begin,
	Black,
	Transparent,
	Red,
	Green,
	Blue,
	Yellow,
	Cyan,
	Magenta,
	Orange,
	Purple,
	Turquoise,
	Silver,
	Emerald,

	End = Emerald,
};

enum class EWeaponTriggerType : uint8
{
	Auto,
	AutoSpool,
	AutoBurst,
	SemiAuto,
	Burst,
	Continuous,
	Charge,
	Launcher,
	Speargun,
	Held
};

enum class EAmmoType : uint8
{
	Rifle,
	Shotgun,
	Sniper,
	Bow,
	Pistol,
};

UENUM(BlueprintType)
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

struct FDamagePair
{
	EDamageType Type;
	float Value;
};

UENUM(BlueprintType)
enum class EWeaponSlotType : uint8
{
	Primary,
	Secondary,
	Melee,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	PrimaryRifle,
	PrimaryShotgun,
	SniperRifle,
	Bow,
};

struct FWarframeConfigSingleton;

class ARoundBase;
class AWeaponBase;
class AWarframeCharacter;
class AWarframe;
class FAbilityObject;
class FStateObject;
class UCharacterWidgetComponent;
class UCharacterWidget;
class UStateMachineComponent;

template<class T>
constexpr std::underlying_type_t<T> CastToUnderlyingType(T value)
{
	return static_cast<std::underlying_type_t<T>>(value);
}

template<class T>
using UnderlyingType = std::underlying_type_t<T>;
