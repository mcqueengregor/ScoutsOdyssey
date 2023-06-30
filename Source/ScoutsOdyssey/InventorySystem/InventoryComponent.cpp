// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/RepLayout.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UInventoryItemDataAsset>
		EmptyHandDA(TEXT("InventoryItemDataAsset'/Game/Blueprints/Inventory/DataAssetItems/DA_EmptyHand.DA_EmptyHand'"));

	if (EmptyHandDA.Object)
		EmptyHandDataAsset = EmptyHandDA.Object;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SelectedItem_Index = 0;

	if (EmptyHandDataAsset)
		AddItem(EmptyHandDataAsset);
}


void UInventoryComponent::AddItem(UInventoryItemDataAsset* Item)
{
	Items.AddUnique(Item);
}

void UInventoryComponent::RemoveSelectedItem()
{
	Items.RemoveAt(SelectedItem_Index);
}

void UInventoryComponent::SwitchItem(float Mouse_AxisValue)
{
	// Early-out if scroll wheel isn't being used:
	if (FMath::IsNearlyZero(Mouse_AxisValue)) return;
	
	int NewIndex = SelectedItem_Index + Mouse_AxisValue;

	//in-case new index overflows
	NewIndex = NewIndex > Items.Num()-1 ? 0 : NewIndex;
	NewIndex = NewIndex < 0 ? Items.Num()-1 : NewIndex;

	SelectedItem_Index = NewIndex;

	if (Items.Num() > 0)
	{
		// If the Tag->Enum map knows about this item, make the player start holding the item:
		if (TagToEnumMap.Contains(Items[SelectedItem_Index]->ItemTag))
		{
			APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

			if (PlayerPawn) PlayerPawn->ChangeItem(*(TagToEnumMap.Find(Items[SelectedItem_Index]->ItemTag)));
		}
	}
}

