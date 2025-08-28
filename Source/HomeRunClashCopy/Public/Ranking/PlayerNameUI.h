// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameUI.generated.h"

class UCanvasPanel;
class UEditableTextBox;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UPlayerNameUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* RootPanel;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameText;
	
	UPROPERTY(meta=(BindWidget))
	UButton* SubmitButton;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* NameInputBox;

	UFUNCTION()
	void OnClickedSubmit();

	UFUNCTION()
	void OnInputTextChanged(const FText& Text);

public:
	UFUNCTION()
	void OnClickPlayerBtn();
};
