// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class WINDWALKERECHO_API ATreasure : public AItem
{
	GENERATED_BODY()


protected:

		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;//this is the callback that we are binding to the OnComponentBeginOverlap Delegate with same input types

	private:



		UPROPERTY(EditAnywhere, Category = "Treasure Porperties")
			int32 Gold;

public:
	FORCEINLINE int32 GetGold() const {return Gold;}
};
