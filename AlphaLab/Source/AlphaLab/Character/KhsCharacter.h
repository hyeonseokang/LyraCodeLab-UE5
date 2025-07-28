// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KhsCharacter.generated.h"

class UKhsCameraComponent;
class UKhsPawnExtensionComponent;

UCLASS()
class ALPHALAB_API AKhsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKhsCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Khs|Character")
	TObjectPtr<UKhsPawnExtensionComponent> PawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Khs|Character")
	TObjectPtr<UKhsCameraComponent> CameraComponent;
};
