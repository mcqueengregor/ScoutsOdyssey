// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSectionVolume.h"

// Sets default values
AStageSectionVolume::AStageSectionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageSectionVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AStageSectionVolume::OnOverlapBegin);
}

// Called every frame
void AStageSectionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageSectionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// If an actor overlaps with this stage section and that actor is the player character,
	// grab its camera component and sweep it to this stage section's camera angle:
	
}

