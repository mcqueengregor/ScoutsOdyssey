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

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

public:
	virtual void Tick(float DeltaSeconds) override;
	
	// Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Dialogue)
	UBehaviorTree* DialogueTree;
	
	class AMyPlayerController* MyPlayerController;
	class AAIController* AIController;

	UMaterialInstanceDynamic* CreateAndAssignDynamicMaterial();

	UFUNCTION()
	void BehaviorTree_Start(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	void Clickable_SetUp();
	void Clickable_CleanUp();

	UFUNCTION()
	void LeftMouseButtonDown();
	FOnLeftMouseClick OnLeftMouseClick;
	UMaterialInstanceDynamic* DynamicMaterial;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* PropCollider;

private:
	UPROPERTY(EditInstanceOnly)
	bool OnlyTriggerOnce = true;
	UPROPERTY(EditInstanceOnly)
	bool DestroyOnDialogueEnd = false;
};
