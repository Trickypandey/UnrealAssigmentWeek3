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

	/*StopCreateNewSpline = NewObject<UInputAction>(this);
	StopCreateNewSpline->ValueType = EInputActionValueType::Boolean;*/



	check(EIC)
		EIC->BindAction(OnClick, ETriggerEvent::Completed, this, &AWallBuilderController::GenerateWall);
		EIC->BindAction(CreateNewSpline, ETriggerEvent::Completed, this, &AWallBuilderController::NewSpline);
		//EIC->BindAction(StopCreateNewSpline,ETriggerEvent::Completed, this, &AWallBuilderController::StopGeneratingWall);

	if (MappingContext) {
		MappingContext->MapKey(OnClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(CreateNewSpline, EKeys::RightMouseButton);
		//MappingContext->MapKey(StopCreateNewSpline, EKeys::H);
	}

	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(SubSystem);
	SubSystem->AddMappingContext(MappingContext, 0);

}

void AWallBuilderController::GenerateWall()
{
	if (ArrayOfSplines.Num() != 0) {
		FHitResult HitonClick;
		GetHitResultUnderCursor(ECC_Visibility, true, HitonClick);
		if (HitonClick.bBlockingHit)
		{

			FVector ClickLocation = HitonClick.Location;
			

			ArrayOfSplines[SplineIndex]->AddSplinePoint(ClickLocation);

			if (ArrayOfSplines[SplineIndex]->SplineComponent->GetNumberOfSplinePoints() >= 2) {
				FString Msg = "On Spline " + FString::FromInt(SplineIndex) + " Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->SplineComponent->GetNumberOfSplinePoints() - 1) + " Generated";
				Message.ExecuteIfBound(Msg);
			
			}
		}
	}
	else {
		FString Msg = "Right Click To Generate Spline";
		
	}
}

void AWallBuilderController::NewSpline()
{
	/*if (ArrayOfSplines.Num() > 0) {
		if (ArrayOfSplines[ArrayOfSplines.Num() - 1]->SplinePoint.Num() >= 2) {
			AWallSpline* Spline = GetWorld()->SpawnActor<AWallSpline>(AWallSpline::StaticClass());
			ArrayOfSplines.Add(Spline);
			SplineIndex = ArrayOfSplines.Num() - 1;

			FString Msg = "New Spline " + FString::FromInt(SplineIndex) + " Generated";
			
		}
		else {
			FString Msg = "At least create a wall before creating a new spline";
		
		}
	}
	else {
		AWallSpline* Spline = GetWorld()->SpawnActor<AWallSpline>(AWallSpline::StaticClass());
		ArrayOfSplines.Add(Spline);
		SplineIndex = ArrayOfSplines.Num() - 1;

		FString Msg = "New Wall Spline " + FString::FromInt(SplineIndex) + " Generated";
		
	}*/

	

	AWallSpline* WallObj = NewObject<AWallSpline>(this);
	ArrayOfSplines.Add(WallObj);
	SplineIndex = ArrayOfSplines.Num() - 1;
}

//void AWallBuilderController::StopGeneratingWall()
//{
//	if (ArrayOfSplines.Num() > 0) {
//		if (ArrayOfSplines[ArrayOfSplines.Num() - 1]->SplineComponent->GetNumberOfSplinePoints() >= 2) {
//			
//			ArrayOfSplines[ArrayOfSplines.Num() - 1]->FinalizeSpline();
//		}
//	}
//}