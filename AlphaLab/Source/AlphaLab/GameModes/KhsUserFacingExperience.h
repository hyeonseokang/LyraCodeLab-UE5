	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KhsUserFacingExperience.generated.h"

	class UCommonSession_HostSessionRequest;
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostringRequest() const;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "KhsExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
