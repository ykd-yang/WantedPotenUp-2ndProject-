#pragma once
#include "CoreMinimal.h"
#include "ItemInfo.generated.h"

USTRUCT()
struct FItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	int32 PowerRate;

	UPROPERTY(EditAnywhere)
	float CriticalRate;
};
