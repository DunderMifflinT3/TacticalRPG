#include "Ability.h"
#include "Unit.h"

// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AAbility::AAbility(FName aName, AUnit *unit)
{
	owningUnit = unit;
	gridManager = owningUnit->GetGridManager();
}


//Activation
void AAbility::ExecuteAbility(FGridIndex targetIndex)
{
	owningUnit->UseStamina(GetStaminaCost(), true);		//Uses stamina once the unit begins executing the ability
}

void AAbility::EndExecution()
{
	owningUnit->AddAbilityCooldown(name, cooldown);		//Adds ability to the unit's cooldowns

	Destroy();	//Destroys this ability instance
}

bool AAbility::canActivate()
{
	return (owningUnit->GetCurrentStamina() >= GetStaminaCost()) && (owningUnit->GetAbilityCooldown(name) <= 0);	//Makes sure that unit has the required stamina and that the ability is not on cooldown
}


//Combat
int AAbility::CalculatePower(const AUnit *targetUnit) const
{
	int power = FMath::Max((owningUnit->GetAttack(attackStat) + basePower) - (targetUnit->GetGuard(defendStat)), 0);	//Power is always at least 0
	return power;
}

int AAbility::CalculateHit(const AUnit *targetUnit) const
{
	return (owningUnit->GetAccuracy() + baseAccuracy) - targetUnit->GetEvasion();
}

int AAbility::CalculateCritical(const AUnit *targetUnit) const
{
	if (canCritical)
	{
		return (owningUnit->GetCritical() + baseCritical) - targetUnit->GetCriticalAvoid();
	}
	else
	{
		return 0;	//Ability cannot crit
	}
}

int AAbility::GetStaminaCost() const
{
	return baseStaminaCost;
}


//Range
TArray<FGridIndex> AAbility::GetIndicesInRange(FGridIndex startIndex) const
{
	TArray<FGridIndex> inRange;
	return inRange;
}

TArray<FGridIndex> AAbility::GetConnectedIndices(FGridIndex unitIndex, FGridIndex targetIndex) const
{
	TArray<FGridIndex> connectedIndices{ targetIndex };		//Returns only the target index for a single target ability
	return connectedIndices;
}

bool AAbility::isValidTarget(FGridIndex targetIndex) const
{
	return indicesInRange.Contains(targetIndex);	//Index is valid if it is within range
}

