// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraLabGameStateBase.h"
#include "LyraExperienceManagerComponent.h"

ALyraLabGameStateBase::ALyraLabGameStateBase()
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULyraExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
