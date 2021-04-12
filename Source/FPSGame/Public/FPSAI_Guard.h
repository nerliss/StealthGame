// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAI_Guard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};


UCLASS()
class FPSGAME_API AFPSAI_Guard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAI_Guard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	/* Original rotation of a pawn*/
	FRotator OriginalRotation;

	/* Timer handler for reset to original rotation*/
	FTimerHandle TimerHandle_ResetOrientation;

	/* Guard state*/ 
	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EAIState GuardState;

	/* Replicated (clientside) guard state function*/
	UFUNCTION()
	void OnRep_GuardState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// PATROLLING

	/* Allows a pawn to patrol*/
	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol")) // meta tag turns to gray or highlights current field based on bPatrol variable
	AActor* PatrolPointOne;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* PatrolPointTwo;

	AActor* PatrolPointCurrent;

	void MoveToNextPatrolPoint();
};
