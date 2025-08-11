// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "AirResistanceLibraryFunction.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("/Game/Asset/Ball/BallMesh.BallMesh"));
	if (BallMeshAsset.Succeeded())
	{
		BallMesh->SetStaticMesh(BallMeshAsset.Object);
		RootComponent = BallMesh;
		BallMesh->SetWorldScale3D(FVector(2.5f, 2.5f, 2.5f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load BallMesh"));
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DragCurveObj(TEXT("/Game/LibraryFunction/Curve_DragCoefficient.Curve_DragCoefficient"));
	if (DragCurveObj.Succeeded())
	{
		DragCoefficientCurve = DragCurveObj.Object;
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMove == true)
	{
		CalculateGravity(DeltaTime);
		CalculateMagnusSimple(DeltaTime);
		Velocity = UAirResistanceLibraryFunction::AirResistanceCpp(Velocity, DragCoefficientCurve);
		UpdateLocation(DeltaTime);
	}
}

void ABall::Init(FBallInfo BI)
{
	BallInfo = BI;
	Velocity = BI.Speed * BI.Dir;
	IsMove = true;
}

void ABall::SetBallHit(FVector HitVelocity)
{
	Velocity = HitVelocity;
	BallInfo.Rotation = FVector(0, 0, 0);
}

void ABall::CalculateGravity(float DeltaTime)
{
	Velocity = Velocity + Gravity * DeltaTime;
}

void ABall::CalculateMagnusSimple(float DeltaTime)
{
	Velocity = Velocity + FVector::CrossProduct(BallInfo.Rotation, Velocity) * MagnusCoeff * DeltaTime;
}

void ABall::UpdateLocation(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
}


