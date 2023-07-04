// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
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
	virtual void OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

private:
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void ToggleAnimation();
	
	UPROPERTY(EditAnywhere, Category=Interact)
	bool bPlayOnStart;
	UPROPERTY(EditAnywhere, Category=Interact)
	float AnimationDuration = 1;
	
private:
	bool bAnimFlipFlop;	// When 'true' animation local time will increase from 0 to 1 (i.e. animation will play from
						// start to end, then stop), when 'false' animation local time will go from 1 to 0 (i.e.
						// animation will play from end to start, then stop).

	float LocalAnimTime;	// Value in the range [0,1], representing how far along an animation is on its local
							// timeline, in percentage (e.g. 0.6 = 60% from start to finish).

protected:	
	class UMaterialInstanceDynamic* DynamicAnimMaterial;
};
