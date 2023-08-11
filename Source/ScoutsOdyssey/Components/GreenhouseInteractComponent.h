// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "GreenhouseInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGreenHouseUnLocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayGreenhouseChangeAudio);

UENUM()
enum class EGreenhouseState : uint8
{
	LOCKED = 0	UMETA(DisplayName = "Locked"),
	OLD = 1		UMETA(DisplayName = "Old"),
	OPEN = 2	UMETA(DisplayName = "Open"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UGreenhouseInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UGreenhouseInteractComponent();
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

	//Enable bees to move to greenhouse
	UPROPERTY(BlueprintAssignable)
	FOnGreenHouseUnLocked OnGreenHouseUnLocked;

protected:
	UPROPERTY(BlueprintAssignable)
	FPlayGreenhouseChangeAudio PlayGreenhouseAudioDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGreenhouseState CurrentState;

	UPROPERTY(EditDefaultsOnly)
	TMap<EGreenhouseState, UTexture2D*> GreenhouseStateTextures;

	UPROPERTY(EditAnywhere)
	AActor* SmokePropActorRef;

	UStaticMeshComponent* SmokePropPlaneMesh;
	
	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* SmokeAnimDynamicMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	USpriteAnimationDataAsset* SmokeAnimDataAsset;
	
	FTimerHandle SwitchToOldHandle;

private:
	void SwitchToOldGreenhouseSprite();
	
	float SmokeLocalAnimTime;
	bool bIsPlayingSmokeAnim;
};
