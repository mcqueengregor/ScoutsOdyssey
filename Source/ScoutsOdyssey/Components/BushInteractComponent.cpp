// Fill out your copyright notice in the Description page of Project Settings.


#include "BushInteractComponent.h"

#include "Math/UnitConversion.h"
#include "ScoutsOdyssey/LoggingMacros.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

UBushInteractComponent::UBushInteractComponent()
{
	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	if(OwnerActor)
	{
		StaticMeshComponent = OwnerActor->GetStaticMeshComponent();
		StaticMeshComponent->SetRelativeScale3D(StartScale);
	} else
	{
		LOG_ERROR("Cast failed!");
	}	
}

void UBushInteractComponent::BeginPlay()
{
	TransformTimer = TransformTime;
}



void UBushInteractComponent::DoTask()
{
	//Clear it regardless
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		
		FVector CurScale = StaticMeshComponent->GetRelativeScale3D() +
			TransformTimer/TransformTime * (EndScale - StartScale);
		StaticMeshComponent->SetRelativeScale3D(CurScale);
		
		if(CurScale.Equals(EndScale, 0.001))
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);	
		}
		TransformTimer -= 0.1f;
		
	}), 0.1f, true);
}
