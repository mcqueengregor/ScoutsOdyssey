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

	CameraType = ECameraType::STATIC;
	PlayerPawnRef = nullptr;
}

// Called when the game starts or when spawned
void AStageSectionVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapEnd);

	// TODO: Find a tidier way of keeping a reference to the player pawn for sections with follow cameras?
	PlayerPawnRef = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AStageSectionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerPawnRef)
	{
		// If this stage section uses a FOLLOW camera, and the player is currently using this section's camera angle,
		// set camera position to match player's y-position:
		if (CameraType == ECameraType::FOLLOW && PlayerPawnRef->LastEnteredSection == this)
		{
			// Find max and min values that camera can move to on the Y-axis, in world-space:
			const float LowerCameraBoundWS = GetActorLocation().Y - (TriggerVolume->GetScaledBoxExtent().Y / 2.0f) + FollowCameraBounds;
			const float UpperCameraBoundWS = GetActorLocation().Y + (TriggerVolume->GetScaledBoxExtent().Y / 2.0f) - FollowCameraBounds;
			float NewYPositionWS = FMath::Clamp(PlayerPawnRef->GetActorLocation().Y, LowerCameraBoundWS, UpperCameraBoundWS);
			
			const FVector CurrentCamPos = StageSectionCameraComponent->GetComponentLocation();
			StageSectionCameraComponent->SetWorldLocation(FVector(
				CurrentCamPos.X,
				NewYPositionWS,
				CurrentCamPos.Z));
		}
	}
}

void AStageSectionVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{	
	// If an actor overlaps with this stage section and that actor is the player character (APlayerPawn),
	// grab its camera component and sweep it to this stage section's camera angle:
	APlayerPawn* Pawn = Cast<APlayerPawn>(OtherActor);
	
	if (Pawn)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		
		// Remember this section as the last section that was entered:
		Pawn->OverlappedStageSections.AddHead(this);
		Pawn->LastEnteredSection = this;
		
		PlayerController->SetViewTarget(this, GetCameraTransitionParams(Pawn));

		Pawn->bHasCameraAngleChangedAlready = true;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
			FString("Entered ") + UKismetSystemLibrary::GetDisplayName(this));
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
		// TODO: Change 'auto' to explicit data type specifier!
		auto SectionToForget = bLeftLastEnteredSection ?
			Pawn->OverlappedStageSections.GetHead() :
			Pawn->OverlappedStageSections.GetTail();

		// If leaving the previously-entered section, set camera angle back to section the player is returning to:
		if (bLeftLastEnteredSection)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController->SetViewTarget(Pawn->OverlappedStageSections.GetTail()->GetValue(), GetCameraTransitionParams(Pawn));
			Pawn->LastEnteredSection = Pawn->OverlappedStageSections.GetTail()->GetValue();
		}
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
