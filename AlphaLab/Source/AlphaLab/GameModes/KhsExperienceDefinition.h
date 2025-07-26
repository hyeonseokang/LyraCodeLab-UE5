// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KhsExperienceDefinition.generated.h"

class UKhsPawnData;
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UKhsExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TObjectPtr<UKhsPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
