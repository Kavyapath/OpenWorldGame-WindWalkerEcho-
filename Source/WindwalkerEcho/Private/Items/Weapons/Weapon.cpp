// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/WindWalkerCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Hit_Interface.h"


AWeapon::AWeapon()
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

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{

	
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);//they directly called or initialzie using Structs constructor
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this,
			EquipSound,
			GetActorLocation());

	}
}



void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex, bFromSweep, SweepResult);

}
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = StartTrace->GetComponentLocation();//world space location
	const FVector End = EndTrace->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}
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
	if (BoxHit.GetActor()) {
		IHit_Interface* HitInterface = Cast<IHit_Interface>(BoxHit.GetActor());//whatevr actor we hit we have to cast it in to Hit_Interface we are hitting the enemy, so the enemy actor is casted in to the hit interface so that we can call the getHit of enemy class which is inherited from the hit interface
		if (HitInterface) {
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
	}

}

