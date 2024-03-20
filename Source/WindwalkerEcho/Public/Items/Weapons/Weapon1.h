// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon1.generated.h"

/**
 * 
 */
UCLASS()
class WINDWALKERECHO_API AWeapon1 : public AItem
{
	GENERATED_BODY()

public:
	void Equip(USceneComponent* InParent, FName InSocketName);
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;//this is the callback that we are binding to the OnComponentBeginOverlap Delegate with same input types
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex) override;
};
