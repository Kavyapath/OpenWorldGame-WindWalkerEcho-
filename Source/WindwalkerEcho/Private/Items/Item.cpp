// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "WindwalkerEcho/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent= ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effect"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	/*SetActorLocation(FVector(0.f, 0.f, 50.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));
	FVector Location = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	UWorld* World = GetWorld();


	DRAW_SPHERE(Location);
	//DRAW_LINE(Location, Location + ForwardVector * 100.f);
	//DRAW_POINT(Location + ForwardVector * 100.f);
	DRAW_VECTOR(Location, Location + ForwardVector * 100.f)*/

	//now ew have to bind the callback to the OnComponentbeginoverlap Delegate
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AItem::SpawnPickUpEffect()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickUpEffect,
			GetActorLocation());

	}
}

void AItem::SpawnPickUpSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickUpSound,
			GetActorLocation()
		);
	}
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * SpeedUpToSin);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * SpeedUpToSin);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);//casting parent to child class pointer,casting the other actor in to the interface so that other actor can call its interface overriden function
	
	if (PickUpInterface) {
		//when sphere overlap begin we make this item as the overlapping item and we have access of overlapping item pointer in the windwalker character by include item ,weapon,weapon1, header and we made a public function called equip in the weapon class to equip item so we will call that fuction from windwalker class to equip weapon and easily pass the windwalkerMesh and socket name;
		PickUpInterface->SetOverlappingItem(this);//reference of this class pointer,it will call the other actor SetOverlappingItem function
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex)
{

	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);//casting parent to child class pointer,casting the other actor in to the interface so that other actor can call its interface overriden function

	if (PickUpInterface) {
		//when sphere overlap begin we make this item as the overlapping item and we have access of overlapping item pointer in the windwalker character by include item ,weapon,weapon1, header and we made a public function called equip in the weapon class to equip item so we will call that fuction from windwalker class to equip weapon and easily pass the windwalkerMesh and socket name;
		PickUpInterface->SetOverlappingItem(nullptr);//reference of this class pointer,it will call the other actor SetOverlappingItem function
	}
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Movement rate in units of 50cm/s
	//MovementRate*DeltaTime (cm/sec)*(sec/frame)=(cm/frame)
/*
	float MovementRate = 50.f;
	float RotationRate = 45.f;


	AddActorWorldOffset(FVector(MovementRate*DeltaTime,0.f,0.f));
	AddActorWorldRotation(FRotator(0.f, RotationRate*DeltaTime,0.f));*/
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering) {
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	





}

