#pragma once


using uint8 = unsigned char;
using uint32 = unsigned int;
using int32 = signed int;

enum class EHealthType : uint32
{
	BEGIN,
	ClonedFlesh = BEGIN,
	Machinery,
	Flesh,
	Robotic,
	Infested,
	InfestedFlesh,
	Fossilized,
	InfestedSinew,
	Object,
	END,
};

enum class EShieldType : uint32
{
	BEGIN,
	None = BEGIN,
	Shield,
	ProtoShield,
	END,
};

enum class EArmorType : uint32
{
	BEGIN,
	None = BEGIN,
	FerriteArmor,
	AlloyArmor,
	END,
};

enum class ECharacterID : uint8
{
	BEGIN,

	// Light Grineer units
	Butcher = BEGIN,
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

	END,
};

struct FCharacterProperties
{
	uint32		baseLevel;
	EHealthType	healthType;
	uint32		health;
	EShieldType	shieldType;
	uint32		shield;
	EArmorType	armorType;
	uint32		armor;
	float		damageReduction;
	float		affinity;
};

struct FCharacterPropertiesWrapper
{
	ECharacterID id;
	FCharacterProperties characterProp;
};

static const char *EHealthTypeNames[] = {
	"ClonedFlesh",
	"Machinery",
	"Flesh",
	"Robotic",
	"Infested",
	"InfestedFlesh",
	"Fossilized",
	"InfestedSinew",
	"Object",
};

static const char *EShieldTypeNames[] = {
	"None",
	"Shield",
	"ProtoShield",
};

static const char *EArmorTypeNames[] = {
	"None",
	"FerriteArmor",
	"AlloyArmor",
};

static const char *ECharacterIDNames[] = {
	// Light Grineer units
	"Butcher",
	"Flameblade",
	"Guardsman",
	"Powerfist",
	"Scorpion",
	"ShieldLancer",

	// Medium Grineer units
	"Ballista",
	"EliteLancer",
	"Eviscerator",
	"Hellion",
	"Lancer",
	"Scorch",
	"Seeker",
	"Trooper",

	// Heavy Grineer units
	"Bailiff",
	"Bombard",
	"Commander",
	"DrahkMaster",
	"HeavyGunner",
	"HyekkaMaster",
	// Manic,
	"Napalm",
	"Nox",

	// ...

	// Corpus crewmen
	"Crewman",
	"DetronCrewman",
	"SniperCrewman",
	"EliteCrewman",
	"Tech",
	"ProdCrewman",
	"NullifierCrewman",
	"Comba",
	"Scrambus",

	// Corpus walkers
	"Moa",
	"ShockwaveMoa",
	"RailgunMoa",
	"FusionMoa",
	"AntiMoa",
	"IsolatorBursa",
	"DroverBursa",
	"DenialBursa",

	// Corpus ospreys
	"LeechOsprey",
	"MineOsprey",
	"OxiumOsprey",
	"ShieldOsprey",
	"SappingOsprey",
	"AttackDrone",
	"ScavengerDrone",

	// ...
};
