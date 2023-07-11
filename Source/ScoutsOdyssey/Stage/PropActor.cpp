// Fill out your copyright notice in the Description page of Project Settings.


#include "PropActor.h"
#include "StageDelegates.h"

// Sets default values
APropActor::APropActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APropActor::BeginPlay()
{
	Super::BeginPlay();
	StageDelegates::OnCameraShake.AddUObject(this, &APropActor::OnCameraShake);
}

// Called every frame
void APropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

