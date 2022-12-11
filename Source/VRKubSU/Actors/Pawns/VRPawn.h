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
#include "VRKubSU/Components/GrabComponent.h"
#include "VRKubSU/Actors/Exhibits/ExhibitBase.h"
#include "VRKubSU/Interfaces/InteractionInterface.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionControllerComponent* MC_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionControllerComponent* MC_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetInteractionComponent* WI_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetInteractionComponent* WI_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* Tracer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TriggerLeftTracer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* TriggerRightTracer;


	float AxisDeadzone;
	float SnapTurnAngle;
	bool TeleportTracing;
	bool ValidTeleportLocation;
	ATeleportRing* TeleportRingRef;
	TArray<FVector> TeleportTracePathPositions;
	FVector ProjectedTeleportLocation;
	UGrabComponent* HeldComponentLeft;
	UGrabComponent* HeldComponentRight;

	bool DoOnceTurn;
	bool DoOnceTeleport;

	bool DoOnceTriggerLeft;
	bool DoOnceTriggerRight;

	bool TriggerLeftTracing;
	bool TriggerRightTracing;

	UPROPERTY(BlueprintReadOnly)
	AActor* TriggerLeftHit;
	UPROPERTY(BlueprintReadOnly)
	AActor* TriggerRightHit;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAxisGreaterThenDeadzone(float axis_value);

	void SnapTurn(bool RightTurn);

	void InputAxis_Turn(float AxisValue);
	void InputAxis_Teleport(float AxisValue);

	void InputAxis_TriggerLeft(float AxisValue);
	void InputAxis_TriggerRight(float AxisValue);

	void InputAction_GrabLeft_Pressed();
	void InputAction_GrabLeft_Released();
	void InputAction_GrabRight_Pressed();
	void InputAction_GrabRight_Released();

	void StartTeleportTrace();
	void EndTeleportTrace();
	void TeleportTrace(FVector Start, FVector ForwardVector);
	void TryTeleport();

	void TriggerTrace(FVector Start, FVector ForwardVector, AActor*& HitActorContainer, UNiagaraComponent* TracerComponent);

	UGrabComponent* GetGrabComponentNearMotionController(UMotionControllerComponent* MotionController);

	FProjectedResult IsValidTeleportLocation(FHitResult Hit);
};
