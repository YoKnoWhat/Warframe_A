
#include "HelperFunction.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"


namespace Warframe
{
	bool GetFileContent(const FString &FilePath, const char *&Begin, const char *&End)
	{
		IFileHandle *TableFile = FPlatformFileManager::Get().GetPlatformFile().OpenRead(*FilePath);
		if (TableFile == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWarframeGameInstance::GetFileContent() Unable to open file(%s)."), *FilePath);
			Begin = End = nullptr;
			return false;
		}
		else // if (TableFile != nullptr)
		{
			// Read in file content.
			BYTE* RawBuffer = new BYTE[TableFile->Size() + 1];
			TableFile->Read(RawBuffer, TableFile->Size());

			Begin = reinterpret_cast<const char*>(RawBuffer);
			End = Begin + TableFile->Size();
			*const_cast<char*>(End) = '\0';

			delete TableFile;

			return true;
		}
	}

	FName GetWord(const char *Text, char Delimiter, uint32 &WordLength)
	{
		static char Buffer[256];

		WordLength = 0;

		while (Text[WordLength] != Delimiter && Text[WordLength] != '\0')
		{
			Buffer[WordLength] = Text[WordLength];
			++WordLength;
		}

		Buffer[WordLength] = '\0';

		return FName(StringCast<TCHAR>(Buffer).Get());
	}
} // End namespace Warframe.
