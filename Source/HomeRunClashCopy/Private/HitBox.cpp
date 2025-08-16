// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBox.h"
#include "Ball.h"
#include "StrikeZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHitBox::AHitBox()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1.컴포넌트 설정
	MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRootComponent;
	CursorPlaneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CursorPlaneRoot"));
	AimRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AimRoot"));

	CursorPlaneRoot ->SetupAttachment(MyRootComponent);
	AimRoot->SetupAttachment(MyRootComponent);

	CursorPlane = CreateDefaultSubobject<UBoxComponent>(TEXT("CursorPlane"));
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitBoxMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT("/Script/Engine.Material'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));

	HitBoxMesh ->SetStaticMesh(tempMesh.Object);
	HitBoxMesh ->SetMaterial(0,tempMaterial.Object);
	
	CursorPlane->SetupAttachment(CursorPlaneRoot);
	HitBox->SetupAttachment(AimRoot);
	HitBoxMesh->SetupAttachment(AimRoot);

	//2.BoxExtenet 설정
	CursorPlane->SetBoxExtent(FVector(5,100000.f,100000.f));
	


	
}

// Called when the game starts or when spawned
void AHitBox::BeginPlay()
{
	Super::BeginPlay();
	GetWorld() -> GetFirstPlayerController() ->SetShowMouseCursor(true);
	
}

void AHitBox::InitCursorPlane()
{
	APlayerController* PlayerController = GetWorld() ->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI Input;
		Input.SetHideCursorDuringCapture(true);
		Input.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController-> SetInputMode(Input);
		HitBox->SetAbsolute(true,false,false);

		CamLoc = PlayerController->PlayerCameraManager->GetCameraLocation();
		CamRot = PlayerController->PlayerCameraManager->GetCameraRotation();
		CamFwd = PlayerController->PlayerCameraManager->GetCameraRotation().Vector().ForwardVector;

		FVector CamInitPosition = CamLoc+(CamFwd*DepthOffset);
		CursorPlaneRoot->SetWorldLocation(CamInitPosition);
		CursorPlaneRoot->SetWorldRotation(CamRot);
		Plane = CursorPlaneRoot-> GetComponentLocation();
	}
}

void AHitBox::RecognizeCursorInPlane()
{
	APlayerController* PlayerController = GetWorld() ->GetFirstPlayerController();
	if (PlayerController)
	{
		ECollisionChannel TraceChannel = ECC_GameTraceChannel1;
		FHitResult HitResult;
		bool BHit = PlayerController->GetHitResultUnderCursorByChannel
		( UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),true,HitResult);

		if (BHit)
		{
			FVector CursorImpactPoint = HitResult.ImpactPoint;
			FVector ToPlane = CursorImpactPoint - Plane;
			float Depth = FVector::DotProduct(ToPlane,CamFwd);
			FVector Fixed2D = CursorImpactPoint-Depth*CamFwd;
			AimRoot->SetWorldLocation(Fixed2D);
		}
		
	}
}

void AHitBox::ApplyHit(float Timing, float HeightBat, float SideBat,class ABall* ball)
{
	//ball.SetBallHit()
	if (Timing >-2 && HeightBat >-2 && SideBat >-2)
	{
		float BallDir = FMath::Lerp(1,-1,(Timing+1)/2); //당겨치기,정타,밀어치기의 비율
		float BallAngle = FMath::Lerp(1,-1,(HeightBat+1)/2); //높이
		float PowerAccuarcy = 1-FMath::Abs(SideBat);//정확도 얼마나 배트의 중심에 맞았는가?
		FVector NewSpeed = {-1.f,BallDir,BallAngle};
		NewSpeed.Normalize();
		FVector FinalSpeed = NewSpeed*PowerAccuarcy*5000;//5000은 파워값!
		ball->SetBallHit(FinalSpeed);
	}
	else
	{
		
	}
	return;
	
}

float AHitBox::CheckTiming(class ABall* Ball)
{
	ABall* BallActor = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(),ABall::StaticClass()));
	AStrikeZone* StrikeZoneActor = Cast<AStrikeZone>(UGameplayStatics::GetActorOfClass(GetWorld(),AStrikeZone::StaticClass()));
	return StrikeZoneActor->GetRatioInStrikeZone(BallActor);
}
float AHitBox::CheckSide(class ABall* Ball)
{
	FTransform BallTransform = Ball->GetActorTransform();
	FTransform StrikeZoneTransform = HitBoxMesh->GetComponentTransform();
	const FVector LocalPositionInBox = StrikeZoneTransform.InverseTransformPosition(BallTransform.GetLocation());
	//1.큐브 Extent
	FVector LocalMin, LocalMax;
	HitBoxMesh->GetLocalBounds(LocalMin, LocalMax);
	FVector CenterLocal = (LocalMin+LocalMax)*0.5f;
	FVector ExtentLocal = (LocalMax-LocalMin)*0.5f; //half
	FVector LocalCenter = LocalPositionInBox - CenterLocal;
	const float SafeEx = FMath::Max(ExtentLocal.Y, 1e-6f);

	// 4) 로컬 X축 기준 정규화
	float Side= (LocalPositionInBox.Y - CenterLocal.Y) / SafeEx;
	if (Side>1 || Side<-1)
	{
		Side= -2;
	}
	Side = FMath::Clamp(Side,-1,1);
	
	return -1;
}
float AHitBox::CheckHeight(class ABall* Ball)
{
	
	FTransform BallTransform = Ball->GetActorTransform();
	FTransform StrikeZoneTransform = HitBoxMesh->GetComponentTransform();
	const FVector LocalPositionInBox = StrikeZoneTransform.InverseTransformPosition(BallTransform.GetLocation());
	//1.큐브 Extent
	FVector LocalMin, LocalMax;
	HitBoxMesh->GetLocalBounds(LocalMin, LocalMax);
	FVector CenterLocal = (LocalMin+LocalMax)*0.5f;
	FVector ExtentLocal = (LocalMax-LocalMin)*0.5f; //half
	FVector LocalCenter = LocalPositionInBox - CenterLocal;
	const float SafeEx = FMath::Max(ExtentLocal.Z, 1e-6f);

	// 4) 로컬 X축 기준 정규화
	float Height= (LocalPositionInBox.Z - CenterLocal.Z) / SafeEx;
	if (Height>1 || Height<-1)
	{
		Height= -2;
	}
	Height = FMath::Clamp(Height,-1,1);
	
	return -1;
}

// Called every frame
void AHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RecognizeCursorInPlane();

}


// Called to bind functionality to input
void AHitBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

}

