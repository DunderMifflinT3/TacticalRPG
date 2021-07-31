#pragma once

#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int minRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int vertRange = 2;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Sword			UMETA(DisplayName = "Sword"),
	Dagger			UMETA(DisplayName = "Dagger"),
	Axe				UMETA(DisplayName = "Axe"),
	Polearm			UMETA(DisplayName = "Polearm"),
	Bow				UMETA(DisplayName = "Bow"),
	Firearm			UMETA(DisplayName = "Firearm"),
	Wand			UMETA(DisplayName = "Wand"),
	Tome			UMETA(DisplayName = "Tome")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Slash			UMETA(DisplayName = "Slash"),
	Pierce			UMETA(DisplayName = "Pierce"),
	Strike			UMETA(DisplayName = "Strike"),
	Fire			UMETA(DisplayName = "Fire"),
	Water			UMETA(DisplayName = "Water"),
	Ice				UMETA(DisplayName = "Ice"),
	Lightning		UMETA(DisplayName = "Lightning"),
	Poison			UMETA(DisplayName = "Poison"),
	Holy			UMETA(DisplayName = "Holy")
};
