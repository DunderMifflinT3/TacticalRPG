#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridIndex.h"
#include "GridActor.generated.h"

class AGridManager;

UCLASS()
class TACTICALRPG_API AGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridActor();

protected:
	AGridManager* gridManager;	//Reference to the grid manager for easy communication
	FGridIndex index;	//The index on the grid that the actor is currently on

public:	
	UFUNCTION(BlueprintCallable)
	AGridManager* GetGridManager() const;

	UFUNCTION(BlueprintCallable)
	FGridIndex GetCurrentIndex() const;		//Gets the index that this grid actor is occupying
};
