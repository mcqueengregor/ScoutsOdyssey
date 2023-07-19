#pragma once
#include "AIAnimationActor.h"
#include "AIBearActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AAIBearActor : public AAIAnimationActor
{
	GENERATED_BODY()
	
public:
	virtual void ChangeAnimation(int Index) override;

	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	AnimationMap<FBearAnimationState> AnimationMap;
};
