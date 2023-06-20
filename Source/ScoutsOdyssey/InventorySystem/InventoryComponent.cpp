// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
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
	int NewIndex = SelectedItem_Index + Mouse_AxisValue;

	//in-case new index overflows
	NewIndex = NewIndex > Items.Num()-1 ? 0 : NewIndex;
	NewIndex = NewIndex < 0 ? Items.Num()-1 : NewIndex;

	SelectedItem_Index = NewIndex;
}

