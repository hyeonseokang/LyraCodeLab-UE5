// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsHeroComponent.h"
#include "KhsPawnData.h"
#include "KhsPawnExtensionComponent.h"
#include "AlphaLab/KhsGameplayTags.h"
#include "AlphaLab/Player/KhsPlayerState.h"

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
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool UKhsHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                           FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void UKhsHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                              FGameplayTag DesiredState)
{
	const FKhsGameplayTags& InitTags = FKhsGameplayTags::Get();

	// 해당 단계가 진행된다면 다 생성이 된 상태이므로 핸들링 진행
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AKhsPlayerState* KhsPs = GetPlayerState<AKhsPlayerState>();
		if (!ensure(Pawn && KhsPs))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UKhsPawnData* PawnData = nullptr;
		if (UKhsPawnExtensionComponent* PawnExtComp = UKhsPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UKhsPawnData>();
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
		InitTags.InitState_GameplayReady,
	};

	ContinueInitStateChain(StateChain);
}
