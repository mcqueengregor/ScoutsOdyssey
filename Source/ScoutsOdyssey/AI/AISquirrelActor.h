#pragma once

#include "AIActor.h"
#include "AISquirrelActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AAISquirrelActor : public AAIActor
{
	GENERATED_BODY()
	
public:
	virtual void ChangeAnimation(int Index) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	AnimationMap<FSquirrelAnimationState> AnimationMap;
};
