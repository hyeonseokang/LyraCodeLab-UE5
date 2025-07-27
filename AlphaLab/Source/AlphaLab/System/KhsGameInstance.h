// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KhsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void Shutdown() override;
};
