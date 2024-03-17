// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "WindwalkerEcho/DebugMacros.h"



// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMEshComponent"));
	RootComponent = ItemMesh;

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


}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * SpeedUpToSin);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * SpeedUpToSin);
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





}

