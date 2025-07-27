// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "KhsPlayerCameraManager.generated.h"

/**
 *  Controller에 바인딩
 */
#define KHS_CAMERA_DEFAULT_FOV (80.0f)
#define KHS_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define KHS_CAMERA_DEFAULT_PITCH_MAX (90.0f)

UCLASS()
class ALPHALAB_API AKhsPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AKhsPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
