// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMillicastActor.h"
#include "MillicastTexture2DPlayer.h"
#include "Audio/MillicastAudioActor.h"

#include "Audio/MillicastAudioInstance.h"
#include "Components/MillicastDirectorComponent.h"
#include "Components/MillicastSubscriberComponent.h"
#include "Subsystems/MillicastAudioSubsystem.h"

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

void AMyMillicastActor::OnAuthenticated(UMillicastDirectorComponent* DirectorComponent, const FMillicastSignalingData& SignalingData)
{
	MillicastSubscriber->Subscribe(DirectorComponent, SignalingData);
}

void AMyMillicastActor::OnAudioTrack(UMillicastAudioTrack* AudioTrack)
{
	if (!IsValid(AudioTrack))
	{
		return;
	}

	if(AudioConsumers.Num() == 0 && AudioConsumerActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[AMyMillicastActor::OnAudioTrack] No valid audio consumers assigned"));
		return;
	}

	auto* Subsystem = GetGameInstance()->GetSubsystem<UMillicastAudioSubsystem>();
	auto RegisterAudioComponent = [=](UAudioComponent* AudioComponent)
	{
		Subsystem->Register(AudioComponent);
		AudioTrack->AddConsumer(Subsystem->GetInstance(AudioComponent));
	};
	
	for(const auto* Consumer : AudioConsumerActors)
	{
		RegisterAudioComponent(Consumer->AudioComponent);
	}

	for(auto* AudioComponent : AudioConsumers)
	{
		RegisterAudioComponent(AudioComponent);
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
