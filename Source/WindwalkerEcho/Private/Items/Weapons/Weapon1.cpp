// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon1.h"
#include "Characters/WindWalkerCharacter.h"


void AWeapon1::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);//they directly called or initialzie using Structs constructor
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon1::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex, bFromSweep, SweepResult);

}
void AWeapon1::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherActorComponent, OtherBodyIndex);
}

