#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FactionData.h"
#include "Unit.h"
#include "TurnManager.generated.h"

USTRUCT()
struct FFactionCombat	//Contains the units in a faction and info on whether the faction is player controlled or not
{
	GENERATED_BODY()

	AUnit *factionLeader;
	TArray<AUnit*> unitsInFaction;
	bool isPlayerFaction = false;
};


UCLASS()
class TACTICALRPG_API ATurnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurnManager();

protected:
	//Turns
	int turn = 1;

	//Faction
	EFaction activeFaction;		//Faction that is currently moving
	TMap<EFaction, FFactionCombat> factionMap;	//List of all factions in combat, as well as their respective units

	//Units
	TArray<AUnit*> unitsInCombat;	//List of all units in combat

	TArray<AUnit*> unitsToAct;	//List of all units in the active faction

public:	
	//Units
	UFUNCTION(BlueprintCallable, category = "Units")
	TArray<AUnit*> GetUnitsInFaction(EFaction faction) const;	//Gets all of the units in a faction

	UFUNCTION(BlueprintCallable, category = "Units")
	void UpdateUnitFaction(AUnit *unit, EFaction newFaction);	//Updates a unit's faction and allows it to act if it switched to the active faction

	UFUNCTION(BlueprintCallable, category = "Units")
	void AddUnitToCombat(AUnit *unit);	//Adds a unit to combat, allowing them to activate

	UFUNCTION(BlueprintCallable, category = "Units")
	void RemoveUnitFromCombat(AUnit *unit);		//Removes a unit from combat so that they can no longer activate

	UFUNCTION(BlueprintCallable, category = "Units")
	void CheckForUnitsToAct();		//Checks if any unit can act and activates them


	//Faction Turn
	UFUNCTION(BlueprintCallable, category = "Faction Turn")
	void BeginFactionTurn(EFaction faction);	//Begins the chosen faction's turn by preparing that faction's units to activate

	UFUNCTION(BlueprintCallable, category = "Faction Turn")	//Ends the active faction's turn by ending the turn of all units in that faction and finding the next faction to move
	void EndActiveFactionTurn();
};
