// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraLabPawnData.generated.h"

/**
 * 
 */
UCLASS()
class LYRACODELAB_API ULyraLabPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraLabPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
