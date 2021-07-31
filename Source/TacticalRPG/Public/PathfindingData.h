#pragma once

#include "GridIndex.h"
#include "PathfindingData.generated.h"

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGridIndex> pathIndexes;	//Array of all indexes on the path

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int pathCost;	//Total movement cost of the path
};


USTRUCT(BlueprintType)
struct FPathScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int costFromStart;	//Cost from the start of the path (G score)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int estimatedRemaining;	//Estimated remaining cost to the end of the path (H score)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int score;	//Total score of the index, found by adding costFromStart with estimatedRemaining (F score)
};


USTRUCT(BlueprintType)
struct FPathfinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGridIndex parentIndex;	//Previous index

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPathScore indexScore;	//Path score
};

