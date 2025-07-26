// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsPlayerState.h"

#include "AlphaLab/GameModes/KhsExperienceManagerComponent.h"
#include "AlphaLab/GameModes/KhsGameModeBase.h"
#include "AlphaLab/Character/KhsPawnData.h"

void AKhsPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UKhsExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UKhsExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnKhsExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AKhsPlayerState::OnExperienceLoaded(const UKhsExperienceDefinition* CurrentExperience)
{
	if (AKhsGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AKhsGameModeBase>())
	{
		const UKhsPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AKhsPlayerState::SetPawnData(const UKhsPawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);

	PawnData = InPawnData;
}
