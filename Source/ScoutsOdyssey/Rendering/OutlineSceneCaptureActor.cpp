// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineSceneCaptureActor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"

// Sets default values
AOutlineSceneCaptureActor::AOutlineSceneCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene capture component"));
	SceneCaptureComponent.PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent.SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AOutlineSceneCaptureActor::BeginPlay()
{
	Super::BeginPlay();

	// Log player character sprite mesh as the only mesh being rendered by this actor:
	if (APlayerPawn* PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld())))
		SceneCaptureComponent.ShowOnlyComponent(PlayerPawn->MeshComponent);

	// Generate render target which matches the viewport's dimensions:
	// TODO: Make this texture 3/4 or 1/2 res?
	
}

// Called every frame
void AOutlineSceneCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

