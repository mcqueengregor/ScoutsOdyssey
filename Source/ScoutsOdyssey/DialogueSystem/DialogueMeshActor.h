// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/StaticMeshActor.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API ADialogueMeshActor : public AStaticMeshActor, public IClickable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
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
};
