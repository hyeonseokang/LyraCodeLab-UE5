// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "KhsPawnExtensionComponent.generated.h"

class UKhsPawnData;
/**
* 컴포넌트 끼리 종속성 해결 최상위 컴포넌트 같은 느낌
 */
UCLASS()
class ALPHALAB_API UKhsPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UKhsPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	static const FName NAME_ActorfeatureName;

	static UKhsPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor){ return (Actor ? Actor->FindComponentByClass<UKhsPawnExtensionComponent>():nullptr);}
	template<class T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}
	void SetPawnData(const UKhsPawnData* InPawnData);
	void SetupPlayerInputComponent();

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FName GetFeatureName() const override final { return NAME_ActorfeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void CheckDefaultInitialization() override;

	UPROPERTY(EditInstanceOnly, Category = "Khs|Pawn")
	TObjectPtr<const UKhsPawnData> PawnData;
};
