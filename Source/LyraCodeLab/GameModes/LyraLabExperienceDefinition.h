// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraLabExperienceDefinition.generated.h"

class ULyraLabPawnData;
/**
 * 
 */
UCLASS()
class LYRACODELAB_API ULyraLabExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraLabExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UPROPERTY(EditDefaultsOnly, Category=GamePlay)
	TObjectPtr<ULyraLabPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category=GamePlay)
	TArray<FString> GameFeaturesToEnable;
};
