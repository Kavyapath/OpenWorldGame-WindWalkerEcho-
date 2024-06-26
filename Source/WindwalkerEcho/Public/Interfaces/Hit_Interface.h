// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hit_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHit_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WINDWALKERECHO_API IHit_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint,AActor* Hitter); //making function blueprintNativeEvent  will automatically generate its virtual function type which is implemtable in c++ and to use that we have to sufix the functions name as _Implementation 
};
