// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KhsExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UKhsExperienceActionSet();
	
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
