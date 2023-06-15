// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSectionVolume.h"
#include "Containers/List.h"		// TLinkedList used by APlayerPawn
#include "../Player/PlayerPawn.h"	// APlayerPawn class reference
#include "Camera/CameraComponent.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values
AStageSectionVolume::AStageSectionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene"));
	SceneComponent->SetupAttachment(GetRootComponent());
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Stage section trigger volume"));
	TriggerVolume->InitBoxExtent(FVector(300.0f, 250.0f, 150.0f));	// TODO: Set extent to appropriate
	TriggerVolume->SetupAttachment(SceneComponent);								// values once these are figured out!!
	this->bGenerateOverlapEventsDuringLevelStreaming = true;	// Detect BeginOverlap events on game startup.
	
	StageSectionCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera angle component"));
	StageSectionCameraComponent->SetRelativeLocation(FVector(-750.0f, 0.0f, 0.0f));
	StageSectionCameraComponent->SetupAttachment(TriggerVolume);
}

// Called when the game starts or when spawned
void AStageSectionVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapEnd);
}

// Called every frame
void AStageSectionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageSectionVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{	
	// If an actor overlaps with this stage section and that actor is the player character (APlayerPawn),
	// grab its camera component and sweep it to this stage section's camera angle:
	APlayerPawn* Pawn = Cast<APlayerPawn>(OtherActor);
	
	if (Pawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
			FString("Player entered ") + UKismetSystemLibrary::GetDisplayName(this));

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		// Remember this section as the last section that was entered:
		Pawn->OverlappedStageSections.AddHead(this);
		Pawn->LastEnteredSection = this;
		
		PlayerController->SetViewTarget(this, GetCameraTransitionParams(Pawn));

		Pawn->bHasCameraAngleChangedAlready = true;
	}
}

void AStageSectionVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(OtherActor);

	if (Pawn && Pawn->LastEnteredSection)
	{
		// Determine which stage section the player should "forget"
		// about, and update camera angle if necessary:
		const bool bLeftLastEnteredSection = Pawn->LastEnteredSection == this;

		// (Head represents the last-entered section)
		auto SectionToForget = bLeftLastEnteredSection ?
			Pawn->OverlappedStageSections.GetTail() :
			Pawn->OverlappedStageSections.GetHead();

		FString DebugMsg = bLeftLastEnteredSection ?
			FString("Removed tail!") :
			FString("Removed head!");

		// If leaving the previously-entered section, set camera angle back to section the player is returning to:
		if (bLeftLastEnteredSection)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController->SetViewTarget(Pawn->OverlappedStageSections.GetTail()->GetValue(), GetCameraTransitionParams(Pawn));
			Pawn->LastEnteredSection = Pawn->OverlappedStageSections.GetTail()->GetValue();
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, DebugMsg);
		Pawn->OverlappedStageSections.RemoveNode(SectionToForget);
	}
}

FViewTargetTransitionParams AStageSectionVolume::GetCameraTransitionParams(APlayerPawn* const Pawn) const
{
	FViewTargetTransitionParams TransitionParams;

	// Use either sweeping transition or instant location change based on whether this is the first
	// camera angle transition or not:
	TransitionParams.BlendFunction = Pawn->bHasCameraAngleChangedAlready ?
			EViewTargetBlendFunction::VTBlend_EaseInOut :
			EViewTargetBlendFunction::VTBlend_Linear;
	TransitionParams.BlendTime = Pawn->bHasCameraAngleChangedAlready * Pawn->CameraTransitionDuration;
	TransitionParams.BlendExp = 2.0f;
	
	return TransitionParams;
}

