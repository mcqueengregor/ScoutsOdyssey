#pragma once
#include "AIAnimationActor.h"
#include "AIBearActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AAIBearActor : public AAIAnimationActor
{
	GENERATED_BODY()
	AAIBearActor();
	
public:
	virtual void ChangeAnimation(int Index) override;

	UAudioComponent* SFXAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHoneyBootPlaced;

	UPROPERTY(EditAnywhere)
	AActor* WallRef;
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	AnimationMap<FBearAnimationState> AnimationMap;
};
