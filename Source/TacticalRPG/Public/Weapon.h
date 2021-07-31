#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "WeaponData.h"
#include "Engine/DataTable.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText weaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText weaponDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStats attackStat = EStats::Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStats defendStat = EStats::Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatStats, int> combatStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float criticalModifier = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int weight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, int> statModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, float> growthModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool useWithShield = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool useWithMount = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName attackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName counterAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> unitRestrictions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int cost;
};


UCLASS()
class TACTICALRPG_API AWeapon : public AEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	AWeapon(FName weaponName);

protected:
	EWeaponType weaponType;
	EDamageType damageType;
	EStats attackStat = EStats::Strength;	//Stat that this weapon uses to deal damage
	EStats defendStat = EStats::Defense;	//Stat that this weapon is blocked by
	FRange range;
	int weight = 0;
	float criticalModifier = 2;

public:
	//Equipping
	bool canBeEquipped(const AUnit *unit) const;
};
