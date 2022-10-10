// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Pawns/VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
	:
	AxisDeadzone(0.7f),
	SnapTurnAngle(-45.f),
	ProjectedTeleportLocation(FVector(0,0,0))
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

	Tracer = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Tracer"));
	Tracer->SetAsset(ConstructorHelpers::FObjectFinder<UNiagaraSystem>(TEXT("/Game/Assets/VFX/NS_TeleportTrace.NS_TeleportTrace")).Object);
	Tracer->SetVisibility(false);
	Tracer->SetupAttachment(DefaultSceneRoot);
}

////// BASIC EVENTS //////

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

////// INPUTS //////

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AVRPawn::InputAxis_Turn);
}

void AVRPawn::InputAxis_Turn(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f)) {
		bool DoOnce = false;
		if (IsAxisGreaterThenDeadzone(AxisValue)) {
			if (DoOnce == false) {
				DoOnce = true;
				SnapTurn(AxisValue > 0.f);
			}
		}
		else {
			DoOnce = false;
		}
	}
}

////// FUNCTIONS //////

bool AVRPawn::IsAxisGreaterThenDeadzone(float axis_value) {
	return FMath::Abs(axis_value) >= AxisDeadzone;
}

void AVRPawn::SnapTurn(bool RightTurn) {
	float LocalYawDelta = FMath::FloatSelect(FMath::Abs(SnapTurnAngle), SnapTurnAngle, RightTurn);
	FVector LocalCameraPosition = Camera->GetComponentLocation();
	FTransform LocalCameraRelativeTransform = Camera->GetRelativeTransform();
	FTransform LocalNewTransform = FTransform(this->GetActorRotation().Add(0, 0, LocalYawDelta), this->GetActorLocation(), FVector(1, 1, 1));
	AddActorWorldRotation(FRotator(0, 0, LocalYawDelta), false, NULL, ETeleportType::TeleportPhysics);
	FVector NewLocation = (LocalCameraPosition - (LocalCameraRelativeTransform * LocalNewTransform).GetLocation()) + GetActorLocation();
	SetActorLocation(NewLocation, false, NULL, ETeleportType::TeleportPhysics);
}

void AVRPawn::StartTeleportTrace() {
	TeleportTracing = true;
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;
	TeleportRingRef = GetWorld()->SpawnActor<ATeleportRing>(ATeleportRing::StaticClass(), FTransform(), SpawnParams);
}

void AVRPawn::EndTeleportTrace() {
	TeleportTracing = false;
	if (IsValid(TeleportRingRef)) {
		TeleportRingRef->Destroy();
	}
}

void AVRPawn::TeleportTrace(FVector Start, FVector ForwardVector) {
	float LocalTeleportLaunchSpeed = 650.f;
	float LocalNavMeshCellHeight = 8.f;

	FPredictProjectilePathParams PredictParams = FPredictProjectilePathParams();
	PredictParams.StartLocation = Start;
	PredictParams.LaunchVelocity = FVector(LocalTeleportLaunchSpeed) * ForwardVector;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	PredictParams.ObjectTypes = ObjectTypes;
	PredictParams.bTraceComplex = false;


	FPredictProjectilePathResult PathResults;
	UGameplayStatics::PredictProjectilePath(this, PredictParams, PathResults);

	TeleportTracePathPositions = TArray<FVector>();
	TeleportTracePathPositions.Add(Start);
	for (FPredictProjectilePathPointData Point : PathResults.PathData) {
		TeleportTracePathPositions.Add(Point.Location);
	}
	FProjectedResult Projected = IsValidTeleportLocation(PathResults.HitResult);
	ProjectedTeleportLocation = FVector(Projected.ProjectedLocation.Location.X,
										Projected.ProjectedLocation.Location.Y,
										Projected.ProjectedLocation.Location.Z - LocalNavMeshCellHeight);

	if (ValidTeleportLocation != Projected.Return) {
		ValidTeleportLocation = Projected.Return;
		TeleportRingRef->GetRootComponent()->SetVisibility(ValidTeleportLocation, true);
	}

	TeleportRingRef->SetActorLocation(ProjectedTeleportLocation);

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(Tracer, TEXT("User.PointsArray"), TeleportTracePathPositions);
}

FProjectedResult AVRPawn::IsValidTeleportLocation(FHitResult Hit) {
	FProjectedResult Result = FProjectedResult();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());;
	Result.Return = NavSys->ProjectPointToNavigation(FVector(Hit.Location),Result.ProjectedLocation);
	return Result;
}