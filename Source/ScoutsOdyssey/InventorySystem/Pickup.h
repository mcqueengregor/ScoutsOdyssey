// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SCOUTSODYSSEY_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline bool getIsPickupInstant() { return bIsPickupInstant; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void InstantPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurrentItem ItemType;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPickupInstant;
};
