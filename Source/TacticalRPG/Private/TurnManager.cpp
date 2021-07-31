#include "TurnManager.h"

// Sets default values
ATurnManager::ATurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//Units
TArray<AUnit*> ATurnManager::GetUnitsInFaction(EFaction faction) const
{
	return factionMap[faction].unitsInFaction;
}

void ATurnManager::UpdateUnitFaction(AUnit *unit, EFaction newFaction)
{
	unit->SetFaction(newFaction);

	if (newFaction == activeFaction)	//Allows unit to activate if it is switching to the active faction
	{
		unitsToAct.Add(unit);
	}
}

void ATurnManager::AddUnitToCombat(AUnit *unit)
{
	unitsInCombat.Add(unit);	//Adds unit to the list of all units on the grid

	if (unit->GetFaction() == activeFaction)		//If the unit being added is of the active faction, allows it to take its turn
	{
		unitsToAct.Add(unit);
	}
}

void ATurnManager::RemoveUnitFromCombat(AUnit *unit)
{
	unitsInCombat.Remove(unit);	//Removes unit from the list of units on the grid

	if (unit->GetFaction() == activeFaction)		//Prevents unit from taking their turn, even if their faction is active
	{
		unitsToAct.Remove(unit);
	}
}

void ATurnManager::CheckForUnitsToAct()
{
	if (unitsToAct.Num() <= 0)
	{
		EndActiveFactionTurn();		//Ends the active faction's turn if there are no more units that can move
	}
	else if (factionMap[activeFaction].isPlayerFaction)		//Checks if the active faction is the player's faction to allow for the player to select another unit
	{
		//Allows player input
	}
	else
	{
		//Finds the next AI unit that can act
	}
}


//Faction Turn
void ATurnManager::BeginFactionTurn(EFaction faction)
{
	activeFaction = faction;	//Sets the new faction as the active faction

	if (GetUnitsInFaction(faction).Num() > 0)	//Checks if there are any units left in the chosen faction
	{
		unitsToAct = GetUnitsInFaction(faction);	//Sets all the units in the new faction as ready to act
		for (int i = 0; i < unitsToAct.Num(); i++)
		{
			//Begins turn of each unit to act
		}

		CheckForUnitsToAct();	//Checks for units that can act
	}
	else
	{
		EndActiveFactionTurn();
	}
}

void ATurnManager::EndActiveFactionTurn()
{
	if (unitsToAct.Num() > 0)
	{
		//Ends the turn of every unit that still needs to act
	}

	TArray<EFaction> factionList;
	factionMap.GenerateKeyArray(factionList);	//Generates an array from the factionMap keys
	if (activeFaction == factionList.Last())	//Checks if the recently active faction is the last one to move
	{
		turn++;	//Increments the turn counter
		BeginFactionTurn(factionList[0]);	//Loops to the beginning of the faction list and starts the first faction's turn
	}
	else
	{
		BeginFactionTurn(factionList[factionList.Find(activeFaction) + 1]);	//Begins the turn of the next faction in the list
	}
}


