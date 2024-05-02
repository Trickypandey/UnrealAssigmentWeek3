// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputMappingContext.h"
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include "TopDownPawn.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREALASSIGMENTWEEK3_API ATopDownPawn : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ATopDownPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY()
	UInputAction* MoveAction;

	UPROPERTY()
	UInputAction* ScrollAction;
	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* FloatingMovement;

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& ActionValue);
	UFUNCTION(BlueprintCallable)
	void ZoomInOut(const FInputActionValue& ActionValue);

};
