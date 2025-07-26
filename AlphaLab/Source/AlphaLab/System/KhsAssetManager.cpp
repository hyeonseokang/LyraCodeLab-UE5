// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsAssetManager.h"

UKhsAssetManager::UKhsAssetManager()
{
}

UKhsAssetManager& UKhsAssetManager::Get()
{
	check(GEngine);

	if (UKhsAssetManager* Singleton = Cast<UKhsAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	return *NewObject<UKhsAssetManager>();
}

bool UKhsAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));

	return bLogAssetLoads;
}

UObject* UKhsAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(
				TEXT("synchronous loaded assets {%s}"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UKhsAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
