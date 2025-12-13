// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/ObjectPoolSubsystem.h"

void UObjectPoolSubsystem::AddPool(TSubclassOf<AActor> ClassPool, int32 InitialSize)
{
	if (!GetWorld())
	{
		return;
	}
	
	if (ObjectPoolMap.Contains(ClassPool))
	{
		return;
	}
	
	FObjectPool ObjectPoolToCreate;

	// Se vogliamo aggiungere piu' parametri per lo spawn
	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn initial actors to use
	for (int i = 0; i < InitialSize; i++)
	{		
		// Spawn objects to add in the pool
		AActor* ActorRef = GetWorld()->SpawnActor<AActor>(ClassPool, SpawnParameters);

		// Add the spawned actor to the pool
		ObjectPoolToCreate.UsablePoolingObjects.AddUnique(ActorRef);
	}

	ObjectPoolMap.Add(ClassPool, ObjectPoolToCreate);
}

TScriptInterface<IObjectPoolInterface> UObjectPoolSubsystem::GetObjectFromPool(TSubclassOf<AActor> ClassPool)
{
	if (!GetWorld())
	{
		return nullptr;
	}

	if (!ObjectPoolMap.Contains(ClassPool))
	{
		return nullptr;
	}

	FObjectPool* PoolObject = ObjectPoolMap.Find(ClassPool);

	//ObjectPoolMap.FindOrAdd()

	TScriptInterface<IObjectPoolInterface> ActorToReturn;

	// We control that the pool is not empty
	if (PoolObject->UsablePoolingObjects.IsEmpty())
	{
		// Spawn new object to use, he will auto go back to the pooling system
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorToReturn = GetWorld()->SpawnActor<AActor>(ClassPool, SpawnParameters);

		PoolObject->ActivePoolingObjects.AddUnique(ActorToReturn);
	}
	else
	{
		// Pick the first usable object
		ActorToReturn = PoolObject->UsablePoolingObjects[0];
		// Remove from usable objects
		PoolObject->UsablePoolingObjects.Remove(ActorToReturn);
		// Register it to the current active objects
		PoolObject->ActivePoolingObjects.AddUnique(ActorToReturn);
	}

	return ActorToReturn;
}

void UObjectPoolSubsystem::ReturnObjectToPool(TSubclassOf<AActor> ClassPool,
	TScriptInterface<IObjectPoolInterface> ActorToReturn)
{
	if (!GetWorld())
	{
		return;
	}

	if (!ObjectPoolMap.Contains(ClassPool))
	{
		return;
	}

	FObjectPool* PoolObject = ObjectPoolMap.Find(ClassPool);

	if (!PoolObject->ActivePoolingObjects.Contains(ActorToReturn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to add to the pool an object that wasn't registered before"))
	}

	// unregister from active objects
	PoolObject->ActivePoolingObjects.Remove(ActorToReturn);

	// return back to being use in the pool of usable objects
	PoolObject->UsablePoolingObjects.AddUnique(ActorToReturn);
}


