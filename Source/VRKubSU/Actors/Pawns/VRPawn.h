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
	UPROPERTY(BlueprintReadOnly, Category = "System|Teleporting")
	bool TeleportTracing;
	UPROPERTY(BlueprintReadOnly, Category = "System|Teleporting")
	bool ValidTeleportLocation;
	UPROPERTY(BlueprintReadOnly, Category = "System|Teleporting")
	ATeleportRing* TeleportRingRef;
	TArray<FVector> TeleportTracePathPositions;
	UPROPERTY(BlueprintReadOnly, Category = "System|Teleporting")
	FVector ProjectedTeleportLocation;

	bool DoOnceTurn;
	bool DoOnceTeleport;

	bool DoOnceTriggerLeft;
	bool DoOnceTriggerRight;

	UPROPERTY(BlueprintReadOnly, Category = "System|Interacting")
	bool TriggerLeftTracing;
	UPROPERTY(BlueprintReadOnly, Category = "System|Interacting")
	bool TriggerRightTracing;

	UPROPERTY(BlueprintReadOnly, Category = "System|Interacting")
	AActor* TriggerLeftHit;
	UPROPERTY(BlueprintReadOnly, Category = "System|Interacting")
	AActor* TriggerRightHit;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	FProjectedResult IsValidTeleportLocation(FHitResult Hit);

public:	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Components")
	UMotionControllerComponent* MC_Right;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "Components")
	UMotionControllerComponent* MC_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWidgetInteractionComponent* WI_Right;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWidgetInteractionComponent* WI_Left;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* Tracer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* TriggerLeftTracer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* TriggerRightTracer;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls settings")
	float AxisDeadzone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls settings")
	float SnapTurnAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controls settings")
	float InteractionDistance;
	
	UPROPERTY(BlueprintReadOnly, Category = "System|Grabbing")
	UGrabComponent* HeldComponentLeft;
	UPROPERTY(BlueprintReadOnly, Category = "System|Grabbing")
	UGrabComponent* HeldComponentRight;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UGrabComponent* GetGrabComponentNearMotionController(UMotionControllerComponent* MotionController);
};
