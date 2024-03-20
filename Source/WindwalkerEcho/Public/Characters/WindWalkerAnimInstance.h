// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "WindWalkerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WINDWALKERECHO_API UWindWalkerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float Deltatime) override;


	UPROPERTY(BlueprintReadOnly)
		class AWindWalkerCharacter* WindWalker;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* WindWalkerCharacterMovement;

	UPROPERTY(BlueprintReadOnly,Category=Movement)
		float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Charcter State")
	ECharacterState CharacterState;
};
