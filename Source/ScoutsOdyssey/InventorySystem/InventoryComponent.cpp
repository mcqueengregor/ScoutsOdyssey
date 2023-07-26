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

	// Add "empty hand" to inventory:
	if (EmptyHandDataAsset)
		AddItem(EmptyHandDataAsset);

	APlayerPawn* OwnerPawn = Cast<APlayerPawn>(GetOwner());
	if (OwnerPawn) PlayerPawnRef = OwnerPawn;
}

void UInventoryComponent::AddItem(UInventoryItemDataAsset* Item)
{
	Items.AddUnique(Item);
}

void UInventoryComponent::RemoveSelectedItem()
{
	// If player has another item in their inventory, remove
	// the currently, held one and switch to another item:
	if (Items.Num() > 1)
	{
		Items.RemoveAt(SelectedItem_Index);
		SelectedItem_Index = SelectedItem_Index = 0 ? Items.Num() - 1 : SelectedItem_Index - 1;
		RefreshCurrentItem();
	}
}

void UInventoryComponent::SwitchItem(float Mouse_AxisValue)
{
	// Early-out if scroll wheel isn't being used or player is in the middle of switching items:
	if (FMath::IsNearlyZero(Mouse_AxisValue) || (PlayerPawnRef && PlayerPawnRef->GetIsChangingItem()))
			return;
	
	int NewIndex = SelectedItem_Index + Mouse_AxisValue;

	//in-case new index overflows
	NewIndex = NewIndex > Items.Num()-1 ? 0 : NewIndex;
	NewIndex = NewIndex < 0 ? Items.Num()-1 : NewIndex;

	SelectedItem_Index = NewIndex;

	// Don't tell the player to change item if there isn't another item to change to:
	if (Items.Num() > 1)
	{
		// If the Tag->Enum map knows about this item, make the player start holding the item:
		if (TagToEnumMap.Contains(Items[SelectedItem_Index]->ItemTag) && PlayerPawnRef)
		{
			PlayerPawnRef->ChangeItem(*(TagToEnumMap.Find(Items[SelectedItem_Index]->ItemTag)));
		}
	}
}

void UInventoryComponent::RefreshCurrentItem()
{
	if (TagToEnumMap.Contains(Items[SelectedItem_Index]->ItemTag) && PlayerPawnRef)
	{
		PlayerPawnRef->ChangeItem(*(TagToEnumMap.Find(Items[SelectedItem_Index]->ItemTag)), false);
	}
}

