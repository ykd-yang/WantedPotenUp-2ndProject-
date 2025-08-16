// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBox.h"
#include "Ball.h"
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

void AHitBox::ApplyHit(float Timing, float HeightBat, float SideBat,class ABall* ball)
{
	
	if (Timing >-2 && HeightBat >-2 && SideBat >-2)
	{
		float BallDir = FMath::Lerp(1,-1,(Timing+1)/2); //당겨치기,정타,밀어치기의 비율
		float BallAngle = FMath::Lerp(1,-1,(HeightBat+1)/2); //높이
		float PowerAccuarcy = 1-FMath::Abs(SideBat);//정확도 얼마나 배트의 중심에 맞았는가?
		FVector NewSpeed = {-1.f,BallDir,BallAngle};
		NewSpeed.Normalize();
		FVector FinalSpeed = NewSpeed*PowerAccuarcy*5000;//5000은 파워값!
		ball->SetActorRotation(FRotator(0,0,0));
		ball->SetBallHit(FinalSpeed);
		UE_LOG(LogTemp, Warning, TEXT("Debug Values -> Timing: %f, HeightBat: %f, SideBat: %f"), Timing, HeightBat, SideBat);
	}
	
	return;
	
}

float AHitBox::CheckTiming(ABall* Ball)
{
	if (!Ball) return -999.f;

	// 기준 컴포넌트: HitBox(UBoxComponent)가 있으면 그걸, 없으면 Mesh
	const USceneComponent* Comp =
		HitBox ? static_cast<const USceneComponent*>(HitBox)
			   : static_cast<const USceneComponent*>(HitBoxMesh);
	if (!Comp) return -999.f;

	// 배트 중심/타이밍 축(로컬 X+) 기준
	const FVector Center  = Comp->GetComponentLocation();
	const FVector Forward = Comp->GetForwardVector();   // 타이밍 축

	// 공이 배트 중심에서 타이밍 축으로 얼마나 앞/뒤에 있는지
	const float offsetX = FVector::DotProduct(Ball->GetActorLocation() - Center, Forward);

	// 허용 범위(반폭). 예: ±60cm 안이면 유효 타격 구간
	constexpr float kHalfDepth = 60.f;

	// 범위 밖이면 -2
	if (FMath::Abs(offsetX) > kHalfDepth)
		return -2.f;

	// 범위 안이면 -1..1로 매핑: -kHalfDepth -> -1, 0 -> 0, +kHalfDepth -> +1
	float ratio = offsetX / kHalfDepth;

	// 만약 네 게임에서 "빠른 타이밍이 음수/양수" 방향이 반대라면 아래처럼 부호만 뒤집어:
	// ratio = -ratio;

	// (선택) 디버그
	// UE_LOG(LogTemp, Log, TEXT("[Timing] offsetX=%.1f, half=%.1f, ratio=%.3f"),
	//        offsetX, kHalfDepth, ratio);

	return ratio;
}




float AHitBox::CheckSide(class ABall* Ball)
{
	// 1) 공의 월드 위치를 배트(히트박스) 로컬로 변환
	const FTransform BoxXform = HitBoxMesh->GetComponentTransform();
	const FVector   BallLocal = BoxXform.InverseTransformPosition(Ball->GetActorLocation());

	// 2) 히트박스의 로컬 바운드(min/max) 얻기  ← 올바른 API 사용
	FVector LocalMin, LocalMax;
	HitBoxMesh->GetLocalBounds(LocalMin, LocalMax);

	const FVector CenterLocal  = (LocalMin + LocalMax) * 0.5f;
	const FVector ExtentLocal  = (LocalMax - LocalMin) * 0.5f;
	const float   SafeExtentY  = FMath::Max(ExtentLocal.Y, 1e-6f); // 0 나눗셈 보호

	// 3) 배트 로컬 Y축 기준 좌우 비율 (-1~1)
	const float Side = (BallLocal.Y - CenterLocal.Y) / SafeExtentY;

	// 4) 바운드 밖이면 -2, 안이면 -1~1 반환
	return (Side < -1.f || Side > 1.f) ? -2.f : Side;
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

