// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsExperienceManagerComponent.h"

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "KhsExperienceDefinition.h"
#include "AlphaLab/System/KhsAssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "KhsExperienceActionSet.h"

void UKhsExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnKhsExperienceLoaded::FDelegate&& Delegate)
{
	// 로딩이 되어있으면 바로 Delegate 호출
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(Delegate);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void UKhsExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UKhsAssetManager& AssetManager = UKhsAssetManager::Get();

	TSubclassOf<UKhsExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	const UKhsExperienceDefinition* Experience = GetDefault<UKhsExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}
PRAGMA_ENABLE_OPTIMIZATION

void UKhsExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EKhsExperienceLoadState::Unloaded);

	LoadState = EKhsExperienceLoadState::Loading;

	UKhsAssetManager& AssetManager = UKhsAssetManager::Get();

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
		{}, false, FStreamableDelegate(),
		FStreamableManager::AsyncLoadHighPriority);

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

void UKhsExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	
	check(LoadState == EKhsExperienceLoadState::Loading);
	check(CurrentExperience);

	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Cntext, const TArray<FString>& FeaturePluginList)
	{
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
		}
	};

	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	NumGameFeaturePluginLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginLoading)
	{
		LoadState = EKhsExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL:GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(
				PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(
					this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void UKhsExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginLoading--;
	if (NumGameFeaturePluginLoading == 0)
	{
		OnExperienceFullLoadCompleted();
	}
}

void UKhsExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EKhsExperienceLoadState::Loaded);

	/* Experience Action Set */
	{
		LoadState = EKhsExperienceLoadState::ExecutingActions;

		FGameFeatureActivatingContext Context;
		{
			const FWorldContext* ExistingWordlContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWordlContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWordlContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating();
				}
			}
		};

		ActivateListOfActions(CurrentExperience->Actions);
		for (const TObjectPtr<UKhsExperienceActionSet>& ActionSet: CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}
	
	LoadState = EKhsExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UKhsExperienceDefinition* UKhsExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EKhsExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
