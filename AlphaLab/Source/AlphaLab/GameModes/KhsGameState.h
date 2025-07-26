// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KhsGameState.generated.h"

class UKhsExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class ALPHALAB_API AKhsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AKhsGameState();

	UPROPERTY()
	TObjectPtr<UKhsExperienceManagerComponent> ExperienceManagerComponent;
};
