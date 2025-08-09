// Fill out your copyright notice in the Description page of Project Settings.


#include "Class_Batter.h"

// Sets default values
AClass_Batter::AClass_Batter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClass_Batter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClass_Batter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AClass_Batter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

