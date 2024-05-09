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




DECLARE_DELEGATE_OneParam(GenerateMsg, FString)
DECLARE_DELEGATE_OneParam(ChangeCreationMode, bool);

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

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* UndoAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* NextSpline;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* PrevSpline;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* ChangeModeAction;

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

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void UndoPreviousPoint();

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void MoveToNextOrPreviousSpline(bool bMoveToNext);

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void ToggleCreationMode();


	//UFUNCTION(BlueprintCallable, Category = "WallController")
	//void StopGeneratingWall();

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	int32 SplineIndex;

	/*UPROPERTY(BlueprintAssignable, Category = "Wall")
	FWallSegmentStartedDelegate OnWallSegmentStarted;*/

	GenerateMsg Message;
	
	ChangeCreationMode ToogleDelgate;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyMessage(const FString& msg);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMode(bool toggle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	bool bWallCreationMode = false;
	
};
