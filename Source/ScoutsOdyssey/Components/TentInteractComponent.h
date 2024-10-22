// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "TentInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTentBuilt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailToInteract);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayStartToMiddleAudio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayMiddleToEndAudio);

UENUM()
enum class ETentState : uint8
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
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETentState CurrentState;

	UPROPERTY(EditDefaultsOnly)
	TMap<ETentState, FTentDetails> TentStateTextures;

	UPROPERTY(EditAnywhere, Category=Tent)
	int RequiredNumberOfTents = 4;
	
	static int NumberOfTents;
	
	UPROPERTY(BlueprintAssignable)
	FOnAllTentBuilt OnAllTentBuilt;

	UPROPERTY(BlueprintAssignable)
	FOnFailToInteract OnFailToInteract;

protected:
	UPROPERTY(BlueprintAssignable)
	FPlayStartToMiddleAudio PlayStartToMiddleAudio;

	UPROPERTY(BlueprintAssignable)
	FPlayMiddleToEndAudio PlayMiddleToEndAudio;
	
private:
	FTimerHandle InteractionTimerHandle;
	
	FVector OriginalLocation;
	float OriginalScaleMultiplier;
};
