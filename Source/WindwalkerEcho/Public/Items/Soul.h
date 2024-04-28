// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class WINDWALKERECHO_API ASoul : public AItem
{
	GENERATED_BODY()

protected:
virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;//this is the callback that we are binding to the OnComponentBeginOverlap Delegate with same input types

private:
	UPROPERTY(EditAnywhere,Category="SoulProperties")
	int32 Souls;

public:
	FORCEINLINE int32 GetSouls() const { return Souls; }

	FORCEINLINE void SetSouls(int32 NumberOfSouls )  {  Souls = NumberOfSouls; }
};
