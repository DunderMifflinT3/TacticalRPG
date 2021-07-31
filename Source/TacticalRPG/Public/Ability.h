#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridIndex.h"
#include "StatData.h"
#include "WeaponData.h"
#include "Engine/DataTable.h"
#include "Ability.generated.h"

class AUnit;
class AGridManager;

USTRUCT(BlueprintType)
struct FAbilityData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText abilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText abilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStats attackStat = EStats::Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStats defendStat = EStats::Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int basePower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int baseAccuracy = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int baseCritical = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canCritical = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float criticalModifier = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int baseStaminaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int cooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool targetFoes = true ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool targetAllies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool targetSelf = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool targetStructures = false;
};


UCLASS()
class TACTICALRPG_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();
	AAbility(FName abilityName, AUnit *unit);

protected:
	FName name;

	AUnit* owningUnit;	//The unit that is using this ability
	AGridManager* gridManager;	//reference to the grid manager

	//Attributes
	FText abilityName;
	FText abilityDescription;
	class UTexture2D* icon;
	EDamageType damageType;
	EStats attackStat;
	EStats defendStat;
	int basePower = 0;
	int baseAccuracy = 0;
	int baseCritical = 0;
	bool canCritical = true;
	float criticalModifier = 2;
	int baseStaminaCost = 0;
	int cooldown = 0;

	//Range
	FRange range;
	TArray<FGridIndex> indicesInRange;

	//Targeting
	bool targetFoes = true;
	bool targetAllies = false;
	bool targetSelf = false;
	bool targetStructures = true;

public:	
	//Activation
	UFUNCTION(BlueprintCallable, category = "Activation")
	virtual void ExecuteAbility(FGridIndex targetIndex);

	UFUNCTION(BlueprintCallable, category = "Activation")
	virtual void EndExecution();

	UFUNCTION(BlueprintCallable, category = "Activation")
	virtual bool canActivate();


	//Combat
	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual int CalculatePower(const AUnit *unit) const;		//Calculates the total damage or healing of an ability on a target

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual int CalculateHit(const AUnit *unit) const;		//Calculates the chance to hit the target

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual int CalculateCritical(const AUnit *unit) const;		//Calculates the chance to perform a critical hit on the target

	UFUNCTION(BlueprintCallable, category = "Combat")
	int GetStaminaCost() const;		//Calculates the stamina cost to use the ability


	//Range
	UFUNCTION(BlueprintCallable, category = "Range")
	virtual TArray<FGridIndex> GetIndicesInRange(FGridIndex startIndex) const;	//Gets the indexes in range starting from the specified index

	UFUNCTION(BlueprintCallable, category = "Range")
	virtual TArray<FGridIndex> GetConnectedIndices(FGridIndex unitIndex, FGridIndex targetIndex) const;		//Gets indices connected to the target index in relation to the location of the user

	UFUNCTION(BlueprintCallable, category = "Range")
	virtual bool isValidTarget(FGridIndex targetIndex) const;	//Returns whether an index can be targeted or not
};
