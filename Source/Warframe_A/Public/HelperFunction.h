
#include "CoreMinimal.h"


namespace Warframe
{
	// Free the memory by deleting $Begin.
	bool GetFileContent(const FString &FilePath, const char *&Begin, const char *&End);

	// $WordLength: Not including delimiter.
	FName GetWord(const char *Text, char Delimiter, uint32 &WordLength);

	// Copy data from $Src to $Dst, then offset $Src.
	template<class T>
	FORCEINLINE void ReadIn(T &Dst, const char *&Src)
	{
		Dst = *reinterpret_cast<const T*>(Src);
		Src += sizeof(Dst);
	}
} // End namespace Warframe.
