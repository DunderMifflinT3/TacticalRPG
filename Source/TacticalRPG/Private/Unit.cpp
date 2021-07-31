#include "Unit.h"
#include "GridManager.h"
#include "TurnManager.h"
#include "UnitController.h"
#include "Equipment.h"
#include "Weapon.h"
#include "Mount.h"
#include "StatusEffect.h"

// Sets default values
AUnit::AUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AUnit::AUnit(FName uName)
{

}


//Attributes
EFaction AUnit::GetFaction() const
{
	return faction;
}

void AUnit::SetFaction(EFaction newFaction)
{
	faction = newFaction;
}


//Class
FClassData AUnit::GetClassData() const
{
	return classData;
}

void AUnit::SetClass(FName className)
{

}


//Stats
int AUnit::GetBaseStat(EStats stat) const
{
	return baseStats[stat];
}

int AUnit::GetStatValue(EStats stat) const
{
	int statValue = GetBaseStat(stat);

	return statValue;	//Adds the base stat to all of that stat's modifiers to get the true stat value
}


//CombatStats
int AUnit::GetCombatStatModifier(ECombatStats stat) const
{
	int modifier = 0;

	return modifier;
}

int AUnit::GetAttack(EStats attackStat) const
{
	return GetStatValue(attackStat);
}

int AUnit::GetGuard(EStats defendStat) const
{
	return GetStatValue(defendStat);
}

int AUnit::GetAccuracy() const
{
	return GetStatValue(EStats::Dexterity) + GetCombatStatModifier(ECombatStats::Accuracy);
}

int AUnit::GetEvasion() const
{
	return GetStatValue(EStats::Agility) + GetCombatStatModifier(ECombatStats::Evasion);
}

int AUnit::GetCritical() const
{
	return (GetStatValue(EStats::Dexterity) / 2) + GetCombatStatModifier(ECombatStats::Critical);
}

int AUnit::GetCriticalAvoid() const
{
	return (GetStatValue(EStats::Dexterity) / 2) + GetCombatStatModifier(ECombatStats::CriticalAvoid);
}


//Growth
bool AUnit::canReceiveExperience() const
{
	return (!isAiControlled() && level < 50); //Unit can only gain exp if they are controlled by the player and are below the max level
}

float AUnit::GetStatGrowth(EStats stat) const
{
	return baseGrowths[stat];
}

void AUnit::GainExperience(int expGained)
{
	if (currentExperience + expGained >= 100)
	{	
		LevelUp();				//Unit levels up if their current experience would get to 100
		currentExperience = (currentExperience + expGained) - 100;		//new current experince is reset back to zero after level up and continues to increase based on the leftover gains
	}
	else
	{
		currentExperience += expGained;		//Adds experience gained to the current experience
	}
}

void AUnit::LevelUp()
{
	level++;

	TArray<EStats> stats;
	baseStats.GenerateKeyArray(stats);	//Creates an array of all stats

	for (int i = 0; i < stats.Num(); i++)
	{
		int statGain = currentGrowthExperience[stats[i]] / 100;		//Stat increases for every 100 growth exp points
		currentGrowthExperience.Add(stats[i], currentGrowthExperience[stats[i]] - (statGain * 100));	//Removes growth experience that was used

		baseStats.Add(stats[i], baseStats[stats[i]] + statGain);	//Adds the stat gain to the base stat
	}

	classLevels.Add(unitClass, classLevels[unitClass] + 1);		//Increases the level of the unit's current class
}

void AUnit::ApplyGrowthExperience(int expGained)
{
	TArray<EStats> stats;
	currentGrowthExperience.GenerateKeyArray(stats);	//Creates an array of all stats

	for (int i = 0; i < stats.Num(); i++)	//Loops through each stat and updates the growth exp based on the experience that was gained
	{
		float gain = GetStatGrowth(stats[i]) * expGained;
		currentGrowthExperience.Add(stats[i], fmax(currentGrowthExperience[stats[i]] + gain, 0));	//Updates the unit's current growth experience for a stat by adding the exp gained
	}
}


//Movement
FMovementStats AUnit::GetMovementStats() const
{
	if (false)	//Check for mount
	{
		return { 0,0 };
	}
	else //Return class movement stats
	{
		return { 0,0 };
	}
}

int AUnit::GetTerrainCost(ETerrainType) const
{
	if (false)	//Check for mount
	{
		return 1;
	}
	else //Return class terrain costs
	{
		return 1;
	}
}

bool AUnit::canMoveThroughIndex(FGridIndex targetIndex) const
{
	return !(gridManager->isIndexOccupied(targetIndex));
}

void AUnit::EnterIndex(FGridIndex newIndex)
{
	if (!gridManager->isIndexOccupied(newIndex))
	{
		gridManager->AddIndexUnit(newIndex, this);		//Adds this unit to the new index
	}
	else
	{
		//Make other unit move over
	}

	index = newIndex;	//Sets the unit's new index
	//Activate any move over events
}

void AUnit::ExitIndex(FGridIndex oldIndex)
{
	if (gridManager->GetIndexUnit(oldIndex) == this)	//Checks if the unit occupying this index is not this unit
	{
		gridManager->RemoveIndexUnit(oldIndex);		//Removes self from the index
	}
	//If another unit is occupying, and this unit is just passing through, does not remove the other unit from the grid
}


//Health
int AUnit::GetCurrentHealth()
{
	return currentHealth;
}

void AUnit::ReduceHealth(int damage, bool lethal)
{
	if (mountEquipped)
	{
		mount->ReduceHealth(damage, lethal);	//Damages mount and processes its potential death before unit is damaged

		//TODO: Unequip mount if it died
	}

	if (lethal)
	{
		currentHealth -= damage;
		if (currentHealth <= 0)
		{
			Death();	//Kills unit if health is reduced to zero
		}
	}
	else
	{
		currentHealth = FMath::Max(currentHealth - damage, 1);	//Non-lethal damage will leave the unit's HP at one if it would have killed
	}
}

void AUnit::Death()
{
	deathCount++;	//Increments this unit's death count

	//Send unit's inventory to the player's inventory

	gridManager->RemoveIndexUnit(GetCurrentIndex());	//Removes the unit from the grid
	turnManager->RemoveUnitFromCombat(this);	//Removes the unit from combat by removing them from the turn manager
	unitController->Destroy();	//Destroys the controller attached to this unit
}


//Stamina
int AUnit::GetCurrentStamina()
{
	return currentStamina;
}

void AUnit::UseStamina(int stamina, bool exhaust)
{
	if (exhaust)
	{
		currentStamina -= stamina;
		if (currentStamina <= 0)
		{
			Exhaust();	//Exhausts unit if stamina is reduced to zero
		}
	}
	else
	{
		currentStamina = FMath::Max(currentStamina - stamina, 1);	//If using stamina does not exhaust the unit, leaves their stamina at one if it would have
	}
}

void AUnit::Exhaust()
{
	//Ends unit's turn and applies the exhaust status for one turn
}


//Inventory
void AUnit::InitializeInventory(FName mWeapon, FName sWeapon, FName s, FName m, FName i)
{
	if (mWeapon != "None")	//Main weapon
	{
		AWeapon newMainWeapon(mWeapon);
		AddMainWeapon(&newMainWeapon);
	}
	if (sWeapon != "None")	//Second weapon
	{
		AWeapon newSecondWeapon(sWeapon);
		AddSecondWeapon(&newSecondWeapon);
	}
	if (s != "None")	//Shield
	{
		AWeapon newShield(s);
		AddShield(&newShield);
	}
	if (m != "None")	//Mount
	{
		AMount newMount(m);
		AddMount(&newMount);
	}
	if (i != "None")	//Item
	{
	}
}

void AUnit::AddMainWeapon(AWeapon* newWeapon)
{
	if (mainWeapon != NULL)
	{
		RemoveMainWeapon();
	}

	mainWeapon = newWeapon;
	ToggleWeaponEquipped(true);
}

void AUnit::AddSecondWeapon(AWeapon* newWeapon)
{
	if (secondWeapon != NULL)
	{
		RemoveSecondWeapon();
	}

	secondWeapon = newWeapon;
}

void AUnit::AddShield(AWeapon* newShield)
{
	if (shield != NULL)
	{
		RemoveShield();
	}

	shield = newShield;
	ToggleShieldEquipped(true);
}

void AUnit::AddMount(AMount* newMount)
{
	if (mount != NULL)
	{
		RemoveMount();
	}

	mount = newMount;
	ToggleMounted(true);
}

void AUnit::RemoveMainWeapon()
{
	mainWeapon = NULL;
	ToggleWeaponEquipped(false);
}

void AUnit::RemoveSecondWeapon()
{
	secondWeapon = NULL;
}

void AUnit::RemoveShield()
{
	shield = NULL;
	ToggleShieldEquipped(false);
}

void AUnit::RemoveMount()
{
	mount = NULL;
	ToggleMounted(false);
}

void AUnit::ToggleWeaponEquipped(bool equipped)
{
	if (equipped)
	{
		weaponEquipped = mainWeapon->canBeEquipped(this);
	}
	else
	{
		weaponEquipped = equipped;
	}
}

void AUnit::ToggleShieldEquipped(bool equipped)
{
	if (equipped)
	{
		shieldEquipped = shield->canBeEquipped(this);
	}
	else
	{
		shieldEquipped = equipped;
	}
}

void AUnit::ToggleMounted(bool equipped)
{
	if (equipped)
	{
		mountEquipped = mount->canBeEquipped(this);
	}
	else
	{
		mountEquipped = equipped;
	}

}

TArray<AEquipment*> AUnit::GetEquippedItems() const
{
	TArray<AEquipment*> equippedItems;
	
	//Checks which items are equipped and adds them to equippedItems if they are
	if (weaponEquipped)
	{
		equippedItems.Add(mainWeapon);
	}
	if (shieldEquipped)
	{
		equippedItems.Add(shield);
	}
	if (mountEquipped)
	{
		equippedItems.Add(mount);
	}

	return equippedItems;
}

TSet<FName> AUnit::GetInventoryAbilities() const
{
	TArray<AEquipment*> equippedItems = GetEquippedItems();		//Gets a list of all equipped items
	TSet<FName> inventoryAbilities;	//List of all abilities in the unit's inventory

	for (int i = 0; i < equippedItems.Num(); i++)
	{
		inventoryAbilities.Append(equippedItems[i]->GetAbilities());	//Adds the abilities of the current item into inventoryAbilities
	}

	return inventoryAbilities;
}


//Abilities
TSet<FName> AUnit::GetAbilities() const
{
	TSet<FName> unitAbilities;

	unitAbilities.Append(GetInventoryAbilities());		//Adds inventory abilities to the list of abilities

	return unitAbilities;
}

void AUnit::AddAbilityCooldown(FName ability, int cooldown)
{
	abilityCooldowns.Add(ability, cooldown);
}

void AUnit::ReduceAbilityCooldowns(int reductionAmount)
{
	TArray<FName> cooldownList;
	abilityCooldowns.GenerateKeyArray(cooldownList);	//Turns the abilities in the cooldown map into an array

	for (int i = 0; i < cooldownList.Num(); i++)
	{
		int newCooldown = (abilityCooldowns[cooldownList[i]]) - reductionAmount;

		if (newCooldown <= 0)		//If the cooldown will be set to 0, removes the ability from the cooldown map
		{
			abilityCooldowns.Remove(cooldownList[i]);
		}
		else
		{
			abilityCooldowns.Add(cooldownList[i], newCooldown);		//Sets the new cooldown for the ability
		}
	}
}

int AUnit::GetAbilityCooldown(FName ability)
{
	return abilityCooldowns[ability];
}

//Initiative
bool AUnit::canAct() const
{
	return true;
}

bool AUnit::isAiControlled() const
{
	return false;
}


//Status Effects
void AUnit::AddStatusEffect(UStatusEffect* status)
{
	if (status->canBeApplied(this))		//Checks if the unit can have the status applied to them
	{
		statusEffects.Add(status);
		status->OnApplied(this);
	}
}

TSet<UStatusEffect*> AUnit::GetStatusEffects()
{
	return statusEffects;
}