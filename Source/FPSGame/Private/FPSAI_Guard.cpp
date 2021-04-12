// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAI_Guard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAI_Guard::AFPSAI_Guard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// Binding events
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAI_Guard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAI_Guard::OnNoiseHeard);

	GuardState = EAIState::Idle;

}

// Called when the game starts or when spawned
void AFPSAI_Guard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAI_Guard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) // safecheck
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Green, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()); // Get gamemode state
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false); // Mission fail statement 
	}

	SetGuardState(EAIState::Alerted);

	// Stop movement while patrolling when a pawn seen
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}

}

void AFPSAI_Guard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) // do nothing if alerted
	{
		return;
	}

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

	SetGuardState(EAIState::Suspicious);
	
	// Stop movement while patrolling when a noise heard
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAI_Guard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) // do nothing if alerted
	{
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	// Stopped investigating if a patrolling pawn, pick a new patrol point
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAI_Guard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) // safecheck
	{
		return;
	}

	GuardState = NewState;
	OnRep_GuardState(); // to make sure there is no difference between server and client
}

void AFPSAI_Guard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

// Function that applies a default rule to update GuardState variable
void AFPSAI_Guard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOPERLIFETIME(AFPSAI_Guard, GuardState);
}

// Called every frame
void AFPSAI_Guard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Patrol goal check
	if (PatrolPointCurrent)
	{
		FVector Delta = GetActorLocation() - PatrolPointCurrent->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		// Check if a pawn is within 50 units of a goal, if so - take a new point
		if (DistanceToGoal < 100.f)
		{
			MoveToNextPatrolPoint();
		}
	}

}

void AFPSAI_Guard::MoveToNextPatrolPoint()
{
	// Assign next patrol point
	if (PatrolPointCurrent == nullptr || PatrolPointCurrent == PatrolPointTwo)
	{
		PatrolPointCurrent = PatrolPointOne;
	}
	else
	{
		PatrolPointCurrent = PatrolPointTwo;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PatrolPointCurrent);
}

