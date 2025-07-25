// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraExperienceManagerComponent.generated.h"

class ULyraLabExperienceDefinition;

enum class ELyraExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraExpereienceLoaded, const ULyraLabExperienceDefinition*);

UCLASS()
class LYRACODELAB_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	bool IsExperienceLoaded() const {return (LoadState == ELyraExperienceLoadState::Loaded) && (CurrentExperience != nullptr);}

	void CallOnRegister_OnExperienceLoaded(FOnLyraExpereienceLoaded::FDelegate&& Delegate);
	void ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	const ULyraLabExperienceDefinition* GetCurrentExperienceChecked() const;
public:
	UPROPERTY()
	TObjectPtr<const ULyraLabExperienceDefinition> CurrentExperience;

	ELyraExperienceLoadState LoadState = ELyraExperienceLoadState::Unloaded;

	FOnLyraExpereienceLoaded OnExperienceLoaded;
};
