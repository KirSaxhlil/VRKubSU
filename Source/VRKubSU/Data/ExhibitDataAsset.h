// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ExhibitDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VRKUBSU_API UExhibitDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;
	FText Name;
	FText Description;
	// Image
	// Audio
	
};
