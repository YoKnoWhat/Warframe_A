
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>

#include "Header.h"


bool DoParse(const char *srcFilePath, const char *dstFilePath);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Incorrect arguments count.\n\r");
		return 0;
	}

	if (DoParse(argv[1], argv[2]) == false)
	{
		printf("Failed parsing.\n\r");
	}

	return 0;
}

bool DoParse(const char *srcFilePath, const char *dstFilePath)
{
	std::ifstream srcFile(srcFilePath);
	if (srcFile.is_open() == false)
	{
		printf("Unable to open src file %s.\n\r", srcFilePath);
		return false;
	}

	std::ofstream dstFile(dstFilePath, std::ios_base::out | std::ios_base::binary);
	if (dstFile.is_open() == false)
	{
		printf("Unable to open dst file %s.\n\r", dstFilePath);
		return false;
	}

	// Init maps
	std::unordered_map<std::string, EHealthType> EHealthTypeMap;
	std::unordered_map<std::string, EShieldType> EShieldTypeMap;
	std::unordered_map<std::string, EArmorType> EArmorTypeMap;
	std::unordered_map<std::string, ECharacterID> ECharacterIDMap;

	for (uint32 i = static_cast<uint32>(EHealthType::BEGIN); i < static_cast<uint32>(EHealthType::END); ++i)
	{
		EHealthTypeMap.insert({ std::string(EHealthTypeNames[i]), static_cast<EHealthType>(i) });
	}
	for (uint32 i = static_cast<uint32>(EShieldType::BEGIN); i < static_cast<uint32>(EShieldType::END); ++i)
	{
		EShieldTypeMap.insert({ std::string(EShieldTypeNames[i]), static_cast<EShieldType>(i) });
	}
	for (uint32 i = static_cast<uint32>(EArmorType::BEGIN); i < static_cast<uint32>(EArmorType::END); ++i)
	{
		EArmorTypeMap.insert({ std::string(EArmorTypeNames[i]), static_cast<EArmorType>(i) });
	}
	for (uint32 i = static_cast<uint32>(ECharacterID::BEGIN); i < static_cast<uint32>(ECharacterID::END); ++i)
	{
		ECharacterIDMap.insert({ std::string(ECharacterIDNames[i]), static_cast<ECharacterID>(i) });
	}

	std::vector<FCharacterPropertiesWrapper> characterPropEntries;
	std::stringstream sstream;
	std::string line;
	std::string tmpString;
	uint32 lineIndex = 2;

	// Skip attribute titles
	std::getline(srcFile, line);

	while (true)
	{
		if (!std::getline(srcFile, line))
		{
			break;
		}

		characterPropEntries.push_back({});

		FCharacterPropertiesWrapper &newEntry = characterPropEntries.back();

		sstream.str(line);

		// Name
		std::getline(sstream, tmpString, ',');
		{
			auto result = ECharacterIDMap.find(tmpString);
			if (result == ECharacterIDMap.end())
			{
				printf("Invalid character name(%s) at line(%u).\n\r", tmpString.c_str(), lineIndex);
				return false;
			}
			newEntry.id = result->second;
		}

		// BaseLevel
		sstream >> newEntry.characterProp.baseLevel;
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		// HealthType
		std::getline(sstream, tmpString, ',');
		{
			auto result = EHealthTypeMap.find(tmpString);
			if (result == EHealthTypeMap.end())
			{
				printf("Invalid health type(%s) at line(%u).\n\r", tmpString.c_str(), lineIndex);
				return false;
			}
			newEntry.characterProp.healthType = result->second;
		}

		// Health
		sstream >> newEntry.characterProp.health;
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		// ShieldType
		std::getline(sstream, tmpString, ',');
		{
			auto result = EShieldTypeMap.find(tmpString);
			if (result == EShieldTypeMap.end())
			{
				printf("Invalid shield type(%s) at line(%u).\n\r", tmpString.c_str(), lineIndex);
				return false;
			}
			newEntry.characterProp.shieldType = result->second;
		}
		
		// Shield
		sstream >> newEntry.characterProp.shield;
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		// ArmorType
		std::getline(sstream, tmpString, ',');
		{
			auto result = EArmorTypeMap.find(tmpString);
			if (result == EArmorTypeMap.end())
			{
				printf("Invalid armor type(%s) at line(%u).\n\r", tmpString.c_str(), lineIndex);
				return false;
			}
			newEntry.characterProp.armorType = result->second;
		}

		// Armor
		sstream >> newEntry.characterProp.armor;
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		// DamageReduction
		sstream >> newEntry.characterProp.damageReduction;
		sstream.ignore(std::numeric_limits<std::streamsize>::max(), ',');

		// Affinity
		sstream >> newEntry.characterProp.affinity;

		if (sstream.fail())
		{
			printf("Stringstream failed at line(%u): %s.\n\r", lineIndex, line.c_str());
			return false;
		}

		++lineIndex;
	}

	dstFile.write(reinterpret_cast<const char*>(characterPropEntries.data()), characterPropEntries.size() * sizeof(FCharacterPropertiesWrapper));

	return true;
}