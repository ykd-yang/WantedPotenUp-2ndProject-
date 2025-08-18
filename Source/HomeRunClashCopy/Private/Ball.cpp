// Fill out your copyright notice in the Description page of Project Settings.
#include "Ball.h"
#include "AirResistanceLibraryFunction.h"
#include "BaseBallGameMode.h"
#include "Engine/EngineTypes.h"
#include "Components/PrimitiveComponent.h"
#include "Indicator.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("/Game/Asset/Ball/BallMesh.BallMesh"));
	if (BallMeshAsset.Succeeded())
	{
		BallMesh->SetStaticMesh(BallMeshAsset.Object);
		BallMesh->SetWorldScale3D(FVector(2.5f, 2.5f, 2.5f));
		BallMesh->SetNotifyRigidBodyCollision(true);
		BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BallMesh->SetCollisionObjectType(ECC_GameTraceChannel3);
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECR_Overlap);
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Overlap);
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Block);
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Block);
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECR_Block); //homerun
		BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECR_Block); //ground
		
		// Block 이벤트 받기위해 physics만 켜주기
		BallMesh->SetSimulatePhysics(true);
		BallMesh->SetEnableGravity(false);
		BallMesh->SetLinearDamping(0.f);
		BallMesh->SetAngularDamping(0.f);
		
		SetRootComponent(BallMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load BallMesh"));
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> DragCurveObj(TEXT("/Game/LibraryFunction/Curve_DragCoefficient.Curve_DragCoefficient"));
	if (DragCurveObj.Succeeded())
	{
		DragCoefficientCurve = DragCurveObj.Object;
	}

	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Trail->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailEffect(TEXT("/Game/Effect/NS_BallTail.NS_BallTail"));
	if (TrailEffect.Succeeded())
	{
		Trail->SetAsset(TrailEffect.Object);
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	Gm = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	if (Gm == nullptr)
	{	
		UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
	}
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSimulate)
	{
		ElapsedTime += DeltaTime;
		OnSimulate(ElapsedTime);
		
		if (ElapsedTime > 1.5)
		{
			IsSimulate = false;
		}
	}

	if (IsMove == true)
	{
		CalculateGravity(Velocity);
		CalculateMagnusSimple(Velocity);
		Velocity = UAirResistanceLibraryFunction::AirResistanceCpp(Velocity, DragCoefficientCurve);
		UpdateLocation(DeltaTime);
		UpdateRotation(DeltaTime);
	}
}

void ABall::Init(FBallInfo BI, FVector Location)
{
	BallInfo = BI;
	BI.Dir.Normalize();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *BI.Dir.ToString());
	Velocity = BI.Speed * BI.Dir;
	StartLocation = Location;

	//Simulate - to Show indicator
	SimulLocation = StartLocation;
	SimulVelocity = Velocity;
	IsSimulate = true;

	Trail->Deactivate();
	//Trail->Activate(true);
}

void ABall::SetBallMove()
{
	SetActorLocation(StartLocation);
	IsMove = true;
}

void ABall::SetBallHit(FVector HitVelocity)
{
	Velocity = HitVelocity;
	BallInfo.Rotation = FVector(0, 0, 0);
	
	Trail->Activate(true);
}

void ABall::CalculateGravity(FVector& Vel)
{
	Vel = Vel + Gravity * GetWorld()->GetDeltaSeconds();
}

void ABall::CalculateMagnusSimple(FVector& Vel)
{
	Vel = Vel + FVector::CrossProduct(BallInfo.Rotation, Vel) * MagnusCoeff * GetWorld()->GetDeltaSeconds();
}

void ABall::UpdateLocation(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
}

void ABall::UpdateRotation(float DeltaTime)
{
	if (BallInfo.Rotation.IsZero())
	{
		FVector RotAxis = FVector::CrossProduct(FVector::UpVector, Velocity);
		float Angle = Velocity.Size()/ 2.5f * DeltaTime;
		FQuat QuatRot(RotAxis, FMath::DegreesToRadians(Angle));
		AddActorWorldRotation(QuatRot);
	}
	else
	{
		FRotator Rot = FRotator(BallInfo.Rotation.Y, BallInfo.Rotation.Z, BallInfo.Rotation.X);
		AddActorWorldRotation(Rot * DeltaTime);
	}
}

void ABall::OnSimulate(float ElapTime)
{
	PrevLocation = SimulLocation;
	CalculateGravity(SimulVelocity);
	CalculateMagnusSimple(SimulVelocity);
	SimulVelocity = UAirResistanceLibraryFunction::AirResistanceCpp(SimulVelocity, DragCoefficientCurve);
	SimulLocation = PrevLocation + SimulVelocity * GetWorld()->GetDeltaSeconds();

	//Debug Line
	// DrawDebugLine(GetWorld(), PrevLocation, SimulLocation, FColor::Red,
	// 	false, 1.f, 0, 5);

	FHitResult HitResult;
	bool Hit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		PrevLocation,
		SimulLocation,
		ECollisionChannel::ECC_Visibility);

	if (Hit)
	{
		AIndicator* Ind = Cast<AIndicator>(HitResult.GetActor());
		if (Ind)
		{
			IsSimulate = false;
			Ind->OnLinetraceHit(HitResult.Location);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Indicator"));
		}
	}
}


//바닥에 통통튀는 것
void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (Velocity.Size() < 10) return;
	
	// if 지면 or homruen인지 확인
	if (IsFall == false)
	{
		IsFall = true;
		BallInfo.Rotation = FVector(0, 0, 0);
		ECollisionChannel Channel = Hit.GetComponent()->GetCollisionObjectType();
		if (Channel == ECC_GameTraceChannel6)
		{
			Gm->isHomerun = true;
			UE_LOG(LogTemp, Warning, TEXT("HomeRun HIT"));
		}
		else if (Channel == ECC_GameTraceChannel7)
		{
			Gm->isHomerun = false;
			UE_LOG(LogTemp, Warning, TEXT("Ground HIT"));
		}

		Gm->didBallFall = true;
	}
	
	FVector ReflectVec = Velocity -2 * FVector::DotProduct(Velocity, Hit.Normal) * Hit.Normal;
	Velocity = ReflectVec * 0.35f;
}
