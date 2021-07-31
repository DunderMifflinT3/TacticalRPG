#pragma once

#include "StatData.generated.h"

UENUM(BlueprintType)
enum class EStats : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Health			UMETA(DisplayName = "Health"),
	Stamina			UMETA(DisplayName = "Stamina"),
	Strength		UMETA(DisplayName = "Strength"),
	Mind			UMETA(DisplayName = "Mind"),
	Defense			UMETA(DisplayName = "Defense"),
	Resistance		UMETA(DisplayName = "Resistance"),
	Agility			UMETA(DisplayName = "Agility"),
	Dexterity		UMETA(DisplayName = "Dexterity")
};


UENUM(BlueprintType)
enum class ECombatStats : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Attack			UMETA(DisplayName = "Health"),
	Guard			UMETA(DisplayName = "Stamina"),
	Speed			UMETA(DisplayName = "Strength"),
	Accuracy		UMETA(DisplayName = "Mind"),
	Evasion			UMETA(DisplayName = "Defense"),
	Critical		UMETA(DisplayName = "Resistance"),
	CriticalAvoid	UMETA(DisplayName = "Agility"),
};


USTRUCT(BlueprintType)
struct FMovementStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int move = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int jump = 0;
};