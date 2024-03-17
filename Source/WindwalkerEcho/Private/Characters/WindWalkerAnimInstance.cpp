// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WindWalkerAnimInstance.h"
#include "Characters/WindWalkerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void  UWindWalkerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	WindWalker=Cast<AWindWalkerCharacter>(TryGetPawnOwner()); //cast it in to windwalker parent class pointer to the child class in c++ we have static_cast for compile time and Dynamic_Cast for the run time 
	if (WindWalker) {

		WindWalkerCharacterMovement = WindWalker->GetCharacterMovement();
	}

}
void  UWindWalkerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (WindWalkerCharacterMovement) {
	
		GroundSpeed=UKismetMathLibrary::VSizeXY(WindWalkerCharacterMovement->Velocity) ;//find the magnitude of velocity vector only in XY Direction sqrt(X^2 + Y^2);
		IsFalling = WindWalkerCharacterMovement->IsFalling();
	}
}
