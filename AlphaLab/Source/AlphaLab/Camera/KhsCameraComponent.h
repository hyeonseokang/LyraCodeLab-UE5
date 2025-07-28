// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "KhsCameraComponent.generated.h"

class UKhsCameraMode;
class UKhsCameraModeStack;
/**
 * 
 */
template <class TClass>
class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UKhsCameraMode>, FKhsCameraModeDelegate);

UCLASS()
class ALPHALAB_API UKhsCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UKhsCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UKhsCameraComponent* FindCameraComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<UKhsCameraComponent>() : nullptr);
	}


	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	UPROPERTY()
	TObjectPtr<UKhsCameraModeStack> CameraModeStack;

	FKhsCameraModeDelegate DetermineCameraModeDelegate;
};
