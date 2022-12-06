// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "GrabComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRKUBSU_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool SimulateOnDrop;
	bool IsHeld;
	UMotionControllerComponent* MotionControllerRef;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetShouldSimulateOnDrop();

	bool TryGrab(UMotionControllerComponent* MotionController);

	bool TryRelease();

	void SetPrimitiveComponentPhysics(bool Simulate);

	bool AttachParentToMotionController(UMotionControllerComponent* MotionController);
};
