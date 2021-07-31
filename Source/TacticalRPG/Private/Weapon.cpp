#include "Weapon.h"
#include "Unit.h"

AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AWeapon::AWeapon(FName wName)
{

}

bool AWeapon::canBeEquipped(const AUnit *unit) const
{
	return unit->GetClassData().usableWeapons.Contains(weaponType);	//Checks if a unit's class allows for the use of this weapon type
}
