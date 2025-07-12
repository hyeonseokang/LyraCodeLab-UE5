// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraLabGameModeBase.generated.h"

class ULyraLabPawnData;
class ULyraLabExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACODELAB_API ALyraLabGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALyraLabGameModeBase();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	void OnMatchAssignmentGiven(const FPrimaryAssetId& ExperienceId) const;
	void HandleMatchAssignmentIfNotExpectingOne();
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraLabExperienceDefinition* CurrentExperience);
	const ULyraLabPawnData* GetPawnDataForController(const AController* InController) const;
};
