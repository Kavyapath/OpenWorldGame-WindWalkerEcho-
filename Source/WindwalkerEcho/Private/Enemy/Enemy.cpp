// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Characters/WindWalkerCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Soul.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);


	

	HealthBarWidget= CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("Health"));
	HealthBarWidget->SetupAttachment(GetRootComponent());


	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	


}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Enemy"));
	if (PawnSensing) {
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
	InitializeEnemy();

}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());//We are accessing the AI controller by casting the controller in to the ai contoller , and from that we are accessing the MoveTo  which return and take parameters as struct which has several members and functions

	HideHealthBar();
	MoveToTarget(PatrolTarget);
	SpawnDefaultWeapon();
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}



void AEnemy::StartDodgeTimer()
{
	EnemyState = EEnemyState::EES_Dodging;
	const float DodgeTime = FMath::RandRange(0.1f, 0.2f);
	GetWorldTimerManager().SetTimer(DodgeTimer, this, &AEnemy::Dodge, DodgeTime);
}

void AEnemy::Dodge()
{
	if (CombatTarget == nullptr) { return; }
	EnemyState = EEnemyState::EES_Engaged;
	int Selection=PlayRandomMontageSections(DashDodgeMontage,DodgeMontageSections);

}

void AEnemy::ClearDodgeTimer()
{
	GetWorldTimerManager().ClearTimer(DodgeTimer);
}

void AEnemy::StopDodgeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, DashDodgeMontage);
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{

	Super::GetHit_Implementation(ImpactPoint,Hitter);
	//DRAW_SPHERE_COLOR(ImpactPoint,FColor::Blue);

	//Showing Health Bar widget here
	if(!IsDead())ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponcollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
	

}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) {//if our enemy state is greater than patrolling means attacking or chasing the we are going to check the combat target
		CheckCombatTarget();
	}
	else {
		CheckPatrolTarget();
	}
	
	


	
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();//Choosing Random  Patrol Target
		//MoveToTarget(PatrolTarget);
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);

	}
}

void AEnemy::CheckCombatTarget()
{

	if (IsOutsideCombatRadius()) {

		//outside of combat radius looses intrest
		ClearAttackTimer();//so that when character in not in the enemy range enemy does not attack unnecessarily
		ClearDodgeTimer();
		LoseInterest();
		if (!IsEngaged()) {
			StartPatrolling();
		}
		
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		//outside attack range , chase character
		ClearAttackTimer();
		ClearDodgeTimer();
		if (!IsEngaged()) {
			ChaseTarget();
		}
		
	}
	else if (CanOneHandedSwordAttack())
	{
		//Player is inside the attack radius
		
		
	
		StartAttackTimer();
		
		
	}
}



//this function will automatically called when apply function is called in any weapon class
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DashDodgeMontage) {
		StartDodgeTimer();
	}

	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();//which ever player is causing damage we are setting it to the combat target

	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius()) {
		ClearDodgeTimer();
		ChaseTarget();
		
	}
	
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon) {
		EquippedWeapon->Destroy();
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}



void AEnemy::HideHealthBar()
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;

	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);

}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::CanOneHandedSwordAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;

}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attribute && HealthBarWidget) {

		
		HealthBarWidget->SetHealthBarPercentage(Attribute->GetHealthPercent());

	}
}


void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::Die()
{
	Super::Die();
	EnemyState = EEnemyState::EES_Dead;
	
	HideHealthBar();
	ClearAttackTimer();
	ClearDodgeTimer();
	DisableCapsule();
	SetLifeSpan(SetDeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponcollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnSoul();
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attribute) {
	
		ASoul* SpawnSoul=World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnSoul)
		{
			SpawnSoul->SetSouls(Attribute->GetSoul());
		}
	}
}

bool AEnemy::InTargetRange(AActor* Target,double Radius)
{
	if (Target == nullptr) {
		return false;
	}
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{

	if (EnemyController == nullptr || Target == nullptr) {
		return;
	}
	

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);//Setting the patrol target
		MoveRequest.SetAcceptanceRadius(50.f);

		//FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest);//return FStruct, EnemyController->MoveTo(MoveRequest, &NavPath)
		/*
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();//it returns the TArray of FnavPath points an this is what we require for our enemy to patrol at multiple targets locations
		for (auto& Points : PathPoints) {
			//FNavPathPoints is a struct which has its member and functions
			const FVector Location = Points.Location;
			DRAW_SPHERE_COLOR(Location, FColor::Green);
		}
		*/


	

}

AActor* AEnemy::ChoosePatrolTarget()
{
	
	TArray<AActor*> ValidTargets;// as tick function is calling perpetually so we get the posibility that every time we get the same patrol target when tick is called so we are making another array in which we are storing the elements which are not equal to our current patrol target
	for (AActor* Target : PatrolTargets) {

		if (Target != PatrolTarget) {
			ValidTargets.AddUnique(Target);//it has one less than size of PatrolTargets
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0) {
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
		

		

	}

	return nullptr;
}



void AEnemy::PawnSeen(APawn* SeenPawn)
{
	//AWindWalkerCharacter* WindWalker = Cast<AWindWalkerCharacter>(SeenPawn);  casting will be a more expensive operation every time we are casting
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("WindWalker"));


	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearDodgeTimer();
		ClearPatrolTimer();
		ChaseTarget();
	}

	/*
	if (EnemyState == EEnemyState::EES_Chasing) return;


	if (SeenPawn->ActorHasTag(FName("WindWalker")))
	{
	
		
		//Now clear the timer of Enemy patrolling function
		
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	

		//if enemy is attacking we do not want our enemy to go in to chasing state as the onPawnsee function is calling itselg on every frame
		if (EnemyState != EEnemyState::EES_Attacking) {

			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
			
		}
		
		
	}
	*/
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) { return; }
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();

}

