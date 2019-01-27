// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


UCLASS()
class WARFRAME_A_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void BeginFire();

	// Allowing to call multiple times.
	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void StopReload();

	UFUNCTION(BlueprintCallable)
	bool IsReloading()const;

	// Property getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFireRate()const
	{
		return FireRate;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAccuracy()const
	{
		return Accuracy;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetReloadTime()const
	{
		return ReloadTime;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetZoom()const
	{
		return Zoom;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxAmmo()const
	{
		return MaxAmmo;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMagazineSize()const
	{
		return MagazineSize;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRemainingAmmo()const
	{
		return RemainingAmmo;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRemainingMagazine()const
	{
		return RemainingMagazine;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnFired();

private:
	void DoFire();

	bool DoFire_Auto();

	bool DoFire_SemiAuto();

	bool DoFire_Charge();

	void DoReload();

protected:
	EWeaponTriggerType TriggerType;

	float FireRate;
	float Accuracy;
	float ReloadTime;
	float Zoom;
	float Damage;
	int32 MaxAmmo;
	int32 MagazineSize;
	int32 RemainingAmmo;
	int32 RemainingMagazine;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	bool bIsReloading;
};
