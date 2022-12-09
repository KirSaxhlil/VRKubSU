// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

#include "VRKubSU/Data/ExhibitDataAsset.h"
#include "VRKubSU/Interfaces/InteractionInterface.h"

#include "ExhibitBase.generated.h"

UCLASS()
class VRKUBSU_API AExhibitBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExhibitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetComponent* WidgetTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetComponent* WidgetMain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UExhibitDataAsset* Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector WidgetScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InfoShowed;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	/*virtual*/ void ShowInfo(EUMGSequencePlayMode::Type State);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
