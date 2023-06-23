// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDataAsset.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;
	
public:	
	UFUNCTION(BlueprintCallable)
	void AddItem(UInventoryItemDataAsset* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveSelectedItem();
	UFUNCTION(BlueprintCallable)
	void SwitchItem(float Mouse_AxisValue);
	
private:
	UPROPERTY(VisibleAnywhere, Category=Inventory)
	TArray<UInventoryItemDataAsset*> Items;
	UPROPERTY(VisibleAnywhere, Category=Inventory)
	int SelectedItem_Index;
};
