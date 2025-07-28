// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KhsCameraMode.generated.h"

/**
 * 
 */

class UKhsCameraComponent;

struct FKhsCameraModeView
{
	FKhsCameraModeView();

	void Blend(const FKhsCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;

	float FieldOfView;
};

UENUM(BlueprintType)
enum class EKhsCameraModeBlendFunction:uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

UCLASS(Abstract, NotBlueprintable)
class ALPHALAB_API UKhsCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UKhsCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	UKhsCameraComponent* GetKhsCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	FKhsCameraModeView View	;

	UPROPERTY(EditDefaultsOnly, Category = "View", meta=(UIMin = "5.0", UIMax="170", ClampMin="5.0", ClampMax="170"))
	float FieldOfView;
	UPROPERTY(EditDefaultsOnly, Category = "View", meta=(UIMin = "-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, Category = "View", meta=(UIMin = "-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMax;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;
	float BlendAlpha;
	float BlendWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	EKhsCameraModeBlendFunction BlendFunction;
};

UCLASS()
class UKhsCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UKhsCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UKhsCameraMode* GetCameraModeInstance(TSubclassOf<UKhsCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UKhsCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FKhsCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FKhsCameraModeView& OutCameraModeView);
public:
	UPROPERTY()
	TArray<TObjectPtr<UKhsCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UKhsCameraMode>> CameraModeStack;
};
