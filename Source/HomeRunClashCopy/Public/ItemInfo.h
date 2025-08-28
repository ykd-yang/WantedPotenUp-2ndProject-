#pragma once
#include "CoreMinimal.h"
#include "ItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FItemInformation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	int32 PowerRate;

	UPROPERTY(EditAnywhere)
	float CriticalRate;
};
