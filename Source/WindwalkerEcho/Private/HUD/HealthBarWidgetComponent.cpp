// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidgetComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"


void UHealthBarWidgetComponent::SetHealthBarPercentage(float Percent) {

	//we all calling this function we ever we do damage to the enemy and we don't want to cast it multiple times so we check if it is null we will cast 
	if (HealthBarUserWidget==nullptr) {
		HealthBarUserWidget = Cast<UHealthBar>(GetUserWidgetObject());//GetUserWidgetObject() this function return the parent user widget class which is use by the hud in our case we are using HealthBar as parent class

	}
	if (HealthBarUserWidget && HealthBarUserWidget->HealthBar) {
		HealthBarUserWidget->HealthBar->SetPercent(Percent);//From our Actor health bar component we are accessing the user widget Progess bar which we name as health bar and from that we are calling the function to set the percentage
	}

}