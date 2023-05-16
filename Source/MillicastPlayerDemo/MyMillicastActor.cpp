// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMillicastActor.h"
#include "Audio/MillicastAudioActor.h"

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

void AMyMillicastActor::OnAuthenticated(UMillicastDirectorComponent* DirectorComponent, const FMillicastSignalingData& SignalingData)
{
	MillicastSubscriber->Subscribe(DirectorComponent, SignalingData);
}

// Called when the game starts or when spawned
void AMyMillicastActor::BeginPlay()
{
	Super::BeginPlay();	

	MillicastDirector->Initialize(MillicastMediaSource);
	MillicastSubscriber->Initialize(MillicastMediaSource);

	MillicastDirector->OnAuthenticated.AddDynamic(this, &AMyMillicastActor::OnAuthenticated);

	for(const auto* Consumer : AudioConsumerActors)
	{
		MillicastSubscriber->RegisterAudioComponent(Consumer->AudioComponent);
	}

	for(auto* AudioComponent : AudioConsumers)
	{
		MillicastSubscriber->RegisterAudioComponent(AudioComponent);
	}
	
	MillicastDirector->Authenticate();
}
