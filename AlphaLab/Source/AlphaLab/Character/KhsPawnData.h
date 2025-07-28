// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KhsPawnData.generated.h"

class UKhsInputConfig;
class UKhsCameraMode;
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UKhsPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Khs|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Khs|Camera")
	TSubclassOf<UKhsCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Khs|InputConfig")
	TObjectPtr<UKhsInputConfig> InputConfig;
};
