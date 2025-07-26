// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsGameModeBase.h"

#include "KhsExperienceDefinition.h"
#include "KhsExperienceManagerComponent.h"
#include "KhsGameState.h"
#include "AlphaLab/Character/KhsCharacter.h"
#include "AlphaLab/Player/KhsPlayerController.h"
#include "AlphaLab/Player/KhsPlayerState.h"
#include "AlphaLab/Character/KhsPawnData.h"

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

void AKhsGameModeBase::InitGameState()
{
	Super::InitGameState();

	UKhsExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UKhsExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnKhsExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AKhsGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UKhsPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

PRAGMA_DISABLE_OPTIMIZATION

void AKhsGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}
PRAGMA_ENABLE_OPTIMIZATION

APawn* AKhsGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AKhsGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid())
	{
		// 아직 로딩 x 이 파일을 스캔할수있도록 가져오기만
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("KhsExperienceDefinition"), FName("B_KhsDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void AKhsGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());

	UKhsExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UKhsExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

bool AKhsGameModeBase::IsExperienceLoaded()
{
	check(GameState);
	UKhsExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UKhsExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AKhsGameModeBase::OnExperienceLoaded(const UKhsExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(Iterator->Get());

		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UKhsPawnData* AKhsGameModeBase::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const AKhsPlayerState* PlayerState = InController->GetPlayerState<AKhsPlayerState>())
		{
			if (const UKhsPawnData* PawnData = PlayerState->GetPawnData<UKhsPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	UKhsExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UKhsExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UKhsExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	return nullptr;
}
