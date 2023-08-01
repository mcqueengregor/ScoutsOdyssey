// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "TentInteractComponent.h"
#include "BeehiveInteractComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SCOUTSODYSSEY_API UBeehiveInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UBeehiveInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

	UPROPERTY(BlueprintAssignable)
	FOnFailToInteract OnFailToInteract;
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bAreBeesPresent;
	
private:
	// TODO: Add bee actors here:
	FGameplayTag HoneyBootTag;

	UInventoryItemDataAsset* HoneyBootDA;
};
