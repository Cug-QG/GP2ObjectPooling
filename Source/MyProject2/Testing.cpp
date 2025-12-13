// Fill out your copyright notice in the Description page of Project Settings.


#include "Testing.h"

// Sets default values
ATesting::ATesting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATesting::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Ciao dal C++!"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("NO!"));
    }
}

// Called every frame
void ATesting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

