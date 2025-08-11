#pragma once
#include "CoreMinimal.h"
#include "BallInfo.generated.h"

UENUM(BlueprintType)
enum class EBallType : uint8
{
	Straight	UMETA(DisplayName = "Straight"),
	Curve		UMETA(DisplayName = "Curve"),
	Slider		UMETA(DisplayName = "Slider"),
	Fork        UMETA(DisplayName = "Fork"),
	ChangeUp    UMETA(DisplayName = "ChangeUp"),
	Knuckle     UMETA(DisplayName = "Knuckle")
};

USTRUCT(BlueprintType)
struct FBallInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBallType BallType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Dir;
};
