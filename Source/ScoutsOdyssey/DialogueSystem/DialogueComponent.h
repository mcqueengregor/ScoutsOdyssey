// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UDialogueComponent : public UActorComponent, public IClickable
{
	GENERATED_BODY()

public:	
	UDialogueComponent();

	UFUNCTION()
	virtual void Click_Implementation(UPrimitiveComponent* TouchedComponent ,FKey ButtonPressed) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
};


