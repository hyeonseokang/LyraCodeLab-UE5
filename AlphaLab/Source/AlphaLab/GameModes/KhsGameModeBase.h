// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KhsGameModeBase.generated.h"

class UKhsPawnData;
class UKhsExperienceDefinition;
/**
 * 
 */
UCLASS()
class ALPHALAB_API AKhsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKhsGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded();
	void OnExperienceLoaded(const UKhsExperienceDefinition* CurrentExperience);

	const UKhsPawnData* GetPawnDataForController(const AController* InController) const;
};
