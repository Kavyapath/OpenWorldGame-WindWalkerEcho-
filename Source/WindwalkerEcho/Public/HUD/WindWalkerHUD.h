// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WindWalkerHUD.generated.h"

/**
 * 
 */
class UWindWalkerOverlay;

UCLASS()
class WINDWALKERECHO_API AWindWalkerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly,Category=WindWalker)
	TSubclassOf<UWindWalkerOverlay> WindWalkerOverlayClass;
	UPROPERTY()
	 UWindWalkerOverlay* WindWalkerOverlay;

public:
	FORCEINLINE UWindWalkerOverlay* GetWindWalkerOverlay() const { return  WindWalkerOverlay; }
};
