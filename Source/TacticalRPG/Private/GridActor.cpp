#include "GridActor.h"

// Sets default values
AGridActor::AGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AGridManager* AGridActor::GetGridManager() const
{
	return gridManager;
}

FGridIndex AGridActor::GetCurrentIndex() const
{
	return index;
}

