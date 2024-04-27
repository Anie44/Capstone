// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"




UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Player Character
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Get player location to use as an origin
		auto const PlayerLocation = Player->GetActorLocation();
			if (SearchRandom)
			{
				FNavLocation Loc;

				// Get the Navigation System and Generate a Random Location Near the Player
				if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					// Try to get a Random Location Near the Player
					if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
					}
				}

			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
	}
	return EBTNodeResult::Failed;
}