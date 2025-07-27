// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "KhsCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UKhsCameraComponent(FObjectInitializer ObjectInitializer = FObjectInitializer::Get());
};
