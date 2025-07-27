// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KhsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALPHALAB_API AKhsPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKhsPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
