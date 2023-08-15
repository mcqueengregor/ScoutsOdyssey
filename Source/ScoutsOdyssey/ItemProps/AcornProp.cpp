// Fill out your copyright notice in the Description page of Project Settings.


#include "AcornProp.h"

#include "Components/BoxComponent.h"

// Sets default values
AAcornProp::AAcornProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	
	BoxColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box collider"));
	BoxColliderComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BoxColliderComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxColliderComponent->SetSimulatePhysics(true);
	BoxColliderComponent->SetupAttachment(RootComponent);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Prop plane mesh"));
	StaticMeshComponent->SetStaticMesh(PlaneMesh.Object);
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetupAttachment(BoxColliderComponent);
}

// Called when the game starts or when spawned
void AAcornProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAcornProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAcornProp::StartDestroyTimer(float DestroyTime)
{
	FTimerHandle DestroyHandle;
	FTimerDelegate DestroyDelegate;

	DestroyDelegate.BindLambda([=](){
		Destroy();
	});
	
	GetWorldTimerManager().SetTimer(DestroyHandle, DestroyDelegate, 1.0f, false, DestroyTime);
}

