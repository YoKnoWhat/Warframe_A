
#include "Gameplay/CoverGenerator/CoverPoint.h"
#include "Character/WarframeCharacter.h"


UCoverPoint::UCoverPoint()
	: Location(FVector::ZeroVector), _DirectionToWall(FVector::ZeroVector)
{}

UCoverPoint::UCoverPoint(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer), Location(FVector::ZeroVector), _DirectionToWall(FVector::ZeroVector)
{}

UCoverPoint::UCoverPoint(const FVector & InLocation, const FVector & InDirectionToWall)
	: Location(InLocation), _DirectionToWall(InDirectionToWall)
{}

UCoverPoint::UCoverPoint(const UCoverPoint& InCopy)
	: Location(InCopy.Location), _DirectionToWall(InCopy.DirectionToWall())
{}

AWarframeCharacter* UCoverPoint::GetOccupant()const
{
	return OccupiedBy;
}

bool UCoverPoint::IsOccupied()const
{
	return this->OccupiedBy != nullptr;
}

void UCoverPoint::Occupy(AWarframeCharacter* Character)
{
	if (this->OccupiedBy == nullptr)
	{
		this->OccupiedBy = Character;
	}
}

void UCoverPoint::Release()
{
	this->OccupiedBy = nullptr;
}

void UCoverPoint::DirectionToWall(FVector val)
{
	_DirectionToWall = val;
	// Set rotator from direction 
	RotatorXToWall = FRotationMatrix::MakeFromX(_DirectionToWall).Rotator();
}
