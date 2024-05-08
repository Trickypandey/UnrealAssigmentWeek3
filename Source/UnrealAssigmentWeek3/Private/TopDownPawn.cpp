#include "TopDownPawn.h"
#include "EnhancedInputComponent.h"
// Sets default values

static void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate = false
	, bool bSwizzle = false, EInputAxisSwizzle SwizzeleOrder = EInputAxisSwizzle::YXZ)
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
		Swizzle->Order = SwizzeleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

ATopDownPawn::ATopDownPawn()
{

	PrimaryActorTick.bCanEverTick = true;

	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	//CapsuleComponent->SetCapsuleHalfHeight(100);
	//CapsuleComponent->SetCapsuleRadius(40);
	////CapsuleComponent->SetEnableGravity(true);
	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = -211.599960;
	SpringArm->TargetOffset = FVector(-520, 0, 540);
	SpringArm->SetupAttachment(SpringArm);
	RootComponent = SpringArm;
	float Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(Angle, 0, 0));
	Camera->SetupAttachment(SpringArm);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
}

// Called when the game starts or when spawned
void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* Player = Cast<APlayerController>(GetController());

	MappingContext = NewObject<UInputMappingContext>();
	MoveAction = NewObject<UInputAction>();
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);
	KeyMap(MappingContext,MoveAction, EKeys::W);
	KeyMap(MappingContext,MoveAction, EKeys::S , true);
	KeyMap(MappingContext,MoveAction, EKeys::D , false,true);
	KeyMap(MappingContext,MoveAction, EKeys::A , true,true);


	//For Zooming

	ScrollAction = NewObject<UInputAction>();
	ScrollAction->ValueType = EInputActionValueType::Axis1D;
	KeyMap(MappingContext,ScrollAction, EKeys::MouseWheelAxis);

	if (EIC) {
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Move);
		EIC->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &ATopDownPawn::ZoomInOut);
	}

	//Binding Actions
	if (Player) {
		ULocalPlayer* LocalPlayer = Player->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(MappingContext, 0);
	}

}

void ATopDownPawn::Move(const FInputActionValue& ActionValue)
{
	FVector InputAxis = ActionValue.Get<FVector>();
	AddMovementInput(InputAxis, 2.0F);
}

void ATopDownPawn::ZoomInOut(const FInputActionValue& ActionValue)
{
	float Value = ActionValue.Get<float>();

	float TargetArmLength = SpringArm->TargetArmLength;
	float TargetOffsetZ = SpringArm->TargetOffset.Z;

	if (TargetArmLength >= -460) {
		SpringArm->TargetOffset.Z -= Value * 20;
	}
	if (TargetOffsetZ <= 1500) {
		SpringArm->TargetArmLength -= Value * 9.2;
	}

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, -500, -0);
	SpringArm->TargetOffset.Z = FMath::Clamp(SpringArm->TargetOffset.Z, 0, 1500);

	FString FloatAsString = FString::SanitizeFloat(SpringArm->TargetArmLength);
	FString DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("armlength: %f"), SpringArm->TargetArmLength));

	FloatAsString = FString::SanitizeFloat(SpringArm->TargetOffset.Z);
	DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("offsetZ: %f"), SpringArm->TargetOffset.Z));

	float Angle;
	if (SpringArm->TargetArmLength == 0) {
		Angle = -90;
	}
	else {
		Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));
		Angle = FMath::Clamp(Angle, -90, 0);
	}

	if (Angle < -90) {
		Angle = -90;
	}
	if (Angle > 0) {
		Angle = 0;
	}

	FloatAsString = FString::SanitizeFloat(Angle);
	DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Angle: %f"), Angle));

	Camera->SetRelativeRotation(FRotator(Angle, 0, 0));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("ROTATION: %f"), *Camera->GetRelativeRotation().ToString()));

}