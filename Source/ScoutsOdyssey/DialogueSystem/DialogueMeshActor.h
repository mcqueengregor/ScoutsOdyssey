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

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void ToggleAnimation();

	UPROPERTY(EditAnywhere, Category=Interact)
	bool PlayOnStart;
	UPROPERTY(EditAnywhere, Category=Interact)
	float AnimationSpeed = 1;
	
private:
	bool bAnimFlipFlop;	// When 'true' animation local time will increase from 0 to 1 (i.e. animation will play from
						// start to end, then stop), when 'false' animation local time will go from 1 to 0 (i.e.
						// animation will play from end to start, then stop).

	float LocalAnimTime;	// Value in the range [0,1], representing how far along an animation is on its local
							// timeline, in percentage (e.g. 0.6 = 60% from start to finish).

protected:	
	class UMaterialInstanceDynamic* DynamicAnimMaterial;
};
