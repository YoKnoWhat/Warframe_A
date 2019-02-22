
#include "TableFormatter.h"
#include "StringUtil.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <functional>

#include "rapidjson/document.h"
#include "rapidjson/reader.h"


size_t Read_Uint8(const std::string& str, void *dst)
{
	*reinterpret_cast<uint8*>(dst) = std::atoi(str.c_str());

	return sizeof(uint8);
}

size_t Read_Uint32(const std::string& str, void *dst)
{
	*reinterpret_cast<uint32*>(dst) = std::atoi(str.c_str());

	return sizeof(uint32);
}

size_t Read_Int32(const std::string& str, void *dst)
{
	*reinterpret_cast<int32*>(dst) = std::atoi(str.c_str());

	return sizeof(int32);
}

size_t Read_Float(const std::string& str, void *dst)
{
	*reinterpret_cast<float*>(dst) = static_cast<float>(std::atof(str.c_str()));

	return sizeof(float);
}

template<class T>
size_t Read_CustomType(const std::string& str, std::unordered_map<std::string, size_t>& map, void *dst, const std::string &TypeName = std::string())
{
	std::string newStr = StringUtil::GetWord(str, '#');

	auto result = map.find(newStr);
	if (result == map.end())
	{
		printf("Invalid %s(%s) at line(%zu).\n\r", TypeName.c_str(), str.c_str(), TableFormatter::m_lineIndex);
		return 0;
	}
	*reinterpret_cast<T*>(dst) = static_cast<T>(result->second);

	return sizeof(T);
}



void TableFormatter::Parse(const std::string &path)
{
	this->m_rootFilePath = path;

	// Read and parse root rule file.
	{
		std::shared_ptr<uint8> buffer = this->ReadFile(m_rootFilePath);
		if (buffer == nullptr)
		{
			printf("Unable to read rule file(%s).\n\r", m_rootFilePath.c_str());
			return;
		}

		rapidjson::Document doc;
		if (doc.ParseInsitu(reinterpret_cast<char*>(buffer.get())).HasParseError())
		{
			printf("Failed to parse rule file %s.\n\r", m_rootFilePath.c_str());
			return;
		}

		m_includeFilePath = doc["Include"].GetString();
		m_mainFilePath = doc["Main"].GetString();
	}

	// Read and parse included rule file.
	{
		std::shared_ptr<uint8> buffer = this->ReadFile(m_includeFilePath);
		if (buffer == nullptr)
		{
			printf("Unable to read rule file(%s).\n\r", m_includeFilePath.c_str());
			return;
		}

		if (this->ParseIncludeRuleFile(buffer) == false)
		{
			return;
		}
	}

	// Read and parse main rule file.
	{
		std::shared_ptr<uint8> buffer = this->ReadFile(m_mainFilePath);
		if (buffer == nullptr)
		{
			printf("Unable to read rule file(%s).\n\r", m_mainFilePath.c_str());
			return;
		}

		if (this->ParseMainRuleFile(buffer) == false)
		{
			return;
		}
	}

	// Read and parse source file.
	{
		std::shared_ptr<uint8> srcFileBuffer = this->ReadFile(m_srcFilePath);
		if (srcFileBuffer == nullptr)
		{
			printf("Unable to read src file(%s).\n\r", m_srcFilePath.c_str());
			return;
		}

		if (this->ParseSrcFile(srcFileBuffer) == false)
		{
			return;
		}
	}

	this->WriteToDstFile();
}

std::shared_ptr<uint8> TableFormatter::ReadFile(const std::string &path)
{
	std::shared_ptr<uint8> buffer;

	FILE *ruleFile = std::fopen(path.c_str(), "rb");
	if (ruleFile == nullptr)
	{
		printf("Unable to open rule file %s.\n\r", path.c_str());
		return nullptr;
	}

	std::fseek(ruleFile, 0, SEEK_END);
	long fileSize = std::ftell(ruleFile);
	std::fseek(ruleFile, 0, SEEK_SET);

	buffer = std::shared_ptr<uint8>(new uint8[fileSize + 1]);
	if (buffer == nullptr)
	{
		printf("Failed to read rule file %s due to memory alloc error.\n\r", path.c_str());
		std::fclose(ruleFile);
		return nullptr;
	}

	std::fread(buffer.get(), sizeof(uint8), fileSize, ruleFile);
	buffer.get()[fileSize] = '\0';

	std::fclose(ruleFile);
	return buffer;
}

bool TableFormatter::ParseIncludeRuleFile(std::shared_ptr<uint8> buffer)
{
	rapidjson::Document doc;

	// Parse rule file.
	if (doc.ParseInsitu(reinterpret_cast<char*>(buffer.get())).HasParseError())
	{
		printf("Failed to parse rule file %s.\n\r", m_includeFilePath.c_str());
		return false;
	}

	auto &customTypes = doc["CustomType"];
	for (uint32 i = 0; i < customTypes.Size(); ++i)
	{
		CustomTypeInfo customTypeInfo;

		auto customType = customTypes[i].GetObject();

		std::string customTypeName = customType["Name"].GetString();
		
		std::string customTypeType = customType["Type"].GetString();
		if (customTypeType == "uint8")
		{
			customTypeInfo.innateType = ETypeEnum::Uint8;
		}
		else if (customTypeType == "uint16")
		{
			customTypeInfo.innateType = ETypeEnum::Uint16;
		}
		else if (customTypeType == "uint32")
		{
			customTypeInfo.innateType = ETypeEnum::Uint32;
		}
		else
		{
			printf("Undefined innate type(%s) of custom type(%s).\n\r", customTypeType.c_str(), customTypeName.c_str());
			return false;
		}

		auto &customTypeDef = customType["Definition"];
		customTypeInfo.stringList.resize(customTypeDef.Size());
		customTypeInfo.stringIndexToStringID.reserve(customTypeDef.Size());
		customTypeInfo.totalStringLengthIncludingZero = 0;

		for (uint32 j = 0; j < customTypeDef.Size(); ++j)
		{
			std::string customTypeStr = customTypeDef[j].GetString();

			customTypeInfo.stringList[j] = customTypeStr;
			customTypeInfo.stringIndexToStringID[customTypeStr] = j;
			customTypeInfo.totalStringLengthIncludingZero += customTypeStr.size() + 1;
		}

		m_customTypeMap[customTypeName] = std::move(customTypeInfo);
	}

	return true;
}

bool TableFormatter::ParseMainRuleFile(std::shared_ptr<uint8> buffer)
{
	rapidjson::Document doc;

	if (doc.ParseInsitu(reinterpret_cast<char*>(buffer.get())).HasParseError())
	{
		printf("Failed to parse rule file %s.\n\r", m_mainFilePath.c_str());
		return false;
	}

	m_srcFilePath = doc["SrcFilePath"].GetString();
	m_dstFilePath = doc["DstFilePath"].GetString();

	// Read and format output custom types.
	{
		auto &outputCustomType = doc["OutputCustomType"];

		m_formattedOutputCustomTypesBufferSize = 0;

		// Compute required buffer size for all formatted output custom types.
		for (uint32 i = 0; i < outputCustomType.Size(); ++i)
		{
			std::string customTypeName = outputCustomType[i].GetString();

			auto result = m_customTypeMap.find(customTypeName);
			if (result == m_customTypeMap.end())
			{
				printf("Invalid output custom type name(%s).\n\r", customTypeName.c_str());
				return false;
			}
			m_formattedOutputCustomTypesBufferSize += sizeof(uint32) + result->second.totalStringLengthIncludingZero;
		}

		// Allocate memory.
		m_formattedOutputCustomTypes = std::shared_ptr<uint8>(new uint8[m_formattedOutputCustomTypesBufferSize]);
		if (m_formattedOutputCustomTypes == nullptr)
		{
			printf("Unable to allocate space to format output custom types.\n\r");
			return false;
		}

		// Formatting.
		uint8 *buffer = m_formattedOutputCustomTypes.get();
		size_t offset = 0;

		for (uint32 i = 0; i < outputCustomType.Size(); ++i)
		{
			std::string customTypeName = outputCustomType[i].GetString();
			std::vector<std::string> &stringList = m_customTypeMap[customTypeName].stringList;

			*reinterpret_cast<uint32*>(buffer + offset) = static_cast<uint32>(stringList.size());
			offset += sizeof(uint32);

			for (auto& string :stringList)
			{
				std::memcpy(buffer + offset, string.data(), string.size());
				offset += string.size();
				buffer[offset] = '\0';
				++offset;
			}
		}
	}

	// Read in column types and alloc buffer for all table entries.
	m_tableEntrySize = 0;

	auto& columnTypes = doc["ColumnType"];
	for (uint32 i = 0; i < columnTypes.Size(); ++i)
	{
		std::string columnTypeStr = columnTypes[i].GetString();

		if (columnTypeStr == "uint32")
		{
			m_readInFunctions.push_back(std::bind(Read_Uint32, std::placeholders::_1, std::placeholders::_2));
			m_tableEntrySize += sizeof(uint32);
		}
		else if (columnTypeStr == "int32")
		{
			m_readInFunctions.push_back(std::bind(Read_Int32, std::placeholders::_1, std::placeholders::_2));
			m_tableEntrySize += sizeof(int32);
		}
		else if (columnTypeStr == "float")
		{
			m_readInFunctions.push_back(std::bind(Read_Float, std::placeholders::_1, std::placeholders::_2));
			m_tableEntrySize += sizeof(float);
		}
		else // Custom types.
		{
			auto result = m_customTypeMap.find(columnTypeStr);
			if (result == m_customTypeMap.end())
			{
				printf("Invalid column type %s.\n\r", columnTypeStr.c_str());
				return false;
			}
			ETypeEnum innateType = result->second.innateType;

			if (innateType == ETypeEnum::Uint8)
			{
				m_readInFunctions.push_back(std::bind(Read_CustomType<uint8>, std::placeholders::_1, m_customTypeMap[columnTypeStr].stringIndexToStringID, std::placeholders::_2, columnTypeStr));
				m_tableEntrySize += sizeof(uint8);
			}
			else if (innateType == ETypeEnum::Uint16)
			{
				m_readInFunctions.push_back(std::bind(Read_CustomType<uint16>, std::placeholders::_1, m_customTypeMap[columnTypeStr].stringIndexToStringID, std::placeholders::_2, columnTypeStr));
				m_tableEntrySize += sizeof(uint16);
			}
			else if (innateType == ETypeEnum::Uint32)
			{
				m_readInFunctions.push_back(std::bind(Read_CustomType<uint32>, std::placeholders::_1, m_customTypeMap[columnTypeStr].stringIndexToStringID, std::placeholders::_2, columnTypeStr));
				m_tableEntrySize += sizeof(uint32);
			}
		}
	}
	return true;
}

bool TableFormatter::ParseSrcFile(std::shared_ptr<uint8> buffer)
{
	m_formattedTableEntries.reserve(256 * m_tableEntrySize);
	{
		size_t columnSizeInBytes;
		size_t numColumns = m_readInFunctions.size();
		size_t baseOffset = 0;
		const char *text = reinterpret_cast<const char*>(buffer.get());

		StringUtil::SkipWhiteSpace(text);
		StringUtil::SkipLine(text);
		StringUtil::SkipWhiteSpace(text);

		m_lineIndex = 1;

		while (*text != '\0')
		{
			m_formattedTableEntries.resize(baseOffset + m_tableEntrySize);

			for (size_t i = 0; i < numColumns - 1; ++i)
			{
				columnSizeInBytes = m_readInFunctions[i](StringUtil::GetWord(text, ','), &m_formattedTableEntries[baseOffset]);
				++text;
				if (columnSizeInBytes == 0)
				{
					return false;
				}

				baseOffset += columnSizeInBytes;
			}

			m_readInFunctions.back()(StringUtil::GetWord(text, '\n'), &m_formattedTableEntries[baseOffset]);
			++text;
			if (columnSizeInBytes == 0)
			{
				return false;
			}

			baseOffset += columnSizeInBytes;

			StringUtil::SkipWhiteSpace(text);
			++m_lineIndex;
		}
	}
	return true;
}

void TableFormatter::WriteToDstFile()
{
	FILE *dstFile = std::fopen(m_dstFilePath.c_str(), "wb");
	if (dstFile == nullptr)
	{
		printf("Unknown error when trying to open dst file(%s).\n\r", m_dstFilePath.c_str());
		return;
	}

	std::fwrite(m_formattedOutputCustomTypes.get(), sizeof(uint8), m_formattedOutputCustomTypesBufferSize, dstFile);
	std::fwrite(m_formattedTableEntries.data(), sizeof(uint8), m_formattedTableEntries.size(), dstFile);

	std::fclose(dstFile);
}
