// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class HOMERUNCLASHCOPY_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim", meta=(EditInline="true"))
	UStaticMeshComponent* Cube = nullptr;

	/** 큐브가 부모 회전 영향 안 받도록 위치만 절대좌표로(자식일 때 추천) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	bool bAbsoluteLocationForCube = true;

	/** (선택) 스트라이크존 기준으로 로컬 클램프할지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	bool bClampInZone = false;

	/** (선택) 스트라이크존(회전/스케일 포함) – 여기에 로컬로 변환해서 클램프 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim", meta=(EditInline="true"))
	USceneComponent* StrikeZone = nullptr;

	/** 로컬 클램프 범위(StrikeZone 로컬 기준) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float MinLocalX = -50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float MaxLocalX =  50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float MinLocalY = -50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	float MaxLocalY =  50.f;

		
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float ScreenPlaneDistance = 0.f;

	/** 필요 시 카메라/큐브 변경 후 호출해서 스크린 평면 거리 갱신 */
	void CacheScreenPlaneDistance();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
