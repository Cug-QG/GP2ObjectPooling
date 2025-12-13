// Fill out your copyright notice in the Description page of Project Settings.


#include "DiskThrower.h"

#include "ObjectPool/PoolableDisk.h"

// Sets default values
ADiskThrower::ADiskThrower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ADiskThrower::BeginPlay()
{
	Super::BeginPlay();
	PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
	PoolSubsystem->AddPool(disk, poolSize);
	
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	TimerManager.SetTimer(
		TimerHandle,
		this,
		&ADiskThrower::SpawnItem,
		spawnSpeed,
		true
	);

}

// Called every frame
void ADiskThrower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADiskThrower::SpawnItem()
{
	FObjectPoolActivationData data;
	data.ObjectPoolTransform = this->GetTransform();
	TScriptInterface<IObjectPoolInterface> diskInstance = PoolSubsystem->GetObjectFromPool(disk);
	diskInstance.GetInterface()->Active(data);
}

