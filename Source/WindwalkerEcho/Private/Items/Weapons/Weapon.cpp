// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/WindWalkerCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Hit_Interface.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{


	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SowrdMesh"));
	SwordMesh->SetupAttachment(RootComponent);

	SwordMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableCollision();
	DeactivateCollision();
}

void AWeapon::DisableCollision()
{
	if (Sphere) {
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::DeactivateCollision()
{
	if (ItemEffect) {
		ItemEffect->Deactivate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{

	
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);//they directly called or initialzie using Structs constructor
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	EquippingSound();
}

void AWeapon::EquippingSound()
{
	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this,
			EquipSound,
			GetActorLocation());

	}
}





void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorsIsSameType(OtherActor)) {
		return;
	}
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	if (BoxHit.GetActor()) {
		if (ActorsIsSameType(BoxHit.GetActor())) {
			return;
		}
		
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),// using GetInstigator() we will get the pawn owner of this weapon and from the pawn we can access its controller using GetController()
			this,
			UDamageType::StaticClass()
		);

		ExecuteGetHit(BoxHit);
		
		CreateFields(BoxHit.ImpactPoint);

	
	}

}

bool AWeapon::ActorsIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHit_Interface* HitInterface = Cast<IHit_Interface>(BoxHit.GetActor()); //whatevr actor we hit we have to cast it in to Hit_Interface we are hitting the enemy, so the enemy actor is casted in to the hit interface so that we can call the getHit of enemy class which is inherited from the hit interface
	if (HitInterface) {

		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint,GetOwner()); //you have to pass the hit actor so that it call call its overriden get hit
																			//HitInterface->GetHit(BoxHit.ImpactPoint);
		if (HitVisualEffect && GetWorld()) {

			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitVisualEffect,
				BoxHit.ImpactPoint

			);
		}

	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = StartTrace->GetComponentLocation();//world space location
	const FVector End = EndTrace->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}



	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		StartTrace->GetComponentRotation(),//Rotation of start trace
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebug? EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

