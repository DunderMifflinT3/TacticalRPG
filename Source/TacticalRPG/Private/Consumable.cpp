#include "Consumable.h"

AConsumable::AConsumable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AConsumable::AConsumable(FName cName)
{

}

void AConsumable::UseConsumable(AUnit* unit)
{

}