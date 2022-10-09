// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "HAL/IConsoleManager.h"

#include "VRPawn.generated.h"

UCLASS()
class VRKUBSU_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	USceneComponent* DefaultSceneRoot;
	UMotionControllerComponent* MC_Right;
	UMotionControllerComponent* MC_Left;
	UCameraComponent* Camera;
	UWidgetInteractionComponent* WI_Right;
	UWidgetInteractionComponent* WI_Left;

	float AxisDeadzone;
	float SnapTurnAngle;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAxisGreaterThenDeadzone(float axis_value);

	void SnapTurn(bool RightTurn);

	void InputAxis_Turn(float AxisValue);

};
