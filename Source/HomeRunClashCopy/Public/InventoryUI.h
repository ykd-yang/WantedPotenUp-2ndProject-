// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "InventoryUI.generated.h"

class UBaseBallGameInstance;
class UCanvasPanelSlot;
class UTextBlock;
class UButton;
class UOverlay;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item1  UMETA(DisplayName="Item1"),
	Item2   UMETA(DisplayName="Item2"),
};
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	EItemType ItemType = EItemType::Item1;

public:
	// Inventory Overlay
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> InventoryOverlay;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> InventoryItemButton1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> InventoryItemButton2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Equipped1Text;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Equipped2Text;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> BtnSlot;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> TextSlot;
	//Equipment Overlay
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> EquipmentOverlay;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> EquipItemImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> EquipmentText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> EquipButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> EquipStateText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Texture2D")
	TObjectPtr<UTexture2D> EquipItemTexture1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Texture2D")
	TObjectPtr<UTexture2D> EquipItemTexture2;
	// Item Status Box
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PowerStatusText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AccuracyStatusText;
	UPROPERTY()
	TObjectPtr<UBaseBallGameInstance> GI;
	
	// Button Events
	UFUNCTION()
	void OnItemPressed();
	UFUNCTION()
	void OnEquipPressed();

	void ChangeItemType(EItemType newItem);
	
	void ChangeEquipItemImage();
	void ChangeEquipmentText();
	void ChangePowerStatusText();
	void ChangeAccuracyStatusText();
	void ChangeEquipStateTextReverse();
	void ChangeEquipStateText();
	void MoveEquippedText();
	
	bool bReverse;

	void SendItemType();
};
