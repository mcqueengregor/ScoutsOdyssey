// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractComponentBase.h"
#include "BushInteractComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UBushInteractComponent : public UInteractComponentBase
{
	GENERATED_BODY()
	UBushInteractComponent();


protected:
	virtual void BeginPlay() override;

public:
	virtual void DoTask() override;

	FVector StartScale = FVector(3);
	FVector EndScale = FVector(1);
	float TransformTime = 1;

private:
	UStaticMeshComponent* StaticMeshComponent;
	FTimerHandle TimerHandle;
	float TransformTimer;
};
