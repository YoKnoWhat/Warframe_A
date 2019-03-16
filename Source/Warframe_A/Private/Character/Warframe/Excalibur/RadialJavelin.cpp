
#include "Character/Warframe/Excalibur/RadialJavelin.h"
#include "Character/Warframe/Excalibur/Excalibur.h"


FRadialJavelin::FRadialJavelin(AWarframe* Warframe) :
	FAbilityObject(Warframe, true, false, 1.0f, 2.5f)
{
	this->OnLevelChanged(0);
}

void FRadialJavelin::Tick(float DeltaTime)
{
	FAbilityObject::Tick(DeltaTime);
}

void FRadialJavelin::OnLevelChanged(uint32 NewLevel)
{
	FAbilityObject::OnLevelChanged(NewLevel);

	switch (NewLevel)
	{
	case 0:
		Damage = 500.0f;
		Range = 1500;
		JavelinCount = 5;
		break;
	case 1:
		Damage = 650.0f;
		Range = 1800;
		JavelinCount = 7;
		break;
	case 2:
		Damage = 800.0f;
		Range = 2200;
		JavelinCount = 10;
		break;
	case 3:
		Damage = 1000.0f;
		Range = 2500;
		JavelinCount = 12;
		break;
	default:
		break;
	}

	Damage *= Warframe->GetAbilityStrength() * 0.01f;
	Range = static_cast<uint32>(Warframe->GetAbilityRange() * 0.01f * static_cast<float>(Range));

	return;
}

void FRadialJavelin::PreCast()
{
	
}

void FRadialJavelin::DoCast()
{
	
}

void FRadialJavelin::PostCast()
{}
