// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPoint.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"


UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Patrol Point");
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Attempt to get the NPC's Controller
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// Attempt to get the blackboard component from the behavior tree
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			// Get the current patrol path fromt he blackboard
			auto const Index = bc->GetValueAsInt((GetSelectedBlackboardKey()));

			// Get the NPC
			if (auto* npc = Cast<ANPC>(cont->GetPawn()))
			{
				// Get the current patrol path vector from the NPC (Local to the patrol path actor)
				auto const Point = npc->GetPatrolPath()->GetPatrolPoint(Index);

				// Convert the local vector to a global point
				auto const GlobalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}


	return EBTNodeResult::Failed;
}
