// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSound.h"

#include "BaseBallGameMode.h"
#include "Components/AudioComponent.h"

ACrowdSound::ACrowdSound()
{
    PrimaryActorTick.bCanEverTick = true;

    CrowdAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CrowdAudio"));
    CrowdAudioComponent->SetupAttachment(RootComponent);

    CheerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CheerAudio"));
    CheerAudioComponent->SetupAttachment(RootComponent);
}

void ACrowdSound::BeginPlay()
{
    Super::BeginPlay();
    
    if (CrowdAudioComponent && CrowdSound)
    {
        CrowdAudioComponent->SetSound(CrowdSound);
        CrowdAudioComponent->bIsUISound = true;
        CrowdAudioComponent->bAllowSpatialization = false; 
        CrowdAudioComponent->bAutoActivate = false;
        CrowdAudioComponent->Stop();
    }

    if (CheerAudioComponent && CheerSound)
    {
        CheerAudioComponent->SetSound(CheerSound);
        CheerAudioComponent->bIsUISound = true;
        CheerAudioComponent->bAllowSpatialization = false; 
        CheerAudioComponent->bAutoActivate = false;
        CheerAudioComponent->Stop();
    }

    ABaseBallGameMode* Gm = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
    Gm->SetCrowdSound(this);
}

void ACrowdSound::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACrowdSound::PlayRandomCheer()
{
    if (!CheerAudioComponent || !CheerSound || !CrowdAudioComponent) return;
    
    float RandomStart = FMath::FRandRange(0.f, CheerSound->GetDuration()- 10.f);
    CheerAudioComponent->Stop();
    CheerAudioComponent->Play(RandomStart);
    CheerAudioComponent->FadeIn(0.3, 1);

    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
    {
        if (CheerAudioComponent->IsPlaying())
        {
            CheerAudioComponent->FadeOut(2.0f, 0.0f);
        }
    }, 1.5f, false);
}

void ACrowdSound::PlayCrowdSound()
{
    CrowdAudioComponent->Play();
}

void ACrowdSound::StopCrowdSound()
{
    CrowdAudioComponent->Stop();
    CheerAudioComponent->Stop();
}
