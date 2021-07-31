#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridIndex.h"
#include "PathfindingData.h"
#include "TerrainData.h"
#include "FactionData.h"
#include "WeaponData.h"
#include "GridManager.generated.h"

class AGridActor;
class AUnit;

USTRUCT(BlueprintType)
struct FIndexData	//Holds data that is attributed to each index in the grid
{
	GENERATED_BODY()

	FVector location;
	ETerrainType terrain;
	int altitude = 0;
};


UCLASS()
class TACTICALRPG_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();
	AGridManager(FGridIndex gridSize);

protected:
	//Grid Attributes
	FGridIndex gridSize = { 10,10 };	//Maximum size of the grid using X and Y coordinates
	float indexSize = 200;		//Specifies the length and width of each index
	float indexHeight = 50;		//Specifies the height difference at which an index rises in elevation
	TMap<FGridIndex, FIndexData> gridMap;	//Map containing all indices and their respective data
	TMap<FGridIndex, AUnit*> unitMap;	//Map containing all units on the grid
	TMap<FGridIndex, AGridActor*> obstacleMap;		//Map containing all grid actors that block the index
	TMap<FGridIndex, AGridActor*> passableActorMap;		//Map containing all grid actors that do not block the index

public:	
	//Utility
	UFUNCTION(BlueprintCallable, category = "Utility")
	bool isValidIndex(FGridIndex index) const;	//Checks if the index is in the grid

	UFUNCTION(BlueprintCallable, category = "Utility")
	FGridIndex LocationToIndex(FVector location) const;	//Converts a world location to an index in the grid

	UFUNCTION(BlueprintCallable, category = "Utility")
	int GetDistanceBetweenIndices(FGridIndex startIndex, FGridIndex targetIndex) const;	//Gets the Manhattan distance between two indices

	UFUNCTION(BlueprintCallable, category = "Utility")
	int GetAltitudeDifference(FGridIndex startIndex, FGridIndex targetIndex) const;	//Gets the difference in height from one index to another

	UFUNCTION(BlueprintCallable, category = "Utility")
	TArray<FGridIndex> GetSurroundingIndices(FGridIndex index) const;	//Gets the four indices surrounding the specified index

	UFUNCTION(BlueprintCallable, category = "Utility")
	bool isIndexOccupied(FGridIndex index) const;	//Checks if something is blocking the specified index


	//Range
	UFUNCTION(BlueprintCallable, category = "Attributes")
	TArray<FGridIndex> GetIndicesInRange(FGridIndex startIndex, FRange range) const;	//Gets the location of a specific index


	//Index Attributes
	UFUNCTION(BlueprintCallable, category = "Attributes")
	FIndexData GetIndexData(FGridIndex index) const;	//Gets the data of the specified index

	UFUNCTION(BlueprintCallable, category = "Attributes")
	FTerrainData GetTerrainData(ETerrainType terrain) const;	//Gets the data associated with a terrain type


	//Grid Actors
	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	AUnit* GetIndexUnit(FGridIndex index) const;	//Gets the unit at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void AddIndexUnit(FGridIndex index, AUnit* unit);	//Updates the unit at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void RemoveIndexUnit(FGridIndex index);	//Removes the unit from a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	AGridActor* GetIndexObstacle(FGridIndex index) const;	//Gets the grid actor at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void AddIndexObstacle(FGridIndex index, AGridActor* gridActor);	//Updates the obstacle at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void RemoveIndexObstacle(FGridIndex index);		//Removes the obstacle from a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	AGridActor* GetIndexPassableActor(FGridIndex index) const;	//Gets the passable grid actor at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void AddIndexPassableActor(FGridIndex index, AGridActor* gridActor);	//Updates the passable actor at a specific index

	UFUNCTION(BlueprintCallable, category = "Grid Actors")
	void RemoveIndexPassableActor(FGridIndex index);	//Removes the passableActor from a specific index


	//Pathfinding
	UFUNCTION(BlueprintCallable, category = "Pathfinding")
	TSet<FGridIndex> FindReachableIndices(FGridIndex startIndex, const AUnit *unit);	//Finds indexes reachable by a unit using the breadth first search algorithm

	UFUNCTION(BlueprintCallable, category = "Pathfinding")
	FPath FindPathToIndex(FGridIndex startIndex, FGridIndex targetIndex, const AUnit *unit);	//Finds a path between two indexes using the A* algorithm

	UFUNCTION(BlueprintCallable, category = "Pathfinding")
	FPathScore FindIndexScore(FGridIndex currentStep, FGridIndex parentIndex, FGridIndex targetIndex, TMap<FGridIndex, FPathfinding> pathMap, const AUnit *unit);	//Scores an index based on the amount of movement it takes to get there

	UFUNCTION(BlueprintCallable, category = "Pathfinding")
	FGridIndex FindLowestScoreInOpenList(TSet<FGridIndex> openList, TMap<FGridIndex, FPathfinding> pathMap);	//Returns the best index to continue pathfinding from
};

