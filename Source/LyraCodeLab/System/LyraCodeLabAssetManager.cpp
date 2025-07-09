// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LyraCodeLabAssetManager.h"
#include "LyraCodeLab/LyraCodeLabLogChannels.h"

ULyraCodeLabAssetManager::ULyraCodeLabAssetManager()
{
}

ULyraCodeLabAssetManager& ULyraCodeLabAssetManager::Get()
{
	check(GEngine);

	if (ULyraCodeLabAssetManager* Singleton = Cast<ULyraCodeLabAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLyraCodeLab, Fatal,
	       TEXT(
		       "invalid AssetManagerClassname in DefaultEngine.ini(project setting); it must be LyraCodeLabAssetManager"
	       ));

	return *NewObject<ULyraCodeLabAssetManager>();
}

bool ULyraCodeLabAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* ULyraCodeLabAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronus lodead aseets {%s}"),
			                                                        *AssetPath.ToString()), nullptr);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULyraCodeLabAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
