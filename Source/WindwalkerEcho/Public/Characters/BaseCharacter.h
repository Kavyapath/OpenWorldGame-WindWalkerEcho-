// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hit_Interface.h"
#include"Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
class IHit_Interface;

UCLASS()
class WINDWALKERECHO_API ABaseCharacter : public ACharacter, public  IHit_Interface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

private:

	


protected:
	void PlayMontageSection(UAnimMontage* Monatge, const FName& SectionName);
	virtual void BeginPlay() override;
	int32 PlayRandomMontageSections(UAnimMontage* Monatge, const TArray<FName>& SectionName);
	virtual void Attack();
	virtual void Die();
	void DirectionalHit(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	bool IsAlive();
	virtual bool CanOneHandedSwordAttack();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void DisAbleMeshCollision();
	
/*
playing animation montages
*/
	void PlayHitReactMontages(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget(AActor* CombatTarget1);

	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget(AActor* CombatTarget1);

	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void SetWeaponcollisionEnabled(ECollisionEnabled::Type CollisionEnabled);


	UPROPERTY(VisibleAnywhere , Category=Weapon)
		AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeaponLeft;

	UPROPERTY(BlueprintReadWrite, Category = Combat)
		AActor* CombatTarget;


	UPROPERTY(EditAnywhere,Category = Combat)
	double WarpTargetDistance = 50.f;
	
	UPROPERTY(VisibleAnywhere)
		UAttributeComponent* Attribute;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

private:

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		UParticleSystem* HitParticle;

	/*
*  animation montages
*/
	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* HitReactMontage;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }


};
