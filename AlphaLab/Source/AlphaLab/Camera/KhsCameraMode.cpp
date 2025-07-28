// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsCameraMode.h"

#include "KhsCameraComponent.h"
#include "KhsPlayerCameraManager.h"

FKhsCameraModeView::FKhsCameraModeView()
	: Location(ForceInit),
	  Rotation(ForceInit),
	  ControlRotation(ForceInit),
	  FieldOfView(KHS_CAMERA_DEFAULT_FOV)
{
}

void FKhsCameraModeView::Blend(const FKhsCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		*this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UKhsCameraMode::UKhsCameraMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	FieldOfView = KHS_CAMERA_DEFAULT_FOV;
	ViewPitchMin = KHS_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = KHS_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = EKhsCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void UKhsCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UKhsCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UKhsCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case EKhsCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EKhsCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EKhsCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EKhsCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EKhsCameraModeBlendFunction::COUNT:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction {%d}\n"), (uint8)BlendFunction);
		break;
	}
}

UKhsCameraComponent* UKhsCameraMode::GetKhsCameraComponent() const
{
	return CastChecked<UKhsCameraComponent>(GetOuter());
}

AActor* UKhsCameraMode::GetTargetActor() const
{
	const UKhsCameraComponent* KHsCameraComponent = GetKhsCameraComponent();
	return KHsCameraComponent->GetTargetActor();
}

FVector UKhsCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UKhsCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

UKhsCameraModeStack::UKhsCameraModeStack(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

UKhsCameraMode* UKhsCameraModeStack::GetCameraModeInstance(TSubclassOf<UKhsCameraMode>& CameraModeClass)
{
	check(CameraModeClass)

	for (UKhsCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	UKhsCameraMode* NewCameraMode = NewObject<UKhsCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);
	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void UKhsCameraModeStack::PushCameraMode(TSubclassOf<UKhsCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UKhsCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UKhsCameraModeStack::EvaluateStack(float DeltaTime, FKhsCameraModeView& OutCameraModeView)
{
	// Top Bottom 순서
	UpdateStack(DeltaTime);
	// Bottom Top 순서
	BlendStack(OutCameraModeView);
}

void UKhsCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0;StackIndex < StackSize;++StackIndex)
	{
		UKhsCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UKhsCameraModeStack::BlendStack(FKhsCameraModeView& OutCameraModeView)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	const UKhsCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->View;

	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
