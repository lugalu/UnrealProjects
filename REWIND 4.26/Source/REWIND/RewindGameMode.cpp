// Fill out your copyright notice in the Description page of Project Settings.


#include "RewindGameMode.h"
#include "SaveManager.h"

ARewindGameMode::ARewindGameMode(){
}



void ARewindGameMode::InitGameState(){
	USaveManager::QuerryAllSaveInterfaces();
	USaveManager::LoadGame();

	Super::InitGameState();

	
}
