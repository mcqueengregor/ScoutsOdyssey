// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ActorComponent.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueComponent.generated.h"

UENUM()
enum EBubble
{
	One,
	Two,
	Narrator
};


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCOUTSODYSSEY_API UDialogueComponent : public UActorComponent, public IClickable
{
	GENERATED_BODY()

public:
	// Defaults
	UDialogueComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	virtual void Click_Implementation(UPrimitiveComponent* TouchedComponent ,FKey ButtonPressed) override;

	// Widgets
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> SpeechBubble_WidgetAsset;
	class USpeechBubbleUserWidget* BubbleOne;
	class USpeechBubbleUserWidget* BubbleTwo;
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> NarratorSpeechBubble_WidgetAsset;
	class USpeechBubbleUserWidget* BubbleNarrator;
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	FVector2D BubbleOneOffSet;
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	FVector2D BubbleTwoOffSet;

	// Behavior Tree
	UPROPERTY(EditInstanceOnly, Category=Dialogue)
	UBehaviorTree* DialogueTree;
	
protected:
	virtual void BeginPlay() override;

private:
	// SetUp Methods
	void ClickableSetUp();
	void WidgetSetUp();
	void BehaviorTreeSetUp();

	// Widget Utility Methods
	void SwitchToBubble(EBubble Bubble) const;

	void End();
	
	class AMyPlayerController* MyPlayerController;
	class AAIController* AIController;
};



