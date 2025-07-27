// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsPlayerController.h"

#include "AlphaLab/Camera/KhsPlayerCameraManager.h"

AKhsPlayerController::AKhsPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AKhsPlayerCameraManager::StaticClass();
}
