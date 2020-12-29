#pragma once

#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "SaveMetadata.generated.h"

USTRUCT(BlueprintType)
struct FSaveMetadata{
	GENERATED_BODY()
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Default")
	FString SlotName;															//Name of the slot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	FDateTime Date;																//When the slot was created.
	
};	