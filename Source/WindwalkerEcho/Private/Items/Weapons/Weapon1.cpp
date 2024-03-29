// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon1.h"
#include "Characters/WindWalkerCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AWeapon1::AWeapon1()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox Collision"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	StartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	StartTrace->SetupAttachment(GetRootComponent());
	EndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	EndTrace->SetupAttachment(GetRootComponent());
}

void AWeapon1::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon1::OnBoxOverlap);
}


void AWeapon1::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon1::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);//they directly called or initialzie using Structs constructor
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this,
			EquipSound,
			GetActorLocation());

	}
}
void AWeapon1::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex, bFromSweep, SweepResult);

}
void AWeapon1::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex);
}

void AWeapon1::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = StartTrace->GetComponentLocation();//world space location
	const FVector End = EndTrace->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;


	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		StartTrace->GetComponentRotation(),//Rotation of start trace
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
	);

}
