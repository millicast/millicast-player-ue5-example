// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMillicastActor.h"
#include "MillicastAudioActor.h"
#include "MillicastTexture2DPlayer.h"

#include "Components/MillicastAudioComponent.h"
#include "Components/MillicastDirectorComponent.h"
#include "Components/MillicastSubscriberComponent.h"

// Sets default values
AMyMillicastActor::AMyMillicastActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MillicastDirector = CreateDefaultSubobject<UMillicastDirectorComponent>(TEXT("MilicastDirector"));
	MillicastSubscriber = CreateDefaultSubobject<UMillicastSubscriberComponent>(TEXT("MillicastSub"));

	AddOwnedComponent(MillicastDirector);
	AddOwnedComponent(MillicastSubscriber);
}

void AMyMillicastActor::OnAuthenticated(const FMillicastSignalingData& SignalingData)
{
	MillicastSubscriber->Subscribe(SignalingData, nullptr);
}

void AMyMillicastActor::OnAudioTrack(UMillicastAudioTrack* AudioTrack)
{
	if (IsValid(AudioTrack))
	{
		AudioTrack->AddConsumer(MillicastAudioComponent->MillicastAudioComponent);
	}
	else
	{
		
	}
}

void AMyMillicastActor::OnVideoTrack(UMillicastVideoTrack* VideoTrack)
{
	VideoTrack->AddConsumer(MillicastTexturePlayer);
}

void AMyMillicastActor::OnSubscribed()
{
}

// Called when the game starts or when spawned
void AMyMillicastActor::BeginPlay()
{
	Super::BeginPlay();	

	MillicastDirector->Initialize(MillicastMediaSource);
	MillicastSubscriber->Initialize(MillicastMediaSource);

	MillicastDirector->OnAuthenticated.AddDynamic(this, &AMyMillicastActor::OnAuthenticated);
	MillicastSubscriber->OnSubscribed.AddDynamic(this, &AMyMillicastActor::OnSubscribed);
	MillicastSubscriber->OnAudioTrack.AddDynamic(this, &AMyMillicastActor::OnAudioTrack);
	MillicastSubscriber->OnVideoTrack.AddDynamic(this, &AMyMillicastActor::OnVideoTrack);

	MillicastDirector->Authenticate();
}
