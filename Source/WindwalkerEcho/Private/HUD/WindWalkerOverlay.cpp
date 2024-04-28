// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WindWalkerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWindWalkerOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}
void UWindWalkerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UWindWalkerOverlay::SetGold(int32 Gold)
{
	if (GoldText) {

		const FString GoldString = FString::Printf(TEXT("%d"), Gold);
		const FText GoldTextValue = FText::FromString(GoldString);

		GoldText->SetText(GoldTextValue);
	}


}

void UWindWalkerOverlay::SetSoul(int32 Soul)
{
	if (SoulText) {
		const FString SoulString = FString::Printf(TEXT("%d"), Soul);
		const FText SoulTextValue = FText::FromString(SoulString);

		SoulText->SetText(SoulTextValue);
	}

}
