// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBox.h"
#include "Ball.h"
#include "BaseBallGameMode.h"
#include "InGameUI.h"
#include "StrikeZone.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
	if (AimWidgetClass)
	{
		// 위젯을 생성하고 변수에 저장한 뒤 화면에 추가합니다.
		AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetClass);
		if (AimWidget)
		{
			AimWidget->AddToViewport();
		}
	}
	GetWorld() -> GetFirstPlayerController() ->SetShowMouseCursor(true);
	//StrikeZoneActor = Cast<AStrikeZone>(UGameplayStatics::GetActorOfClass(GetWorld(),AStrikeZone::StaticClass()));
	
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
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    // 0) 마우스 화면 좌표 → 월드 레이
    float MX, MY;
    if (!PC->GetMousePosition(MX, MY)) return;

    FVector RayOrigin, RayDir;
    PC->DeprojectScreenPositionToWorld(MX, MY, RayOrigin, RayDir);

    // 1) "투영할 평면"을 박스 기준으로 정의
    //    - 정중앙 평면: 박스 중심 & 박스 Forward 를 노멀로 사용
    //    - 앞면에 딱 붙이고 싶다면 FaceCenter = Center + Forward * Extent.X 로 변경
    const FVector Center     = HitBoxMesh->GetComponentLocation();
    const FVector PlaneN     = HitBoxMesh->GetForwardVector();   // 박스가 바라보는 방향(노멀)
    const FVector PlanePoint = Center;                           // 정중앙 평면

    // 2) 레이-평면 교점
    const float denom = FVector::DotProduct(PlaneN, RayDir);
    if (FMath::IsNearlyZero(denom)) return;      // 평행

    const float t = FVector::DotProduct(PlaneN, (PlanePoint - RayOrigin)) / denom;
    if (t < 0.f) return;                          // 뒤쪽

    FVector HitOnPlane = RayOrigin + t * RayDir;

  

    // 4) 3D 위치 적용
    AimRoot->SetWorldLocation(HitOnPlane);

    // 5) 위젯은 DPI 보정 포함 좌표로 갱신
    if (AimWidget)
    {
        FVector2D WidgetPos;
        if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, HitOnPlane, WidgetPos, false))
        {
            static const FName Fn = TEXT("UpdateAimPosition");
            if (UFunction* F = AimWidget->FindFunction(Fn))
            {
                struct { FVector2D ScreenPosition; } Params{ WidgetPos };
                AimWidget->ProcessEvent(F, &Params);
            }
        }
    }
}

bool AHitBox::ApplyHit(float Timing, float HeightBat, float SideBat,class ABall* ball)
{
	
	if (Timing >-2 && HeightBat >-2 && SideBat >-2)
	{
		float BallDir = FMath::Lerp(4.5,-4.5,(Timing+1)/2); //당겨치기,정타,밀어치기의 비율
		float BallAngle = FMath::Lerp(5,2,(HeightBat+1)/2); //높이
		float PowerAccuarcy = 1-FMath::Abs(SideBat);//정확도 얼마나 배트의 중심에 맞았는가?
		PowerAccuarcy = FMath::Max(PowerAccuarcy,0.3f);
		FVector NewSpeed = {-1.f,BallDir,BallAngle};
		NewSpeed.Normalize();
		FVector FinalSpeed = NewSpeed*PowerAccuarcy*5000;//5000은 파워값!
		ball->SetActorRotation(FRotator(0,0,0));
		ball->SetBallHit(FinalSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Debug Values -> Timing: %f, HeightBat: %f, SideBat: %f"), Timing, HeightBat, SideBat);
		AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
		ABaseBallGameMode* MyGameMode = Cast<ABaseBallGameMode>(GameModeBase);
		if (MyGameMode)
		{
			MyGameMode->InGameUI->DisplayBallHitDirection(Timing);
		}
		
		return true;
	}
	else
	{
		return false;
	}
	
	
}

float AHitBox::CheckTiming(ABall* Ball)
{
	if (!Ball) return -999.f;

	// 기준 컴포넌트(Z 중심): HitBox가 있으면 그걸, 없으면 Mesh
	const USceneComponent* Comp =
		HitBox ? static_cast<const USceneComponent*>(HitBox)
			   : static_cast<const USceneComponent*>(HitBoxMesh);
	if (!Comp) return -999.f;

	const float centerX = Comp->GetComponentLocation().X;
	const float ballX   = Ball->GetActorLocation().X;
	const float deltaX  = ballX - centerX;

	constexpr float kHalfTiming=100.f;               

	if (FMath::Abs(deltaX) > kHalfTiming)
		return -2.f;                                   // 범위 밖

	// 범위 안: -40 -> -1, 0 -> 0, +40 -> +1
	return deltaX / kHalfTiming;
}




float AHitBox::CheckSide(class ABall* Ball)
{
	if (!Ball) return -999.f;

	// 기준 컴포넌트(Z 중심): HitBox가 있으면 그걸, 없으면 Mesh
	const USceneComponent* Comp =
		HitBox ? static_cast<const USceneComponent*>(HitBox)
			   : static_cast<const USceneComponent*>(HitBoxMesh);
	if (!Comp) return -999.f;

	const float centerY = Comp->GetComponentLocation().Y;
	const float ballY   = Ball->GetActorLocation().Y;
	const float deltaY  = ballY - centerY;             // +면 위, -면 아래

	constexpr float kHalfSide=40.f;                // 허용 범위: -40 ~ +40

	if (FMath::Abs(deltaY) > kHalfSide)
		return -2.f;                                   // 범위 밖

	// 범위 안: -40 -> -1, 0 -> 0, +40 -> +1
	return deltaY / kHalfSide;
}



float AHitBox::CheckHeight(ABall* Ball)
{
	if (!Ball) return -999.f;

	// 기준 컴포넌트(Z 중심): HitBox가 있으면 그걸, 없으면 Mesh
	const USceneComponent* Comp =
		HitBox ? static_cast<const USceneComponent*>(HitBox)
			   : static_cast<const USceneComponent*>(HitBoxMesh);
	if (!Comp) return -999.f;

	const float centerZ = Comp->GetComponentLocation().Z;
	const float ballZ   = Ball->GetActorLocation().Z;
	const float deltaZ  = ballZ - centerZ;             // +면 위, -면 아래

	constexpr float kHalfHeight = 40.f;                // 허용 범위: -40 ~ +40

	if (FMath::Abs(deltaZ) > kHalfHeight)
		return -2.f;                                   // 범위 밖

	// 범위 안: -40 -> -1, 0 -> 0, +40 -> +1
	return deltaZ / kHalfHeight;
}

bool AHitBox::ApplyHitReal(float Timing, float HeightBat, float SideBat, ABall* Ball)
{
    if (!Ball) return false;
    if (Timing <= -1.9f || HeightBat <= -1.9f || SideBat <= -1.9f) return false;

    // ==== 튜닝 파라미터 ====
    const float mBall          = 0.145f;   // kg (스케일용)
    const float PowerBase      = 2000.f;   // 전체 파워 스케일
    const float MinAccFloor    = 0.30f;    // 정확도 하한
    const float MinPitchDeg    = 5.f;      // 최소 발사각(지상타 방지)
    const float MaxPitchDeg    = 40.f;     // 최대 발사각(너무 뜨는 것 방지)
    const float VminClamp      = 1600.f;   // 속도 하한
    const float VmaxClamp      = 5000.f;   // 속도 상한

    // ==== 1) 입력 → 파워/방향 성분 ====
    float Accuracy = 1.f - FMath::Abs(FMath::Clamp(SideBat, -1.f, 1.f));
    Accuracy = FMath::Max(Accuracy, MinAccFloor);

    const float Power   = PowerBase;

    // X 성분: 항상 –(Power*Accuracy) — 전진은 절대값으로 고정
    const float Xcomp   = -Power * Accuracy;

    //  Y 성분: Timing 기반 ‘절대’ 좌우, X와 무관하게 그대로 반영 (보정/스케일 제거)
    const float BallDir = FMath::Lerp(1.5f, -1.5f, (Timing + 1.f) * 0.5f);
    const float Ycomp   = BallDir * Power * Accuracy;

    // ✅ Z 성분: HeightBat 기반, 이후 발사각 보정 단계에서 각도만 클램프
    const float BallAng = FMath::Lerp(5.f, 2.f, (HeightBat + 1.f) * 0.5f);
    float Zcomp         = BallAng * Power * Accuracy;

    FVector Vtarget(Xcomp, Ycomp, Zcomp);

    // ==== 2) 발사각 강제(지상타 방지/너무 뜨는 것 방지) ====
    {
        const float flatMag = FVector(Vtarget.X, Vtarget.Y, 0.f).Size();
        float up = Vtarget.Z;

        const float minUp = flatMag * FMath::Tan(FMath::DegreesToRadians(MinPitchDeg));
        const float maxUp = flatMag * FMath::Tan(FMath::DegreesToRadians(MaxPitchDeg));
        if (up <  minUp) up =  minUp;
        if (up >  maxUp) up =  maxUp;

        Vtarget.Z = up;
    }

  
    {
        const float spd = Vtarget.Size();
        if (spd > KINDA_SMALL_NUMBER)
        {
            const float clamped = FMath::Clamp(spd, VminClamp, VmaxClamp);
            Vtarget = Vtarget * (clamped / spd);
        }
        else
        {
            Vtarget = FVector(-1.f, 0.f, 0.f) * VminClamp;
        }
    }

    // ==== 4) 역학 적용: J로 정당화 (Vout = Vin + J/m = Vtarget) ====
    const FVector Vin = Ball->GetVelocity();
    const FVector dV  = Vtarget - Vin;
    const FVector J   = mBall * dV;

    FVector Vout = Vin + J / mBall; // = Vtarget

    
    if (Vout.X > 0.f) Vout.X = -FMath::Abs(Vout.X);

    // 최종 정규화 + 크기 재확정
    {
        const float Spd = FMath::Clamp(Vout.Size(), VminClamp, VmaxClamp);
        Vout = Vout.GetSafeNormal() * Spd;
    }

    // ==== 5) 적용 ====
    Ball->SetActorRotation(FRotator::ZeroRotator);
    Ball->SetActorLocation(StrikeZoneActor->GetActorLocation());
    Ball->SetBallHit(Vout);

    UE_LOG(LogTemp, Warning,
        TEXT("ApplyHitReal -> T:%f H:%f S:%f | Acc:%f | Power:%f | Vin:%s Vout:%s | J:%s"),
        Timing, HeightBat, SideBat, Accuracy, Power, *Vin.ToString(), *Vout.ToString(), *J.ToString());

    if (ABaseBallGameMode* GM = Cast<ABaseBallGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        if (GM->InGameUI) GM->InGameUI->DisplayBallHitDirection(Timing);
    }
    return true;
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

