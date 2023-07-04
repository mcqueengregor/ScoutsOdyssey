// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineSceneCaptureActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"

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
	ResizeRenderTarget(Viewport, 0);
	
	// Add render target resize function to resize delegate:
	Viewport->ViewportResizedEvent.AddUObject(this, &AOutlineSceneCaptureActor::ResizeRenderTarget);
}

// Called every frame
void AOutlineSceneCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawnRef && PlayerPawnRef->LastEnteredSection)
	{
		const FTransform NewWorldTransform =
			PlayerPawnRef->LastEnteredSection->StageSectionCameraComponent->GetComponentTransform();

		SceneCaptureComponent->SetWorldTransform(NewWorldTransform);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red,
			FString("PlayerPawnRef is null!"));
}

void AOutlineSceneCaptureActor::ResizeRenderTarget(FViewport* Viewport, uint32 val)
{
	if (SceneCaptureComponent->TextureTarget)
		SceneCaptureComponent->TextureTarget->ResizeTarget(Viewport->GetSizeXY().X, Viewport->GetSizeXY().Y);
}

