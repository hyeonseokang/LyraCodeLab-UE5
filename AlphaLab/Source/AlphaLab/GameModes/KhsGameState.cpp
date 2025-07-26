// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsGameState.h"
#include "KhsExperienceManagerComponent.h"

AKhsGameState::AKhsGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UKhsExperienceManagerComponent>(
		TEXT("ExperienceManagerComponent"));
}
