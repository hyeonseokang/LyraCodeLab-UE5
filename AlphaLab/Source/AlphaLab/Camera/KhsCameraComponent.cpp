// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsCameraComponent.h"

#include "KhsCameraMode.h"

UKhsCameraComponent::UKhsCameraComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UKhsCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UKhsCameraModeStack>(this);
	}
}

void UKhsCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);
	UpdateCameraModes();

	FKhsCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (const APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;


	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = ProjectionMode;
	DesiredView.PostProcessBlendWeight =  PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UKhsCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<UKhsCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
