// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "KhsInputConfig.h"
#include "KhsInputComponent.generated.h"

class UKhsInputConfig;
/**
 * 
 */
UCLASS()
class ALPHALAB_API UKhsInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UKhsInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UKhsInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
	                      UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UKhsInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UKhsInputComponent::BindNativeAction(const UKhsInputConfig* InputConfig, const FGameplayTag& InputTag,
                                          ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func,
                                          bool bLogIfNotFound)
{
	check(InputConfig);

	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UKhsInputComponent::BindAbilityActions(const UKhsInputConfig* InputConfig, UserClass* Object,
                                            PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                            TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FKhsInputAction& Action: InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
