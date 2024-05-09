// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"


void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);//casting parent to child class pointer,casting the other actor in to the interface so that other actor can call its interface overriden function

	if (PickUpInterface) {
		//when sphere overlap begin we make this item as the overlapping item and we have access of overlapping item pointer in the windwalker character by include item ,weapon,weapon1, header and we made a public function called equip in the weapon class to equip item so we will call that fuction from windwalker class to equip weapon and easily pass the windwalkerMesh and socket name;
		PickUpInterface->AddSouls(this);//reference of this class pointer,it will call the other actor SetOverlappingItem function
		SpawnPickUpEffect();
		SpawnPickUpSound();
		Destroy();
	
	}
	
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsTypes;
	ObjectsTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(this,
		Start,
		End,
		ObjectsTypes,
		false, 
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);
	DesiredZ=HitResult.ImpactPoint.Z + 60.f ;
}


void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const double LocationZ = GetActorLocation().Z;
	if (DesiredZ < LocationZ) {
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate*DeltaTime);
		AddActorWorldOffset(DeltaLocation);
	}
}