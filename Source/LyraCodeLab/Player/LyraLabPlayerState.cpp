// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraLabPlayerState.h"

#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameModes/LyraLabGameModeBase.h"

void ALyraLabPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOnRegister_OnExperienceLoaded(FOnLyraExpereienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraLabPlayerState::OnExperienceLoaded(const ULyraLabExperienceDefinition* CurrentExperience)
{
	if (ALyraLabGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALyraLabGameModeBase>())
	{
		const ULyraLabPawnData* newPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(newPawnData);

		SetPawnData(newPawnData);
	}
}

void ALyraLabPlayerState::SetPawnData(const ULyraLabPawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);
	
	PawnData = InPawnData;
}
