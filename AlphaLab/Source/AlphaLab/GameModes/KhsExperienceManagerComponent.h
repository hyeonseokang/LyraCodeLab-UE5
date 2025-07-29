// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturePluginOperationResult.h"
#include "Components/GameStateComponent.h"
#include "KhsExperienceManagerComponent.generated.h"

class UKhsExperienceDefinition;

enum class EKhsExperienceLoadState
{
	Unloaded,
	Loading, // Experience 관련 로딩
	LoadingGameFeatures, // Game Feature Loading
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnKhsExperienceLoaded, const UKhsExperienceDefinition*);
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() const
	{
		return (LoadState == EKhsExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
	}

	void CallOrRegister_OnExperienceLoaded(FOnKhsExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	const UKhsExperienceDefinition* GetCurrentExperienceChecked() const;
public:
	UPROPERTY()
	TObjectPtr<const UKhsExperienceDefinition> CurrentExperience;

	EKhsExperienceLoadState LoadState = EKhsExperienceLoadState::Unloaded;

	FOnKhsExperienceLoaded OnExperienceLoaded;
	
	// Game Feature
	int32 NumGameFeaturePluginLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
