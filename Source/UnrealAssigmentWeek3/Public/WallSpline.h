// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "WallSpline.generated.h"


UCLASS()
class UNREALASSIGMENTWEEK3_API AWallSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallSpline();

	void OnConstruction(const FTransform& Transform) override;
	UPROPERTY()
	TArray<FVector> PreviousSplinePoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMesh* WallMesh;

	UFUNCTION()
	void AddSplinePoint(const FVector& Points);


	UFUNCTION()
	void CreateWalls();


	UFUNCTION()
	void CreateWall(const FVector& StartPos, const FVector& StartTangent, const FVector& EndPos, const FVector& EndTangent);

	bool UndoLastAction();

	/*UFUNCTION()
	void FinalizeSpline();*/

	
	UPROPERTY()
	TArray<class USplineMeshComponent*> WallMeshComponents;


	UPROPERTY(EditAnywhere, Category = "Spline")
	float WallThickness;

	UPROPERTY(EditAnywhere, Category = "Spline")
	float WallHeight;
};
