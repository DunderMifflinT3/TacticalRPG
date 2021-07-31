#include "UnitController.h"
#include "Unit.h"
#include "GridManager.h"
#include "Ability.h"

// Sets default values
AUnitController::AUnitController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AUnitController::AUnitController(AUnit* unit)
{
	owningUnit = unit;
}


//Initiative
void AUnitController::ActivateUnit()
{
	//Clears arrays of abilities in case ability list is different from last activation
	validAbilities.Empty();
	invalidAbilities.Empty();

	indicesInRange = FindIndicesInMoveRange(owningUnit->GetCurrentIndex());	//Sets indices in range to all indices within the unit's movement range

	//Makes an array of all a unit's abilities and spawns them
	TSet<FName> activeAbilities = owningUnit->GetAbilities();

	for (FName ability : activeAbilities)	//loops through all the possible active abilities to see which are valid
	{
		AAbility currentAbility(ability, owningUnit);
		if (currentAbility.canActivate())	//Sorts abilities based on if they can be activated or not
		{
			validAbilities.Add(&currentAbility);
		}
		else
		{
			invalidAbilities.Add(&currentAbility);
		}
	}
}

void AUnitController::EndUnitAction()
{
	ClearAbilities();
}


//Range
TSet<FGridIndex> AUnitController::FindIndicesInMoveRange(FGridIndex startIndex)
{
	return owningUnit->GetGridManager()->FindReachableIndices(startIndex, owningUnit);
}


//Utility
void AUnitController::ClearAbilities()
{
	//Destroys each ability instance
	for (int i = 0; i < validAbilities.Num(); i++)
	{
		validAbilities[i]->Destroy();
	}
	for (int i = 0; i < invalidAbilities.Num(); i++)
	{
		invalidAbilities[i]->Destroy();
	}
}

