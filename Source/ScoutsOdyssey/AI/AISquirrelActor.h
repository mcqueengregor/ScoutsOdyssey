﻿#pragma once

#include "AIAnimationActor.h"
#include "AISquirrelActor.generated.h"

UCLASS()
class SCOUTSODYSSEY_API AAISquirrelActor : public AAIAnimationActor
{
	GENERATED_BODY()
	
public:
	virtual void ChangeAnimation(int Index) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAcornThrown;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	AnimationMap<FSquirrelAnimationState> AnimationMap;
};
