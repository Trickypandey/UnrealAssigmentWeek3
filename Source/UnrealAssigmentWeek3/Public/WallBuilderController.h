// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WallSpline.h"
#include "Delegates/Delegate.h" 
#include "WallBuilderController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALASSIGMENTWEEK3_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()

public:
	AWallBuilderController();

	virtual void SetupInputComponent() override;


	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* OnClick;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* CreateNewSpline;

	//UPROPERTY(EditDefaultsOnly, Category = "WallController")
	//UInputAction* StopCreateNewSpline;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	TArray<AWallSpline*> ArrayOfSplines;

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void GenerateWall();

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void NewSpline();

	//UFUNCTION(BlueprintCallable, Category = "WallController")
	//void StopGeneratingWall();

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	int32 SplineIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
