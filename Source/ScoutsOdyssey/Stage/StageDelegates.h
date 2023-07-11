#pragma once

namespace StageDelegates
{
	DECLARE_MULTICAST_DELEGATE(FOnCameraShake);
	
	extern FOnCameraShake OnCameraShake;
};
