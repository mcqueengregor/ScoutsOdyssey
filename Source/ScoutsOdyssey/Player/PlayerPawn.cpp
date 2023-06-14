// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include <string>

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene"));
	SceneComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh component"));
	MeshComponent->SetStaticMesh(SphereMesh.Object);
	MeshComponent->SetupAttachment(SceneComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm component"));
	SpringArmComponent->SetupAttachment(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->TargetArmLength = 150.0f;
	SpringArmComponent->bDoCollisionTest = false;

	HoriMoveSpeed = 100.0f;
	VertMoveSpeed = 100.0f;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, FString("Working!"));
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Overlapped with ") + UKismetSystemLibrary::GetDisplayName(OtherActor));;
}
