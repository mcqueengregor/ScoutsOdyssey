// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include "../Level/StageSectionVolume.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/LoggingMacros.h"
#include "ScoutsOdyssey/Components/InteractComponentBase.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"
#include "UnrealAudio/Private/UnrealAudioDeviceFormat.h"
#include "Sound/SoundCue.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	BoxColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box collider component"));
	BoxColliderComponent->SetSimulatePhysics(true);
	BoxColliderComponent->SetCollisionProfileName(TEXT("Pawn"));
	BoxColliderComponent->SetBoxExtent(FVector(20.0f, 50.0f, 60.0f));	// Scale box collider to match scout's proportions.
	BoxColliderComponent->BodyInstance.bLockXRotation = true;
	BoxColliderComponent->BodyInstance.bLockYRotation = true;
	BoxColliderComponent->BodyInstance.bLockZRotation = true;
	BoxColliderComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/Art/MeshMaterials/MaterialLibrary/BaseMaterials/M_ScoutAnimBase.M_ScoutAnimBase'"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh component"));
	MeshComponent->SetStaticMesh(PlaneMesh.Object);
	MeshComponent->SetMaterial(0, Material.Object);
	MeshComponent->SetRelativeLocation(FVector(-15.0f, 0.0f, 0.0f));	// Place sprite plane near front of box collider.
	MeshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));	// Rotate sprite plane to face camera.
	MeshComponent->SetRelativeScale3D(FVector(1.0f, 1.2f, 1.0f));	// Scale plane to match scout's proportions.
	MeshComponent->SetupAttachment(BoxColliderComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm component"));
	SpringArmComponent->TargetArmLength = 150.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetupAttachment(BoxColliderComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera component"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	static ConstructorHelpers::FObjectFinder<USoundCue>
		FootstepSC(TEXT("SoundCue'/Game/Audio/SFX/Footstep/SC_GrassFootstep.SC_GrassFootstep'"));
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio component"));
	AudioComponent->SetAutoActivate(false);
	if (FootstepSC.Object)
		AudioComponent->SetSound(FootstepSC.Object);
	AudioComponent->SetupAttachment(RootComponent);
	
	MoveSpeed = 200.0f;

	CameraTransitionDuration = 1.0f;
	bHasCameraAngleChangedAlready = false;
	LastEnteredSection = nullptr;
	bIsChangingItem = false;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Setup dynamic material instances:
	CreateDynamicAnimationMaterial();

	CurrentGameTime = 0.0f;
	OriginalMeshScale = MeshComponent->GetComponentScale();
	CurrentHeldItemType = ECurrentItem::EMPTY;
	CurrentAnimation = AnimationsList.Find(EPlayerAnimation::IDLE);
	ChangeAnimation(EPlayerAnimation::IDLE);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentGameTime += DeltaTime;
	
	// Update animation based on this frame's movement vector, if the player isn't changing item or in an interaction:
	if (!bIsChangingItem && !bIsInteracting)
	{
		if (MovementDirection.IsNearlyZero())
		{
			ChangeAnimation(EPlayerAnimation::IDLE);
		}
		else
		{
			FVector NewLocation = GetActorLocation() + MovementDirection * MoveSpeed * DeltaTime;
			SetActorLocation(NewLocation);
			ChangeAnimation(EPlayerAnimation::WALK);
		
			// Flip sprite mesh based on horizontal movement direction:
			FVector NewScale = OriginalMeshScale;
			NewScale.X *= MovementDirection.Y < 0 ? -OriginalMeshScale.X : OriginalMeshScale.X;
			MeshComponent->SetRelativeScale3D(FVector(
				MovementDirection.Y < 0 ? -OriginalMeshScale.X : OriginalMeshScale.X,
				OriginalMeshScale.Y,
				OriginalMeshScale.Z));
		}
		CalculateLocalAnimTime();
	}
	// If the player is currently playing an "interact with scene" animation (e.g. shrugging, swinging
	// hammer, throwing acorn, etc.), update that animation until the interaction has completed:
	else if (bIsInteracting)
	{
		CalculateInteractLocalAnimTime(DeltaTime);
	}
	// If the player is currently playing a "switch item" animation, figure out
	// which sprite in the spritesheet should be displayed:
	else
	{
		CalculateChangeItemLocalAnimTime(DeltaTime);
	}

	// Reset movement speed so that turning off input axis calls doesn't force player to continue moving:
	MovementDirection = FVector(0.0f);
}
		
// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this,
		&APlayerPawn::InteractWhileHoldingItem);

	UInventoryComponent* InventoryComponent =
		Cast<UInventoryComponent>(UInventoryComponent::StaticClass());

	if (InventoryComponent)
		PlayerInputComponent->BindAxis("MouseScroll", InventoryComponent, &UInventoryComponent::SwitchItem);
}

void APlayerPawn::ChangeAnimation(EPlayerAnimation NewAnimation)
{
	const int32 EnumAsInt = static_cast<int32>(NewAnimation);
	const EPlayerAnimation NewAnimWithItem =
		static_cast<EPlayerAnimation>(EnumAsInt + 2 * static_cast<int32>(CurrentHeldItemType));
	
	// Change animation material instance, if one exists:
	if (!AnimationsList.Contains(NewAnimWithItem))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			FString("ERROR: No details registered for ") + UEnum::GetValueAsString(NewAnimation));
		
		// Default to IDLE animation if no animation details exist for the requested animation state:
		CurrentAnimation = AnimationsList.Find(EPlayerAnimation::IDLE);
		UpdateDynamicMaterialParameters();
		return;
	}

	// If the animation set to play on this frame is different from the currently-used one, update material:
	if (NewAnimWithItem != CurrentAnimation->AnimationType || bIsInteracting)
	{
		CurrentGameTime = 0;
		
		CurrentAnimation = AnimationsList.Find(NewAnimWithItem);
		UpdateDynamicMaterialParameters();

		if (IsCurrentAnimOfType(EPlayerAnimation::WALK))
			StartFootstepSoundCycle();
		else if (IsCurrentAnimOfType(EPlayerAnimation::IDLE))
			StopFootstepSoundCycle();
	}
}

void APlayerPawn::ChangeItem(ECurrentItem NewItem)
{
	PreviouslyHeldItemType = CurrentHeldItemType;
	CurrentHeldItemType = NewItem;
	
	bIsChangingItem = true;
	ChangeItemLocalTime = -1.0f;
}

void APlayerPawn::MoveRight(float Value)
{
	MovementDirection.Y = FMath::Clamp(Value, -1.0f, 1.0f);
}

void APlayerPawn::MoveForward(float Value)
{
	MovementDirection.X = FMath::Clamp(Value, -1.0f, 1.0f);
}

void APlayerPawn::InteractWhileHoldingItem()
{
	TArray<AActor*> OverlappingSceneProps;
	GetOverlappingActors(OverlappingSceneProps, ADialogueMeshActor::StaticClass());
	
	// If standing next to enough scene props, check for interaction component and call OnInteractWithItem on it:
	if (OverlappingSceneProps.Num() >= 1)
	{
		UInventoryComponent* InventoryComponent =
			Cast<UInventoryComponent>(GetComponentByClass(UInventoryComponent::StaticClass()));

		if (InventoryComponent && InventoryComponent->GetCurrentItem())
		{
			UInteractComponentBase* ScenePropInteractComp =
				Cast<UInteractComponentBase>(OverlappingSceneProps[0]->GetComponentByClass(UInteractComponentBase::StaticClass()));

			// If a nearby item has an interaction component associated with it, perform the interaction and update
			// player animation accordingly:
			if (ScenePropInteractComp)
			{
				ECurrentInteraction interactType =
					ScenePropInteractComp->OnInteractWithItem(InventoryComponent->GetCurrentItem(), this);

				// If interaction was successful but there isn't an animation associated with it, do nothing:
				if (interactType == ECurrentInteraction::SUCCESS_NO_ANIM) return;

				// Change current sprite animation to the appropriate interaction anim:
				bIsInteracting = true;
				InteractLocalTime = 0.0f;
				
				CurrentAnimation = InteractAnimationsList.Find(interactType);
				UpdateDynamicMaterialParameters();
			}
		}
	}
}

void APlayerPawn::CreateDynamicAnimationMaterial()
{
	UMaterialInterface* MaterialInterface = MeshComponent->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	MeshComponent->SetMaterial(0, DynamicMaterial);

	CurrentAnimation = AnimationsList.Find(EPlayerAnimation::IDLE);

	// Log animation type enum as part of the details object:
	for (auto& Anim : AnimationsList)
		Anim.Value.AnimationType = Anim.Key;
}

void APlayerPawn::UpdateDynamicMaterialParameters()
{
	const USpriteAnimationDataAsset* CurrentAnimDetails = CurrentAnimation->SpriteAnimDA;
	
	DynamicMaterial->SetTextureParameterValue("AnimationSpritesheet", CurrentAnimDetails->SpritesheetTexture);
	DynamicMaterial->SetScalarParameterValue("NumSpritesheetRows", CurrentAnimDetails->NumSpritesheetRows);
	DynamicMaterial->SetScalarParameterValue("NumSpritesheetColumns", CurrentAnimDetails->NumSpritesheetColumns);
}

void APlayerPawn::CalculateLocalAnimTime()
{
	// If current animation is an IDLE one, use irregular blinking logic:
	if (static_cast<int32>(CurrentAnimation->AnimationType) % 2 == 0)
	{
		// Blinking periods - making these coprime ensures that there won't be any repetition.
		// Shamelessly stolen from https://www.youtube.com/watch?v=8--5LwHRhjk&t=775s:
		float BlinkPeriods[] = { 4.1f, 7.3f };

		for (int32 i = 0; i < 2; ++i)
		{
			float ModResults = FMath::Min(1.0f, FGenericPlatformMath::Fmod(CurrentGameTime, BlinkPeriods[i]));
			float SmoothResults = FMath::SmoothStep(0.0f, 0.1f, ModResults) -
				FMath::SmoothStep(0.18f, 0.4f, ModResults);
			BlinkPeriods[i] = SmoothResults;
		}
		float FinalLocalTimeNorm = FMath::Max(BlinkPeriods[0], BlinkPeriods[1]);
		if (DynamicMaterial) DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", FinalLocalTimeNorm);
		return;
	}

	const USpriteAnimationDataAsset* CurrentAnimDetails = CurrentAnimation->SpriteAnimDA;
    
	const int32 NumSpriteCells = CurrentAnimDetails->NumSpritesheetColumns * CurrentAnimDetails->NumSpritesheetRows;
	const int32 NumSprites = NumSpriteCells - CurrentAnimation->SpriteAnimDA->NumEmptyFrames;

	// Calculate normalised local animation time, adjust for empty frames in spritesheet:
	float LocalTimeNorm = (CurrentAnimDetails->PlaybackFramerate / static_cast<float>(NumSpriteCells)) * CurrentGameTime;
	float AdjustedLocalTimeNorm = FGenericPlatformMath::Fmod(LocalTimeNorm,
		FMath::Max(NumSprites / static_cast<float>(NumSpriteCells), 1e-5f));
    
	if (DynamicMaterial) DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", AdjustedLocalTimeNorm);
}

void APlayerPawn::CalculateChangeItemLocalAnimTime(float DeltaTime)
{
	const USpriteAnimationDataAsset* SpriteDA = ChangeItemLocalTime < 0.0f ?
		ChangeItemAnimationsList.Find(PreviouslyHeldItemType)->SpriteAnimDA :
		ChangeItemAnimationsList.Find(CurrentHeldItemType)->SpriteAnimDA;

	const int NumCells = SpriteDA->NumSpritesheetColumns * SpriteDA->NumSpritesheetRows;
	const int NumSprites = NumCells - SpriteDA->NumEmptyFrames;
	const float AnimDuration = static_cast<float>(NumSprites) / SpriteDA->PlaybackFramerate;
	
	const float AnimAdvanceAmount = DeltaTime / AnimDuration;
	ChangeItemLocalTime += AnimAdvanceAmount;

	// If change item animation has reached the end of the currently-held item's animation,
	// restore input and revert to IDLE animation:
	if (ChangeItemLocalTime >= 1.0f)
	{	
		bIsChangingItem = false;
		ChangeAnimation(EPlayerAnimation::IDLE);
		return;
	}
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetTextureParameterValue("AnimationSpritesheet", SpriteDA->SpritesheetTexture);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetRows", SpriteDA->NumSpritesheetRows);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetColumns", SpriteDA->NumSpritesheetColumns);
		
		// ("1 - LocalTime" is done because the pocket animations start at "holding" and end at "pocket")
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",1.0f - FMath::Abs(ChangeItemLocalTime) *
			(NumSprites / NumCells));
	}
}

void APlayerPawn::CalculateInteractLocalAnimTime(float DeltaTime)
{
	const USpriteAnimationDataAsset* SpriteDA = CurrentAnimation->SpriteAnimDA;
	const int NumCells = SpriteDA->NumSpritesheetColumns * SpriteDA->NumSpritesheetRows;
	const int NumSprites = NumCells - SpriteDA->NumEmptyFrames;
	const float AnimDuration = static_cast<float>(NumSprites) / SpriteDA->PlaybackFramerate;
	
	const float AnimAdvanceAmount = DeltaTime / AnimDuration;
	InteractLocalTime += AnimAdvanceAmount;

	if (InteractLocalTime >= 1.0f)
	{
		ChangeAnimation(EPlayerAnimation::IDLE);
		bIsInteracting = false;
		return;
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",FMath::Abs(InteractLocalTime) *
			(NumSprites / NumCells));
	}
}

void APlayerPawn::StartTeleportationTimer(FVector TeleportLocation, float TeleportWaitTime)
{
	FTimerDelegate TeleportDelegate = FTimerDelegate::CreateUObject(this, &APlayerPawn::Teleport, TeleportLocation);
	
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, TeleportDelegate,
		1.0f, false, TeleportWaitTime);
	bHasTeleported = false;
}

void APlayerPawn::CancelTeleportTimer()
{
	// TODO: This bool check is probably unnecessary since this function should only be called if (!bHasTeleported)
	if (!bHasTeleported)
		GetWorldTimerManager().ClearTimer(TeleportTimerHandle);			
}

void APlayerPawn::Teleport(FVector TeleportLocation)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Teleported!"));
	bHasTeleported = true;
	bHasCameraAngleChangedAlready = false;
	SetActorLocation(TeleportLocation);

	if (LastTriggerVolumeEntered)
		LastTriggerVolumeEntered->ForceFadeToSceneColour();
}

void APlayerPawn::OnAudioFinishPlaying()
{
	if (AudioComponent->Sound && IsCurrentAnimOfType(EPlayerAnimation::WALK))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, FString("Finished playing!"));
		AudioComponent->Play();
	}
}

void APlayerPawn::StartFootstepSoundCycle()
{
	const USpriteAnimationDataAsset* WalkAsset = AnimationsList.Find(EPlayerAnimation::WALK)->SpriteAnimDA;
	const float FootstepPlayRate = (1.0f / WalkAsset->PlaybackFramerate) *
		((WalkAsset->NumSpritesheetColumns * WalkAsset->NumSpritesheetRows) / 2.0f);
	
	GetWorldTimerManager().SetTimer(FootstepSoundTimerHandle, this, &APlayerPawn::PlayFootstepSoundCue,
		FootstepPlayRate, true, 0.0f);
}

void APlayerPawn::StopFootstepSoundCycle()
{
	GetWorldTimerManager().ClearTimer(FootstepSoundTimerHandle);
}

void APlayerPawn::PlayFootstepSoundCue()
{
	if (AudioComponent->Sound)
	{
		AudioComponent->Play();
	}
}

bool APlayerPawn::IsCurrentAnimOfType(EPlayerAnimation BaseAnimType)
{
	// TODO: This code is absolutely disgusting
	switch (BaseAnimType)
	{
	case EPlayerAnimation::IDLE:
		return static_cast<int32>(CurrentAnimation->AnimationType) % 2 == 0;
	case EPlayerAnimation::WALK:
		return static_cast<int32>(CurrentAnimation->AnimationType) % 2 == 1;

	default:
		return false;
	}
}
