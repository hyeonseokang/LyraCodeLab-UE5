// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCodeLab.h"
#include "LyraCodeLabLogChannels.h"
#include "Modules/ModuleManager.h"

class FLyraCodeLabModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(LogLyraCodeLab, Log, TEXT("Startup Module: LyraCodeLab"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogLyraCodeLab, Log, TEXT("Shutdown Module: LyraCodeLab"));
	}
};


IMPLEMENT_PRIMARY_GAME_MODULE(FLyraCodeLabModule, LyraCodeLab, "LyraCodeLab");
