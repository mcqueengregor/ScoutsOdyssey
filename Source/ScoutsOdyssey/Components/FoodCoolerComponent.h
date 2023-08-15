// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "FoodCoolerComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class SCOUTSODYSSEY_API UFoodCoolerComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UFoodCoolerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

protected:
	void RemoveMarshmallowsFromCooler();
	
	UPROPERTY(EditDefaultsOnly)
	UInventoryItemDataAsset* MarshmallowDA;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CoolerNoMarshmallowsTex;
};
