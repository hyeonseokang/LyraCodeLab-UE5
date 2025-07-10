// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraLabUserFacingExperience.generated.h"

/**
 * 
 */
UCLASS()
class LYRACODELAB_API ULyraLabUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Expereince, meta = (AllowedTypes = "LyraLabExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
