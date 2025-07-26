// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsGameModeBase.h"

#include "KhsGameState.h"
#include "AlphaLab/Character/KhsCharacter.h"
#include "AlphaLab/Player/KhsPlayerController.h"
#include "AlphaLab/Player/KhsPlayerState.h"

AKhsGameModeBase::AKhsGameModeBase()
{
	GameStateClass = AKhsGameState::StaticClass();
	PlayerControllerClass = AKhsPlayerController::StaticClass();
	PlayerStateClass = AKhsPlayerState::StaticClass();
	DefaultPawnClass = AKhsCharacter::StaticClass();
}

void AKhsGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AKhsGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
}
