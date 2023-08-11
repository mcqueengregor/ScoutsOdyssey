// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoutsFunctionLibrary.h"

float UScoutsFunctionLibrary::GetEditorTime()
{
	// TL;DR: Unreal is a flawless piece of software with no bugs or lack of documentation whatsoever hahahaahah
	
	// For whatever reason, the "Time" node in material editors correspond to the elapsed time of the game
	// session (i.e. how long it's been since "Play" was pressed or the level was loaded in the executable), returning
	// the same value as the "Get Game Time In Seconds" BP node.  What Epic decided not to document, however, is that
	// this is the case for materials OTHER THAN ones which use the "User Interface" material domain.
	// Smh. -_-
	// Source: https://forums.unrealengine.com/t/the-time-material-node-returns-editor-up-time-instead-of-game-time-if-the-material-is-set-to-the-material-domain-user-interface/398968/14

	float EditorTime = FApp::GetCurrentTime() - GStartTime;
	return EditorTime;
}
