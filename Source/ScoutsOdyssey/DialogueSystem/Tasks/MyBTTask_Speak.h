// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "..\DialogueEnums.h"
#include "MyBTTask_Speak.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API UMyBTTask_Speak : public UBTTaskNode
{
	GENERATED_BODY()

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
public:
	UMyBTTask_Speak();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	// Delegate Binding To Finish Task
	void Delegate_SetUp();
	void SpeakTaskFinish();
	bool SpeakTaskFinished;
	FDelegateHandle SpeakTaskFinish_DelegateHandle;

	// Behavior Tree variables
	class UDialogueComponent* DialogueComponent;
	
	// Behavior Tree editable
	UPROPERTY(EditAnywhere, Category=Speak)
	FBlackboardKeySelector DialogueComponent_BlackboardKey;
	UPROPERTY(EditAnywhere, Category=Speak, meta = (AllowPrivateAccess = true))
	FString String;
	UPROPERTY(EditAnywhere, Category=Speak, meta = (AllowPrivateAccess = true))
	EBubble BubbleChoice;
	UPROPERTY(EditAnywhere, Category=Speak, meta = (AllowPrivateAccess = true))
	EVoiceType VoiceType = EVoiceType::High;

};
