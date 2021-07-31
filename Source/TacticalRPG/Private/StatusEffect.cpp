#include "StatusEffect.h"
#include "Unit.h"

UStatusEffect::UStatusEffect()
{
}

UStatusEffect::UStatusEffect(AUnit* unit)
{
}

UStatusEffect::~UStatusEffect()
{
}


//Activation
void UStatusEffect::ActivateStatus()
{
}

void UStatusEffect::RemoveStatus()
{
}


//Application
bool UStatusEffect::canBeApplied(AUnit* unit) const
{
	return true;
}

void UStatusEffect::OnApplied(AUnit* unit)
{
	owningUnit = unit;
}


//Combat
void UStatusEffect::OnTurnStart()
{
}

void UStatusEffect::OnTurnEnd()
{
}

void UStatusEffect::OnUnitInitate(AAbility* ability)
{
}

void UStatusEffect::OnFoeInitiate(AAbility* ability)
{
}

void UStatusEffect::OnEnterCombat()
{
}

void UStatusEffect::OnEndCombat(AAbility* initiatingAbility, bool unitInitiated)
{
}
