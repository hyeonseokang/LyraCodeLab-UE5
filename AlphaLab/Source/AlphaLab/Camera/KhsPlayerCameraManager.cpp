// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsPlayerCameraManager.h"

AKhsPlayerCameraManager::AKhsPlayerCameraManager(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	DefaultFOV = KHS_CAMERA_DEFAULT_FOV;
	ViewPitchMin = KHS_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = KHS_CAMERA_DEFAULT_PITCH_MAX;
}
