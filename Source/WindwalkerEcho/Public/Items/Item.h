// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class WINDWALKERECHO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AItem();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	template<typename T>
	T Avg(T First, T Second);

	UFUNCTION(BlueprintPure)
		float TransformedSin();

	UFUNCTION(BlueprintPure)
		float TransformedCos();

	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherActorComponent,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);//this is the callback that we are binding to the OnComponentBeginOverlap Delegate with same input types
	
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;
private:
	float RunningTime;
	float Amplitude = 0.50f;
	float SpeedUpToSin = 10.f;



};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
