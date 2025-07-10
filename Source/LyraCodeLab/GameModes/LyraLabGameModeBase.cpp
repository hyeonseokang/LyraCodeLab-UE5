// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraLabGameModeBase.h"

#include "LyraLabGameStateBase.h"
#include "Character/LyraLabCharacter.h"
#include "Player/LyraLabPlayerController.h"
#include "Player/LyraLabPlayerState.h"

ALyraLabGameModeBase::ALyraLabGameModeBase()
{
	GameStateClass = ALyraLabGameStateBase::StaticClass();
	PlayerControllerClass = ALyraLabPlayerController::StaticClass();
	PlayerStateClass = ALyraLabPlayerState::StaticClass();
	DefaultPawnClass = ALyraLabCharacter::StaticClass();
}
