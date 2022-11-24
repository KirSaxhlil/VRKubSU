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
	void Interact();
	void Grabbed();
};