// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Pawns/VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
	:
	AxisDeadzone(0.7f),
	SnapTurnAngle(-45.f),
	ProjectedTeleportLocation(FVector(0,0,0)),
	DoOnceTurn(false),
	DoOnceTeleport(false)
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

	PlayerInputComponent->BindAxis("Turning", this, &AVRPawn::InputAxis_Turn);
	PlayerInputComponent->BindAxis("Teleport", this, &AVRPawn::InputAxis_Teleport);
	PlayerInputComponent->BindAction("GrabLeft", EInputEvent::IE_Pressed, this, &AVRPawn::InputAction_GrabLeft_Pressed);
	PlayerInputComponent->BindAction("GrabLeft", EInputEvent::IE_Released, this, &AVRPawn::InputAction_GrabLeft_Released);
	PlayerInputComponent->BindAction("GrabRight", EInputEvent::IE_Pressed, this, &AVRPawn::InputAction_GrabRight_Pressed);
	PlayerInputComponent->BindAction("GrabRight", EInputEvent::IE_Released, this, &AVRPawn::InputAction_GrabRight_Released);
}

void AVRPawn::InputAxis_Turn(float AxisValue)
{
	if (Controller != NULL) {
		if (IsAxisGreaterThenDeadzone(AxisValue)) {
			if (!DoOnceTurn) {
				DoOnceTurn = true;
				SnapTurn(AxisValue > 0.f);
			}
		}
		else {
			DoOnceTurn = false;
		}
	}
}

void AVRPawn::InputAxis_Teleport(float AxisValue)
{
	if (Controller != NULL) {
		if (IsAxisGreaterThenDeadzone(AxisValue) && AxisValue > 0.f) {
			if (!DoOnceTeleport) {
				DoOnceTeleport = true;
				StartTeleportTrace();
			}
			TeleportTrace(MC_Right->GetComponentLocation(), MC_Right->GetForwardVector());
		}
		else {
			if (TeleportTracing) {
				EndTeleportTrace();
				TryTeleport();
				DoOnceTeleport = false;
			}
		}
	}
}

void AVRPawn::InputAction_GrabLeft_Pressed() {
	UE_LOG(LogTemp, Warning, TEXT("Grab is pressed"));
	UGrabComponent* Component = GetGrabComponentNearMotionController(MC_Left);
	if (IsValid(Component)) {
		UE_LOG(LogTemp, Warning, TEXT("Found valid component"));
		if (Component->TryGrab(MC_Left)) {
			HeldComponentLeft = Component;
			if (HeldComponentLeft == HeldComponentRight) {
				HeldComponentRight = NULL;
			}
			UE_LOG(LogTemp, Warning, TEXT("Grabbed"));
			IInteractionInterface* Obj = Cast<IInteractionInterface>(Component->GetOwner());
			if (Obj) {
				UE_LOG(LogTemp, Warning, TEXT("Grabbed Executed"));
				Obj->Execute_Grabbed(Component->GetOwner(), this, false);
			}
		}
	}
}

void AVRPawn::InputAction_GrabLeft_Released() {
	if (IsValid(HeldComponentLeft)) {
		if (HeldComponentLeft->TryRelease()) {
			
			IInteractionInterface* Obj = Cast<IInteractionInterface>(HeldComponentLeft->GetOwner());
			if (Obj) {
				Obj->Execute_Ungrabbed(HeldComponentLeft->GetOwner());
			}

			HeldComponentLeft = NULL;
		}
	}
}

void AVRPawn::InputAction_GrabRight_Pressed() {
	UGrabComponent* Component = GetGrabComponentNearMotionController(MC_Right);
	if (IsValid(Component)) {
		if (Component->TryGrab(MC_Right)) {
			HeldComponentRight = Component;
			if (HeldComponentLeft == HeldComponentRight) {
				HeldComponentLeft = NULL;
			}
			IInteractionInterface* Obj = Cast<IInteractionInterface>(Component->GetOwner());
			if (Obj) {
				UE_LOG(LogTemp, Warning, TEXT("Grabbed Executed"));
				Obj->Execute_Grabbed(Component->GetOwner(), this, true);
			}
		}
	}
}

void AVRPawn::InputAction_GrabRight_Released() {
	if (IsValid(HeldComponentRight)) {
		if (HeldComponentRight->TryRelease()) {

			IInteractionInterface* Obj = Cast<IInteractionInterface>(HeldComponentRight->GetOwner());
			if (Obj) {
				Obj->Execute_Ungrabbed(HeldComponentRight->GetOwner());
			}

			HeldComponentRight = NULL;
		}
	}
}

////// FUNCTIONS //////

bool AVRPawn::IsAxisGreaterThenDeadzone(float axis_value) {
	return FMath::Abs(axis_value) >= AxisDeadzone;
}

void AVRPawn::SnapTurn(bool RightTurn) {
	float LocalYawDelta = (RightTurn ? FMath::Abs(SnapTurnAngle) : SnapTurnAngle);//FMath::FloatSelect(FMath::Abs(SnapTurnAngle), SnapTurnAngle, RightTurn);
	FVector LocalCameraPosition = Camera->GetComponentLocation();
	FTransform LocalCameraRelativeTransform = Camera->GetRelativeTransform();
	FTransform LocalNewTransform = FTransform(this->GetActorRotation().Add(0, LocalYawDelta, 0), this->GetActorLocation(), FVector(1, 1, 1));
	AddActorWorldRotation(FRotator(0, LocalYawDelta, 0), false, NULL, ETeleportType::TeleportPhysics);
	FVector NewLocation = (LocalCameraPosition - (LocalCameraRelativeTransform * LocalNewTransform).GetLocation()) + GetActorLocation();
	SetActorLocation(NewLocation, false, NULL, ETeleportType::TeleportPhysics);
}

void AVRPawn::StartTeleportTrace() {
	TeleportTracing = true;
	Tracer->SetVisibility(true);
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;
	TeleportRingRef = GetWorld()->SpawnActor<ATeleportRing>(ATeleportRing::StaticClass(), FTransform(), SpawnParams);
}

void AVRPawn::EndTeleportTrace() {
	TeleportTracing = false;
	if (IsValid(TeleportRingRef)) {
		TeleportRingRef->Destroy();
	}
	Tracer->SetVisibility(false);
}

void AVRPawn::TeleportTrace(FVector Start, FVector ForwardVector) {
	float LocalTeleportLaunchSpeed = 650.f;
	float LocalNavMeshCellHeight = 8.f;
	float LocalTeleportProjectileRadius = 3.6f;

	FPredictProjectilePathParams PredictParams = FPredictProjectilePathParams();
	PredictParams.StartLocation = Start;
	PredictParams.LaunchVelocity = FVector(LocalTeleportLaunchSpeed) * ForwardVector;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = LocalTeleportProjectileRadius;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	PredictParams.ObjectTypes = ObjectTypes;
	PredictParams.bTraceComplex = false;
	PredictParams.SimFrequency = 15;
	PredictParams.MaxSimTime = 2;
	PredictParams.OverrideGravityZ = 0;

	UE_LOG(LogTemp, Warning, TEXT("Pathing"));
	FPredictProjectilePathResult PathResults;
	if (UGameplayStatics::PredictProjectilePath(this, PredictParams, PathResults)) {
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
	}

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

void AVRPawn::TryTeleport()
{
	if (ValidTeleportLocation) {
		ValidTeleportLocation = false;
		FVector TargetLocation = FVector(Camera->GetRelativeLocation().X,
										 Camera->GetRelativeLocation().Y,
										 0.f);
		TargetLocation = ProjectedTeleportLocation - FRotator(0, GetActorRotation().Yaw, 0).RotateVector(TargetLocation);
		FRotator TargetRotation = FRotator(0, GetActorRotation().Yaw, 0);
		TeleportTo(TargetLocation, TargetRotation);
	}
}

FProjectedResult AVRPawn::IsValidTeleportLocation(FHitResult Hit) {
	FProjectedResult Result = FProjectedResult();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
	Result.Return = NavSys->ProjectPointToNavigation(FVector(Hit.Location),Result.ProjectedLocation,FVector(0,0,0));
	//Result.Return = GetWorld()->GetNavigationSystem()->ProjectPointToNavigation();
	//Result.Return = NavSys->ProjectPointToNavigation(FVector(Hit.Location.X, Hit.Location.Y, Hit.Location.Z), Result.ProjectedLocation, FVector(0, 0, 0), &AgentProps);
	return Result;
}

UGrabComponent* AVRPawn::GetGrabComponentNearMotionController(UMotionControllerComponent* MotionController) {
	float LocalNearestComponentDistance = MAX_FLT;
	UGrabComponent* LocalNearestGrabComponent = NULL;
	FVector LocalGripPosition = MotionController->GetComponentLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
	TArray<AActor*> IgnoredActors = TArray<AActor*>(); // <..., FDefaultAllocator>
	FHitResult TraceResult;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LocalGripPosition, LocalGripPosition, 6, ObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, TraceResult, true, FLinearColor::Green, FLinearColor::Red, 0)) {
		//TArray<UActorComponent*> GrabComponents = TraceResult.GetActor()->GetComponentsByClass(TSubclassOf<UGrabComponent>());
		TArray<UActorComponent*> GrabComponents = TraceResult.GetActor()->GetComponentsByClass(UGrabComponent::StaticClass());
		if (GrabComponents.Num() > 0) {
			for (UActorComponent* Component : GrabComponents) {
				UGrabComponent* CurrentGrabComponent = Cast<UGrabComponent>(Component);
				float CurrentVectorLength = (CurrentGrabComponent->GetComponentLocation() - LocalGripPosition).SquaredLength();
				if (CurrentVectorLength <= LocalNearestComponentDistance) {
					LocalNearestComponentDistance = CurrentVectorLength;
					LocalNearestGrabComponent = CurrentGrabComponent;
				}
			}
		}
	}
	return LocalNearestGrabComponent;
}