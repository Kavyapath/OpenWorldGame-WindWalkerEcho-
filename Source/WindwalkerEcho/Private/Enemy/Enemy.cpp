// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "WindwalkerEcho/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);





}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint,FColor::Blue);

	
	DirectionalHit(ImpactPoint);
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,
			HitSound,
			ImpactPoint
			);
	}


}

void AEnemy::DirectionalHit(const FVector& ImpactPoint)
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
	if (CrossProduct.Z<0) {
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
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta is :%f"), Theta));
	}
	//Drawing Debug Arrow
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60, 5.f, FColor::Green, 5.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::PlayHitReactMontages(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

