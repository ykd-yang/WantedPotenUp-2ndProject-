// Fill out your copyright notice in the Description page of Project Settings.


#include "CallHitObejct.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACallHitObejct::ACallHitObejct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACallHitObejct::UpdateRate()
{
	LifeCount++;
}

UNiagaraComponent* ACallHitObejct::SpawnDoor()
{
	Door = UNiagaraFunctionLibrary::SpawnSystemAtLocation
	(GetWorld(),DoorSystem,GetActorLocation(),GetActorRotation(),FVector(0.f,10.f,7.f),false,true,ENCPoolMethod::None,true);
	if (Door)
	{
		Door->SetAutoDestroy(false);
		
	}

	return Door;
}

UNiagaraComponent* ACallHitObejct::GetDoor()
{
	return Door;
}

void ACallHitObejct::DestroyDoor()
{
	Door->DeactivateImmediate();
	Door = nullptr;
	
}

void ACallHitObejct::SetDoor(UNiagaraComponent* NewDoor)
{
	Door = NewDoor;
}

// Called when the game starts or when spawned
void ACallHitObejct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACallHitObejct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





