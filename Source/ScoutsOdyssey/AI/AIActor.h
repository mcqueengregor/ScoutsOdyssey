// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIAnimationEnums.h"
#include "AIController.h"
#include "AnimationMap.h"
#include "GameFramework/Actor.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"
#include "AIActor.generated.h"


UCLASS()
class SCOUTSODYSSEY_API AAIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIActor();
	
	UFUNCTION(BlueprintCallable)
	virtual void ChangeAnimation(int Index) PURE_VIRTUAL(AAIActor::ChangeAnimation, );
	
	UPROPERTY(EditAnywhere, Category=AI)
	TArray<USpriteAnimationDataAsset*> AnimationDAs;

	UPROPERTY(EditAnywhere, Category=AI)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category=AI)
	bool LoopCurAnim = false;

protected:
	virtual void BeginPlay() override;
	
	void CreateDynamicAnimationMaterial();

	void NewAnimation_SetUp();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	UMaterialInstanceDynamic* DynamicMaterial;
	USpriteAnimationDataAsset* CurrentAnimDetails;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Current animation duration and running time
	float AnimDuration;
	float AnimRunningTime;

	AAIController* AIController;

	void UpdateDynamicMaterialParameters();
	void ResetAnimationTimes();

	void BehaviorTree_SetUp();

};

