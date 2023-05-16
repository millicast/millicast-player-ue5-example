// Copyright Millicast 2022. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MyMillicastActor.generated.h"

class AMillicastAudioActor;
class UMillicastMediaSource;
struct FMillicastSignalingData;
class UMillicastDirectorComponent;
class UMillicastSubscriberComponent;
class UMillicastTexture2DPlayer;

UCLASS(BlueprintType, Blueprintable, Category = "MillicastDemo", META = (DisplayName = "My Millicast Actor"))
class MILLICASTPLAYERDEMO_API AMyMillicastActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties", META = (DisplayName = "Millicast Audio Consumer"))
	TArray<AMillicastAudioActor*> AudioConsumerActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties", META = (DisplayName = "Millicast Audio Consumer"))
	TArray<UAudioComponent*> AudioConsumers;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties", META = (DisplayName = "Millicast Media Source"))
	UMillicastMediaSource* MillicastMediaSource;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties", META = (DisplayName = "Millicast Texture Player"))
	UMillicastTexture2DPlayer* MillicastTexturePlayer;

	UPROPERTY()
	UMillicastDirectorComponent* MillicastDirector;

	UPROPERTY()
	UMillicastSubscriberComponent* MillicastSubscriber;

public:	
	// Sets default values for this actor's properties
	AMyMillicastActor();

	UFUNCTION()
	void OnAuthenticated(UMillicastDirectorComponent* DirectorComponent, const FMillicastSignalingData& SignalingData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
