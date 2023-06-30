// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueMeshActor.h"
#include "DialogueComponent.h"
#include "ScoutsOdyssey/Player/MyPlayerController.h"
#include "AIController.h"
#include "DialogueChoiceObject.h"
#include "Blueprint/UserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/LoggingMacros.h"
#include "ScoutsOdyssey/Components/InteractComponentBase.h"

ADialogueMeshActor::ADialogueMeshActor()
{
	PropCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PropCollider"));
	PropCollider->SetupAttachment(RootComponent);
}

void ADialogueMeshActor::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	// Spawn AI Controller 
	AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), FVector(0), FRotator(0));
	Clickable_SetUp();

	// Start input component. will start on all DialogueMeshActors in scene. 
	StartPlayerInputComponent();
	
	if(PlayOnStart)
	{
		LocalAnimTime = 0.0f;
		bAnimFlipFlop = true;	
	} else
	{
		LocalAnimTime = 0.0f;
		bAnimFlipFlop = false;
	}
	
	DynamicAnimMaterial = UMaterialInstanceDynamic::Create(
		GetStaticMeshComponent()->GetMaterial(0), this);
	
	GetStaticMeshComponent()->SetMaterial(0, DynamicAnimMaterial);
}

UInputComponent* ADialogueMeshActor::CreatePlayerInputComponent()
{
	static const FName InputComponentName(TEXT("DialogueMeshActor_InputComponent"));
	return NewObject<UInputComponent>(this, UInputSettings::GetDefaultInputComponentClass(), InputComponentName);
}

/**
 * @brief It sets consumeInput to false, after getting a FInputActionBinding& reference. By reference, take note.
 * @param PlayerInputComponent 
 */
void ADialogueMeshActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	FInputActionBinding& ClickBinding = PlayerInputComponent->BindAction("Click", IE_Pressed, this, &ADialogueMeshActor::LeftMouseButtonDown);
	ClickBinding.bConsumeInput = false;
}

void ADialogueMeshActor::StartPlayerInputComponent()
{
	InputComponent = CreatePlayerInputComponent();
	if(InputComponent)
	{
		SetupPlayerInputComponent(InputComponent);
		InputComponent->RegisterComponent();
	}
}

void ADialogueMeshActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO: Extend this to choose animation transition duration!
	LocalAnimTime += bAnimFlipFlop ? DeltaSeconds * AnimationSpeed : -DeltaSeconds * AnimationSpeed;
	LocalAnimTime = FMath::Clamp(LocalAnimTime, 0.0f, 1.0f);	
	
	if (DynamicAnimMaterial)
		DynamicAnimMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", LocalAnimTime);
}

void ADialogueMeshActor::BehaviorTree_Start(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if(AIController)
	{		
		AIController->RunBehaviorTree(DialogueTree);
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			UDialogueComponent* DiagueComponent =
				Cast<UDialogueComponent>(GetComponentByClass(UDialogueComponent::StaticClass()));
			if (DiagueComponent)
			{
				Blackboard->SetValueAsObject("DialogueComponent", DiagueComponent);
				UE_LOG(LogTemp, Warning, TEXT("Registered dialogue component reference in blackboard!"));
			}

			UInteractComponentBase* InteractComponent =
				Cast<UInteractComponentBase>(GetComponentByClass(UInteractComponentBase::StaticClass()));

			if (InteractComponent)
			{
				Blackboard->SetValueAsObject("InteractionComponent", InteractComponent);
				UE_LOG(LogTemp, Warning, TEXT("Registered interaction component reference in blackboard!"));
			}
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("BlackBoardComponent is not assigned in AIController!"));	
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller was not constructed For Dialogue Tree!"));		
	}
}

void ADialogueMeshActor::Clickable_SetUp()
{
	UDialogueComponent* DialogueComponent =
		Cast<UDialogueComponent>(GetComponentByClass(UDialogueComponent::StaticClass()));

	UStaticMeshComponent* MyStaticMeshComponent =
		Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	
	if (DialogueComponent && MyStaticMeshComponent)
	{
		MyStaticMeshComponent->OnClicked.AddDynamic(DialogueComponent, &UDialogueComponent::Click_Implementation);
		MyStaticMeshComponent->OnClicked.AddDynamic(this, &ADialogueMeshActor::BehaviorTree_Start);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue mesh actor OnClicked delegate wasn't set up (DialogueComponent was nullptr)"));
	}
}

void ADialogueMeshActor::ToggleAnimation()
{
	bAnimFlipFlop = !bAnimFlipFlop;
}

void ADialogueMeshActor::LeftMouseButtonDown()
{
	LOG_ACTOR("current left mouse button down actor!");
	if(OnLeftMouseClick.IsBound())
	{
		OnLeftMouseClick.Execute();
	} else
	{
		PRINT("delegate not bound? for some reason?");
	}
}
