// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/XtionsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Connections/ConnectionBox.h"
#include "GameInstances/XtionsGameInstance.h"
#include "Structs/LevelInfo.h"
#include "Interfaces/Interactable.h"

AXtionsCharacter::AXtionsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 92.f));
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 25.f;
	CameraBoom->CameraRotationLagSpeed = 25.f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	ItemSphere->SetupAttachment(GetRootComponent());
	ItemSphere->SetSphereRadius(350.f);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	OverlappingActors.Empty();
}

void AXtionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingActors.Num() > 0)
	{
		LineTraceForInteractable();
	}
}

void AXtionsCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	/*if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(CharMappingContext, 0);
		}
	}*/

	GameInstance = Cast<UXtionsGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->InitHUD(this, PlayerController);
	}
	/*TArray<int32> Completions = GameInstance->GetLevelCompletions();
	if (Completions.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are only %i Level Completions"), Completions.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level Completions: %i, %i, %i"), Completions[0], Completions[1], Completions[2]);
	}*/

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);

	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AXtionsCharacter::OnItemSphereOverlap);
	ItemSphere->OnComponentEndOverlap.AddDynamic(this, &AXtionsCharacter::OnItemSphereEndOverlap);
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
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	float MovementScaleX = InvertXAxis() ? LookAxisValue.X * GetXSensitivity() * -1.f : LookAxisValue.X * GetXSensitivity();
	float MovementScaleY = InvertYAxis() ? LookAxisValue.Y * GetYSensitivity() * -1.f : LookAxisValue.Y * GetYSensitivity();

	AddControllerYawInput(MovementScaleX);
	AddControllerPitchInput(MovementScaleY);
}

void AXtionsCharacter::Interact(const FInputActionValue& value)
{
	if (!bAlive) return;

	/* Check for item pickup first */
	if (OverlappingActors.Num() > 0)
	{
		FHitResult HitResult;
		if (PerformLineTrace(HitResult))
		{
			if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
			{
				int32 Index = -1;
				if (RequiredObjects.Find(Interactable->GetInteractableID(), Index))
				{
					CurrentObjectCounts[Index] += 1;
					//UE_LOG(LogTemp, Warning, TEXT("Interact(): Picked up 1 additional ObjectID %i. Total: %i"), Interactable->GetInteractableID(), CurrentObjectCounts[Index]);
					bool bReadyForHighlight = CurrentObjectCounts[Index] >= ObjectCounts[Index] ? true : false;
					OnPickedUpInteractable.Broadcast(Interactable->GetInteractableID(), CurrentObjectCounts[Index], ObjectCounts[Index], bReadyForHighlight);

					if (ReadyToRepair())
					{
						OnUpdateIntraMission.Broadcast(1, false);
					}
				}

				Interactable->Interact();
				return;
			}
		}
	}

	/* Check for Connection Box usage next */
	if (!OverlappedBox || bIsDodging) return;

	if (ReadyToRepair())
	{
		OnOverlappingBox.Broadcast(false, false);
		OverlappedBox->Use();

		for (int i = 0; i < RequiredObjects.Num(); i++)
		{
			CurrentObjectCounts[i] -= ObjectCounts[i];
			bool bReadyForHighlight = CurrentObjectCounts[i] >= ObjectCounts[i] ? true : false;
			OnPickedUpInteractable.Broadcast(RequiredObjects[i], CurrentObjectCounts[i], ObjectCounts[i], bReadyForHighlight);
		}

		if (ReadyToRepair())
		{
			OnUpdateIntraMission.Broadcast(1, false);
		}
		else
		{
			OnUpdateIntraMission.Broadcast(0, false);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Interact(): Not ready to repair"));
		OnNotReadyToRepair.Broadcast();
	}
}

void AXtionsCharacter::Dodge(const FInputActionValue& value)
{
	if (bIsDodging) return;
	if (NumDodges == 0)
	{
		OnDodgesUpdated.Broadcast(NumDodges);
		return;
	}

	bIsDodging = true;
	NumDodges -= 1;
	OnDodgesUpdated.Broadcast(NumDodges);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage, 2.f);
	}
}

void AXtionsCharacter::Dash()
{
	if (bIsDashing) return;
	if (NumDashes == 0)
	{
		OnDashesUpdated.Broadcast(NumDashes);
		return;
	}

	bIsDashing = true;
	GetCharacterMovement()->GravityScale = 0.f;
	OnDashesUpdated.Broadcast(NumDashes);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DashMontage)
	{
		AnimInstance->Montage_Play(DashMontage, 1.f);
	}
}

void AXtionsCharacter::Jump()
{
	bIsJumping = true;
	Super::Jump();
}

void AXtionsCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bIsJumping = false;
}

void AXtionsCharacter::LeaveGame(const FInputActionValue& value)
{
	// Quit to level selection screen
}

void AXtionsCharacter::SkipCurrentLevel()
{
	OnLevelSkipRequested.Broadcast();
}

void AXtionsCharacter::LineTraceForInteractable()
{
	FHitResult HitResult;

	if (PerformLineTrace(HitResult))
	{
		IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor());
		if (Interactable)
		{
			if (HoveredInteractable)
			{
				// Hit Result && Hovered: either they are the same or different
				if (HitResult.GetActor() == HoveredInteractable) return;
				else
				{
					HoveredInteractable = HitResult.GetActor();
					OnHoveredInteractable.Broadcast(Interactable->GetInteractableName());
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::LineTraceForInteractable(): Hit an Interactable; setting HoveredInteractable"));
				// Hit Result && NO Hovered: set Hovered
				HoveredInteractable = HitResult.GetActor();
				OnHoveredInteractable.Broadcast(Interactable->GetInteractableName());
			}
		}
		else // Hit something like landscape or tree that isn't interactable
		{
			// If HoveredInteractable, need to unset
			if (HoveredInteractable)
			{
				//UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::LineTraceForInteractable(): Hit a non-interactable; unsetting current HoveredInteractable"));
				HoveredInteractable = nullptr;
				OnHoveredInteractable.Broadcast(FString());
			}
		}
	}
	else
	{
		if (HoveredInteractable)
		{
			// NO Hit Result && Hovered: Unset Hovered
			HoveredInteractable = nullptr;
			OnHoveredInteractable.Broadcast(FString());
		}
	}
}

bool AXtionsCharacter::PerformLineTrace(FHitResult& HitResult)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// how to get the camera location & rotation in world space
	FVector MouseOrigin = FollowCamera->GetComponentLocation();
	FRotator MouseRotation = FollowCamera->GetComponentRotation();
	FVector DirectionOfLineTrace = MouseRotation.Vector() * (LineTraceDistance + CameraBoom->TargetArmLength);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), MouseOrigin, MouseOrigin + DirectionOfLineTrace, ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	return HitResult.bBlockingHit;
}

bool AXtionsCharacter::ReadyToRepair()
{
	//UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::ReadyToRepair()"));
	bool bReady = true;
	for (int i = 0; i < ObjectCounts.Num(); i++)
	{
		if (CurrentObjectCounts[i] < ObjectCounts[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("ReadyToRepair(): Object %i: %i < %i"), RequiredObjects[i], CurrentObjectCounts[i], ObjectCounts[i]);
			bReady = false;
			break;
		}
	}

	return bReady;
}

void AXtionsCharacter::OpenLevelSelect()
{
	OnLevelSelectOpened.Broadcast();
	GameInstance->ShowLevelSelectionScreen();
}

void AXtionsCharacter::Die()
{
	bAlive = false;
}

void AXtionsCharacter::OnItemSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::OnItemSphereOverlap()"));
	IInteractable* InteractableObject = Cast<IInteractable>(OtherActor);
	if (InteractableObject)
	{
		OverlappingActors.AddUnique(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("AXtionsCharacter::OnItemSphereOverlap(): Found Interactable: %s"), *OtherActor->GetName());
	}
}

void AXtionsCharacter::OnItemSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractable* InteractableObject = Cast<IInteractable>(OtherActor);
	if (InteractableObject)
	{
		OverlappingActors.Remove(OtherActor);
	}

	if (HoveredInteractable && OverlappingActors.Num() == 0)
	{
		HoveredInteractable = nullptr;
		OnHoveredInteractable.Broadcast(FString(""));
	}
}

void AXtionsCharacter::DodgeFinish()
{
	bIsDodging = false;
	// Can interrupt a dash with a dodge, so let's make sure to set the state appropriately
	bIsDashing = false;
}

void AXtionsCharacter::DashFinish()
{
	bIsDashing = false;
	// Can interrupt a dodge with a dash, so let's make sure to set the state appropriately
	bIsDodging = false;
	GetCharacterMovement()->GravityScale = 4.f;
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AXtionsCharacter::Jump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AXtionsCharacter::Dash);
		EnhancedInputComponent->BindAction(LevelSelectAction, ETriggerEvent::Completed, this, &AXtionsCharacter::OpenLevelSelect);
		EnhancedInputComponent->BindAction(LevelSkipAction, ETriggerEvent::Completed, this, &AXtionsCharacter::SkipCurrentLevel);
	}
}

void AXtionsCharacter::Damage(int32 Amount)
{
	if (!bAlive || bIsDodging) return;

	Hits++;
	OnCharacterHit.Broadcast();

	PlaySound(OnDamageSound);
}

// Aka level complete / set up new level
void AXtionsCharacter::TransportCharacter(FLevelInfo& LevelInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("Transporting Character"));
	GameInstance->SetShowOverlay(true);
	SetActorLocation(LevelInfo.Location);
	PlayerController->SetControlRotation(LevelInfo.Rotation);
	SetActorRotation(LevelInfo.Rotation);

	NumDodges = LevelInfo.Dodges;
	OnDodgesUpdated.Broadcast(NumDodges);

	Hits = 0;
	RequiredObjects.Empty();
	if (LevelInfo.RequiredObjects.Num() > 0)
	{
		for (int32 Item : LevelInfo.RequiredObjects)
		{
			RequiredObjects.Add(Item);
		}
	}
	ObjectCounts.Empty();
	CurrentObjectCounts.Empty();
	if (LevelInfo.ObjectCounts.Num() > 0)
	{
		for (int32 Item : LevelInfo.ObjectCounts)
		{
			ObjectCounts.Add(Item);
			CurrentObjectCounts.Add(0);
		}
	}

	OnUpdateIntraMission.Broadcast(0, true);
	bAlive = true;
}

void AXtionsCharacter::SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox)
{
	if (InOverlappedBox)
	{
		if (ReadyToRepair())
		{
			OnOverlappingBox.Broadcast(true, true);
		}
		else
		{
			OnOverlappingBox.Broadcast(true, false);
		}
	}
	else
	{
		OnOverlappingBox.Broadcast(false, false);
	}

	OverlappedBox = InOverlappedBox;
}

void AXtionsCharacter::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}
}
