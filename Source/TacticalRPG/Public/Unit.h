#pragma once

#include "CoreMinimal.h"
#include "GridActor.h"
#include "FactionData.h"
#include "ClassData.h"
#include "StatData.h"
#include "TerrainData.h"
#include "WeaponData.h"
#include "Engine/DataTable.h"
#include "Unit.generated.h"

class AUnitController;
class ATurnManager;
class AAbility;
class AEquipment;
class AWeapon;
class AMount;
class UStatusEffect;

USTRUCT(BlueprintType)
struct FUnitData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText unitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName unitClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFaction faction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int baseLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, int> baseStats = { {EStats::Health, 0}, {EStats::Stamina, 0}, {EStats::Strength, 0}, {EStats::Mind, 0}, {EStats::Defense, 0}, {EStats::Resistance, 0}, {EStats::Agility, 0}, {EStats::Dexterity, 0} };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, float> baseGrowths = { {EStats::Health, 0}, {EStats::Stamina, 0}, {EStats::Strength, 0}, {EStats::Mind, 0}, {EStats::Defense, 0}, {EStats::Resistance, 0}, {EStats::Agility, 0}, {EStats::Dexterity, 0} };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName mainWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName secondWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName shield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName mount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool useClassDefaults = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> abilities;
};


UCLASS()
class TACTICALRPG_API AUnit : public AGridActor
{
	GENERATED_BODY()

protected:
	FName name;

	AUnitController* unitController;

	ATurnManager* turnManager;

	//Attributes
	FText unitName;
	EFaction faction;

	//Class
	FName unitClass;
	FClassData classData;
	TMap<FName, int> classLevels;

	//Stats
	TMap<EStats, int> baseStats;

	//Growth
	int level = 1;
	int currentExperience = 0;
	TMap<EStats, float> baseGrowths;
	TMap<EStats, float> currentGrowthExperience;

	//Status
	int deathCount = 0;
	int currentHealth = 0;
	int currentStamina = 0;

	//Inventory
	AWeapon* mainWeapon;
	AWeapon* secondWeapon;
	AWeapon* shield;
	AMount* mount;
	bool weaponEquipped = false;
	bool shieldEquipped = false;
	bool mountEquipped = false;

	//Abilities
	TSet<FName> abilities;	//List of the unit's abilities
	TMap<FName, int> abilityCooldowns;	//Map of abilities that cannot be used due to being on cooldown


	//Status Effects
	TSet<UStatusEffect*> statusEffects;	//List of all status effects on the unit

public:
	//Sets default values for this actor's properties
	AUnit();
	AUnit(FName unitName);

	//Attributes
	UFUNCTION(BlueprintCallable, category = "Attributes")
	EFaction GetFaction() const;	//Returns a unit's faction

	UFUNCTION(BlueprintCallable, category = "Attributes")
	void SetFaction(EFaction newFaction);	//Updates a unit's faction

	//Class
	UFUNCTION(BlueprintCallable, category = "Class")
	FClassData GetClassData() const;	//Returns the data associated with the unit's class

	UFUNCTION(BlueprintCallable, category = "Class")
	void SetClass(FName className);		//Updates a unit's class and all the data that goes along with it	


	//Stats
	UFUNCTION(BlueprintCallable, category = "Stats")
	int GetBaseStat(EStats stat) const;		//Gets a unit's base stat value, including the unit's base stats and class base stats

	UFUNCTION(BlueprintCallable, category = "Stats")
	int GetStatValue(EStats stat) const;		//Gets the value of a stat, including any modifiers


	//CombatStats
	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetCombatStatModifier(ECombatStats stat) const;		//Gets all modifiers to the specified combat stat

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetAttack(EStats attackStat) const;		//Gets a unit's attack, or their ability to do damage

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetGuard(EStats defendStat) const;		//Gets a unit's guard, or their ability to block damage

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetAccuracy() const;		//Gets a unit's accuracy, or their ability to hit their target

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetEvasion() const;		//Gets a unit's evasion, or their ability to dodge attacks

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetCritical() const;		//Gets a unit's critical, or the likelihood that the unit will perform a critical hit

	UFUNCTION(BlueprintCallable, category = "Combat Stats")
	int GetCriticalAvoid() const;		//Gets a unit's critical avoid, or their ability to prevent incoming critical hits


	//Growth
	UFUNCTION(BlueprintCallable, category = "Growth")
	bool canReceiveExperience() const;	//Determines if the unit is able to receieve experience points and level up

	UFUNCTION(BlueprintCallable, category = "Growth")
	float GetStatGrowth(EStats stat) const;	//Gets the total growth rate, including any modifiers from class or equipment

	UFUNCTION(BlueprintCallable, category = "Growth")
	void GainExperience(int expGained);		//Updates current experience, updates growth experience, and decides if the unit should level up

	UFUNCTION(BlueprintCallable, category = "Growth")
	void LevelUp();	//Levels up the unit once their current experience reaches 100

	UFUNCTION(BlueprintCallable, category = "Growth")
	void ApplyGrowthExperience(int expGained);	//Gets the growth experience modifiers from all sources and multiplies that value by the experience gained


	//Movement
	UFUNCTION(BlueprintCallable, category = "Movement")
	FMovementStats GetMovementStats() const;		//Gets unit's move and jump stats

	UFUNCTION(BlueprintCallable, category = "Movement")
	int GetTerrainCost(ETerrainType terrain) const;		//Gets the cost for the unit to move through the specified terrain type

	UFUNCTION(BlueprintCallable, category = "Movement")
	bool canMoveThroughIndex(FGridIndex targetIndex) const;		//Checks if an index is blocked by an obstacle or another unit of a different faction

	UFUNCTION(BlueprintCallable, category = "Movement")
	void EnterIndex(FGridIndex newIndex);	//Adds a unit to an index

	UFUNCTION(BlueprintCallable, category = "Movement")
	void ExitIndex(FGridIndex oldIndex);	//Removes a unit from an index


	//Health
	UFUNCTION(BlueprintCallable, category = "Health")
	int GetCurrentHealth();

	UFUNCTION(BlueprintCallable, category = "Health")
	void ReduceHealth(int damage, bool lethal);	//Subtracts the specified damage from current health and checks if the unit should die

	UFUNCTION(BlueprintCallable, category = "Health")
	void Death();	//Kills the unit and removes them from combat


	//Stamina
	UFUNCTION(BlueprintCallable, category = "Stamina")
	int GetCurrentStamina();

	UFUNCTION(BlueprintCallable, category = "Stamina")
	void UseStamina(int stamina, bool exhaust);		//Subtracts the specified stamina from current stamina and checks if the unit should be exhausted

	UFUNCTION(BlueprintCallable, category = "Stamina")
	void Exhaust();		//Ends the unit's turn and prevents them from moving for a turn


	//Inventory
	UFUNCTION(BlueprintCallable, category = "Inventory")
	void InitializeInventory(FName mainWeapon, FName secondWeapon, FName shield, FName item, FName mount);	//Initializes inventory by spawning the specified equipment and using data tables

	//The following functions add items to their designated inventory slots
	UFUNCTION(BlueprintCallable, category = "Inventory")
	void AddMainWeapon(AWeapon *newWeapon);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void AddSecondWeapon(AWeapon* newWeapon);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void AddShield(AWeapon* newShield);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void AddMount(AMount* newMount);

	//The following functions remove items from their designated inventory slots
	UFUNCTION(BlueprintCallable, category = "Inventory")
	void RemoveMainWeapon();

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void RemoveSecondWeapon();

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void RemoveShield();

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void RemoveMount();

	//The following functions toggle whether an item is equipped or not
	UFUNCTION(BlueprintCallable, category = "Inventory")
	void ToggleWeaponEquipped(bool equipped);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void ToggleShieldEquipped(bool equipped);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	void ToggleMounted(bool equipped);

	UFUNCTION(BlueprintCallable, category = "Inventory")
	TArray<AEquipment*> GetEquippedItems() const;	//Gets all of a unit's items that are currently equipped

	UFUNCTION(BlueprintCallable, category = "Inventory")
	TSet<FName> GetInventoryAbilities() const;	//Gets all abilities from all equipped items


	//Abilities
	UFUNCTION(BlueprintCallable, category = "Inventory")
	TSet<FName> GetAbilities() const;	//Gets all abilities that a unit can perform

	UFUNCTION(BlueprintCallable, category = "Abilities")
	void AddAbilityCooldown(FName ability, int cooldown);		//Adds ability to the cooldown map based on its name

	UFUNCTION(BlueprintCallable, category = "Abilities")
	void ReduceAbilityCooldowns(int reductionAmount);		//Reduces all ability cooldowns by the specified amount

	UFUNCTION(BlueprintCallable, category = "Abilities")
	int GetAbilityCooldown(FName ability);


	//Initiative
	UFUNCTION(BlueprintCallable, category = "Initiative")
	bool canAct() const;	//Checks if a unit can be activated by checking if they are exhausted

	UFUNCTION(BlueprintCallable, category = "Initiative")
	bool isAiControlled() const;	//Checks if a unit is controlled by the AI


	//Status Effects
	UFUNCTION(BlueprintCallable, category = "Status Effects")
	void AddStatusEffect(UStatusEffect* status);		//Adds a status effect to the unit

	UFUNCTION(BlueprintCallable, category = "Status Effects")
	TSet<UStatusEffect*> GetStatusEffects();	//Returns all of the status effects currently on the unit


	bool operator==(const AUnit& otherUnit) const
	{
		return (name == otherUnit.name);
	}
};
