// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ActorComponent.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueEnums.h"
#include "Blueprint/UserWidget.h"
#include "DialogueComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpeakFinish);
DECLARE_MULTICAST_DELEGATE(FOnChoiceFinish);
DECLARE_MULTICAST_DELEGATE(FOnDialogueEnd);


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

	// Widget Utility Methods
	void SwitchToBubble(EBubble Bubble) const;
	void SwitchBubbleOneState(EBubbleState BubbleState) const;
	void SetTextBlockText(const FText& Text, const UUserWidget& Parent) const;
	void ConstructListView(const FText& Text);
	
	// Behavior Tree
	UPROPERTY(EditInstanceOnly, Category=Dialogue)
	UBehaviorTree* DialogueTree;

	// Tasks Setup Methods
	void Speak(const FText& Text, const EBubble Bubble) const;
	void Choice(TArray<FText>& Choices);
	
	// Delegates
	FOnSpeakFinish OnSpeakFinish;
	FOnChoiceFinish OnChoiceFinish;
	FOnDialogueEnd OnDialogueEnd;
	void SpeakFinish() const;
	
protected:
	virtual void BeginPlay() override;

private:
	// SetUp Methods
	void Clickable_SetUp();
	void Widget_SetUp();
	void BehaviorTree_SetUp();
	void Delegate_SetUp();

	void DialogueEnd_CleanUp() const;
	
	class AMyPlayerController* MyPlayerController;
	class AAIController* AIController;
};



