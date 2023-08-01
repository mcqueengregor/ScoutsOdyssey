// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ActorComponent.h"
#include "ScoutsOdyssey/Interfaces/Clickable.h"
#include "DialogueEnums.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"
#include "DialogueComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpeakFinish);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChoiceFinish, int);
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
	virtual void Click_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;

	UFUNCTION(BlueprintCallable)
	bool StartDialogue();
	
	// Widgets
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> PlayerSpeechBubble_WidgetAsset;
	class USpeechBubbleUserWidget* BubbleOne;
	
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> NPCSpeechBubble_WidgetAsset;
	class USpeechBubbleUserWidget* BubbleTwo;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* SpeakerTwo;
	
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> NarratorSpeechBubble_WidgetAsset;
	class USpeechBubbleUserWidget* BubbleNarrator;
	UPROPERTY(EditAnywhere, Category=Widgets)
	FVector2D BubbleOneOffSet_PercentageViewPort;
	UPROPERTY(EditAnywhere, Category=Widgets)
	FVector2D BubbleTwoOffSet_PercentageViewPort;
	
	// Widget Utility Methods
	void SwitchToBubble(EBubble Bubble) const;
	void SwitchBubbleOneState(EBubbleState BubbleState) const;
	void SetTextBlockText(const FString& String, class UTextBlock& TextWidget) const;

	// Tasks Setup Methods
	void Speak(const FString& String, const EBubble Bubble, const EVoiceType VoiceType,  const int FontSize, const float TalkRate);
	void Choice(TArray<FText>& Choices);
	
	// Delegates
	FOnSpeakFinish OnSpeakFinish;
	FOnChoiceFinish OnChoiceFinish;
	FOnDialogueEnd OnDialogueEnd;

	// Only Trigger Once || Trigger repeatedly
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasTriggered = false;
	bool OnlyTriggerOnce = false;
	
	void SpeakFinish();
	UFUNCTION()
	void ChoiceFinish(const int ReplyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharacter;
	
protected:
	virtual void BeginPlay() override;

private:
	// SetUp Methods
	void Widget_SetUp();
	void Delegate_SetUp();

	// Clean up
	void DialogueEnd_CleanUp() const;

	// Typewriter Effect
	class UTextBlock* CurTextBlock;
	FString CurFullString;
	FString CurSpeakString;
	int CurChar_Index;
	FTimerHandle SpeakTimerHandle;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category=TypeWriterEffect)
	float LetterTypeRate = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category=TypeWriterEffect)
	TArray<USoundCue*> LowVoices;
	UPROPERTY(EditDefaultsOnly, Category=TypeWriterEffect)
	TArray<USoundCue*> HighVoices;
	void TypeNextLetter(class UTextBlock* TextBlock,  const FString& String, const EVoiceType VoiceType);
	void PlayRandomVoice(EVoiceType VoiceType) const;
	void PlayVoiceAtIndex(EVoiceType VoiceType, int Index) const;
	// Check number of clicks. First click would complete typewriting immediately, while second click would go to next string.
	// can be -1, 0 and 1. 
	int SpeakClickCount;
};