// Fill out your copyright notice in the Description page of Project Settings.


#include "StageFailUI.h"

void UStageFailUI::NativeConstruct()
{
	Super::NativeConstruct();

	// InGameUI 불러서

	// FPlayOST PlayClearOST 바인드

	//FPlayOST PlayFailOST 바인드
}

void UStageFailUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStageFailUI::PlayFailAnim()
{
	// 슬퍼하는 애니메이션 연결
}

void UStageFailUI::PlayFailOST()
{
	// 실패 노래 재생
}
