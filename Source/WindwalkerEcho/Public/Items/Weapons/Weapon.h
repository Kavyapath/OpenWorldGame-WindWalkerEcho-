// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

class USoundBase;
class UBoxComponent;
class USceneComponent;

UCLASS()
class WINDWALKERECHO_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void Equip(USceneComponent* InParent,FName InSocketName,AActor* NewOwner,APawn* NewInstigator);
	void DisableCollision();
	void DeactivateNiagaraEffect();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	void EquippingSound();



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* SwordMesh;
	virtual void BeginPlay() override;

	

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorsIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);

private:


	UPROPERTY(EditAnywhere , Category = "Weapon Sound")
		USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* StartTrace;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* EndTrace;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		UParticleSystem* HitVisualEffect;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		bool bShowDebug=false;



	void BoxTrace(FHitResult& BoxHit);

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	TArray<AActor*> IgnoreActors;

};
