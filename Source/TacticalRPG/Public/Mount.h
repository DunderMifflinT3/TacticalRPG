#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "TerrainData.h"
#include "ClassData.h"
#include "Engine/DataTable.h"
#include "Mount.generated.h"

USTRUCT(BlueprintType)
struct FMountData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText mountName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText mountDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementStats moveStatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ETerrainType, int> terrainCosts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, int> statModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECombatStats, int> combatStatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, float> growthModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDamageType, int> damageModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<EClassType> validClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool flying = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> unitRestrictions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int cost;
};


UCLASS()
class TACTICALRPG_API AMount : public AEquipment
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMount();
	AMount(FName mountName);

protected:
	int currentHealth;
	FMovementStats moveStatModifiers;
	TMap<ETerrainType, int> terrainCosts;
	bool flying = false;

public:
	//Equipping
	bool canBeEquipped(const AUnit *unit) const;

	//Combat
	UFUNCTION(BlueprintCallable, category = "Combat")
	int GetDamageModifier(EDamageType damageType) const;	//Returns the mount's modifier to the specified damage type

	//Health
	UFUNCTION(BlueprintCallable, category = "Health")
	void ReduceHealth(int damage, bool lethal);

	UFUNCTION(BlueprintCallable, category = "Health")
	void Death();
};

