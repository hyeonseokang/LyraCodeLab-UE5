// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsGameplayTags.h"

#include "GameplayTagsManager.h"

FKhsGameplayTags FKhsGameplayTags::GameplayTags;
// 에디터가 켜지기도 전 실행
void FKhsGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FKhsGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ") + FString(TagComment)));
}

void FKhsGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initally spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initalized for this actor/component");
	AddTag(InitState_GameplayReady, "InitState.GameplayRead", "4: The actor/component is fully ready for active gmaeplay");

	AddTag(InputTag_Move, "InputTag.Move", "");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "");
}
