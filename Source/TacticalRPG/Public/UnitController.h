#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridIndex.h"
#include "UnitController.generated.h"

class AUnit;
class AAbility;

UCLASS()
class TACTICALRPG_API AUnitController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitController();
	AUnitController(AUnit* unit);

protected:
	AUnit* owningUnit;	//The unit that this actor controls
	TSet<FGridIndex> indicesInRange;	//List of all indices in movement range

	//Abilities
	AAbility* selectedAbility;	//The ability that is currently in use
	TArray<AAbility*> validAbilities;	//List of abilities that can be performed by the unit
	TArray<AAbility*> invalidAbilities;	//List of abilities that cannot be performed by the unit

	//Status
	int movementUsed = 0;
	bool movementLocked = false;
	bool usedAction = false;

public:
	//Initiative
	UFUNCTION(BlueprintCallable, category = "Initiative")
	virtual void ActivateUnit();	//Allows the unit to perform actions

	UFUNCTION(BlueprintCallable, category = "Initiative")
	virtual void EndUnitAction();	//Ends the unit's current action, deciding what to do based on if the unit moved or used an ability


	//Range
	UFUNCTION(BlueprintCallable, category = "Range")
	TSet<FGridIndex> FindIndicesInMoveRange(FGridIndex startIndex);	//Finds all indices that the unit can move to


	//Utility
	UFUNCTION(BlueprintCallable, category = "Range")
	void ClearAbilities();	//Clears valid and invalid abilities lists
};
