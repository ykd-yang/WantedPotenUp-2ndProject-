// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AirResistanceLibraryFunction.generated.h"

/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UAirResistanceLibraryFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FVector AirResistanceCpp(FVector Velocity, UCurveFloat* DragCoefficient);
	
};
