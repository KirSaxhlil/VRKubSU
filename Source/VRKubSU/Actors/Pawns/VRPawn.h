// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "NiagaraComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

#include "VRKubSU/Actors/Pawns/TeleportRing.h"

#include "VRPawn.generated.h"

USTRUCT()
struct FProjectedResult {
	GENERATED_BODY()
public:
	FNavLocation ProjectedLocation;
	bool Return;
};

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
	UNiagaraComponent* Tracer;


	float AxisDeadzone;
	float SnapTurnAngle;
	bool TeleportTracing;
	bool ValidTeleportLocation;
	ATeleportRing* TeleportRingRef;
	TArray<FVector> TeleportTracePathPositions;
	FVector ProjectedTeleportLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAxisGreaterThenDeadzone(float axis_value);

	void SnapTurn(bool RightTurn);

	void InputAxis_Turn(float AxisValue);

	void StartTeleportTrace();

	void EndTeleportTrace();

	void TeleportTrace(FVector Start, FVector ForwardVector);

	FProjectedResult IsValidTeleportLocation(FHitResult Hit);
};
