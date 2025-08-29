// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/RankingUI.h"

#include "BaseBallGameInstance.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Ranking/RankingDataUI.h"
#include "Ranking/RankingData.h"
#include "Ranking/RankingDataManager.h"

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

	//로컬 랭킹 가져오기
	// UBaseBallGameInstance* GI = Cast<UBaseBallGameInstance>(GetGameInstance());
	// if (GI)
	// {
	// 	RankingList->ClearChildren();
	// 	TArray<FRankingData> RankingDataArray= GI->GetRankingData();
	// 	
	// 	for (int i = 0; i < RankingDataArray.Num(); i++)
	// 	{
	// 		URankingDataUI* RankingUI = CreateWidget<URankingDataUI>(GetWorld(), RankingUIClass);
	// 		RankingUI->SetUIRankText(i+1, RankingDataArray[i]);
	// 		RankingList->AddChild(RankingUI);
	// 	}
	// }

	//FireBase에서 랭킹 로드
	RankingDataManager::OnRankingLoaded.BindLambda([this](const TArray<FRankingData>& Data)
	{
		RankingData_FB.Empty(RankingListCount);
		
		for (FRankingData Elem : Data)
		{
			RankingData_FB.Add(Elem);
		}

		RankingData_FB.Sort([](const FRankingData& A, const FRankingData& B)
		{
			if (A.ThrowBallCount == B.ThrowBallCount)
				return A.Score > B.Score;

			return A.ThrowBallCount < B.ThrowBallCount;
		});

		
		for (int i = 0; i<RankingData_FB.Num(); i++)
		{
			if (i >= RankingListCount)
			{
				break;
			}
			
			URankingDataUI* RankingUI = CreateWidget<URankingDataUI>(GetWorld(), RankingUIClass);
			RankingUI->SetUIRankText(i+1, RankingData_FB[i]);
			RankingList->AddChild(RankingUI);
		}

		UE_LOG(LogTemp, Warning, TEXT("Ranking data loaded %d"), RankingData_FB.Num());
	});
	
	RankingDataManager::LoadOnline();

	//버튼 바인드
	RefreshButton->OnClicked.AddDynamic(this, &URankingUI::OnPressRefreshButton);
}

void URankingUI::OnPressRefreshButton()
{
	RankingList->ClearChildren();
	RankingDataManager::LoadOnline();
}
