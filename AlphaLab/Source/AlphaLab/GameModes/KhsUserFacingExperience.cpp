// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsUserFacingExperience.h"

#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* UKhsUserFacingExperience::CreateHostringRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
