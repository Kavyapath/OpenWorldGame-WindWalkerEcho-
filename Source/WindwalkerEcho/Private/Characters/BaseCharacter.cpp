// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"
#include"WindwalkerEcho/DebugMacros.h"


ABaseCharacter::ABaseCharacter()
{
 
	PrimaryActorTick.bCanEverTick = true;
	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
			
	}
}



bool ABaseCharacter::CanOneHandedSwordAttack()
{
	return false;
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticle && GetWorld()) {

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint

		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attribute ) {

		Attribute->ReceiveDamage(DamageAmount);
		
	}
}
int32 ABaseCharacter::PlayAttackMontage()
{
	 return PlayRandomMontageSections(AttackMontage, AttackMontageSections);

}

int32 ABaseCharacter::PlayRandomMontageSections(UAnimMontage* Montage, const TArray<FName>& SectionName)
{
	if (SectionName.Num() <= 0) return -1;
	const int32 MaxSelectionIndex = SectionName.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSelectionIndex);
	PlayMontageSection(Montage, SectionName[Selection]);

	return Selection;
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSections(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection); //wraper of Enum that can take ENum values as Constructor convert Enum value in to bytes of integer
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
	
	
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f,AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget(AActor* CombatTarget1)
{
	if(CombatTarget1==nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget1->GetActorLocation();
	const FVector Location = GetActorLocation();

	 FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe*= WarpTargetDistance;
	
	return CombatTargetLocation + TargetToMe;

}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	
	}
}

bool ABaseCharacter::IsAlive()
{
	return Attribute && Attribute->IsAlive();
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::DirectionalHit(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();//it is already normalize
	const FVector ImpactPointLower(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPointLower - GetActorLocation()).GetSafeNormal();//to get the normalize vector whose magnitude is 1

																				  //Dot Product
																				  //ForwardVector.ToHit=|ForwardVector|.|ToHit|*cosin(theta);
																				  //|ForwardVector|=1,|ToHit|=1;
																				  //ForwardVector.ToHit=Cosin(theta)



	const double CosTheta = FVector::DotProduct(ForwardVector, ToHit);
	double Theta = FMath::Acos(CosTheta);   //ArcCos is inverse of Cosin(inverse trignomatry function)
											//we get angle in radian so we have to convert it

	Theta = FMath::RadiansToDegrees(Theta);

	//if cross product points down angle should be negative, if it points upward angle(theta)  is positive 
	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	//but in unreal engine 5 instead of folling right hand rule for finding direction we use left hand rule so it will negate the effect of right hand rule 
	if (CrossProduct.Z < 0) {
		Theta *= -1.f;//that's why we multiply here by negative one
	}

	FName SectionName("Back");
	if (Theta >= -45.f && Theta < 45.f) {
		SectionName = FName("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f) {
		SectionName = FName("Left");

	}
	else if (Theta >= 45.f && Theta < 135.f) {
		SectionName = FName("Right");

	}

	PlayHitReactMontages(SectionName);
	/*
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta is :%f"), Theta));
		}
		//Drawing Debug Arrow
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60, 5.f, FColor::Red, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60, 5.f, FColor::Green, 5.f);
		*/
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	
	if (IsAlive() && Hitter) {

		if (HitCameraShakeClass) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
		DirectionalHit(Hitter->GetActorLocation());
	}
	else {
		//playing the death montages
		if (DeathCameraShakeClass) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
		}
		Die();
	}

	PlayHitSound(ImpactPoint);


	SpawnHitParticles(ImpactPoint);
	SetWeaponcollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ABaseCharacter::DisAbleMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitReactMontages(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

FVector ABaseCharacter::GetRotationWarpTarget(AActor* CombatTarget1)
{
	if (CombatTarget1) {
		return CombatTarget1->GetActorLocation();
	}
	return FVector();

}

void ABaseCharacter::AttackEnd()
{
}


void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponcollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{

	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
	if (EquippedWeaponLeft && EquippedWeaponLeft->GetWeaponBox()) {
		EquippedWeaponLeft->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeaponLeft->IgnoreActors.Empty();
	}

}



