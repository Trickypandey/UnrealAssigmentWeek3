// TopDownPawn.cpp
#include "TopDownPawn.h"

static void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate = false
	, bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	auto& Mapping = InputMappingContext->MapKey(InputAction, Key);
	auto* Outer = InputMappingContext->GetOuter();
	if (bNegate)
	{
		auto* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}
	if (bSwizzle)
	{
		auto* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

ATopDownPawn::ATopDownPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;
	SpringArm->bDoCollisionTest = false;

	TopDownPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(SpringArm);
}

void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;
	KeyMap(PawnMappingContext, MoveAction, EKeys::W);
	KeyMap(PawnMappingContext, MoveAction, EKeys::S, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::A, true, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::D, false, true);
	/*KeyMap(PawnMappingContext, MoveAction, EKeys::Q, true, true, EInputAxisSwizzle::ZYX);
	KeyMap(PawnMappingContext, MoveAction, EKeys::E, false, true, EInputAxisSwizzle::ZYX);*/

	//LookAction = NewObject<UInputAction>(this);
	//LookAction->ValueType = EInputActionValueType::Axis2D;
	//KeyMap(PawnMappingContext, LookAction, EKeys::MouseY);
	//KeyMap(PawnMappingContext, LookAction, EKeys::MouseX, false, true);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;

	KeyMap(PawnMappingContext, ZoomAction, EKeys::MouseScrollUp);
	KeyMap(PawnMappingContext, ZoomAction, EKeys::MouseScrollDown, true);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* FPC = GetWorld()->GetFirstPlayerController();
	check(EIC && FPC);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::MoveForward);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Look);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATopDownPawn::ZoomIn);

	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);
}

void ATopDownPawn::MoveForward(const FInputActionValue& ActionValue)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "In MoveForward");
	FVector Input = ActionValue.Get<FVector>();
	Input.Z = 0;
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
}

//void ATopDownPawn::Look(const FInputActionValue& ActionValue)
//{
//	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
//	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
//	Input += GetActorRotation();
//	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
//	Input.Roll = 0;
//	SetActorRotation(Input);
//}

void ATopDownPawn::ZoomIn(const FInputActionValue& ActionValue)
{
	if (SpringArm)
	{
		float value = ActionValue.Get<float>();

		
		SpringArm->TargetArmLength -= value * 100;
		if (SpringArm->TargetArmLength < 100)
		{
			return;
		}
		FString temp = FString::Printf(TEXT("WSpringarm: %.2f, Yaw: %.2f, Roll: %.2f"), SpringArm->TargetArmLength, 0.0f, 0.0f);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, temp);


		FVector NewTargetOffset = SpringArm->TargetOffset;
		NewTargetOffset.Z = FMath::Clamp(NewTargetOffset.Z + value * 10, -500.0f, 0.0f); 
		SpringArm->TargetOffset = NewTargetOffset;

		
		float NewPitch = FMath::Clamp(SpringArm->GetRelativeRotation().Pitch + value * 2, -80.0f, -10.0f);
		if (NewPitch > -25)
		{
			SpringArm->SetRelativeRotation(FRotator(NewPitch, 0, 0));
		}


		FString RotationString = FString::Printf(TEXT("Pitch: %.2f, Yaw: %.2f, Roll: %.2f"), NewPitch, 0.0f, 0.0f);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, RotationString);
	}
}

