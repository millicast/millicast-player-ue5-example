// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MillicastAudioActor.h"
#include "MillicastSubscriberComponent.h"
#include "MillicastDirectorComponent.h"
#include "MillicastTexture2DPlayer.h"

#include "MyMillicastActor.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "MillicastDemo",
	META = (DisplayName = "My Millicast Actor", BlueprintSpawnableComponent))
class MILLICASTPLAYERDEMO_API AMyMillicastActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties",
		META = (DisplayName = "Millicast Audio Consumer", AllowPrivateAccess = true))
	AMillicastAudioActor* MillicastAudioComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties",
		META = (DisplayName = "Millicast Media Source", AllowPrivateAccess = true))
	UMillicastMediaSource* MillicastMediaSource;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties",
		META = (DisplayName = "Millicast Texture Player", AllowPrivateAccess = true))
	UMillicastTexture2DPlayer* MillicastTexturePlayer;

	UMillicastDirectorComponent* MillicastDirector;
	UMillicastSubscriberComponent* MillicastSubscriber;

public:	
	// Sets default values for this actor's properties
	AMyMillicastActor();

	UFUNCTION()
	void OnAuthenticated(const FMillicastSignalingData& SignalingData);

	UFUNCTION()
	void OnAudioTrack(UMillicastAudioTrack * AudioTrack);
	UFUNCTION()
	void OnVideoTrack(UMillicastVideoTrack * VideoTrack);
	UFUNCTION()
	void OnSubscribed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
