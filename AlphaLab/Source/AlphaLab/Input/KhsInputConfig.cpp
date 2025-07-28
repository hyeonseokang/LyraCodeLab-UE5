// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsInputConfig.h"

UKhsInputConfig::UKhsInputConfig(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

const UInputAction* UKhsInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FKhsInputAction& Action:NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		// Error
	}

	return nullptr;
}

const UInputAction* UKhsInputConfig::FindAbilityINputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FKhsInputAction& Action:AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		// Error
	}

	return nullptr;
}
