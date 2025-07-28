// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "KHsMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

/**
 * 
 */
USTRUCT()
struct FKhsMappableConfigPair
{
	GENERATED_BODY()

public:
	FKhsMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};
