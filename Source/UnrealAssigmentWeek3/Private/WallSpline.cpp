// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpline.h"


// Sets default values
AWallSpline::AWallSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->ClearSplinePoints();
	WallMeshComponents.Empty();


	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);

	}

}

void AWallSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("in CreateWall: %d"), SplineComponent->GetNumberOfSplinePoints()));
	//if (!WallMesh)
	//{
	//	return;
	//}
	//for (int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints()); SplineCount++) {
	//	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	//	SplineMesh->SetStaticMesh(WallMesh);
	//	SplineMesh->SetMobility(EComponentMobility::Movable);
	//	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	//	SplineMesh->RegisterComponentWithWorld(GetWorld());
	//	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//	

	//	const FVector StartPos = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
	//	const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
	//	const FVector EndPos = SplineComponent->GetLocationAtSplinePoint(SplineCount+1, ESplineCoordinateSpace::Local);
	//	const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount+1, ESplineCoordinateSpace::Local);
	//	SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent , true);
	//	SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//}
}

// Called when the game starts or when spawned
void AWallSpline::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("in CreateWall: %d"), SplineComponent->GetNumberOfSplinePoints()));
	/*AddSplinePoint(FVector(0, 0, 0));
	AddSplinePoint(FVector(100, 0, 0));
	AddSplinePoint(FVector(100, 100, 0));*/

	
	
}

// Called every frame
void AWallSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}


void AWallSpline::AddSplinePoint(const FVector& Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("in CreateWall: %d"), SplineComponent->GetNumberOfSplinePoints()));
	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	if (SplineComponent->GetNumberOfSplinePoints() > 1)
	{
		CreateWalls();
	}
}

void AWallSpline::CreateWalls()
{
    // Clear existing wall mesh components
    for (UStaticMeshComponent* WallMeshComponent : WallMeshComponents)
    {
        if (WallMeshComponent) {
            WallMeshComponent->DestroyComponent();
        }
    }
    WallMeshComponents.Empty();

    // Create walls between each pair of spline points
    int32 NoOfSplinePoints = SplineComponent->GetNumberOfSplinePoints() - 1;
    for (int32 SplineCount = 0; SplineCount < NoOfSplinePoints; ++SplineCount)
    {
        FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
        FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);
        FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
        FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);
        CreateWall(StartLocation, StartTangent, EndLocation, EndTangent);
    }
}

void AWallSpline::CreateWall(const FVector& StartPos, const FVector& StartTangent, const FVector& EndPos , const FVector& EndTangent)
{

	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	WallMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200'"));
	SplineMesh->SetStaticMesh(WallMesh);
	SplineMesh->SetMobility(EComponentMobility::Movable);
	SplineMesh->RegisterComponentWithWorld(GetWorld());
	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WallMeshComponents.Add(SplineMesh);

	SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	//SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


//void AWallSpline::FinalizeSpline()
//{
//	if (SplineComponent->GetNumberOfSplinePoints() >= 2)
//	{
//		
//		SplineComponent->SetClosedLoop(true);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Cannot finalize spline: Not enough points"));
//	}
//}