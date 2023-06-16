// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Speak.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"


UMyBTTask_Speak::UMyBTTask_Speak()
{
	NodeName = TEXT("Speak");

	bNotifyTick = true;
	
	DialogueComponent_BlackboardKey.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UMyBTTask_Speak, DialogueComponent_BlackboardKey),
		UDialogueComponent::StaticClass());
}

EBTNodeResult::Type UMyBTTask_Speak::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get blackboard values
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	DialogueComponent = Cast<UDialogueComponent>(
		BlackboardComponent->GetValueAsObject(DialogueComponent_BlackboardKey.SelectedKeyName));

	if (DialogueComponent)
	{
		DialogueComponent->SwitchToBubble(BubbleChoice);

		// Assign task finish delegate
		Delegate_SetUp();

		// Speak
		DialogueComponent->Speak(Text, BubbleChoice);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueComponent is Null: %s"), *GetClass()->GetName());
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_Speak::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (TaskFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask Finished: %s"), *GetClass()->GetName());
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
	}
}


FString UMyBTTask_Speak::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *UEnum::GetValueAsString(BubbleChoice), *Text.ToString());
}


void UMyBTTask_Speak::Delegate_SetUp()
{
	FOnDialogueEnd::FDelegate TaskFinish_Delegate;
	TaskFinish_Delegate.BindUObject(this, &UMyBTTask_Speak::TaskFinish);
	TaskFinish_DelegateHandle = DialogueComponent->OnSpeakFinish.Add(TaskFinish_Delegate);
	UE_LOG(LogTemp, Warning, TEXT("TaskFinish Delegate Setup Successfully On OnSpeakFinish!"));
}

void UMyBTTask_Speak::TaskFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Task Finished Called!"));
	if (DialogueComponent)
	{
		// Remove Task Finish Delegate
		DialogueComponent->OnSpeakFinish.Remove(TaskFinish_DelegateHandle);
		TaskFinished = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Task cannot finish as Dialogue Component is Null"));
	}
}


void UMyBTTask_Speak::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		DialogueComponent_BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning,
		       TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}
