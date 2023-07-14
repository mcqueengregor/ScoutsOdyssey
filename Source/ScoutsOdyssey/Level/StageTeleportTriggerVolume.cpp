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
}

// Called when the game starts or when spawned
void AStageTeleportTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AStageTeleportTriggerVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AStageTeleportTriggerVolume::OnOverlapEnd);
}

void AStageTeleportTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	if (PawnRef && TeleportLocationActor)
	{
		PawnRef->StartTeleportationTimer(TeleportLocationActor->GetActorLocation(), TeleportDuration);
		
	}

	// If this teleport trigger volume doesn't know where to teleport the player, print a debug message:
	else if (TeleportLocationActor)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
			UKismetSystemLibrary::GetDisplayName(this) +
			FString(" doesn't reference a teleport location actor!"));
}

void AStageTeleportTriggerVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	if (PawnRef && !PawnRef->GetHasTeleported()) PawnRef->CancelTeleportTimer();

	
}

// Called every frame
void AStageTeleportTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AStageTeleportTriggerVolume::GetFadeToBlackMatRef_Implementation()
{
	
}

