// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Speak.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"
#include "Sound/SoundCue.h"

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
	SpeakTaskFinished = false;
	
	// Get blackboard values
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	DialogueComponent = Cast<UDialogueComponent>(
		BlackboardComponent->GetValueAsObject(DialogueComponent_BlackboardKey.SelectedKeyName));

	if (DialogueComponent)
	{
		DialogueComponent->SwitchToBubble(BubbleChoice);
		DialogueComponent->SwitchBubbleOneState(EBubbleState::Speak);

		// Assign task finish delegate
		Delegate_SetUp();

		if(SoundCue)
			UGameplayStatics::PlaySound2D(this, SoundCue);
		
		// Speak
		DialogueComponent->Speak(String, BubbleChoice, Fontsize, TalkRate);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueComponent is Null: %s"), *GetClass()->GetName());
	}

	return EBTNodeResult::InProgress;
}

void UMyBTTask_Speak::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (SpeakTaskFinished)
	{
		OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
	}
}


FString UMyBTTask_Speak::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *UEnum::GetValueAsString(BubbleChoice), *String.Left(20));
}


void UMyBTTask_Speak::Delegate_SetUp()
{
	FOnSpeakFinish::FDelegate SpeakTaskFinish_Delegate;
	SpeakTaskFinish_Delegate.BindUObject(this, &UMyBTTask_Speak::SpeakTaskFinish);
	SpeakTaskFinish_DelegateHandle = DialogueComponent->OnSpeakFinish.Add(SpeakTaskFinish_Delegate);
}

void UMyBTTask_Speak::SpeakTaskFinish()
{
	if (DialogueComponent)
	{
		// Remove Task Finish Delegate
		DialogueComponent->OnSpeakFinish.Remove(SpeakTaskFinish_DelegateHandle);
		
		SpeakTaskFinished = true;
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
