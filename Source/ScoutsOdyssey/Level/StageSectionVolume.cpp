// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSectionVolume.h"
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
	TriggerVolume->InitBoxExtent(FVector(150.0f));	// TODO: Set extent's x- and z- components to
	TriggerVolume->SetupAttachment(SceneComponent); // appropriate values once these are figured out!!
	this->bGenerateOverlapEventsDuringLevelStreaming = true;	// Detect BeginOverlap events on game startup.
	
	StageSectionCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera angle component"));
	StageSectionCameraComponent->SetRelativeLocation(FVector(-500.0f, 0.0f, 0.0f));
	StageSectionCameraComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AStageSectionVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapBegin);
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
		FViewTargetTransitionParams TransitionParams;

		// Use either sweeping transition or instant location change based on whether this is the first
		// camera angle transition or not:
		TransitionParams.BlendFunction = Pawn->bHasCameraAngleChangedAlready ?
				EViewTargetBlendFunction::VTBlend_EaseInOut :
				EViewTargetBlendFunction::VTBlend_Linear;
		TransitionParams.BlendTime = Pawn->bHasCameraAngleChangedAlready * Pawn->CameraTransitionDuration;
		TransitionParams.BlendExp = 2.0f;

		Pawn->bHasCameraAngleChangedAlready = true;
		
		PlayerController->SetViewTarget(this, TransitionParams);
	}
}

