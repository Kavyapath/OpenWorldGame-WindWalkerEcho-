// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WindWalkerHUD.h"
#include "HUD/WindWalkerOverlay.h"


void AWindWalkerHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController=World->GetFirstPlayerController();
		if (PlayerController && WindWalkerOverlayClass) {
			 WindWalkerOverlay=CreateWidget<UWindWalkerOverlay>(PlayerController, WindWalkerOverlayClass);
			WindWalkerOverlay->AddToViewport();
		}
	}
	
}