// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void AMyPawn::CacheScreenPlaneDistance()
{
	Super::BeginPlay();

	if (!Cube)
	{
		// 자동으로 첫 StaticMeshComponent를 잡아주고 싶으면 주석 해제
		// Cube = FindComponentByClass<UStaticMeshComponent>();
	}

	if (bAbsoluteLocationForCube && Cube)
	{
		if (USceneComponent* SC = Cast<USceneComponent>(Cube))
		{
#if ENGINE_MAJOR_VERSION >= 5
			SC->SetAbsolute(true, false, false);   // UE5.x
#else
			SC->SetAbsolute(true, false, false);        // UE4.x
#endif
		}
	}

	// 마우스 커서 보이기(원하면 GameAndUI)
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameOnly());
	}

	CacheScreenPlaneDistance();
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	if (bAbsoluteLocationForCube && Cube)
	{
		if (USceneComponent* SC = Cast<USceneComponent>(Cube))
		{
			// UE5: SetUsingAbsolute / UE4: SetAbsolute
			SC->SetAbsolute(true, false, false);
		}
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeGameOnly());

		// 큐브가 처음에 서있는 '화면 평면' 깊이(카메라 기준 전방 거리) 기록
		const FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		const FVector CamFwd = PC->PlayerCameraManager->GetCameraRotation().Vector();
		const FVector CubeLoc = Cube ? Cube->GetComponentLocation() : GetActorLocation();
		// 카메라 기준 전방 거리(양수면 카메라 앞)
		ScreenPlaneDistance = FVector::DotProduct(CubeLoc - CamLoc, CamFwd);
	}
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC || !Cube) return;

	// 1) 스크린 → 월드 광선
	FVector RayOrigin, RayDir;
	if (!PC->DeprojectMousePositionToWorld(RayOrigin, RayDir)) return;

	// 2) '스크린 평면' 정의: 카메라 Forward에 수직이고, 카메라에서 ScreenPlaneDistance만큼 앞
	const FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
	const FVector CamFwd = PC->PlayerCameraManager->GetCameraRotation().Vector();
	const FVector PlanePoint = CamLoc + CamFwd * ScreenPlaneDistance;
	const FPlane  ScreenPlane(PlanePoint, CamFwd);

	// 3) 광선-평면 교점 = 화면상 동일 위치의 월드 좌표
	FVector Intersection;
	const bool bHit = FMath::SegmentPlaneIntersection(
		RayOrigin, RayOrigin + RayDir * 100000.f, ScreenPlane, Intersection);
	if (!bHit) return;

	// 4) (선택) 스트라이크존 로컬 공간에서 XY 클램프
	if (bClampInZone && StrikeZone)
	{
		const FTransform ZoneXf = StrikeZone->GetComponentTransform();
		FVector Local = ZoneXf.InverseTransformPosition(Intersection);
		Local.X = FMath::Clamp(Local.X, MinLocalX, MaxLocalX);
		Local.Y = FMath::Clamp(Local.Y, MinLocalY, MaxLocalY);
		Intersection = ZoneXf.TransformPosition(Local);
	}

	// 5) 적용(충돌 때문에 끊기면 bSweep=false로 테스트)
	Cube->SetWorldLocation(Intersection, /*bSweep*/true);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

