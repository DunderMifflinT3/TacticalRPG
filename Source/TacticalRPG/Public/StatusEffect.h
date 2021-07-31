#pragma once

#include "CoreMinimal.h"
#include "StatusEffect.generated.h"

class AUnit;
class AAbility;

UCLASS()
class TACTICALRPG_API UStatusEffect :public UObject
{
	GENERATED_BODY()

protected:
	FName name;

	AUnit* owningUnit;

	//Attributes
	FText statusName;
	FText statusDescription;
	class UTexture2D* icon;

public:
	UStatusEffect();
	UStatusEffect(AUnit* unit);
	~UStatusEffect();

	//Activation
	UFUNCTION(BlueprintCallable, category = "Activation")
	virtual void ActivateStatus();

	UFUNCTION(BlueprintCallable, category = "Activation")
	virtual void RemoveStatus();


	//Application
	UFUNCTION(BlueprintCallable, category = "Application")
	virtual bool canBeApplied(AUnit* unit) const;

	UFUNCTION(BlueprintCallable, category = "Application")
	virtual void OnApplied(AUnit* unit);


	//Combat
	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnTurnStart();

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnTurnEnd();

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnUnitInitate(AAbility* ability);

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnFoeInitiate(AAbility* ability);

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnEnterCombat();

	UFUNCTION(BlueprintCallable, category = "Combat")
	virtual void OnEndCombat(AAbility* initiatingAbility, bool unitInitiated);


	bool operator==(const UStatusEffect& otherStatus) const
	{
		return (name == otherStatus.name);
	}
};
