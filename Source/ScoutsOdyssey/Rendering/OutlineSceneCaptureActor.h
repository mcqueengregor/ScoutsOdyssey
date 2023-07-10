// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OutlineSceneCaptureActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AOutlineSceneCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOutlineSceneCaptureActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void ResizeRenderTarget(FViewport* Viewport, uint32 val);

	UPROPERTY(BlueprintReadOnly)
		class APlayerPawn* PlayerPawnRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneCaptureComponent2D* SceneCaptureComponent;

	FIntPoint CurrentViewportDimensions;
};
