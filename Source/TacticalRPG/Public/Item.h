#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AUnit;

UCLASS()
class TACTICALRPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	FName name;
	FText itemName;
	FText itemDescription;

	int cost = 0;
};
