// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraLabPlayerState.generated.h"

class ULyraLabPawnData;
class ULyraLabExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACODELAB_API ALyraLabPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override final;

	void OnExperienceLoaded(const ULyraLabExperienceDefinition* CurrentExperience);

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULyraLabPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const ULyraLabPawnData> PawnData;
};
