// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsCameraMode_ThirdPerson.h"

#include "Curves/CurveVector.h"

UKhsCameraMode_ThirdPerson::UKhsCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UKhsCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// Pitch에 맞춰서 Curve 데이터 벡터 값을 가져옴
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
