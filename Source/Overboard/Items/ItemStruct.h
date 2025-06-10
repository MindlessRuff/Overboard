#pragma once

#include "CoreMinimal.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;;

	FItemData() {}

	FItemData(const FString& InName, const FString& InType, const int32 InQuantity)
		: Name(InName), Type(InType), Quantity(InQuantity) {}
};

