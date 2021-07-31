#pragma once

#include "StatData.h"
#include "TerrainData.generated.h"

USTRUCT(BlueprintType)
struct FTerrainData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText terrainName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText terrainDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECombatStats, int> combatStatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool flammable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool freezable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool wet = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool stealth = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool indoor = false;
};


UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Grass			UMETA(DisplayName = "Grass"),
	TallGrass		UMETA(DisplayName = "Tall Grass"),
	Forest			UMETA(DisplayName = "Forest"),
	Mountain		UMETA(DisplayName = "Mountain"),
	Swamp			UMETA(DisplayName = "Swamp"),
	ShallowWater	UMETA(DisplayName = "Shallow Water"),
	DeepWater		UMETA(DisplayName = "Deep Water"),
	Sand			UMETA(DisplayName = "Sand")
};

