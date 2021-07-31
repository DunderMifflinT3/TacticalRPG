#pragma once

#include "GridIndex.generated.h"

USTRUCT(BlueprintType)
struct FGridIndex	//Contains X and Y coordinates for working with a grid
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y;

	bool operator<(const FGridIndex& otherIndex) const
	{
		if (X != otherIndex.X) return X < otherIndex.X;
		return Y < otherIndex.Y;
	}

	bool operator==(const FGridIndex& otherIndex) const
	{
		return (X == otherIndex.X) && (Y == otherIndex.Y);
	}

	bool operator!=(const FGridIndex& otherIndex) const
	{
		return (X != otherIndex.X) || (Y != otherIndex.Y);
	}
};

FORCEINLINE uint32 GetTypeHash(const FGridIndex& index)
{
	uint32 Hash = FCrc::MemCrc32(&index, sizeof(FGridIndex));
	return Hash;
}