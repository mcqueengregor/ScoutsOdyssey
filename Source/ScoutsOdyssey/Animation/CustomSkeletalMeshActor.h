// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "CustomSkeletalMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API ACustomSkeletalMeshActor : public ASkeletalMeshActor
{
	GENERATED_BODY()

	ACustomSkeletalMeshActor();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Custom animation controls")
	void ToggleAnimationPlayback();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
	
protected:
	virtual void BeginPlay() override;
	
	class UCustomAnimInstance* AnimInstance;

	class UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* OverlapBoxCollider;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BlockingBoxCollider;
};
