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

USTRUCT()
struct FTentDetails
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* TentStateTexture;
	FVector TextureScale;
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

	UPROPERTY(EditDefaultsOnly)
	TMap<FTentState, FTentDetails> TentStateTextures;

private:
	UMaterialInstanceDynamic* DynamicMaterial;	// Dynamic material instance applied to owner mesh, used to change the
												// tent texture to match the tent's state.

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ValidItemTag;

	FVector OriginalLocation;
};