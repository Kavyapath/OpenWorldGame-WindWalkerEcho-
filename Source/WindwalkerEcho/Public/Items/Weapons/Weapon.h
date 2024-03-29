// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

class USoundBase;
class UBoxComponent;
class USceneComponent;

UCLASS()
class WINDWALKERECHO_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void Equip(USceneComponent* InParent,FName InSocketName);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);



protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;//this is the callback that we are binding to the OnComponentBeginOverlap Delegate with same input types
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



private:
	UPROPERTY(EditAnywhere , Category = "Weapon Sound")
		USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* StartTrace;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* EndTrace;

	

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	TArray<AActor*> IgnoreActors;

};
