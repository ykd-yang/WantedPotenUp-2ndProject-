// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "BaseBallGameInstance.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	GI = Cast<UBaseBallGameInstance>(GetGameInstance());

	InventoryItemButton1->OnClicked.AddDynamic(this, &UInventoryUI::OnItemPressed);
	InventoryItemButton2->OnClicked.AddDynamic(this, &UInventoryUI::OnItemPressed);
	EquipButton->OnClicked.AddDynamic(this, &UInventoryUI::OnEquipPressed);
}

void UInventoryUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
}

void UInventoryUI::OnItemPressed()
{
	if (InventoryItemButton1->HasUserFocus(GetOwningPlayer())) // 1번 아이템, 나무배트 
	{
		// Change ItemType
		ChangeItemType(EItemType::Item1);
		// EquipItemImage 바꾸기
		ChangeEquipItemImage();
		// EquipmentText 바꾸기
		ChangeEquipmentText();
		// PowerStatusText 바꾸기
		ChangePowerStatusText();
		// AccuracyStatusText 바꾸기
		ChangeAccuracyStatusText();
		// EquipStateText 교체, 장착중 바꾸기
		if (bReverse)
		{
			ChangeEquipStateTextReverse();
		}
		else
		{
			ChangeEquipStateText();
		}
	}
	else if (InventoryItemButton2->HasUserFocus(GetOwningPlayer())) // 2번 아이템, 티타늄배트
	{
		// Change ItemType
		ChangeItemType(EItemType::Item2);
		// EquipItemImage 바꾸기
		ChangeEquipItemImage();
		// EquipmentText 바꾸기
		ChangeEquipmentText();
		// PowerStatusText 바꾸기
		ChangePowerStatusText();
		// AccuracyStatusText 바꾸기
		ChangeAccuracyStatusText();
		// EquipStateText 교체, 장착중 바꾸기
		if (bReverse)
		{
			ChangeEquipStateText();
		}
		else
		{
			ChangeEquipStateTextReverse();
		}
	}
}

void UInventoryUI::OnEquipPressed()
{
	// EquipStateText 장착중 바꾸기
	ChangeEquipStateText();
	// EquippedText 옮기기
	MoveEquippedText();
	// Change Material
	// Change Status
	SendItemType();
}

void UInventoryUI::ChangeItemType(EItemType newItem)
{
	if (ItemType == newItem)
		return;
	ItemType = newItem;
}

void UInventoryUI::ChangeEquipItemImage()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 시
		if (EquipItemImage && EquipItemTexture1)
		{
			// FSlateBrush Brush;
			// Brush.SetResourceObject(EquipItemTexture1);
			// EquipItemImage->SetBrush(Brush);
		}
		break;
	case EItemType::Item2: // 2번 아이템 장착 시
		if (EquipItemImage && EquipItemTexture2)
		{
			// FSlateBrush Brush;
			// Brush.SetResourceObject(EquipItemTexture2);
			// EquipItemImage->SetBrush(Brush);
		}
		break;
	}
}

void UInventoryUI::ChangeEquipmentText()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 시
		EquipmentText->SetText(FText::FromString(TEXT("나무배트")));
		break;
	case EItemType::Item2: // 2번 아이템 장착 시
		EquipmentText->SetText(FText::FromString(TEXT("티타늄배트")));
		break;
	}
}

void UInventoryUI::ChangePowerStatusText()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 시
		PowerStatusText->SetText(FText::FromString(TEXT("75")));
		break;
	case EItemType::Item2: // 2번 아이템 장착 시
		PowerStatusText->SetText(FText::FromString(TEXT("67")));
		break;
	}
}

void UInventoryUI::ChangeAccuracyStatusText()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 시
		AccuracyStatusText->SetText(FText::FromString(TEXT("89")));
		break;
	case EItemType::Item2: // 2번 아이템 장착 시
		AccuracyStatusText->SetText(FText::FromString(TEXT("97")));
		break;
	}
}

void UInventoryUI::ChangeEquipStateTextReverse()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 중이면
		EquipStateText->SetText(FText::FromString(TEXT("교체")));
		break;
	case EItemType::Item2: // 2번 아이템 장착 중이면
		EquipStateText->SetText(FText::FromString(TEXT("교체")));
		break;
	}
}

void UInventoryUI::ChangeEquipStateText()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 중이면
		EquipStateText->SetText(FText::FromString(TEXT("장착중")));
		break;
	case EItemType::Item2: // 2번 아이템 장착 중이면
		EquipStateText->SetText(FText::FromString(TEXT("장착중")));
		break;
	}
}

void UInventoryUI::MoveEquippedText()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 중이면
		bReverse = false;
		Equipped1Text->SetVisibility(ESlateVisibility::Visible);
		Equipped2Text->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EItemType::Item2: // 2번 아이템 장착 중이면
		bReverse = true;
		Equipped2Text->SetVisibility(ESlateVisibility::Visible);
		Equipped1Text->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}

void UInventoryUI::SendItemType()
{
	switch (ItemType)
	{
	case EItemType::Item1: // 1번 아이템 장착 중이면
		GI->UpdateItemInfo(1);
		break;
	case EItemType::Item2: // 2번 아이템 장착 중이면
		GI->UpdateItemInfo(2);
		break;
	}
}
