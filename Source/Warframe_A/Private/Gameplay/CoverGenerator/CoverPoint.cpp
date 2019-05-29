
#include "Gameplay/CoverGenerator/CoverPoint.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/CoverGenerator/CoverGenerator.h"

#include "Runtime/Engine/Classes/Engine/World.h"


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

bool UCoverPoint::IsLocationShootable(const FVector& Location)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	ACoverGenerator* CoverGenerator = ACoverGenerator::GetCoverGenerator(nullptr);

	const FVector LocalRight = FVector::CrossProduct(FVector::UpVector, this->_DirectionToWall);
	const FVector SourceLocation = this->Location + LocalRight * CoverGenerator->GetOffsetWhenLeaningSides() + this->_DirectionToWall * CoverGenerator->GetOffsetFrontAim();

	TArray<FHitResult> OutHits;
	return !CoverGenerator->GetWorld()->LineTraceMultiByObjectType(OutHits, SourceLocation, Location, ObjectQueryParams, QueryParams);
}

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
