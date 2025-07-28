// Fill out your copyright notice in the Description page of Project Settings.


#include "KhsCharacter.h"

#include "KhsPawnExtensionComponent.h"
#include "AlphaLab/Camera/KhsCameraComponent.h"

// Sets default values
AKhsCharacter::AKhsCharacter()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
 	PrimaryActorTick.bCanEverTick = false;

	PawnExtensionComponent = CreateDefaultSubobject<UKhsPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	
	CameraComponent = CreateDefaultSubobject<UKhsCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
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

