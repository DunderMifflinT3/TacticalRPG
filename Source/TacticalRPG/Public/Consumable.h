#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/DataTable.h"
#include "Consumable.generated.h"

class AUnit;

USTRUCT(BlueprintType)
struct FConsumableData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText consumableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText consumableDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UTexture2D* icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName useAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxUses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int cost;
};


UCLASS()
class TACTICALRPG_API AConsumable : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AConsumable();
	AConsumable(FName consumableName);

protected:
	int remainingUses = 0;

public:
	void UseConsumable(AUnit* unit);
};
