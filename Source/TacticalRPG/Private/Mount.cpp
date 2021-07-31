#include "Mount.h"
#include "Unit.h"

AMount::AMount()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AMount::AMount(FName mName)
{

}

bool AMount::canBeEquipped(const AUnit *unit) const
{
	//Check if only specific units can equip
	//Check if unit's class can equip
	return true;
}

//Combat
int AMount::GetDamageModifier(EDamageType damageType) const
{
	return 0;
}

//Health
void AMount::ReduceHealth (int damage, bool lethal)
{
	if (lethal)
	{
		currentHealth -= damage;

		if (currentHealth <= 0)
		{
			Death();	//Kills mount if health is reduced to zero
		}
	}
	else
	{
		currentHealth = FMath::Max(currentHealth - damage, 1);	//Non-lethal damage will leave the mount's HP at one if it would have killed
	}
}

void AMount::Death()
{
	Destroy();	//Deletes the mount on death
}