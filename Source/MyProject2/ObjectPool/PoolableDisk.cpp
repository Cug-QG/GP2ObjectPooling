// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PoolableDisk.h"

// Sets default values
APoolableDisk::APoolableDisk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void APoolableDisk::BeginPlay()
{
	Super::BeginPlay();
	PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
}

// Called every frame
void APoolableDisk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * DeltaTime * speed));
}

void APoolableDisk::NativeActive(FObjectPoolActivationData ObjectPoolData)
{
	IObjectPoolInterface::NativeActive(ObjectPoolData);
	
	SetActorLocation(ObjectPoolData.ObjectPoolTransform.GetLocation());
	SetActorTickEnabled(true);
	
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate TimerDel;
	FObjectPoolDeactivationData ObjectPoolDatas;
	TimerDel.BindUObject(this, &APoolableDisk::Deactivate, ObjectPoolDatas);
	TimerManager.SetTimer(
		TimerHandle,
		TimerDel,
		lifeTime,
		false
	);
}

void APoolableDisk::NativeDeactive(FObjectPoolDeactivationData ObjectPoolData)
{
	IObjectPoolInterface::NativeDeactive(ObjectPoolData);
	//this->Destroy();
	SetActorTickEnabled(false);
	PoolSubsystem->ReturnObjectToPool(GetClass(),this);
	SetActorLocation(ObjectPoolData.ObjectPoolTransform.GetLocation());
	
	//PrimaryActorTick.bCanEverTick = false;
}

