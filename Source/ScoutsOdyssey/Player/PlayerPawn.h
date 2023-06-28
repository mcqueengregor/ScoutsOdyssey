// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Level/StageSectionVolume.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Animation/SpriteAnimationDataAsset.h"
#include "PlayerPawn.generated.h"

UENUM(BlueprintType)
enum class FPlayerAnimation : uint8
{
	IDLE = 0 UMETA(DisplayName = "Idle animation"),
	WALK = 1 UMETA(DisplayName = "Walk animation"),
};

USTRUCT(BlueprintType)
struct FSpriteAnimDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USpriteAnimationDataAsset* SpriteAnimDA;
	FPlayerAnimation AnimationType;
};

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

	UFUNCTION(BlueprintCallable)
		void ChangeAnimation(FPlayerAnimation NewAnimation);
	
protected:
	// Action/axis methods:
	void MoveRight(float Value);
	void MoveForward(float Value);

	// Animation methods:
	void CreateDynamicAnimationMaterial();
	void UpdateDynamicMaterialParameters();
	void CalculateLocalAnimTime();

	UPROPERTY(EditAnywhere)
	TMap<FPlayerAnimation, FSpriteAnimDetails> AnimationsList;	// List of details for each animation, exposed in the
																// editor to be populated by a designer. Used to update
																// a dynamic material instance when playing and/or
																// switching animations.
	UMaterialInstanceDynamic* DynamicMaterial;
	FSpriteAnimDetails* CurrentAnimation;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxColliderComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float HoriMoveSpeed;	// Left/right speed in Unreal units per second.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float VertMoveSpeed;	// Front/back speed in Unreal units per second.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float CameraTransitionDuration;	// The amount of time a camera angle transition takes, in seconds.
	
	bool bHasCameraAngleChangedAlready;	// Whether or not the camera component has changed location already.
										// Set to 'false' on first frame of gameplay which triggers instant location
										// change, then set to 'true' which uses EaseInOut transitions.
	
	class TDoubleLinkedList<AStageSectionVolume*> OverlappedStageSections;	// List of currently-overlapping stage
																			// sections, used to accurately determine
																			// the current camera angle to use.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc.")
	AStageSectionVolume* LastEnteredSection;	// Pointer to stage section that was most-recently entered.

private:
	FVector MovementDirection;	// Direction the player will move on the current frame, in Unreal units.
	FVector OriginalMeshScale;	// The scale of 'MeshComponent' when BeginPlay is triggered.
	float CurrentGameTime;	// The amount of time that has passed since the game started, in seconds.
};
