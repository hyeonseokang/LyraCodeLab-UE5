// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KhsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALPHALAB_API AKhsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKhsGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleMatchAssignmentIfNotExpectingOne();
};
