// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KhsPlayerState.generated.h"

class UKhsExperienceDefinition;
class UKhsPawnData;
/**
 * 
 */
UCLASS()
class ALPHALAB_API AKhsPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override;


	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData);};
	void OnExperienceLoaded(const UKhsExperienceDefinition* CurrentExperience);
	void SetPawnData(const UKhsPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const UKhsPawnData> PawnData;
};
