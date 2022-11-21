

#pragma once

#include "CoreMinimal.h"
#include "VRKubSU/Actors/Exhibits/ExhibitBase.h"



#include "ExhibitCup.generated.h"

/**
 * 
 */

UCLASS()
class VRKUBSU_API AExhibitCup : public AExhibitBase
{
	GENERATED_BODY()
public:
	AExhibitCup();

	virtual void ShowInfo() override;

	void ReturnPosition();
};
