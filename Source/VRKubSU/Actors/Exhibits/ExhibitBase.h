// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

#include "VRKubSU/Data/ExhibitDataAsset.h"
#include "VRKubSU/Interfaces/InteractionInterface.h"
#include "VRKubSU/Actors/Pawns/VRPawn.h"

#include "ExhibitBase.generated.h"

UCLASS()
class VRKUBSU_API AExhibitBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExhibitBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "System")
	FVector WidgetScale;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* WidgetTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* WidgetMain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	UExhibitDataAsset* Data;

	UPROPERTY(BlueprintReadWrite, Category = "System")
	bool InfoShowed;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInfo(EUMGSequencePlayMode::Type State);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDistanceToCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
