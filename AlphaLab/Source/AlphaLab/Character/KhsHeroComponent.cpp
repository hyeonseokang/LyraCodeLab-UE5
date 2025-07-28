// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "KhsPawnData.h"
#include "KhsPawnExtensionComponent.h"
#include "PlayerMappableInputConfig.h"
#include "AlphaLab/KhsGameplayTags.h"
#include "AlphaLab/Camera/KhsCameraComponent.h"
#include "AlphaLab/Input/KhsInputComponent.h"
#include "AlphaLab/Input/KhsInputConfig.h"
#include "AlphaLab/Player/KhsPlayerController.h"
#include "AlphaLab/Player/KhsPlayerState.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UKhsHeroComponent::NAME_ActorFeatureName("Hero");

UKhsHeroComponent::UKhsHeroComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UKhsHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		return;
	}

	RegisterInitStateFeature();
}

void UKhsHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(UKhsPawnExtensionComponent::NAME_ActorfeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(FKhsGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UKhsHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UKhsHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FKhsGameplayTags& InitTags = FKhsGameplayTags::Get();

	if (Params.FeatureName == UKhsPawnExtensionComponent::NAME_ActorfeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UKhsHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                           FGameplayTag DesiredState) const
{
	check(Manager);

	const FKhsGameplayTags& InitTags = FKhsGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AKhsPlayerState* KhsPS = GetPlayerState<AKhsPlayerState>();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!KhsPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return KhsPS && Manager->HasFeatureReachedInitState(Pawn, UKhsPawnExtensionComponent::NAME_ActorfeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UKhsHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                              FGameplayTag DesiredState)
{
	const FKhsGameplayTags& InitTags = FKhsGameplayTags::Get();

	// 해당 단계가 진행된다면 다 생성이 된 상태이므로 핸들링 진행
	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AKhsPlayerState* KhsPS = GetPlayerState<AKhsPlayerState>();
		if (!ensure(Pawn && KhsPS))
		{
			return;
		}
		
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UKhsPawnData* PawnData = nullptr;
		if (UKhsPawnExtensionComponent* PawnExtComp = UKhsPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UKhsPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (UKhsCameraComponent* CameraComponent = UKhsCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (AKhsPlayerController* KhsPC = GetController<AKhsPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UKhsHeroComponent::CheckDefaultInitialization()
{
	const FKhsGameplayTags& InitTags = FKhsGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = {
		InitTags.InitState_Spawned,
		InitTags.InitState_DataAvailable,
		InitTags.InitState_DataInitialized,
		InitTags.InitState_GameplayReady
	};
	
	ContinueInitStateChain(StateChain);
}

TSubclassOf<UKhsCameraMode> UKhsHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UKhsPawnExtensionComponent* PawnExtComp = UKhsPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UKhsPawnData* PawnData = PawnExtComp->GetPawnData<UKhsPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

PRAGMA_DISABLE_OPTIMIZATION
void UKhsHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC)

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP)

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();


	if (const UKhsPawnExtensionComponent* PawnExtComp = UKhsPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UKhsPawnData* PawnData = PawnExtComp->GetPawnData<UKhsPawnData>())
		{
			if (const UKhsInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FKhsGameplayTags& GameplayTags = FKhsGameplayTags::Get();

				for (const FKhsMappableConfigPair& Pair: DefaultInputConfig)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						for (const auto ConfigObject = Pair.Config.LoadSynchronous(); const auto& MappingContextPair : ConfigObject->GetMappingContexts())
						{
							const UInputMappingContext* MappingContext = MappingContextPair.Key;
							const int32 Priority = MappingContextPair.Value;
							Subsystem->AddMappingContext(MappingContext, Priority, Options);
						}
					}
				}

				UKhsInputComponent* KhsIC = CastChecked<UKhsInputComponent>(PlayerInputComponent);
				{
					KhsIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					KhsIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}
}

void UKhsHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn? Pawn->GetController<AController>() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UKhsHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
		
}

PRAGMA_ENABLE_OPTIMIZATION