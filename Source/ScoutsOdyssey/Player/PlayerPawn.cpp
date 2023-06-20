// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
		Material(TEXT("Material'/Game/Art/MeshMaterials/M_Scout.M_Scout'"));
	
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

	// Setup dynamic material instance:
	UMaterialInterface* mat = MeshComponent->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(mat, this);
	MeshComponent->SetMaterial(0, DynamicMaterial);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float CurrentTimeSecs = UGameplayStatics::GetTimeSeconds(GetWorld()) * 5.0f;
	
	// Pulse material emission over time:
	DynamicMaterial->SetScalarParameterValue("EmissiveStrength",
		(FMath::Sin(CurrentTimeSecs) * 0.5f + 0.5f) * 10.0f);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
}

void APlayerPawn::MoveRight(float Value)
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	const FVector NewLocation = GetActorLocation() + FVector(0.0f, HoriMoveSpeed * Value * DeltaTime, 0.0f);
	SetActorLocation(NewLocation);
}

void APlayerPawn::MoveForward(float Value)
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	const FVector NewLocation = GetActorLocation() + FVector(HoriMoveSpeed * Value * DeltaTime, 0.0f, 0.0f);
	SetActorLocation(NewLocation);
}
