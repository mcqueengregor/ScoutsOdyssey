// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Exit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ScoutsOdyssey/LoggingMacros.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"


UMyBTTask_Exit::UMyBTTask_Exit()
{
	NodeName = TEXT("Exit");
	
	DialogueComponent_BlackboardKey.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UMyBTTask_Exit, DialogueComponent_BlackboardKey),
		UDialogueComponent::StaticClass());
}

void UMyBTTask_Exit::InitializeFromAsset(UBehaviorTree& Asset)
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

EBTNodeResult::Type UMyBTTask_Exit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	UDialogueComponent* DialogueComponent = Cast<UDialogueComponent>(
		BlackboardComponent->GetValueAsObject(DialogueComponent_BlackboardKey.SelectedKeyName));

	if(DialogueComponent)
	{
		DialogueComponent->OnDialogueEnd.Broadcast();
		if(DisableDialogueAfterExit)
		{
			ADialogueMeshActor* DialogueMeshActor = Cast<ADialogueMeshActor>(DialogueComponent->GetOwner());
			if(DialogueMeshActor)
			{
				DialogueMeshActor->Clickable_CleanUp();
			} else
			{
				LOG_ERROR("Cast failed!");
			}	
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("DialogueComponent is Null: %s"), *GetClass()->GetName());
	
	return EBTNodeResult::Succeeded;
}
