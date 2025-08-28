// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CallHitObjectSpawner.generated.h"

/**
 * 
 */
UCLASS()
class HOMERUNCLASHCOPY_API UAN_CallHitObjectSpawner : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
