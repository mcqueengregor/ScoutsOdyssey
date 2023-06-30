// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Level/StageSectionVolume.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Animation/SpriteAnimationDataAsset.h"
#include "PlayerPawn.generated.h"

// NOTE: Order of animations with items MUST match order of items in FCurrentItem!
UENUM(BlueprintType)
enum class FPlayerAnimation : uint8
{
	IDLE = 0				UMETA(DisplayName = "Idle animation"),
	WALK = 1				UMETA(DisplayName = "Walk animation"),
	IDLE_WITH_HAMMER = 2 	UMETA(DisplayName = "Idle with hammer animation"),
	WALK_WITH_HAMMER = 3 	UMETA(DisplayName = "Walk with hammer animation"),
};

USTRUCT(BlueprintType)
struct FSpriteAnimDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USpriteAnimationDataAsset* SpriteAnimDA;
	FPlayerAnimation AnimationType;
};

UENUM()
enum class FCurrentItem : uint8
{
	EMPTY = 0	UMETA(DisplayName = "Holding no item"),
	HAMMER = 1	UMETA(DisplayName = "Holding Philbert's hammer"),
	// TODO: Add more items!
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

	UFUNCTION(BlueprintCallable)
	void ChangeItem(FCurrentItem NewItem);
	
protected:
	// Action/axis methods:
	void MoveRight(float Value);
	void MoveForward(float Value);
	void InteractWhileHoldingItem();
	
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
	FCurrentItem CurrentHeldItemType;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxColliderComponent;
	
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

private:
	FVector MovementDirection;	// Direction the player will move on the current frame, in Unreal units.
	FVector OriginalMeshScale;	// The scale of 'MeshComponent' when BeginPlay is triggered.
	float CurrentGameTime;	// The amount of time that has passed since the game started, in seconds.
};
