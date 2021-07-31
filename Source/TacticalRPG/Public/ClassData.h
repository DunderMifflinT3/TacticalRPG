#pragma once

#include "StatData.h"
#include "WeaponData.h"
#include "Engine/DataTable.h"
#include "ClassData.generated.h"

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Trainee			UMETA(DisplayName = "Trainee"),
	LightArmor		UMETA(DisplayName = "Light Armor"),
	MediumArmor		UMETA(DisplayName = "Medium Armor"),
	HeavyArmor		UMETA(DisplayName = "Heavy Armor"),
	Mage			UMETA(DisplayName = "Mage")
};


USTRUCT(BlueprintType)
struct FClassData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText className;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText classDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EClassType> classType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EWeaponType> usableWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, int> statModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStats, float> growthModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementStats moveStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDamageType, int> damageModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName classAbility;


};
