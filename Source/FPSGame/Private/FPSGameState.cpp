// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

// Multicast function is required for replicating logic
void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(It->Get());
		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			// Disable input for all players (short version)
			APawn* MyPawn = PlayerController->GetPawn();
			if (MyPawn)
			{
				MyPawn->DisableInput(PlayerController);
			}
		}
	}
}
