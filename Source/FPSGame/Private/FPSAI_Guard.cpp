// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAI_Guard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AFPSAI_Guard::AFPSAI_Guard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// Binding events
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAI_Guard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAI_Guard::OnNoiseHeard);

}

// Called when the game starts or when spawned
void AFPSAI_Guard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

}

void AFPSAI_Guard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) // safecheck
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Green, false, 10.f);
}

void AFPSAI_Guard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 30.f, 12, FColor::Black, false, 10.f);

	// Direction vector
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	// Need this so a pawn can only rotate on Yaw
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	// Timer to reset rotation
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAI_Guard::ResetOrientation, 3.f, false);
}

void AFPSAI_Guard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAI_Guard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

