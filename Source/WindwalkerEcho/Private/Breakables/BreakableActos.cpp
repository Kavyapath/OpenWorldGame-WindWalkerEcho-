// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakables/BreakableActos.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Containers/Array.h"

// Sets default values
ABreakableActos::ABreakableActos()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ABreakableActos::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	if (bBroken) {
		return;
	}
	Tags.Remove(FName("Breakables"));
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0) {
		FVector Location = GetActorLocation()+120.f;
		Location.Z += 100.f;
		const int32 selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		 if (selection >= 0 && selection < TreasureClasses.Num()) {
			 World->SpawnActor<ATreasure>(TreasureClasses[selection], Location, GetActorRotation());
		 }

		
	}
}

// Called when the game starts or when spawned
void ABreakableActos::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Breakables"));
	
}

// Called every frame
void ABreakableActos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

