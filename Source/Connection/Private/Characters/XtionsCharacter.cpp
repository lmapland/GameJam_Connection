// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/XtionsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Connections/ConnectionBox.h"
#include "Widgets/OverlayWidget.h"
#include "Kismet/GameplayStatics.h"

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

	if (Overlay)
	{
		Overlay->SetLives(Health);
		Overlay->DisplayTutorialText();
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
	//UE_LOG(LogTemp, Warning, TEXT("Interact(): Entered"));
	if (!bAlive || !OverlappedBox) return;

	//UE_LOG(LogTemp, Warning, TEXT("Interact(): OverlappedBox exists on character"));
	if (OverlappedBox->Use())
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact(): Level of box: %i "), OverlappedBox->Level);
		// use was successful (box was unconnected); print something to the screen?
		Overlay->DisplayConnectionText();
	}
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

void AXtionsCharacter::SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox)
{
	OverlappedBox = InOverlappedBox;
}

void AXtionsCharacter::Damage(int32 Amount)
{
	if (!bAlive) return;

	Health = FMath::Clamp(Health - Amount, 0, MaxHealth);
	if (Health == 0)
	{
		Die();
	}

	Overlay->UpdateLives(-1);
}

void AXtionsCharacter::LevelComplete(int32 NewLevel, FVector NewLoc, FRotator NewRot)
{
	UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::LevelComplete(): Entered"));

	Overlay->DisplayLevelCompleteText();
	Overlay->DisplayTransportingText();

	if (NewLevel == 5)
	{
		Overlay->DisplayWinText();
		GetWorldTimerManager().SetTimer(TransportTimer, this, &AXtionsCharacter::EndTheGame, 6.f);
	}
	else
	{
		// Transport character to next level
		TransportLoc = NewLoc;
		TransportRot = NewRot;

		GetWorldTimerManager().SetTimer(TransportTimer, this, &AXtionsCharacter::TransportCharacter, 6.f);
	}
}

void AXtionsCharacter::EndTheGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void AXtionsCharacter::Die()
{
	if (!bAlive) return;

	bAlive = false;

	Overlay->DisplayDeathText();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();

	UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::Die(): Calling PostDie();"));
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AXtionsCharacter::PostDie, DeathTimout);
}

void AXtionsCharacter::PostDie()
{
	UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::PostDie(): Loading the MainMenu"))
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void AXtionsCharacter::TransportCharacter()
{
	SetActorLocation(TransportLoc);
	SetActorRotation(TransportRot);
}
