#include "Equipment.h"
#include "Unit.h"

AEquipment::AEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

//Stats
int AEquipment::GetStatModifier(EStats stat) const
{
	return statModifiers[stat];
}

int AEquipment::GetCombatStatModifier(ECombatStats stat) const
{
	return combatStatModifiers[stat];
}

float AEquipment::GetGrowthModifier(EStats stat) const
{
	return growthModifiers[stat];
}

//Abilities
TSet<FName> AEquipment::GetAbilities() const
{
	return abilities;
}


//Equipping
bool AEquipment::isEquipped() const
{
	return equipped;
}

bool AEquipment::canBeEquipped(const AUnit *unit) const
{
	return true;
}

