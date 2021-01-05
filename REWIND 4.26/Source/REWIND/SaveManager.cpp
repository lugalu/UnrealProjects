#include "SaveManager.h"


#include "SaveGameMetadata.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameData.h"
#include "Engine/Engine.h"
// Fill out your copyright notice in the Description page of Project Settings.
FString USaveManager::CurrentSaveSlot;
static const FString KMetadataSaveSlot = "SaveGameMetadata";
static const int32 KMaxSaveSlot = 100;
 TArray<TScriptInterface<ISaveInterface>> USaveManager::SaveInterfaces;

void USaveManager::Init(){
	CurrentSaveSlot = "Default";

	
	USaveGame* SaveGameMetadata = UGameplayStatics::LoadGameFromSlot(KMetadataSaveSlot,0);														//Make sure the game metadata file exists in case the game has never been ran

	if(SaveGameMetadata == nullptr){
		USaveGame* saveGameObject = UGameplayStatics::CreateSaveGameObject(USaveGameMetadata::StaticClass());									//Create save metadata if it doesn't exist

		UGameplayStatics::SaveGameToSlot(saveGameObject,KMetadataSaveSlot,0);
		
	}
}

void USaveManager::QuerryAllSaveInterfaces(){

	SaveInterfaces.Empty();																														//Clear the old Entries

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld,USaveInterface::StaticClass(),actors);
	
	for(AActor* actor : actors){
		SaveInterfaces.Add(actor);
	}
	
}

void USaveManager::SaveGame(){																													//Create a new Save Game instance
	USaveGameData* SaveGameData = Cast<USaveGameData> (UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	
	for (auto& SaveInterface:SaveInterfaces){																									//GO over all the actors that need to be saved and save them.
		if (SaveInterface.GetObject()==nullptr){
			continue;
		}

		SaveInterface->Execute_OnBeforeSave(SaveInterface.GetObject());																			//Let the object know that it's about to be saved
		FString UniqueSaveName = SaveInterface->Execute_GetUniqueSaveName(SaveInterface.GetObject());									//Find the object's save data using it's unique name
		FSaveData& SaveData = SaveGameData->SerializedData.Add(UniqueSaveName);

		FMemoryWriter MemoryWriter = FMemoryWriter(SaveData.Data);
		MemoryWriter.ArIsSaveGame = true;

		SaveInterface.GetObject()->Serialize(MemoryWriter);
		
	}

	UGameplayStatics::SaveGameToSlot(SaveGameData,CurrentSaveSlot,0);																	//Save the game to the Current slot
																																				//Update the metadata file with the new slot
	USaveGameMetadata* SaveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(KMetadataSaveSlot,0));

	FSaveMetadata& SaveMetadata = SaveGameMetadata->SavedGamesMetadata.FindOrAdd(CurrentSaveSlot);

	SaveMetadata.SlotName=CurrentSaveSlot;
	SaveMetadata.Date = FDateTime::Now();

	UGameplayStatics::SaveGameToSlot(SaveGameMetadata,KMetadataSaveSlot,0);																//Save the changes to the metadata file
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,"Saved: "+CurrentSaveSlot);
	}
}

void USaveManager::LoadGame(){
	USaveGameData* SaveGameData =Cast<USaveGameData>( UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot,0));

	if (SaveGameData == nullptr){
		SaveGame();																																//If no saves exists, create one
		SaveGameData = Cast<USaveGameData>( UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot,0));									//Reload the save Game
	}

	//Loop over all the actors that need to load the data
	for (auto& SaveInterface: SaveInterfaces){
		if (SaveInterface.GetObject()==nullptr){
			continue;
		}
		FString UniqueSaveName = SaveInterface->Execute_GetUniqueSaveName(SaveInterface.GetObject());											//Find the object's save data using it's unique name
		FSaveData* SaveData = SaveGameData->SerializedData.Find(UniqueSaveName);
		if (SaveData == nullptr){
			continue;
		}

		FMemoryReader MemoryReader(SaveData->Data);
		MemoryReader.ArIsSaveGame = false;
		SaveInterface.GetObject()->Serialize(MemoryReader);
		
	}
	
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,"Loaded: "+CurrentSaveSlot);
	}
}

void USaveManager::DeleteSlot(const FString& Slot){
	UGameplayStatics::DeleteGameInSlot(Slot,0);																							//Delete the save

	USaveGameMetadata* SaveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(KMetadataSaveSlot,0));			//Loading the metadata slot

	SaveGameMetadata->SavedGamesMetadata.Remove(Slot);

	UGameplayStatics::SaveGameToSlot(SaveGameMetadata,KMetadataSaveSlot,0);																//Save the metadata file
}

FString USaveManager::GetNewSaveSlot(bool& Slot_found){
	Slot_found=false;

	USaveGameMetadata* SaveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(KMetadataSaveSlot,0));			//Loading the metadata file

	for (int32 i=0;i<KMaxSaveSlot;++i){
		FString SlotName = "SaveSlot" + FString::FromInt(i);

		if(!SaveGameMetadata->SavedGamesMetadata.Contains(SlotName)){
			Slot_found=true;
			return SlotName;
		}
		
	}

	return FString();
}

void USaveManager::SetCurrentSaveSlot(const FString& Slot){
	CurrentSaveSlot = Slot;
}

FString USaveManager::GetCurrentSaveSlot(){
	return CurrentSaveSlot;
}

TArray<FSaveMetadata> USaveManager::GetAllSaveMetadata(){
	TArray<FSaveMetadata> Metadata;

	USaveGameMetadata* SaveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(KMetadataSaveSlot,0));

	Metadata.Reserve(SaveGameMetadata->SavedGamesMetadata.Num());

	for (const auto& Saved_game : SaveGameMetadata->SavedGamesMetadata){
		Metadata.Push(Saved_game.Value);
	}
	return Metadata;
}