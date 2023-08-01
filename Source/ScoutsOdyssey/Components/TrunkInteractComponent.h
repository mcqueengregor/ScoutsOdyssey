// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "TentInteractComponent.h"
#include "TrunkInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowHoneyBootDelegate);	// Used to set visibility of honey boot plane on
															// owning trunk actor.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAcornThrown);	

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class SCOUTSODYSSEY_API UTrunkInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()

	UTrunkInteractComponent();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual ECurrentInteraction OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef) override;

	virtual void DoTask() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAISquirrelActor* SquirrelActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SquirrelBarrier;

	DECLARE_MULTICAST_DELEGATE(FOnHoneyBootPlaced);
	static FOnHoneyBootPlaced OnHoneyBootPlaced;

	UPROPERTY(BlueprintAssignable)
	FOnFailToInteract OnFailToInteract;

	UPROPERTY(BlueprintAssignable)
	FOnAcornThrown OnAcornThrown;

protected:
	UPROPERTY(EditDefaultsOnly)
	class AAcornProp* AcornPropActor;

	UPROPERTY(EditDefaultsOnly)
	UClass* AcornPropSpawnClass;

	UPROPERTY(BlueprintAssignable)
	FShowHoneyBootDelegate ShowHoneyBoot;

	UPROPERTY(BlueprintReadWrite)
	bool bAreSquirrelsPresent;
	
private:

	FGameplayTag HoneyBootItemTag;
};
