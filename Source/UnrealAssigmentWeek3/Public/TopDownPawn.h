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
#include "TopDownPawn.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREALASSIGMENTWEEK3_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownPawn();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* TopDownCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
class UFloatingPawnMovement* TopDownPawnMovementComponent;

	UPROPERTY()
	UInputMappingContext* PawnMappingContext;
	UPROPERTY()
	UInputAction* MoveAction;
	UPROPERTY()
	UInputAction* LookAction;
	UPROPERTY()
	UInputAction* ZoomAction;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(const FInputActionValue& ActionValue);

	UFUNCTION()
	void Look(const FInputActionValue& ActionValue);

	UFUNCTION()
	void ZoomIn(const FInputActionValue& ActionValue);
	//void ZoomOut();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MoveScale = 1.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float RotateScale = 50.f;

	float ZoomSpeed = 100.0f;
	float MinZoomDistance = 500.0f;
	float MaxZoomDistance = 2000.0f;
};
