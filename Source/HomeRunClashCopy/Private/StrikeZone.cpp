// Fill out your copyright notice in the Description page of Project Settings.


#include "StrikeZone.h"

// Sets default values
AStrikeZone::AStrikeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT("/Script/Engine.Texture2D'/Game/Asset/UI/InGameImage/StrikeZoneUI.StrikeZoneUI'"));
	MeshComp ->SetStaticMesh(tempMesh.Object);
	MeshComp ->SetMaterial(0,tempMaterial.Object);
	RootComponent = MeshComp;
	

}

float AStrikeZone::GetRatioInStrikeZone(class ABall* Ball)
{
	float Timing=0;
	if (Ball == nullptr)
	{
		return -2;
	}
	else if (Ball)
	{
		FTransform BallTransform = Ball->GetActorTransform();
		FTransform StrikeZoneTransform = MeshComp->GetComponentTransform();
		const FVector LocalPositionInBox = StrikeZoneTransform.InverseTransformPosition(BallTransform.GetLocation());
		//1.큐브 Extent
		FVector LocalMin, LocalMax;
		MeshComp->GetLocalBounds(LocalMin, LocalMax);
		FVector CenterLocal = (LocalMin+LocalMax)*0.5f;
		FVector ExtentLocal = (LocalMax-LocalMin)*0.5f; //half
		FVector LocalCenter = LocalPositionInBox - CenterLocal;
		const float SafeEx = FMath::Max(ExtentLocal.X, 1e-6f);

		// 4) 로컬 X축 기준 정규화
		Timing = (LocalPositionInBox.X - CenterLocal.X) / SafeEx;
		Timing = FMath::Clamp(Timing,-1,1);
		if (Timing>1 || Timing<-1)
		{
			Timing= -2;
		}
		
	
	}
	return Timing;
}

// Called when the game starts or when spawned
void AStrikeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStrikeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

