// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		BallMesh->SetStaticMesh(SphereMeshAsset.Object);
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

	if (IsInit == true)
	{
		CalculateGravity(DeltaTime);
		CalculateMagnusSimple(DeltaTime);
		UpdateLocation(DeltaTime);
	}
}

void ABall::Init(FBallInfo BI)
{
	BallInfo = BI;
	Velocity = BI.Speed * BI.Dir;
	IsInit = true;
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


