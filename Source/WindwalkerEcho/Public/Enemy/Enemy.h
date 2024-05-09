// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"



class UHealthBarWidgetComponent;
class UPawnSensingComponent;
UCLASS()
class WINDWALKERECHO_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	/* Actor */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//this function already exist in actor class you can peek its defination
	virtual void Destroyed() override;
	/* Actor */

	/* IHitInterface */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* IHitInterface */


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
private:
	/* AI Behavior*/
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void SpawnDefaultWeapon();

	void StartDodgeTimer();
	void Dodge();
	FTimerHandle DodgeTimer;
	void ClearDodgeTimer();
	void StopDodgeMontage();
	/*
AIBehavior
*/
	void InitializeEnemy();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	bool IsOutsideCombatRadius();
	void ChaseTarget();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsDead();
	bool IsEngaged();
	bool IsAttacking();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void StartAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();



	UFUNCTION()//which which are bind to the delegates must be UFUNCTION() //callback for OnPawnSeen
		void PawnSeen(APawn* SeenPawn);

	UPROPERTY(EditAnyWhere)
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere)
		UHealthBarWidgetComponent* HealthBarWidget;



	UPROPERTY(EditAnywhere)
		double CombatRadius=700.f;

	UPROPERTY(EditAnywhere)
		double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
		double AcceptanceRadius = 50.f;
	

	UPROPERTY()
		class AAIController* EnemyController;
	/*
	Navigations
	*/

	//Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditAnywhere)
	double PatrolRadius=200.f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;


	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	/*
	Components
	*/
	UPROPERTY(EditAnywhere)
		UPawnSensingComponent* PawnSensing;


	/*
	Combat
	*/
	

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere,Category=Combat)
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float SetDeathLifeSpan = 3.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;

	UPROPERTY(EditDefaultsOnly, Category = Animations_Montages)
		UAnimMontage* DashDodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DodgeMontageSections;

protected:


	/*ABase Character*/
	virtual void Die() override;
	void SpawnSoul();
	virtual void Attack() override;
	virtual bool CanOneHandedSwordAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	/*ABase Character*/

	



	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;


};
