// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAI_Guard.generated.h"

class UPawnSensingComponent;

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

	// Original rotation of a pawn
	FRotator OriginalRotation;

	// Timer handler for reset to original rotation
	FTimerHandle TimerHandle_ResetOrientation;

	UFUNCTION()
	void ResetOrientation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
