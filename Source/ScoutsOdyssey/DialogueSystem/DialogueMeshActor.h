// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/StaticMeshActor.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueMeshActor.generated.h"

DECLARE_DELEGATE(FOnLeftMouseClick);

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API ADialogueMeshActor : public AStaticMeshActor, public IClickable
{
	GENERATED_BODY()
	ADialogueMeshActor();

protected:
	virtual void BeginPlay() override;

	// handling input
	UInputComponent* CreatePlayerInputComponent();	
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	void StartPlayerInputComponent();

public:
	virtual void Tick(float DeltaSeconds) override;
	
	// Behavior Tree
	UPROPERTY(EditInstanceOnly, Category=Dialogue)
	UBehaviorTree* DialogueTree;
	
	class AMyPlayerController* MyPlayerController;
	class AAIController* AIController;

	UFUNCTION()
	void BehaviorTree_Start(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	void Clickable_SetUp();
	void Clickable_CleanUp();

	UFUNCTION()
	void LeftMouseButtonDown();
	FOnLeftMouseClick OnLeftMouseClick;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* PropCollider;

private:
	UPROPERTY(EditInstanceOnly)
	bool OnlyTriggerOnce = true;
};
