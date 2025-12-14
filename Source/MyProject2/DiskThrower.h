// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool/ObjectPoolSubsystem.h"
#include "DiskThrower.generated.h"

UCLASS()
class MYPROJECT2_API ADiskThrower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiskThrower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UObjectPoolSubsystem* PoolSubsystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pool")
	TSubclassOf<AActor> disk;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pool")
	int poolSize = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pool")
	float spawnSpeed = 1;
	
	FTimerHandle TimerHandle;
	void SpawnItem();
};
