// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraLabGameModeBase.h"

#include "LyraCodeLabLogChannels.h"
#include "LyraExperienceManagerComponent.h"
#include "LyraLabExperienceDefinition.h"
#include "LyraLabGameStateBase.h"
#include "Character/LyraLabCharacter.h"
#include "Player/LyraLabPlayerController.h"
#include "Player/LyraLabPlayerState.h"
#include "LyraLabPawnData.h"

ALyraLabGameModeBase::ALyraLabGameModeBase()
{
	GameStateClass = ALyraLabGameStateBase::StaticClass();
	PlayerControllerClass = ALyraLabPlayerController::StaticClass();
	PlayerStateClass = ALyraLabPlayerState::StaticClass();
	DefaultPawnClass = ALyraLabCharacter::StaticClass();
}

void ALyraLabGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraLabGameModeBase::InitGameState()
{
	Super::InitGameState();

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOnRegister_OnExperienceLoaded(FOnLyraExpereienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ALyraLabGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const ULyraLabPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ALyraLabGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ALyraLabGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	UE_LOG(LogLyraCodeLab, Log, TEXT("SpawnDefualtPawnAtTransform_Implemntation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ALyraLabGameModeBase::OnMatchAssignmentGiven(const FPrimaryAssetId& ExperienceId) const
{
	check(ExperienceId.IsValid());

	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

void ALyraLabGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();
	if(!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraLabExperienceDefinition"), FName("B_LyraDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

bool ALyraLabGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraLabGameModeBase::OnExperienceLoaded(const ULyraLabExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const ULyraLabPawnData* ALyraLabGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const ALyraLabPlayerState* PlayerState = InController->GetPlayerState<ALyraLabPlayerState>())
		{
			if (const ULyraLabPawnData* PawnData = PlayerState->GetPawnData<ULyraLabPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	ULyraExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const ULyraLabExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	return nullptr;
}
