// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "BushInteractComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UBushInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UBushInteractComponent();
    
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;
    
	virtual void DoTask() override;

	UPROPERTY(EditAnywhere)
	FVector FinalScale;

	UPROPERTY(EditAnywhere)
	float ShrunkLocationOffsetZ;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsShrinking;
	
private:
	FVector OriginalScale;
	FVector OriginalLocation;
	float OriginalLocationZ;
	float ShrinkDuration;
	float LerpT;
};