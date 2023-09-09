// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/XtionsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Connections/ConnectionBox.h"
#include "GameInstances/XtionsGameInstance.h"

AXtionsCharacter::AXtionsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void AXtionsCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(CharMappingContext, 0);
		}
	}

	UXtionsGameInstance* GameInstance = Cast<UXtionsGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->InitHUD(this, PlayerController);
	}
}

void AXtionsCharacter::Move(const FInputActionValue& Value)
{
	if (!bAlive) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AXtionsCharacter::Look(const FInputActionValue& Value)
{
	if (!bAlive) return;

	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void AXtionsCharacter::Interact(const FInputActionValue& value)
{
	if (!bAlive || !OverlappedBox) return;

	OnOverlappingBox.Broadcast(false);
	OverlappedBox->Use();
}

void AXtionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXtionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXtionsCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AXtionsCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AXtionsCharacter::Interact);
	}
}

void AXtionsCharacter::Damage(int32 Amount)
{
	if (!bAlive) return;

	PlaySound(OnDamageSound);

	Health = FMath::Clamp(Health - Amount, 0, MaxHealth);
	if (Health == 0)
	{
		Die();
	}

	OnLivesUpdated.Broadcast(-1);
}

void AXtionsCharacter::Die()
{
	if (!bAlive) return;

	bAlive = false;

	OnCharacterDeath.Broadcast();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AXtionsCharacter::PostDie, DeathTimout);
}

void AXtionsCharacter::PostDie()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void AXtionsCharacter::TransportCharacter(FVector Location, FRotator Rotation)
{
	SetActorLocation(Location);
	PlayerController->SetControlRotation(Rotation);
	SetActorRotation(Rotation);
}

void AXtionsCharacter::SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox)
{
	if (InOverlappedBox) OnOverlappingBox.Broadcast(true);
	else OnOverlappingBox.Broadcast(false);

	OverlappedBox = InOverlappedBox;
}

void AXtionsCharacter::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}
}
