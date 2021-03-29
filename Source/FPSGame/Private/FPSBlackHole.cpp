// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SimulatingSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SimulatingSphereComp"));
	SimulatingSphereComp->SetupAttachment(MeshComp);
	SimulatingSphereComp->SetSphereRadius(4000.f);
	SimulatingSphereComp->bHiddenInGame = false;

	DestructiveSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestructiveSphereComp"));
	DestructiveSphereComp->SetupAttachment(MeshComp);
	DestructiveSphereComp->SetSphereRadius(200.f);
	DestructiveSphereComp->bHiddenInGame = false;

	DestructiveSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapDestructiveSphere); // Event binding

}

// Destructive sphere implementation
void AFPSBlackHole::OverlapDestructiveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	SimulatingSphereComp->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComponents[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics()) // Checking whether the component is able to simulate physics
		{
			const float SphereRadius = SimulatingSphereComp->GetScaledSphereRadius();
			const float SimulatingForce = -2000; // Negative value pulls objects to center, positive - pushes out

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, SimulatingForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

