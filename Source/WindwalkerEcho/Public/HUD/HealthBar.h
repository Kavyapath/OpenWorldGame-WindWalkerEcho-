// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class WINDWALKERECHO_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta =(BindWidget)) //this is how we bind the c++ variable to the unreal widget and their name must have to match its madetory
	class UProgressBar* HealthBar;
};
