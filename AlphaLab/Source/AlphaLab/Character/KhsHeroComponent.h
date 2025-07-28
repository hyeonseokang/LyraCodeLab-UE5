// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "KhsHeroComponent.generated.h"

class UKhsCameraMode;
/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALPHALAB_API UKhsHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UKhsHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_ActorFeatureName;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FName GetFeatureName() const final override { return NAME_ActorFeatureName;}
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void CheckDefaultInitialization() override;

	TSubclassOf<UKhsCameraMode> DetermineCameraMode() const;
};
