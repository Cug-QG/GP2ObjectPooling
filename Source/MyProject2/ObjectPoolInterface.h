// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolInterface.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolActivationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FTransform ObjectPoolTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FName ObjectPoolName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FString ObjectPoolID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FText ObjectPoolText;
};

USTRUCT(BlueprintType)
struct FObjectPoolDeactivationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FTransform ObjectPoolTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FName ObjectPoolName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FString ObjectPoolID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Object Pool Data")
	FText ObjectPoolText;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT2_API IObjectPoolInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Active version to call in c++
	void Active(FObjectPoolActivationData ObjectPoolData) { NativeActive(ObjectPoolData); };

	//Deactive version to call in c++
	void Deactivate(FObjectPoolDeactivationData ObjectPoolData) { NativeDeactive(ObjectPoolData); };

	// Functions to implement

	// Active to implement in blueprint
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Pool Interface")
	void BP_Activate(FObjectPoolActivationData ObjectPoolData);

	// Active to implement in c++
	virtual void NativeActive(FObjectPoolActivationData ObjectPoolData)
	{
		UObject* Object = Cast<UObject>(this);
		Execute_BP_Activate(Object, ObjectPoolData);
	};

	// Deactive to implement in blueprint
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Pool Interface")
	void BP_Deactivate(FObjectPoolDeactivationData ObjectPoolData);

	// Deactive to implemente in c++
	virtual void NativeDeactive(FObjectPoolDeactivationData ObjectPoolData)
	{
		UObject* Object = Cast<UObject>(this);
		Execute_BP_Deactivate(Object, ObjectPoolData);
	};
};
