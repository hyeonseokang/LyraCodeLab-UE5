// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsGameInstance.h"

#include "AlphaLab/KhsGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

void UKhsGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FKhsGameplayTags& GameplayTags = FKhsGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
		
	}
}

void UKhsGameInstance::Shutdown()
{
	Super::Shutdown();
}