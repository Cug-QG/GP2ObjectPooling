// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolInterface.h"
#include "ObjectPoolSubsystem.h"
#include "GameFramework/Actor.h"
#include "PoolableDisk.generated.h"

UCLASS()
class MYPROJECT2_API APoolableDisk : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolableDisk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NativeActive(FObjectPoolActivationData ObjectPoolData) override;
    	
    virtual void NativeDeactive(FObjectPoolDeactivationData ObjectPoolData) override;

    UPROPERTY(EditAnywhere, Category = "Settings")
    float lifeTime = 2;

    FTimerHandle TimerHandle;
    
    UObjectPoolSubsystem* PoolSubsystem;

	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float speed = 100;
};
