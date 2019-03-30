
#include "Utility/HelperFunction.h"
#include "Character/StateMachineComponent.h"
#include "Character/WarframeCharacterAIController.h"
#include "UI/CharacterHeaderPanel.h"

#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"


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

	FString GetWord(const FString& Str, uint32 Start, TCHAR Delimiter)
	{
		const TCHAR* Text = *Str;

		uint32 End = Start;
		while (Text[End] != Delimiter)
		{
			++End;
		}

		return Str.Mid(static_cast<int32>(Start), static_cast<int32>(End - Start));
	}

	UClass* GetNativeClassByName(FName Name)
	{
		// ClassRefs
		if (Name == "AWarframeCharacterAIController")
		{
			return AWarframeCharacterAIController::StaticClass();
		}
		if (Name == "UStateMachineComponent")
		{
			return UStateMachineComponent::StaticClass();
		}
		if (Name == "UCharacterHeaderPanel")
		{
			return UCharacterHeaderPanel::StaticClass();
		}

		// ResourceRefs
		if (Name == "UMaterial")
		{
			return UMaterial::StaticClass();
		}
		if (Name == "UParticleSystem")
		{
			return UParticleSystem::StaticClass();
		}
		if (Name == "UStaticMesh")
		{
			return UStaticMesh::StaticClass();
		}
		if (Name == "UTexture2D")
		{
			return UTexture2D::StaticClass();
		}
		
		return UObject::StaticClass();
	}

} // End namespace Warframe.
