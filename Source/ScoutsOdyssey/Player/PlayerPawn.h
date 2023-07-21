// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Animation/SpriteAnimationDataAsset.h"
#include "../Level/StageTeleportTriggerVolume.h"
#include "PlayerPawn.generated.h"

// NOTE: Order of animations with items MUST match order of items in ECurrentItem!
UENUM(BlueprintType)
enum class EPlayerAnimation : uint8
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
	EPlayerAnimation AnimationType;
};

UENUM()
enum class ECurrentItem : uint8
{
	EMPTY = 0	UMETA(DisplayName = "Holding no item"),
	HAMMER = 1	UMETA(DisplayName = "Holding Philbert's hammer"),
	// TODO: Add more items!
};

UENUM()
enum class ECurrentInteraction : uint8
{
	NO_INTERACTION = 0		UMETA(DisplayName = "Shrugging (invalid interaction)"),
	SMASH_GREENHOUSE = 1	UMETA(DisplayName = "Smashing greenhouse with hammer"),
	COLLECT_HONEY = 2		UMETA(DisplayName = "Collect honey in boot"),
	HIT_TREE = 3			UMETA(DisplayName = "Hit tree with hammer"),
	THROW_ACORN = 4			UMETA(DisplayName = "Throw acorn"),
	PLACE_BOOT = 5			UMETA(DisplayName = "Placing honey boot at log"),

	SUCCESS_NO_ANIM = 0xFF,	// Used when the interaction is successful, but has no animation associated with it.
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
	void ChangeAnimation(EPlayerAnimation NewAnimation);

	UFUNCTION(BlueprintCallable)
	void ChangeItem(ECurrentItem NewItem);

	UFUNCTION(BlueprintCallable)
	void StartTeleportationTimer(FVector LocationToTeleportTo, float TeleportWaitTime);

	UFUNCTION(BlueprintCallable)
	void CancelTeleportTimer();
	
protected:
	// Action/axis methods:
	void MoveRight(float Value);
	void MoveForward(float Value);
	void InteractWhileHoldingItem();
	
	// Animation methods:
	void CreateDynamicAnimationMaterial();
	void UpdateDynamicMaterialParameters();
	void CalculateLocalAnimTime();
	void CalculateChangeItemLocalAnimTime(float DeltaTime);
	void CalculateInteractLocalAnimTime(float DeltaTime);

	// Teleportation methods:
	void Teleport(FVector TeleportLocation);

	// Audio control methods:
	UFUNCTION()
	void OnAudioFinishPlaying();
	
	void StartFootstepSoundCycle();
	void StopFootstepSoundCycle();
	void PlayFootstepSoundCue();
	
	// Utility methods:
	bool IsCurrentAnimOfType(FPlayerAnimation BaseAnimType);	// Returns whether the current animation is a variant
																// of the passed-in type, e.g. if the current animation
																// is IDLE_WITH_HAMMER, passing in IDLE would return 'true'.
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EPlayerAnimation, FSpriteAnimDetails> AnimationsList;	// List of data assets for IDLE and WALK animations,
																// exposed in the editor to be populated by a designer.
																// Used to update a dynamic material instance when
																// playing and/or switching animations.
	
	UPROPERTY(EditDefaultsOnly)
	TMap<ECurrentItem, FSpriteAnimDetails> ChangeItemAnimationsList;	// As above, but for animations used when
																		// changing items.

	UPROPERTY(EditDefaultsOnly)
	TMap<ECurrentInteraction, FSpriteAnimDetails> InteractAnimationsList;	// As above, but for animations used when
																			// performing interactions.
	
	UMaterialInstanceDynamic* DynamicMaterial;
	FSpriteAnimDetails* CurrentAnimation;
	ECurrentItem CurrentHeldItemType;
	ECurrentItem PreviouslyHeldItemType;
	
public:
	// COMPONENTS:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxColliderComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* AudioComponent;
	
	// AUDIO CLIPS:
	// TODO: Abstract audio playback functionality into AudioManager, instead of bloating actor classes with this logic?
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* FootstepSoundCue;
	
	// ATTRIBUTES:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float MoveSpeed;	// Movement speed in Unreal units per second.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float CameraTransitionDuration;	// The amount of time a camera angle transition takes, in seconds.
	
	// MISC.
	bool bHasCameraAngleChangedAlready;	// Whether or not the camera component has changed location already.
										// Set to 'false' on first frame of gameplay which triggers instant location
										// change, then set to 'true' which uses EaseInOut transitions.
	
	class TDoubleLinkedList<class AStageSectionVolume*> OverlappedStageSections;	// List of currently-overlapping stage
																			// sections, used to accurately determine
																			// the current camera angle to use.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc.")
		class AStageSectionVolume* LastEnteredSection;	// Pointer to stage section that was most-recently entered.
	
	// GETTERS & SETTERS:
	inline bool GetIsChangingItem() { return bIsChangingItem; }
	inline bool GetHasTeleported()	{ return bHasTeleported; }
	
	inline void SetLastTeleportVolumeEntered(AStageTeleportTriggerVolume* Volume) { LastTriggerVolumeEntered = Volume; }
	
private:
	FVector MovementDirection;	// Direction the player will move on the current frame, in Unreal units.
	FVector OriginalMeshScale;	// The scale of 'MeshComponent' when BeginPlay is triggered.
	
	float CurrentGameTime;		// The amount of time that has passed since the game started, in seconds.
	
	bool bIsChangingItem;		// Flag for indicating if the current animation is a "switch item" animation.
	float ChangeItemLocalTime;	// Whereabouts in the animation spritesheet is being drawn onto the player plane.

	bool bIsInteracting;		// Flag for indicating if the player is in the middle of interacting with a scene prop.
	float InteractLocalTime;
	
	bool bHasTeleported;		// Flag for indicating if the player has teleported between stages before entering a
								// new one (set to 'true' when teleport occurs, then 'false' when player re-enters vol).
	FTimerHandle TeleportTimerHandle;
	FTimerHandle FootstepSoundTimerHandle;

	AStageTeleportTriggerVolume* LastTriggerVolumeEntered;
};

