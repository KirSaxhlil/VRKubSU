// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRKubSU/Actors/Exhibits/ExhibitBase.h"
#include "ExhibitStatic.generated.h"

/**
 * 
 */
UCLASS()
class VRKUBSU_API AExhibitStatic : public AExhibitBase
{
	GENERATED_BODY()
public:
	AExhibitStatic();
	virtual void ShowInfo() override;
	
};
