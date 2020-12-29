// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RewindGameMode.generated.h"

/**
* 
*/
UCLASS()
class REWIND_API ARewindGameMode : public AGameModeBase
{
	GENERATED_BODY()
	public:
	ARewindGameMode();
	virtual void InitGameState() override;

	
};