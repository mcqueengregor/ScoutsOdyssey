// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "TentInteractComponent.generated.h"

UENUM()
enum class FTentState : uint8
{
	START = 0	UMETA(DisplayName = "Tent not put up"),
	MIDDLE = 1	UMETA(DisplayName = "Tent partially put up"),
	END = 2		UMETA(DisplayName = "Tent fully put up"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UTentInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UTentInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTentState CurrentState;
};
