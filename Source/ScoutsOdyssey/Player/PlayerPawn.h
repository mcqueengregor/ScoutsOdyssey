// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Level/StageSectionVolume.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SCOUTSODYSSEY_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveRight(float Value);
	void MoveForward(float Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HoriMoveSpeed;	// Left/right speed in Unreal units per second.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VertMoveSpeed;	// Front/back speed in Unreal units per second.

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CameraTransitionDuration;	// The amount of time a camera angle transition takes, in seconds.
	
	bool bHasCameraAngleChangedAlready;	// Whether or not the camera component has changed location already.
										// Set to 'false' on first frame of gameplay which triggers instant location
										// change, then set to 'true' which uses EaseInOut transitions.
	
	class TDoubleLinkedList<AStageSectionVolume*> OverlappedStageSections;	// List of currently-overlapping stage
																			// sections, used to accurately determine
																			// the current camera angle to use.

	AStageSectionVolume* LastEnteredSection;	// Pointer to stage section that was most-recently entered.
};
