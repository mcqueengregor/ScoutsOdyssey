// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSectionVolume.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AStageSectionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageSectionVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* StageSectionCameraComponent;
};
