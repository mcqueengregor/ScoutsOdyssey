// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "TrunkInteractComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UTrunkInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UTrunkInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	class AAcornProp* AcornPropActor;
	
private:
	UPROPERTY(EditInstanceOnly)
	TArray<class AAISquirrelActor*> SquirrelActors;

	FGameplayTag HoneyBootItemTag;

	bool bAreSquirrelsPresent;

};
