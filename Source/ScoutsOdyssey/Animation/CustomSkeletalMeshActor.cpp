// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSkeletalMeshActor.h"

#include <solver/PxSolverDefs.h>

#include "CustomAnimInstance.h"
#include "MovieSceneSequenceID.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/Player/PlayerPawn.h"

ACustomSkeletalMeshActor::ACustomSkeletalMeshActor()
{
	OverlapBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap box collider"));
	OverlapBoxCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	OverlapBoxCollider->SetupAttachment(RootComponent);

	BlockingBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocking box collider"));
	BlockingBoxCollider->SetCollisionProfileName(TEXT("BlockAll"));
	BlockingBoxCollider->SetupAttachment(RootComponent);

	bHasBeenInteractedWith = false;
}

void ACustomSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* Mesh = GetSkeletalMeshComponent();
	if (Mesh)
	{
		AnimInstance = Cast<UCustomAnimInstance>(Mesh->GetAnimInstance());
	}
	
	OnActorBeginOverlap.AddDynamic(this, &ACustomSkeletalMeshActor::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACustomSkeletalMeshActor::OnOverlapEnd);
}

void ACustomSkeletalMeshActor::ToggleAnimationPlayback()
{
	if (AnimInstance)
	{
		AnimInstance->ToggleAnimationPlayback();

		if (!AnimInstance->GetIsPlayingForwards())
		{
			BlockingBoxCollider->SetCollisionProfileName(TEXT("NoCollision"));
			DisableInteractions();
		}
	}
}

void ACustomSkeletalMeshActor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	// If player overlapped this object, turn on pulsing glow effect:
	if (PawnRef && DynamicMaterial)
		DynamicMaterial->SetScalarParameterValue("PulseEmissionStrength", bHasBeenInteractedWith ? 0.0f : 1.0f);
}

void ACustomSkeletalMeshActor::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	// If player stopped overlapping this object, turn off pulsing glow effect:
	if (PawnRef && DynamicMaterial)
		DynamicMaterial->SetScalarParameterValue("PulseEmissionStrength", 0.0f);
}

UMaterialInstanceDynamic* ACustomSkeletalMeshActor::CreateAndAssignDynamicMaterial()
{
	USkeletalMeshComponent* Mesh = GetSkeletalMeshComponent();

	// If no skeletal mesh has been assigned, output error message:
	if (!Mesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			UKismetSystemLibrary::GetDisplayName(GetOwner()) + FString(" has no mesh assigned!"));
		return nullptr;
	}

	UMaterialInterface* MatInterface = Mesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MatInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);

	return DynamicMaterial;
}

void ACustomSkeletalMeshActor::DisableInteractions()
{
	bHasBeenInteractedWith = true;
	
	OverlapBoxCollider->SetGenerateOverlapEvents(false);
	OnActorBeginOverlap.RemoveDynamic(this, &ACustomSkeletalMeshActor::OnOverlapBegin);
	OnActorEndOverlap.RemoveDynamic(this, &ACustomSkeletalMeshActor::OnOverlapEnd);

	if (DynamicMaterial)
		DynamicMaterial->SetScalarParameterValue("PulseEmissionStrength", 0.0f);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald,
		FString("Interactions disabled!"));
}