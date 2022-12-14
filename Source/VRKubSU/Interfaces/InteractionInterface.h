#pragma once

//#include "CoreMinimal.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractionInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Grabbed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Ungrabbed();
};