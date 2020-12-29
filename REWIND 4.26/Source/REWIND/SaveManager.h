// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveMetadata.h"
#include "SaveInterface.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"

/**
 * the tutorial of this save system was made by Reids channel in youtube.
 * The videos can be found at these URLs :
 * https://www.youtube.com/watch?v=doO4g7eUlfs;
 * https://www.youtube.com/watch?v=7NRG55Oy4c8&t=1255s;
 * https://www.youtube.com/watch?v=Ry3CDNcgblc&t=1022s.
 */
UCLASS()
class REWIND_API USaveManager : public UObject
{
	GENERATED_BODY()

private:
	static FString CurrentSaveSlot;										//Current Save Slot;
	static TArray<TScriptInterface<ISaveInterface>> SaveInterfaces;		//All the actors in the game which implement the save interface
	
public:
	static void Init();													//Initialize the class. Called when the game launches;
	static void QuerryAllSaveInterfaces();								//Queries all the actors in the game that implement the save interface;


	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void SaveGame();											//Saves the current state of the game

	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void LoadGame();											//Loads the current state of the game

	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void DeleteSlot(const FString& Slot);					//Deletes the specified Slot

	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static UPARAM(DisplayName="Slot")
		FString GetNewSaveSlot(bool& slot_found);				//Gets a new, unused slot

	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void SetCurrentSaveSlot(const FString& Slot);			//Sets the current save Slot to the specified value

	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static FString GetCurrentSaveSlot();								//Gets the current save Slot to the specified value

	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static TArray<FSaveMetadata> GetAllSaveMetadata();				//Gets the current save Slot to the specified value
	
};