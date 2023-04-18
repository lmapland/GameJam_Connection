

#include "LevelManagement/StartZone.h"
#include "Components/BoxComponent.h"

AStartZone::AStartZone()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	SetRootComponent(BoxCollider);
}

void AStartZone::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AStartZone::EndOverlap);
}

void AStartZone::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("StartZone: OverlapEnd"));
	CharacterExited(Level);
}

void AStartZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartZone::CharacterExited(int32 InLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("StartZone: Broadcasting"));
	OnCharacterExitDelegate.Broadcast(InLevel);
	Destroy();
}

