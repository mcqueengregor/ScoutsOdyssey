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

	void UpdateDynamicMaterial(float EmissionStrength);
public:
	virtual void Tick(float DeltaSeconds) override;
	
	// Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Dialogue)
	UBehaviorTree* DialogueTree;
	
	class AMyPlayerController* MyPlayerController;

	UPROPERTY(BlueprintReadWrite)
	class AAIController* AIController;

	UMaterialInstanceDynamic* CreateAndAssignDynamicMaterial();

	UFUNCTION()
	void BehaviorTree_Start(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION()
	void BehaviourTree_Stop();
	
	void Clickable_SetUp();
	void Clickable_CleanUp();

	inline void DisableInteractions() { bHasInteractionsRemaining = false; UpdateDynamicMaterial(0.0f); }
	
	UFUNCTION()
	void LeftMouseButtonDown();
	FOnLeftMouseClick OnLeftMouseClick;
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool OnlyTriggerOnce = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* PropCollider;

	bool bHasInteractionsRemaining;
	
private:
	UPROPERTY(EditAnywhere)
	bool DestroyOnDialogueEnd = false;
};
