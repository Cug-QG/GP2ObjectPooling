// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PoolableBall.h"

// Sets default values
APoolableBall::APoolableBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APoolableBall::BeginPlay()
{
	Super::BeginPlay();
	PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
}

// Called every frame
void APoolableBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoolableBall::NativeActive(FObjectPoolActivationData ObjectPoolData)
{
	IObjectPoolInterface::NativeActive(ObjectPoolData);
	
	SetActorLocation(ObjectPoolData.ObjectPoolTransform.GetLocation());
	ResetPhysics(true);
	
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerDelegate TimerDel;
	FObjectPoolDeactivationData ObjectPoolDatas;
	TimerDel.BindUObject(this, &APoolableBall::Deactivate, ObjectPoolDatas);
	TimerManager.SetTimer(
		TimerHandle,
		TimerDel,
		lifeTime,
		false
	);
}

void APoolableBall::NativeDeactive(FObjectPoolDeactivationData ObjectPoolData)
{
	IObjectPoolInterface::NativeDeactive(ObjectPoolData);
	
	ResetPhysics(false);
	PoolSubsystem->ReturnObjectToPool(GetClass(),this);
	SetActorLocation(ObjectPoolData.ObjectPoolTransform.GetLocation());
}

void APoolableBall::ResetPhysics(bool input)
{
	UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent());

	if (RootPrim)
	{
		RootPrim->SetPhysicsLinearVelocity(FVector::ZeroVector);
		RootPrim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		
		RootPrim->SetSimulatePhysics(input);

		if (!input)
		{
			RootPrim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			 RootPrim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

