// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "Interfaces/PickUpInterface.h"
#include "CharacterTypes.h"
#include "WindWalkerCharacter.generated.h"





class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;



UCLASS()
class WINDWALKERECHO_API AWindWalkerCharacter : public ABaseCharacter,public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWindWalkerCharacter();
	//virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//this function already exist in actor class you can peek its defination
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem( AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void Jump() override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grooming)
		UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grooming)
		UGroomComponent* EyeBrow;

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
		UInputAction* DualAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* WeaponUnEquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DashDodgeFrontAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DashDodgeBackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DashDodgeLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* DashDodgeRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* BlockStartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* BlockEndAction;

private:
	bool IsUnOccupied();
	void SphereTrace(FHitResult& OutHitResult);
	/* Callback for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void QKeyPressed();
	void Sprint();
	void StopSprinting();
	void StartBlock();
	void EndBlock();

	/*Combat*/
	virtual void Attack() override;
	void DualBladeAttack();
	void DashDodgeFront();
	void DashDodgeBack();
	void DashDodgeLeft();
	void DashDodgeRight();
	virtual void Die() override;

	virtual bool CanOneHandedSwordAttack() override;
	int32 PlayAttackMontageTwoHandedSword();
	bool CanTwoHandedSwordAttack();
	bool CanDisArm();
	bool CanArm();
	void Arm();
	void DisArm();
	void PlayEquipMontage(FName SectionName);
	void EquipWeapon(AWeapon* Weapon);
	void EquipWeaponLeft(AWeapon* Weapon);


	/*Overlays and HUD */
	void InitializeOverlay(APlayerController* PlayerController);
	void SetHUDHealth();
	
	virtual void AttackEnd() override;
	UFUNCTION(BlueprintCallable)
		void HitReactEnd();

	UFUNCTION(BlueprintCallable)
		void DashDodgeEnd();

	

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
		void FinishEquipping();

	UFUNCTION(BlueprintCallable)
		void CustomCameraSetting(float SpringArmLength, float Pitch);

	UFUNCTION(BlueprintCallable)
		void DefaultCameraSetting();

	UFUNCTION(BlueprintCallable)
		void CustomCameraSettingForAnimations();

	UFUNCTION(BlueprintCallable)
		void CustomCameraSettingForDualBladeAnimations();




	/* Character Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
		float RotationRate = 40;

	UPROPERTY(EditAnywhere)
		bool IsSprinting ;

	UPROPERTY(EditAnywhere)
		bool RunToStop;

	UPROPERTY(EditAnywhere)
		bool IsBlocking;

	UPROPERTY(EditAnywhere)
		bool BlockToStop;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	UPROPERTY(EditAnywhere)
		float SprintSpeed = 1000;

	UPROPERTY(EditAnywhere)
		float OriginalSpeed;

	UPROPERTY()
		class UWindWalkerOverlay* WindWalkerOverlay;



	UPROPERTY(EditAnywhere,Category =Combat)
	TArray<FName> DualAttackMontageSections;


	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_UnOccupied;
	/*
	* Animations Montage
	*/

	

	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* DualBladeMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* DashDodgeMontage;


public:
	

	 FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	 FORCEINLINE EActionState GetActionState() const { return ActionState; }

	 FORCEINLINE bool GetIsSprinting() {
		 return IsSprinting;
	 }
	 FORCEINLINE bool GetRunToStop() {
		 return RunToStop;
	 }
	 FORCEINLINE bool GetIsBlocking() {
		 return IsBlocking;
	 }
	 FORCEINLINE bool GetBlockToStop() {
		 return BlockToStop;
	 }

};
