// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include <string>

#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"

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
		Material(TEXT("Material'/Game/Art/MeshMaterials/CharacterMaterials/M_ScoutAnimBase.M_ScoutAnimBase'"));
	
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
	
	HoriMoveSpeed = 200.0f;
	VertMoveSpeed = 200.0f;

	CameraTransitionDuration = 1.0f;
	bHasCameraAngleChangedAlready = false;
	LastEnteredSection = nullptr;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Setup dynamic material instances:
	TempCreateDynamicMaterial();

	CurrentGameTime = 0.0f;
	OriginalMeshScale = MeshComponent->GetComponentScale();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentGameTime += DeltaTime;
	
	TempCalculateLocalAnimTime();
	
	UE_LOG(LogTemp, Warning, TEXT("Moving player..."));
	
	// Update animation based on this frame's movement vector:
	if (MovementDirection.IsNearlyZero())
		TempChangeAnimation(FPlayerAnimation::IDLE);
	else
	{
		FVector NewLocation = GetActorLocation() + MovementDirection * DeltaTime;
		SetActorLocation(NewLocation);
		TempChangeAnimation(FPlayerAnimation::WALK);
		
		// Flip sprite mesh based on horizontal movement direction:
		FVector NewScale = OriginalMeshScale;
		NewScale.X *= MovementDirection.Y < 0 ? -OriginalMeshScale.X : OriginalMeshScale.X;
		MeshComponent->SetRelativeScale3D(FVector(
			MovementDirection.Y < 0 ? -OriginalMeshScale.X : OriginalMeshScale.X,
			OriginalMeshScale.Y,
			OriginalMeshScale.Z));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Move direction: %s"), *MovementDirection.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White,
		MeshComponent->GetComponentScale().ToCompactString());
}
		
// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
}

void APlayerPawn::ChangeAnimation(FPlayerAnimation NewAnimation)
{
	// Change animation material instance, if one exists:
	if (!SpriteAnimations.Contains(NewAnimation))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			FString("ERROR: No material registered for ") + UEnum::GetValueAsString(NewAnimation));
		
		// Default to IDLE animation if no material exists for the requested animation state:
		CurrentAnimation = SpriteAnimations.Find(FPlayerAnimation::IDLE);
		MeshComponent->SetMaterial(0, CurrentAnimation->AnimationMaterial);
		return;
	}

	// If the animation set to play on this frame is different from the currently-used one, update material:
	if (NewAnimation != CurrentAnimation->AnimationType)
	{
		CurrentAnimation = SpriteAnimations.Find(NewAnimation);
		MeshComponent->SetMaterial(0, CurrentAnimation->AnimationMaterial);
	}
	
	// TODO: If there's additional logic for playing back certain animations (e.g. only playing an animation once
	// before reverting to the previous one), put it here!
}

void APlayerPawn::MoveRight(float Value)
{
	MovementDirection.Y = FMath::Clamp(Value, -1.0f, 1.0f) * HoriMoveSpeed;
}

void APlayerPawn::MoveForward(float Value)
{
	MovementDirection.X = FMath::Clamp(Value, -1.0f, 1.0f) * VertMoveSpeed;
}

void APlayerPawn::CreateDynamicAnimationMaterials()
{
	for (const auto& AnimMaterial : AnimationMaterialList)
	{
		UMaterialInterface* MaterialInterface = AnimMaterial.Value;
		UMaterialInstanceDynamic* NewDynamicMat = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		
		// Log sprite animation details used in local animation timeline calculations:
		FSpriteAnimDetails NewSpriteAnimDetails;
		NewSpriteAnimDetails.AnimationMaterial = NewDynamicMat;
		NewSpriteAnimDetails.PlaybackFramerate = NewDynamicMat->K2_GetScalarParameterValue("PlaybackFramerate");
		NewSpriteAnimDetails.NumColumns = NewDynamicMat->K2_GetScalarParameterValue("NumSpritesheetColumns");
		NewSpriteAnimDetails.NumRows = NewDynamicMat->K2_GetScalarParameterValue("NumSpritesheetRows");
		NewSpriteAnimDetails.NumEmptyFrames = NewDynamicMat->K2_GetScalarParameterValue("NumEmptyFrames"); 
		NewSpriteAnimDetails.AnimationType = AnimMaterial.Key;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,FString::Printf(TEXT("%i, %i, %i"),
				NewSpriteAnimDetails.PlaybackFramerate, NewSpriteAnimDetails.NumColumns, NewSpriteAnimDetails.NumColumns));
		
		SpriteAnimations.Add(AnimMaterial.Key, NewSpriteAnimDetails);
	}

	CurrentAnimation = SpriteAnimations.Find(FPlayerAnimation::IDLE);
	MeshComponent->SetMaterial(0, CurrentAnimation->AnimationMaterial);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString::Printf(TEXT("Created %i dynamic material instances!"), SpriteAnimations.Num()));
}

void APlayerPawn::CalculateLocalAnimTime()
{
	const int32 NumSpriteCells = CurrentAnimation->NumColumns * CurrentAnimation->NumRows;
	const int32 NumSprites = NumSpriteCells - CurrentAnimation->NumEmptyFrames;

	// Calculate normalised local animation time, adjust for empty frames in spritesheet:
	float LocalTimeNorm = (CurrentAnimation->PlaybackFramerate / static_cast<float>(NumSpriteCells)) * CurrentGameTime;
	float AdjustedLocalTimeNorm = FGenericPlatformMath::Fmod(LocalTimeNorm, FMath::Max(NumSprites / static_cast<float>(NumSpriteCells), 1e-5f));
	
	if (CurrentAnimation)
		CurrentAnimation->AnimationMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", AdjustedLocalTimeNorm);
}

// TEMPORARY FUNCTIONS: ------------------------------------------------------------------------------------------------
void APlayerPawn::TempCreateDynamicMaterial()
{
	UE_LOG(LogTemp, Warning, TEXT("Using temp anim init!"));
	
	if (UberAnimMaterial)
		UberSpriteAnimDetails.AnimationMaterial = UMaterialInstanceDynamic::Create(UberAnimMaterial, this);

	UMaterialInstanceDynamic* NewMat = UberSpriteAnimDetails.AnimationMaterial;
	UberSpriteAnimDetails.IdleNumRows = 2;
	UberSpriteAnimDetails.IdleNumColumns = 2;
	UberSpriteAnimDetails.WalkNumRows = NewMat->K2_GetScalarParameterValue("WalkNumSpritesheetRows");
	UberSpriteAnimDetails.WalkNumColumns = NewMat->K2_GetScalarParameterValue("WalkNumSpritesheetColumns");
	UberSpriteAnimDetails.IdleNumEmptyFrames = 1;
	UberSpriteAnimDetails.WalkNumEmptyFrames = 0;
	UberSpriteAnimDetails.PlaybackFramerate = 3;

	TempCurrentAnimation = FPlayerAnimation::IDLE;
	const int32 EnumAsInt = static_cast<int32>(TempCurrentAnimation);
	const float IntAsFloat = static_cast<float>(EnumAsInt);

	MeshComponent->SetMaterial(0, UberSpriteAnimDetails.AnimationMaterial);
	NewMat->SetScalarParameterValue("CurrentAnimIndex", IntAsFloat);
}

void APlayerPawn::TempChangeAnimation(FPlayerAnimation NewAnimation)
{
	UE_LOG(LogTemp, Warning, TEXT("Using temp anim change!"));

	const int32 EnumAsInt = static_cast<int32>(NewAnimation);
	const float IntAsFloat = NewAnimation == FPlayerAnimation::IDLE ? 0.0f : 1.0f;
	
	UberSpriteAnimDetails.AnimationMaterial->SetScalarParameterValue("CurrentAnimIndex", IntAsFloat);
	TempCurrentAnimation = NewAnimation;
}

void APlayerPawn::TempCalculateLocalAnimTime()
{
	UE_LOG(LogTemp, Warning, TEXT("Using temp calculate local anim time!"));
	
	const int32 AnimNumRows = TempCurrentAnimation == FPlayerAnimation::IDLE ?
		UberSpriteAnimDetails.IdleNumRows :
		UberSpriteAnimDetails.WalkNumRows;
	const int32 AnimNumColumns = TempCurrentAnimation == FPlayerAnimation::IDLE ?
		UberSpriteAnimDetails.IdleNumColumns :
		UberSpriteAnimDetails.WalkNumColumns;
	const int32 AnimNumEmpty = TempCurrentAnimation == FPlayerAnimation::IDLE ?
		UberSpriteAnimDetails.IdleNumEmptyFrames :
		UberSpriteAnimDetails.WalkNumEmptyFrames;
	
	const int32 NumSpriteCells = AnimNumColumns * AnimNumRows;
	const int32 NumSprites = NumSpriteCells - AnimNumEmpty;

	// Calculate normalised local animation time, adjust for empty frames in spritesheet:
	float LocalTimeNorm = (UberSpriteAnimDetails.PlaybackFramerate / static_cast<float>(NumSpriteCells)) * CurrentGameTime;
	float AdjustedLocalTimeNorm = FGenericPlatformMath::Fmod(LocalTimeNorm,
		FMath::Max(NumSprites / static_cast<float>(NumSpriteCells), 1e-5f));

	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green,
		FString::Printf(TEXT("%f"), AdjustedLocalTimeNorm));
	
	UberSpriteAnimDetails.AnimationMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", AdjustedLocalTimeNorm);
}
