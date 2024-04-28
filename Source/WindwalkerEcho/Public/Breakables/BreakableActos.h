// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hit_Interface.h"
#include "BreakableActos.generated.h"

class UGeometryCollectionComponent;
class IHit_Interface;
UCLASS()
class WINDWALKERECHO_API ABreakableActos : public AActor, public IHit_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActos();

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		class UCapsuleComponent* Capsule;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


	UPROPERTY(EditAnywhere,Category=Treasure)
		TArray<TSubclassOf<class ATreasure>> TreasureClasses; //act as TSubClass Accept blueprint child classes also
	bool bBroken = false;
	
};
