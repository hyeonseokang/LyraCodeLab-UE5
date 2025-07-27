// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsCharacter.h"

#include "KhsPawnExtensionComponent.h"

// Sets default values
AKhsCharacter::AKhsCharacter()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
 	PrimaryActorTick.bCanEverTick = false;

	PawnExtensionComponent = CreateDefaultSubobject<UKhsPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

// Called when the game starts or when spawned
void AKhsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKhsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKhsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtensionComponent->SetupPlayerInputComponent();
}

