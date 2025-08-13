// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBox.h"

#include "Components/BoxComponent.h"

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

