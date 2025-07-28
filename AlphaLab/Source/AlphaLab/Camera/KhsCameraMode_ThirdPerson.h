// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KhsCameraMode.h"
#include "KhsCameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ALPHALAB_API UKhsCameraMode_ThirdPerson : public UKhsCameraMode
{
	GENERATED_BODY()
public:
	UKhsCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
