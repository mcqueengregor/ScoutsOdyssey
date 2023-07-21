// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Player/PlayerPawn.h"
#include "ScoutsOdyssey/InventorySystem/InventoryItemDataAsset.h"
#include "InteractComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract)
class SCOUTSODYSSEY_API UInteractComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
	PURE_VIRTUAL(UInteractComponentBase::OnInteractWithItem, return ECurrentInteraction::NO_INTERACTION;);
	
	UFUNCTION(BlueprintCallable)
	virtual void DoTask() PURE_VIRTUAL(UInteractComponentBase::DoTask,);

protected:
	UMaterialInstanceDynamic* DynamicMaterial;	// Dynamic material instance applied to owner mesh, used to change the
												// tent texture to match the tent's state.

	UPROPERTY(EditAnywhere)
	FGameplayTag ValidItemTag;
};
