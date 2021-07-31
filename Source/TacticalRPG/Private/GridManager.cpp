#include "GridManager.h"
#include "Unit.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AGridManager::AGridManager(FGridIndex gridSize)
{
	//Loops through each x and y index to create the grid
	for (int x = 0; x < gridSize.X - 1; x++)
	{
		for (int y = 0; y < gridSize.Y - 1; y++)
		{
			FVector indexLocation = { ((x * indexSize) + (indexSize / 2)),((y * indexSize) + (indexSize / 2)),(0) };	//Creates index locations based on the size of each index
			ETerrainType indexTerrain = ETerrainType::Default;	//Decides the terrain type of each index
			int indexAltitude = indexLocation.Z / indexHeight;	//Calculates altitude based on the index height
			gridMap.Add({ x,y }, { indexLocation, indexTerrain, indexAltitude });	//Adds the index and its data to the grid
		}
	}
	PrimaryActorTick.bCanEverTick = false;
}


//Utility
bool AGridManager::isValidIndex(FGridIndex index) const
{
	return ((index.X >= 0 && index.X < gridSize.X) && (index.Y >= 0 && index.Y < gridSize.Y));	//Checks if the both coordinates are at least zero and are less than the grid size
}

FGridIndex AGridManager::LocationToIndex(FVector location) const
{
	//converts a float location to an integer index
	int indexX = floorf(location.X / indexSize);
	int indexY = floorf(location.Y / indexSize);

	if (isValidIndex({ indexX,indexY }))	//Checks if index is in the grid
	{
		return { indexX, indexY };	//index found
	}
	else
	{
		return { -1, -1 };	//index not found
	}
}

int AGridManager::GetDistanceBetweenIndices(FGridIndex startIndex, FGridIndex targetIndex) const
{
	return(abs(startIndex.X - targetIndex.X) + abs(startIndex.Y - targetIndex.Y));	//Gets distance between indices using Manhattan distance
}

int AGridManager::GetAltitudeDifference(FGridIndex startIndex, FGridIndex targetIndex) const
{
	return(GetIndexData(startIndex).altitude - GetIndexData(targetIndex).altitude);
}

TArray<FGridIndex> AGridManager::GetSurroundingIndices(FGridIndex index) const
{
	TArray<FGridIndex> surroundingIndices = { {index.X + 1, index.Y}, { index.X, index.Y + 1 }, { index.X - 1, index.Y }, { index.X, index.Y - 1 } };	//Gets all adjacent indices

	return surroundingIndices;
}

bool AGridManager::isIndexOccupied(FGridIndex index) const
{
	return(GetIndexUnit(index) != NULL || GetIndexObstacle(index) != NULL);	//Checks if there is a grid actor that blocks the specified index
}


//Range
TArray<FGridIndex> AGridManager::GetIndicesInRange(FGridIndex startIndex, FRange range) const
{
	TArray<FGridIndex> indicesInRange;

	for (int x = range.maxRange * -1; x < range.maxRange; x++)
	{
		for (int y = range.maxRange - abs(x) * -1; y < range.maxRange - abs(x); y++)
		{
			FGridIndex currentIndex = { startIndex.X + x, startIndex.Y + y };
			if (isValidIndex(currentIndex) && (abs(GetAltitudeDifference(startIndex, currentIndex)) < range.vertRange) && (GetDistanceBetweenIndices(startIndex, currentIndex) >= range.minRange))	//Checks if the index is in the grid, if it is within the vertical range, and if it excludes indices closer than the min range
			{
				indicesInRange.Add(currentIndex);
			}
		}
	}

	return indicesInRange;	//Returns indices in a diamond pattern around the start index
}


//Index Attributes
FIndexData AGridManager::GetIndexData(FGridIndex index) const
{
	return gridMap[index];
}

FTerrainData AGridManager::GetTerrainData(ETerrainType terrain) const
{
	FTerrainData terrainData;

	return terrainData;
}


//Grid Actors
AUnit* AGridManager::GetIndexUnit(FGridIndex index) const
{
	return unitMap[index];
}

void AGridManager::AddIndexUnit(FGridIndex index, AUnit* unit)
{
	unitMap.Add(index, unit);
}

void AGridManager::RemoveIndexUnit(FGridIndex index)
{
	unitMap.Remove(index);
}

AGridActor* AGridManager::GetIndexObstacle(FGridIndex index) const
{
	return obstacleMap[index];
}

void AGridManager::AddIndexObstacle(FGridIndex index, AGridActor* obstacle)
{
	obstacleMap.Add(index, obstacle);
}

void AGridManager::RemoveIndexObstacle(FGridIndex index)
{
	obstacleMap.Remove(index);
}

AGridActor* AGridManager::GetIndexPassableActor(FGridIndex index) const
{
	return passableActorMap[index];
}

void AGridManager::AddIndexPassableActor(FGridIndex index, AGridActor* passableActor)
{
	passableActorMap.Add(index, passableActor);
}

void AGridManager::RemoveIndexPassableActor(FGridIndex index)
{
	passableActorMap.Remove(index);
}


//Pathfinding
TSet<FGridIndex> AGridManager::FindReachableIndices(FGridIndex startIndex, const AUnit *unit)
{
	int currentStep = 0;
	TSet<FGridIndex> reachableIndices;
	TSet<FGridIndex> indicesInCurrentStep;
	TSet<FGridIndex> indicesInNextStep;
	TMap<FGridIndex, int> costFromStart;

	//Starts the algorithm from the start index with a cost of 0
	reachableIndices.Add(startIndex);
	indicesInCurrentStep.Add(startIndex);
	costFromStart.Add(startIndex, 0);

	while (currentStep <= unit->GetMovementStats().move)		//Continues the algorithm until it reaches the move limit
	{
		for (FGridIndex currentIndex : indicesInCurrentStep)	//Loops through every index in the current step
		{
			TArray<FGridIndex> surroundingIndices = GetSurroundingIndices(currentIndex);
			for (int i = 0; i < surroundingIndices.Num(); i++)	//Loops through each index surrounding the current step and checks if it can be moved to
			{
				if (isValidIndex(surroundingIndices[i]))	//First checks if index is valid
				{
					if (!reachableIndices.Contains(surroundingIndices[i]) && abs(GetAltitudeDifference(currentIndex, surroundingIndices[i])) <= unit->GetMovementStats().jump)	//Eliminates index if it is already reachable or if its altitude cannot be reached at all
					{
						int indexCost = unit->GetTerrainCost(GetIndexData(surroundingIndices[i]).terrain) + costFromStart[currentIndex];
						if (indexCost <= currentStep)	//Checks if the cost up until this index + the cost of this index is within the movement range
						{
							if (isIndexOccupied(surroundingIndices[i]))
							{
								if (unit->canMoveThroughIndex(surroundingIndices[i]))	//Checks if unit can move through index, despite it being occupied
								{
									//Adds index to the next step, but does not add it to reachable indices, due to it being occupied
									indicesInNextStep.Add(surroundingIndices[i]);
									costFromStart.Add(surroundingIndices[i], indexCost);
								}
							}
							else
							{
								//Index is not occupied and it is reachable, adding it to the next step
								indicesInNextStep.Add(surroundingIndices[i]);
								reachableIndices.Add(surroundingIndices[i]);
								costFromStart.Add(surroundingIndices[i], indexCost);
							}
						}
						else
						{
							indicesInNextStep.Add(currentIndex);		//The index cannot be reached in this step, so it checks it again in the next step
						}
					}
				}
			}
		}

		indicesInCurrentStep = indicesInNextStep;	//All indices that were set to be checked later will be checked in the next iteration
		indicesInNextStep.Empty();	//Resets indicesInNextStep for the next iteration
		currentStep++;		//Increments to the next step and continues the algorithm
	}

	return reachableIndices;	//Returns all reachable indices once no more indices can be found within the move limit
}

FPath AGridManager::FindPathToIndex(FGridIndex startIndex, FGridIndex targetIndex, const AUnit *unit)
{
	FGridIndex currentIndex;
	TMap<FGridIndex, FPathfinding> pathMap;
	TSet<FGridIndex> openList;	//Indices to check
	TSet<FGridIndex> closedList;	//Checked indices
	TArray<FGridIndex> completedPath;

	if (startIndex == targetIndex)	//First checks if the start and target are the same index
	{
		return { {startIndex}, 0 };	//Returns the path as only the starting index, with a cost of 0
	}
	if (isIndexOccupied(targetIndex))
	{
		return { completedPath, 0 };	//Returns an empty path
	}

	openList.Add(startIndex);	//Initializes the open list with the starting index

	while (openList.Num() > 0)	//Continues the algorithm until there are no more indices to check
	{
		currentIndex = FindLowestScoreInOpenList(openList, pathMap);	//Finds the index that is most likely to make progress towards the target
		closedList.Add(currentIndex);	//Marks the current index as checked so it will not be checked again
		openList.Remove(currentIndex);	//Removes current index from the list of indices that need to be checked

		if (currentIndex == targetIndex)	//Checks if the current index is the target
		{
			while (currentIndex != startIndex);	//Loops backwards through each index's parent to build a path back to the start
			{
				completedPath.Add(currentIndex);	//Adds the current index to the final path
				currentIndex = pathMap[currentIndex].parentIndex;		//makes the new current index the parent of the previous
			}
			return { completedPath, pathMap[completedPath[0]].indexScore.costFromStart };	//Returns the fully built path backwards, derriving the path's cost from the target index's cost from start
		}
		else
		{
			TArray<FGridIndex> surroundingIndices = GetSurroundingIndices(currentIndex);	//Finds surrounding indices in order to determine the best one to check next
			for (int i = 0; i < surroundingIndices.Num(); i++)
			{
				if (isValidIndex(surroundingIndices[i]))	//Makes sure index is in the grid
				{
					if (!closedList.Contains(surroundingIndices[i]))	//Ignores previously checked tiles
					{
						FPathScore indexScore = FindIndexScore(surroundingIndices[i], currentIndex, targetIndex, pathMap, unit);

						if (openList.Contains(surroundingIndices[i]))	//Adds index to open list if it is not already on it to check if a new parent index makes a better path
						{
							if (indexScore.costFromStart < pathMap[surroundingIndices[i]].indexScore.costFromStart)	//Checks if the new parent provides a shorter path than the previous
							{
								pathMap.Add(surroundingIndices[i], { currentIndex, indexScore });	//Replaces the previous parent with the current index to make a better path
							}
						}
						else
						{
							if (indexScore.costFromStart <= unit->GetMovementStats().move)	//Checks if the index is within movement range
							{
								pathMap.Add(surroundingIndices[i], { currentIndex, indexScore });	//Adds the index with its parent to the path map if it can potentially be a part of the completed path
								openList.Add(surroundingIndices[i]);	//Adds the index to the open list
							}
						}
					}
				}
			}
		}
	}

	return { completedPath, 0 };	//Returns an empty path if the algorithm completes without reaching the target
}

FPathScore AGridManager::FindIndexScore(FGridIndex currentStep, FGridIndex parentIndex, FGridIndex targetIndex, TMap<FGridIndex, FPathfinding> pathMap, const AUnit *unit)
{
	int costFromStart;
	int estimatedRemaining;
	int score;

	if (isIndexOccupied(currentStep))
	{
		if (!unit->canMoveThroughIndex(currentStep))		//Checks if unit can move through the index, despite it being occupied
		{
			return { 999, 999, 999 };	//Index is impassable due to being occupied
		}
	}
	if (abs(GetAltitudeDifference(parentIndex, currentStep)) <= unit->GetMovementStats().jump)	//Checks if index can be reached based on its altitude
	{
		costFromStart = (pathMap[parentIndex].indexScore.costFromStart + unit->GetTerrainCost(GetIndexData(currentStep).terrain));	//Finds total cost to reach this index from the starting index
		estimatedRemaining = GetDistanceBetweenIndices(currentStep, targetIndex);	//Gets the Manhattan distance between the current step and the target
		score = costFromStart + estimatedRemaining;	//Adds cost from start with the estimated cost to the target in order to get the index's score

		return { costFromStart, estimatedRemaining, score };	//Returns the score of this index
	}

	return{ 999, 999, 999 };	//Index is impassable due to its altitude
}

FGridIndex AGridManager::FindLowestScoreInOpenList(TSet<FGridIndex> openList, TMap<FGridIndex, FPathfinding> pathMap)
{
	FGridIndex bestIndex;
	int lowestScore = 999;

	for (FGridIndex currentIndex : openList)
	{
		if (pathMap[currentIndex].indexScore.score < lowestScore)	//Checks if the score of the current index is less than that of the lowest scoring index
		{
			//Sets the new best index to the current index and updates the lowest scoree
			bestIndex = currentIndex;
			lowestScore = pathMap[currentIndex].indexScore.score;
		}
	}

	return bestIndex;	//returns the index with the lowest score
}

