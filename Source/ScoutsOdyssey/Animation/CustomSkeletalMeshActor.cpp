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
	OverlapBoxCollider->SetupAttachment(RootComponent);

	BlockingBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocking box collider"));
	BlockingBoxCollider->SetupAttachment(RootComponent);
}

void ACustomSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* Mesh = GetSkeletalMeshComponent();

	// If no skeletal mesh has been assigned, output error message:
	if (!Mesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			UKismetSystemLibrary::GetDisplayName(GetOwner()) + FString(" has no mesh assigned!"));
		return;
	}

	UMaterialInterface* MatInterface = Mesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MatInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);

	UAnimInstance* BaseAnimInstance = Mesh->GetAnimInstance();
	AnimInstance = Cast<UCustomAnimInstance>(BaseAnimInstance);

	// If animation BP on mesh doesn't inherit from CustomAnimInstance, output error message:
	if (!AnimInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			UKismetSystemLibrary::GetDisplayName(GetSkeletalMeshComponent()) + 
			FString("'s ABP doesn't inherit from CustomAnimInstance!"));

		if (BaseAnimInstance)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald,
			FString("Able to access base AnimInstance though!"));
		
		return;
	}

	OnActorBeginOverlap.AddDynamic(this, &ACustomSkeletalMeshActor::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACustomSkeletalMeshActor::OnOverlapEnd);
}

void ACustomSkeletalMeshActor::ToggleAnimationPlayback()
{
	if (AnimInstance)
		AnimInstance->ToggleAnimationPlayback();
}

void ACustomSkeletalMeshActor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	// If player overlapped this object, turn on pulsing glow effect:
	if (PawnRef && DynamicMaterial)
		DynamicMaterial->SetScalarParameterValue("PulseEmissionStrength", 1.0f);

	if (AnimInstance) AnimInstance->ToggleAnimationPlayback();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("Overlap begin!"));
}

void ACustomSkeletalMeshActor::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	APlayerPawn* PawnRef = Cast<APlayerPawn>(OtherActor);

	// If player stopped overlapping this object, turn off pulsing glow effect:
	if (PawnRef && DynamicMaterial)
		DynamicMaterial->SetScalarParameterValue("PulseEmissionStrength", 0.0f);

	if (AnimInstance) AnimInstance->ToggleAnimationPlayback();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("Overlap end!"));
}

