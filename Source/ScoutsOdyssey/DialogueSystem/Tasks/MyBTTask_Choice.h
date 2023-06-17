// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "..\DialogueEnums.h"
#include "MyBTTask_Choice.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API UMyBTTask_Choice : public UBTTaskNode
{
	GENERATED_BODY()
	
	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
public:
	UMyBTTask_Choice();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	// Delegate Binding To Finish Task
	void Delegate_SetUp();
	void ChoiceTaskFinish();
	bool ChoiceTaskFinished;
	FDelegateHandle ChoiceTaskFinish_DelegateHandle;

	// Behavior Tree variables
	class UDialogueComponent* DialogueComponent;
	
	// Behavior Tree editable
	UPROPERTY(EditAnywhere, Category=Choice)
	FBlackboardKeySelector DialogueComponent_BlackboardKey;
	UPROPERTY(EditAnywhere, Category=Choice)
	FBlackboardKeySelector ReplyIndex_BlackboardKey_Out;
	UPROPERTY(EditAnywhere, Category=Choice, meta = (AllowPrivateAccess = true))
	TArray<FText> Choices;
	
};
