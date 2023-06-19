// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Exit.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API UMyBTTask_Exit : public UBTTaskNode
{
	GENERATED_BODY()

	UMyBTTask_Exit();
	
	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Behavior Tree editable
	UPROPERTY(EditAnywhere, Category=Speak)
	FBlackboardKeySelector DialogueComponent_BlackboardKey;
};
