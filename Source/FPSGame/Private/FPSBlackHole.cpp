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
	SimulatingSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SimulatingSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SimulatingSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SimulatingSphereComp->SetupAttachment(MeshComp);


	DestructiveSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestructiveSphereComp"));
	DestructiveSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestructiveSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestructiveSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DestructiveSphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::SimulateBlackHole()
{

}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> Components;

	SimulatingSphereComp->GetOverlappingComponents(Components);

}

