// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/WidgetComponent.h"

#include "VRKubSU/Data/ExhibitDataAsset.h"

#include "ExhibitBase.generated.h"

UCLASS()
class VRKUBSU_API AExhibitBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExhibitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	USceneComponent* DefaultSceneRoot;
	UStaticMeshComponent* Mesh;
	UWidgetComponent* WidgetTitle;
	UWidgetComponent* WidgetMain;

	UExhibitDataAsset* Data;

	void ShowInfo();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
