// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Pawns/VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = DefaultSceneRoot;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);

	MC_Right = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("M2C_Right"));
	MC_Right->bDisplayDeviceModel = true;
	MC_Right->MotionSource = "Right";
	MC_Right->SetupAttachment(DefaultSceneRoot);

	MC_Left = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MC_Left"));
	MC_Left->bDisplayDeviceModel = true;
	MC_Left->MotionSource = "Left";
	MC_Left->SetupAttachment(DefaultSceneRoot);

	WI_Right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WI_Right"));
	WI_Right->PointerIndex = 1;
	WI_Right->TraceChannel = ECollisionChannel::ECC_EngineTraceChannel1;
	WI_Right->SetupAttachment(MC_Right);

	WI_Left = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WI_Left"));
	WI_Left->PointerIndex = 0;
	WI_Left->TraceChannel = ECollisionChannel::ECC_EngineTraceChannel1;
	WI_Left->SetupAttachment(MC_Left);
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
		IConsoleManager::Get().FindConsoleVariable(TEXT("vr.PixelDensity"))->Set(float(1.0));
	}
	
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

