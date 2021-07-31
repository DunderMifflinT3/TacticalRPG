#pragma once

#include "FactionData.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Default		UMETA(DisplayName = "Default"),
	Blue		UMETA(DisplayName = "Blue"),
	Red			UMETA(DisplayName = "Red"),
	Green		UMETA(DisplayName = "Green"),
};


USTRUCT(BlueprintType)
struct FFactionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor color;
};