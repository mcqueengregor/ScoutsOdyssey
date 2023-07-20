// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "ScoutsOdyssey/Animation/CustomSkeletalMeshActor.h"
#include "TreeInteractComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UTreeInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UTreeInteractComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;
	
private:
	UPROPERTY(EditAnywhere)
	AActor* AcornSpawnerRef;

	ACustomSkeletalMeshActor* OwnerActor;
};
