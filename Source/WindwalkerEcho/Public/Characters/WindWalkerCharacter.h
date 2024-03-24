// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "WindWalkerCharacter.generated.h"





class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;
class AWeapon1;

UCLASS()
class WINDWALKERECHO_API AWindWalkerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWindWalkerCharacter();
	//virtual void Jump() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* WindWalkerContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* WeaponEquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* WeaponUnEquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* OneWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DualWeaponAction;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grooming)
		UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grooming)
		UGroomComponent* EyeBrow;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void QKeyPressed();
	void Sprint();
	void StopSprinting();
	void Attack();
	void PlayAttackMontageOneHandedSword();
	bool CanOneHandedSwordAttack();
	bool CanDisArm();
	bool CanArm();
	void PlayEquipMontage(FName SectionName);
	void OneWeaponCharacterState();
	void DualWeaponCharacterState();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	UFUNCTION(BlueprintCallable)
		void DisArm();

	UFUNCTION(BlueprintCallable)
		void Arm();



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;


	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_UnOccupied;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere)
		float RotationRate = 30;

	UPROPERTY(EditAnywhere)
		bool IsSprinting ;

	UPROPERTY(EditAnywhere)
		bool RunToStop;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly)
		AWeapon* EquippedWeapon;

	UPROPERTY(VisibleInstanceOnly)
		AWeapon1* EquippedWeaponLeft;

	UPROPERTY(EditAnywhere)
		float SprintSpeed = 1300;

	UPROPERTY(EditAnywhere)
		float OriginalSpeed ;




	/*
	* Animations Montage
	*/

	UPROPERTY(EditDefaultsOnly,Category=Animations_Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* EquipMontage;

	


public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) {
		OverlappingItem = Item;
	}

	 FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	 FORCEINLINE bool GetIsSprinting() {
		 return IsSprinting;
	 }
	 FORCEINLINE bool GetRunToStop() {
		 return RunToStop;
	 }
		

};
