// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ActivateLaunchPadFX;

	UFUNCTION()
	void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Pitch Angle added on top of character's on overlap*/
	UPROPERTY(EditInstanceOnly, Category = "LaunchPadParams") // Allows to edit in level per instance - means that every object of this class will have its own params
	float LaunchPitchAngle;

	/* Total launch strength added to the character on overlap*/
	UPROPERTY(EditInstanceOnly, Category = "LaunchPadParams") // Allows to edit in level per instance - means that every object of this class will have its own params
	float LaunchStrength;
};
