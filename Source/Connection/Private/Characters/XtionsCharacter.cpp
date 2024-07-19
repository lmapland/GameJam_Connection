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

	// Handle possible controller drift
	if ((MovementVector.X < 0.1f && MovementVector.X > -0.1f) && (MovementVector.Y < 0.1f && MovementVector.Y > -0.1f))
	{
		return;
	}

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

	// Handle possible controller drift
	//UE_LOG(LogTemp, Warning, TEXT("Look X,Y: %f,%f"), LookAxisValue.X, LookAxisValue.Y);
	if ((LookAxisValue.X < 0.1f && LookAxisValue.X > -0.1f) && (LookAxisValue.Y < 0.1f && LookAxisValue.Y > -0.1f))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found controller drift (look)"));
		return;
	}

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void AXtionsCharacter::Interact(const FInputActionValue& value)
{
	if (!bAlive || !OverlappedBox || bIsDodging) return;

	OnOverlappingBox.Broadcast(false);
	OverlappedBox->Use();
}

void AXtionsCharacter::Dodge(const FInputActionValue& value)
{
	if (bIsDodging || NumDodges == 0) return;

	bIsDodging = true;
	NumDodges -= 1;
	OnDodgesUpdated.Broadcast(NumDodges);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage, 2.f);
	}
}

void AXtionsCharacter::LeaveGame(const FInputActionValue& value)
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void AXtionsCharacter::SkipCurrentLevel()
{
	OnLevelSkipRequested.Broadcast();
}

void AXtionsCharacter::DodgeFinish()
{
	bIsDodging = false;
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
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AXtionsCharacter::Dodge);
		EnhancedInputComponent->BindAction(LeaveAction, ETriggerEvent::Completed, this, &AXtionsCharacter::LeaveGame);
		EnhancedInputComponent->BindAction(LevelSkipAction, ETriggerEvent::Completed, this, &AXtionsCharacter::SkipCurrentLevel);
	}
}

void AXtionsCharacter::Damage(int32 Amount)
{
	if (!bAlive || bIsDodging) return;

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

// Aka level complete / set up new level
void AXtionsCharacter::TransportCharacter(FVector Location, FRotator Rotation)
{
	//SetActorTransform(FTransform(Location, Rotation, FVector(1.f)));
	SetActorLocation(Location);
	PlayerController->SetControlRotation(Rotation);
	SetActorRotation(Rotation);

	// Add another dodge
	NumDodges += 1;
	OnDodgesUpdated.Broadcast(NumDodges);
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
