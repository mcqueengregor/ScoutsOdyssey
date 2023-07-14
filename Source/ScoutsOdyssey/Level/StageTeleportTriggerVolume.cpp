// Fill out your copyright notice in the Description page of Project Settings.


#include "StageTeleportTriggerVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"

// Sets default values
AStageTeleportTriggerVolume::AStageTeleportTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene"));
	SceneComponent->SetupAttachment(RootComponent);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleport trigger volume"));
	TriggerVolume->SetupAttachment(SceneComponent);

	bIsFadingToBlack = false;
	TeleportDuration = 1.0f;
	FadeToBlackLerpT = 0.0f;
}

// Called when the game starts or when spawned
void AStageTeleportTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AStageTeleportTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AStageTeleportTriggerVolume::OnOverlapEnd);

	UMaterial* FadeToBlackMaterial;
	GetFadeToBlackMatRef(FadeToBlackMaterial);
	
	if (FadeToBlackMaterial)
	{
		UMaterialInterface* MaterialInterface = FadeToBlackMaterial;
		UMaterialInstanceDynamic::Create(MaterialInterface, this);
		SetupDynamicFadeToBlackMat();	// Set post process material in BP.
	}
}

void AStageTeleportTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	if (PawnRef && TeleportLocationActor)
	{
		PawnRef->StartTeleportationTimer(TeleportLocationActor->GetActorLocation(), TeleportDuration);
		ToggleFadeToBlack();
	}

	// If this teleport trigger volume doesn't know where to teleport the player, print a debug message:
	else if (TeleportLocationActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
			UKismetSystemLibrary::GetDisplayName(this) +
			FString(" doesn't reference a teleport location actor!"));
	}
}

void AStageTeleportTriggerVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	if (PawnRef && !PawnRef->GetHasTeleported())
	{
		PawnRef->CancelTeleportTimer();
		ToggleFadeToBlack();
	}
}

// Called every frame
void AStageTeleportTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FadeToBlackMaterialDynamic)
	{
		// Get scalar for increasing/decreasing T value for lerp (either -1 or 1):
		float LerpTScalar = static_cast<float>(bIsFadingToBlack) * 2.0f - 1.0f;
		FadeToBlackLerpT += (DeltaTime / TeleportDuration * 2.0f) * LerpTScalar;
		FadeToBlackLerpT = FMath::Clamp(FadeToBlackLerpT, 0.0f, 1.0f);

		FadeToBlackMaterialDynamic->SetScalarParameterValue("LerpT", FadeToBlackLerpT);
	}
}

void AStageTeleportTriggerVolume::GetFadeToBlackMatRef_Implementation(UMaterial*& RetrievedMaterial)
{
}

void AStageTeleportTriggerVolume::SetupDynamicFadeToBlackMat_Implementation()
{
}
