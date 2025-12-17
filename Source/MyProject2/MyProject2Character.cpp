// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject2.h"

struct FObjectPool;

AMyProject2Character::AMyProject2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void AMyProject2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyProject2Character::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyProject2Character::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject2Character::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject2Character::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyProject2Character::LookInput);
	}
	else
	{
		UE_LOG(LogMyProject2, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProject2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!canShoot)
	{
		Cooldown(DeltaTime);
	}
}

void AMyProject2Character::BeginPlay()
{
	Super::BeginPlay();
	
	PoolSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UObjectPoolSubsystem>();
	PoolSubsystem->AddPool(projectile, poolSize);
}

void AMyProject2Character::PrintPool()
{
	if (!GEngine) return;

	int32 KeyIndex = 500;

	for (const auto& Pair : PoolSubsystem->ObjectPoolMap)
	{
		TSubclassOf<AActor> ClassKey = Pair.Key;
		const FObjectPool& PoolData = Pair.Value;

		int32 ActiveCount = PoolData.ActivePoolingObjects.Num();
		int32 FreeCount = PoolData.UsablePoolingObjects.Num();
		int32 TotalCount = ActiveCount + FreeCount;
        
		FString Msg = FString::Printf(TEXT("Classe: %s || Attivi: %d || Pullabili: %d || Totale: %d"), 
			*ClassKey->GetName(), 
			ActiveCount, 
			FreeCount, 
			TotalCount
		);
		
		GEngine->AddOnScreenDebugMessage(
			KeyIndex, 
			0.0f, 
			FColor::Cyan, 
			Msg
		);

		// Incrementiamo la Key per la prossima classe nella mappa (così ognuna ha la sua riga)
		KeyIndex++;
	}
}

void AMyProject2Character::TryShoot()
{
	if (canShoot)
	{
		Shoot();
	}
}

void AMyProject2Character::Shoot()
{
	canShoot = false;
	FObjectPoolActivationData data;
	data.ObjectPoolTransform = this->GetTransform();
	TScriptInterface<IObjectPoolInterface> diskInstance = PoolSubsystem->GetObjectFromPool(projectile);
	if (!diskInstance) return;
	diskInstance.GetInterface()->Active(data);
}

void AMyProject2Character::Cooldown(float deltatime)
{
	counter += deltatime;
	
	if (counter >= 1/fireRate)
	{
		counter = 0;
		canShoot = true;
	}
}


void AMyProject2Character::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AMyProject2Character::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AMyProject2Character::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMyProject2Character::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AMyProject2Character::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AMyProject2Character::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}