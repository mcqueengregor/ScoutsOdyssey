#pragma once
#include "AIActor.h"
#include "AIBearActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AAIBearActor : public AAIActor
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
