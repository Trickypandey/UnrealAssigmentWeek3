// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"




void AWallBuilderController::BeginPlay()
{
	Super::BeginPlay();

	AWallSpline* WallObj = NewObject<AWallSpline>(this);
	ArrayOfSplines.Add(WallObj);
	SplineIndex = ArrayOfSplines.Num() - 1;

}

AWallBuilderController::AWallBuilderController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	Message.BindUFunction(this, "NotifyMessage");
	ToogleDelgate.BindUFunction(this, "ChangeMode");
}

void AWallBuilderController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	MappingContext = NewObject<UInputMappingContext>(this);

	OnClick = NewObject<UInputAction>(this);
	OnClick->ValueType = EInputActionValueType::Boolean;

	CreateNewSpline = NewObject<UInputAction>(this);
	CreateNewSpline->ValueType = EInputActionValueType::Boolean;
	
	UndoAction = NewObject<UInputAction>(this);
	UndoAction->ValueType = EInputActionValueType::Boolean;
	
	NextSpline = NewObject<UInputAction>(this);
	NextSpline->ValueType = EInputActionValueType::Boolean;

	PrevSpline = NewObject<UInputAction>(this);
	PrevSpline->ValueType = EInputActionValueType::Boolean;

	ChangeModeAction = NewObject<UInputAction>(this);
	ChangeModeAction->ValueType = EInputActionValueType::Boolean;



	check(EIC)
		EIC->BindAction(OnClick, ETriggerEvent::Completed, this, &AWallBuilderController::GenerateWall);
		EIC->BindAction(CreateNewSpline, ETriggerEvent::Completed, this, &AWallBuilderController::NewSpline);
		EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AWallBuilderController::UndoPreviousPoint);
		EIC->BindAction(NextSpline, ETriggerEvent::Completed, this, &AWallBuilderController::MoveToNextOrPreviousSpline, true);
		EIC->BindAction(PrevSpline, ETriggerEvent::Completed, this, &AWallBuilderController::MoveToNextOrPreviousSpline, false);
		EIC->BindAction(ChangeModeAction, ETriggerEvent::Completed, this, &AWallBuilderController::ToggleCreationMode);

	if (MappingContext) {
		MappingContext->MapKey(OnClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(CreateNewSpline, EKeys::RightMouseButton);
		MappingContext->MapKey(UndoAction, EKeys::Z);
		MappingContext->MapKey(NextSpline, EKeys::E);
		MappingContext->MapKey(PrevSpline, EKeys::Q);
		MappingContext->MapKey(ChangeModeAction, EKeys::C);
	}

	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(SubSystem);
	SubSystem->AddMappingContext(MappingContext, 0);

}

void AWallBuilderController::GenerateWall()
{
	if (ArrayOfSplines.Num() != 0 && bWallCreationMode) {
		FHitResult HitonClick; 
		GetHitResultUnderCursor(ECC_Visibility, true, HitonClick);
		if (HitonClick.bBlockingHit)
		{
			FVector ClickLocation = HitonClick.Location;
			ArrayOfSplines[SplineIndex]->AddSplinePoint(ClickLocation);

			if (ArrayOfSplines[SplineIndex]->SplineComponent->GetNumberOfSplinePoints() >= 2) {
				FString Msg = "On Spline " + FString::FromInt(SplineIndex + 1) + " Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->SplineComponent->GetNumberOfSplinePoints() - 1) + " Generated";
				Message.ExecuteIfBound(Msg);
			
			}
		}
	}
	else {
		FString Msg = "Right Click To Generate Spline or click c to change the mode";
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::NewSpline()
{
	if(bWallCreationMode){
		if (ArrayOfSplines.Num() > 0 && bWallCreationMode) {
			if (ArrayOfSplines[ArrayOfSplines.Num() - 1]->SplineComponent->GetNumberOfSplinePoints() >= 2) {
				AWallSpline* Spline = GetWorld()->SpawnActor<AWallSpline>(AWallSpline::StaticClass());
				ArrayOfSplines.Add(Spline);
				SplineIndex = ArrayOfSplines.Num() - 1;

				FString Msg = "New Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
				Message.ExecuteIfBound(Msg);

			}
			else {
				FString Msg = "At least create a wall before creating a new spline";
				Message.ExecuteIfBound(Msg);

			}
		}
		else {
			AWallSpline* Spline = GetWorld()->SpawnActor<AWallSpline>(AWallSpline::StaticClass());
			ArrayOfSplines.Add(Spline);
			SplineIndex = ArrayOfSplines.Num() - 1;

			FString Msg = "New Wall Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
			Message.ExecuteIfBound(Msg);

		}
	}
	else
	{
		FString Msg = "Your are Not In Wall Creation Mode click c to change the mode";
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::UndoPreviousPoint()
{
	if (ArrayOfSplines.Num() > 0)
	{ 
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, FString::Printf(TEXT("in CreateWall: %d"), ArrayOfSplines.Num()));
		auto IsCompletelyRemoved = ArrayOfSplines.Last()->UndoLastAction();
		FString Msg = "Undo";
		Message.ExecuteIfBound(Msg);
		if (IsCompletelyRemoved)
		{
			SplineIndex--;
			ArrayOfSplines.Pop();
		}
	}
}


void AWallBuilderController::MoveToNextOrPreviousSpline(bool bMoveToNext)
{
	if (ArrayOfSplines.Num() > 0)
	{
		if (bMoveToNext)
		{
			SplineIndex = (SplineIndex + 1) % ArrayOfSplines.Num();
			FString Msg = "Moved to next spline " + FString::FromInt(SplineIndex + 1);
			Message.ExecuteIfBound(Msg);
		}
		else
		{
			SplineIndex = (SplineIndex - 1 + ArrayOfSplines.Num()) % ArrayOfSplines.Num();
			FString Msg = "Moved to previous spline " + FString::FromInt(SplineIndex + 1);
			Message.ExecuteIfBound(Msg);
		}
	}
}


void AWallBuilderController::ToggleCreationMode()
{
	
	bWallCreationMode  = !bWallCreationMode;

	ToogleDelgate.ExecuteIfBound(bWallCreationMode);
}