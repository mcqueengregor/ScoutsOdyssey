// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineSceneCaptureActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"
#include "ScoutsOdyssey/Level/StageSectionVolume.h"

// Sets default values
AOutlineSceneCaptureActor::AOutlineSceneCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene capture component"));
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	SceneCaptureComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AOutlineSceneCaptureActor::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawnRef = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	// Log player character sprite mesh as the only mesh being rendered by this actor:
	if (PlayerPawnRef)
		SceneCaptureComponent->ShowOnlyComponent(PlayerPawnRef->MeshComponent);

	// Init render target dimensions:
	FViewport* Viewport = GEngine->GameViewport->Viewport;

	if (Viewport)
	{
		//ResizeRenderTarget(Viewport, 0);
		// Add render target resize function to resize delegate:
		//Viewport->ViewportResizedEvent.AddUObject(this, &AOutlineSceneCaptureActor::ResizeRenderTarget);
	}

	CurrentViewportDimensions = FIntPoint(0, 0);
}

// Called every frame
void AOutlineSceneCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawnRef && PlayerPawnRef->LastEnteredSection)
	{
		const UCameraComponent* StageSectionCamera = PlayerPawnRef->LastEnteredSection->StageSectionCameraComponent;

		SceneCaptureComponent->SetWorldTransform(StageSectionCamera->GetComponentTransform());
		SceneCaptureComponent->FOVAngle = StageSectionCamera->FieldOfView;
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red,
			FString("PlayerPawnRef is null!"));

	FViewport* Viewport = GEngine->GameViewport->Viewport;

	if (Viewport)
	{
		if (SceneCaptureComponent->TextureTarget && Viewport->GetSizeXY() != CurrentViewportDimensions
			&& Viewport->GetSizeXY().X != 0 && Viewport->GetSizeXY().Y != 0)
		{
			CurrentViewportDimensions = Viewport->GetSizeXY();
			SceneCaptureComponent->TextureTarget->ResizeTarget(CurrentViewportDimensions.X, CurrentViewportDimensions.Y);
		}
	}
}

void AOutlineSceneCaptureActor::ResizeRenderTarget(FViewport* Viewport, uint32 val)
{
<<<<<<< Updated upstream
	if (SceneCaptureComponent->TextureTarget && Viewport)
=======
<<<<<<< HEAD
	if (SceneCaptureComponent->TextureTarget && Viewport->GetSizeXY().X != 0 && Viewport->GetSizeXY().Y != 0)
=======
	if (SceneCaptureComponent->TextureTarget && Viewport)
>>>>>>> b4c4e3e9efe59f687d16027803687cc36830660f
>>>>>>> Stashed changes
		SceneCaptureComponent->TextureTarget->ResizeTarget(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y);
}

