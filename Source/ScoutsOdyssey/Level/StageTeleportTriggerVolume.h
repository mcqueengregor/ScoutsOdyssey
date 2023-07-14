// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageTeleportTriggerVolume.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AStageTeleportTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageTeleportTriggerVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* TriggerVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attributes")
	AActor* TeleportLocationActor;	// Reference to empty actor which simply describes where to teleport to.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float TeleportDuration;			// How much time passes between the player entering the volume and being
									// teleported, in seconds.
	
	UFUNCTION(BlueprintNativeEvent)
	void GetFadeToBlackMatRef();	// BP function which retrieves reference to "fade to black" post process material.
	
private:
	UMaterial* FadeToBlackMaterial;
	bool bIsPlayingFadeToBlack;		// Flag which toggles fading from SceneColor to black or from black to SceneColour.
};
