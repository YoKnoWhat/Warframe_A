#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using int32 = signed int;

enum class ETypeEnum : uint8
{
	Uint8,
	Uint16,
	Uint32,
};

struct CustomTypeInfo
{
	ETypeEnum innateType;
	std::vector<std::string> stringList;
	std::unordered_map<std::string, size_t> stringIndexToStringID;
	size_t totalStringLengthIncludingZero;
};

class TableFormatter
{
	public:
		void Parse(const std::string &path);

	protected:
		std::shared_ptr<uint8> ReadFile(const std::string &path);

		bool ParseIncludeRuleFile(std::shared_ptr<uint8> buffer);

		bool ParseMainRuleFile(std::shared_ptr<uint8> buffer);

		// Parse source file and format all the table entries.
		bool ParseSrcFile(std::shared_ptr<uint8> buffer);

		void WriteToDstFile();

	public:
		inline static size_t m_lineIndex = 0;

	protected:

		// Cached variables.
		std::string m_rootFilePath;
		std::string m_includeFilePath;
		std::string m_mainFilePath;
		std::string m_srcFilePath;
		std::string m_dstFilePath;
		std::unordered_map<std::string, CustomTypeInfo> m_customTypeMap;

		using ReadInFuncType = std::function<size_t(const std::string&, void*)>;

		std::vector<ReadInFuncType> m_readInFunctions;
		size_t m_tableEntrySize; // total size for a row in data table.

		// Output.
		size_t m_formattedOutputCustomTypesBufferSize;
		std::shared_ptr<uint8> m_formattedOutputCustomTypes;
		std::vector<uint8> m_formattedTableEntries;
};
