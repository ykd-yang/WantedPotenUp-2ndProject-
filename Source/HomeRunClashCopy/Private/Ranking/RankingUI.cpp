// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/RankingUI.h"

#include "BaseBallGameInstance.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Ranking/RankingDataUI.h"
#include "Ranking/RankingData.h"

URankingUI::URankingUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<URankingDataUI> WBP_RankingDataUI(TEXT("/Game/BP/WBP_RankingData.WBP_RankingData_C"));
	if (WBP_RankingDataUI.Succeeded())
	{
		RankingUIClass = WBP_RankingDataUI.Class;
	}
}

void URankingUI::NativeConstruct()
{
	Super::NativeConstruct();

	//랭킹 가져오기
	UBaseBallGameInstance* GI = Cast<UBaseBallGameInstance>(GetGameInstance());
	if (GI)
	{
		RankingList->ClearChildren();
		TArray<FRankingData> RankingDataArray= GI->GetRankingData();
		
		for (int i = 0; i < RankingDataArray.Num(); i++)
		{
			URankingDataUI* RankingUI = CreateWidget<URankingDataUI>(GetWorld(), RankingUIClass);
			RankingUI->SetUIRankText(i+1, RankingDataArray[i]);
			RankingList->AddChild(RankingUI);
		}
	}

	//버튼 바인드
	ExitButton->OnClicked.AddDynamic(this, &URankingUI::OnPressExitButton);
}

void URankingUI::OnPressExitButton()
{
	SetVisibility(ESlateVisibility::Hidden);
}
