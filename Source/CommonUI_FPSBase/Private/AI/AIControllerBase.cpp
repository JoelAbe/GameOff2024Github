// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/NPCBaseCharacter.h"

AAIControllerBase::AAIControllerBase()
{
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;

	ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(InPawn);
	if (NPC)
	{
		RunBehaviorTree(NPC->GetBehaviorTree());
	}
}
