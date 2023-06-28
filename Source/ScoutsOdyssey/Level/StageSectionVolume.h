// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSectionVolume.generated.h"

UENUM(BlueprintType)
enum class ECameraType : uint8
{
	STATIC = 0 UMETA(DisplayName = "Static camera"),
	FOLLOW = 1 UMETA(DisplayName = "Follow camera"),
};

UCLASS()
class SCOUTSODYSSEY_API AStageSectionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageSectionVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	FViewTargetTransitionParams GetCameraTransitionParams(class APlayerPawn* const Pawn) const;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* StageSectionCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
		ECameraType CameraType;	// Whether or not this section's camera angle is static or follows the player.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
		float FollowCameraBounds;	// How close the camera can get to the edge of this section in FOLLOW mode
									// before it remains stationary, in Unreal units.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Misc.")
	APlayerPawn* PlayerPawnRef;	// Pointer reference to the player pawn (used for controlling follow camera location).
};
