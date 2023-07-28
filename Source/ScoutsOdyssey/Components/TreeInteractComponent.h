// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "ScoutsOdyssey/Animation/CustomSkeletalMeshActor.h"
#include "TreeInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTreeDisappear);

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

	UPROPERTY(BlueprintAssignable)
	FOnTreeDisappear OnTreeDisappear;
	
private:
	UPROPERTY(EditInstanceOnly)
	class AItemSpawner* AcornSpawnerRef;	// Item spawner which creates acorn object for the player to pick up.

	UPROPERTY(EditInstanceOnly)
	AActor* AcornPropRef;		// Actor which only holds a plane showing the acorn sprite, destroyed when tree is
								// interacted with.
	
	ACustomSkeletalMeshActor* OwnerActor;
};
