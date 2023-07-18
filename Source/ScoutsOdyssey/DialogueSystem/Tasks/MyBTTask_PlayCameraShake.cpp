// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_PlayCameraShake.h"
#include "ScoutsOdyssey/Stage/StageDelegates.h"

void UMyBTTask_PlayCameraShake::OnCameraShake_Broadcast()
{
	StageDelegates::OnCameraShake.Broadcast();
}
