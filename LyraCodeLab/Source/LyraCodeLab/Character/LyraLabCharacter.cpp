// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraLabCharacter.h"

// Sets default values
ALyraLabCharacter::ALyraLabCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALyraLabCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALyraLabCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALyraLabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

