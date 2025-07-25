// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LyraLabGameStateBase.generated.h"

class ULyraExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class LYRACODELAB_API ALyraLabGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALyraLabGameStateBase();

	UPROPERTY()
	TObjectPtr<ULyraExperienceManagerComponent> ExperienceManagerComponent;
};
