// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KhsPawnData.generated.h"

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
};
