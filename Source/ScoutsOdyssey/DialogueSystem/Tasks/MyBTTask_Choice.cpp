// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Choice.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"


UMyBTTask_Choice::UMyBTTask_Choice()
{
	NodeName = TEXT("Choice");

	bNotifyTick = true;

	DialogueComponent_BlackboardKey.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UMyBTTask_Choice, DialogueComponent_BlackboardKey),
		UDialogueComponent::StaticClass());

	ReplyIndex_BlackboardKey_Out.AddIntFilter(
		this, GET_MEMBER_NAME_CHECKED(UMyBTTask_Choice, ReplyIndex_BlackboardKey_Out));
}

EBTNodeResult::Type UMyBTTask_Choice::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ChoiceTaskFinished = false;

	// Get blackboard values
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	DialogueComponent = Cast<UDialogueComponent>(
		BlackboardComponent->GetValueAsObject(DialogueComponent_BlackboardKey.SelectedKeyName));

	if (DialogueComponent)
	{
		DialogueComponent->SwitchBubbleOneState(EBubbleState::Choice);
		DialogueComponent->SwitchToBubble(EBubble::One);
		
		// Assign choice task finish delegate
		Delegate_SetUp();

		// Display Choice
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueComponent is Null: %s"), *GetClass()->GetName());
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_Choice::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (ChoiceTaskFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask Finished: %s"), *GetClass()->GetName());
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
	}
}

// For calling ChoiceTaskFinish when FOnChoiceFinish is called in DialogueComponent. For ending the task. 
void UMyBTTask_Choice::Delegate_SetUp()
{
	FOnChoiceFinish::FDelegate ChoiceTaskFinish_Delegate;
	ChoiceTaskFinish_Delegate.BindUObject(this, &UMyBTTask_Choice::ChoiceTaskFinish);
	ChoiceTaskFinish_DelegateHandle = DialogueComponent->OnChoiceFinish.Add(ChoiceTaskFinish_Delegate);
	UE_LOG(LogTemp, Warning, TEXT("TaskFinish Delegate Setup Successfully On OnChoiceFinish!"));
}

void UMyBTTask_Choice::ChoiceTaskFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Choice Task Finished Called!"));
	if (DialogueComponent)
	{
		// Remove Task Finish Delegate
		DialogueComponent->OnChoiceFinish.Remove(ChoiceTaskFinish_DelegateHandle);
		ChoiceTaskFinished = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Choice Task Can't finish, DialogueComponent is Null: %s"), *GetClass()->GetName());
	}
}


FString UMyBTTask_Choice::GetStaticDescription() const
{
	FString ChoicesString;
	for (int i = 0; i < Choices.Num(); i++)
	{
		ChoicesString += FString::Printf(TEXT("%d: "), i);;
		ChoicesString += Choices[i].ToString();
		ChoicesString += "\n";
	}

	return FString::Printf(TEXT("%s"), *ChoicesString);
}



void UMyBTTask_Choice::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		DialogueComponent_BlackboardKey.ResolveSelectedKey(*BBAsset);
		ReplyIndex_BlackboardKey_Out.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning,
		       TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}
