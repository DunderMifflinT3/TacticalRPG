#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StatData.h"
#include "Equipment.generated.h"

class AUnit;
class AAbility;

UCLASS()
class TACTICALRPG_API AEquipment : public AItem
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AEquipment();

protected:
	bool equipped = false;
	TMap<EStats, int> statModifiers;
	TMap<ECombatStats, int> combatStatModifiers;
	TMap<EStats, float> growthModifiers;
	TSet<FName> abilities;

public:	
	//Stats
	UFUNCTION(BlueprintCallable, category = "Stats")
	int GetStatModifier(EStats stat) const;	//Returns the base stat modifier for the specified stat

	UFUNCTION(BlueprintCallable, category = "Stats")
	int GetCombatStatModifier(ECombatStats stat) const;	//Returns the modifier for the specified combat stat

	UFUNCTION(BlueprintCallable, category = "Stats")
	float GetGrowthModifier(EStats stat) const;	//Returns the growth modifier of the specified stat


	//Abilities
	UFUNCTION(BlueprintCallable, category = "Abilities")
	TSet<FName> GetAbilities() const;	//Returns any abilities that the item grants to the user


	//Equipping
	UFUNCTION(BlueprintCallable, category = "Equipping")
	bool isEquipped() const; 	//Checks if the item is equipped to a unit

	UFUNCTION(BlueprintCallable, category = "Equipping")
	virtual bool canBeEquipped(const AUnit *unit) const;	//Checks if a unit can equip this item
};
