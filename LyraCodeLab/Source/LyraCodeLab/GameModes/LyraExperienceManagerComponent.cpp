// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "LyraLabExperienceDefinition.h"
#include "System/LyraCodeLabAssetManager.h"

void ULyraExperienceManagerComponent::CallOnRegister_OnExperienceLoaded(FOnLyraExpereienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULyraExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId)
{
	ULyraCodeLabAssetManager& AssetManager = ULyraCodeLabAssetManager::Get();

	TSubclassOf<ULyraLabExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const ULyraLabExperienceDefinition* Experience = GetDefault<ULyraLabExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void ULyraExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ELyraExperienceLoadState::Unloaded);

	LoadState = ELyraExperienceLoadState::Loading;

	ULyraCodeLabAssetManager& AssetManager = ULyraCodeLabAssetManager::Get();;

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();
		}));
	}

	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void ULyraExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	OnExperienceFullLoadCompleted();
}

void ULyraExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELyraExperienceLoadState::Loaded);

	LoadState = ELyraExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const ULyraLabExperienceDefinition* ULyraExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ELyraExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
