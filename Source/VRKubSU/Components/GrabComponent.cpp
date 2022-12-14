// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Components/GrabComponent.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	SetShouldSimulateOnDrop();
	Cast<UPrimitiveComponent>(GetAttachParent())->SetCollisionProfileName("PhysicsActor", true);
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UGrabComponent::TryGrab(UMotionControllerComponent* MotionController) {
	SetPrimitiveComponentPhysics(false);

	if (AttachParentToMotionController(MotionController)) {
		MotionControllerRef = MotionController;
		return true;
	}
	else {
		return false;
	}
}

bool UGrabComponent::TryRelease() {
	if (SimulateOnDrop) {
		SetPrimitiveComponentPhysics(true);
		return true;
	}
	else {
		GetAttachParent()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));
		if (Cast<UMotionControllerComponent>(GetAttachParent())) {
			return false;
		}
		else {
			return true;
		}
	}
}

void UGrabComponent::SetShouldSimulateOnDrop() {
	if (Cast<UPrimitiveComponent>(GetAttachParent())->IsAnySimulatingPhysics()) {
		SimulateOnDrop = true;
	}
}

void UGrabComponent::SetPrimitiveComponentPhysics(bool Simulate) {
	UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(GetAttachParent());
	if (IsValid(Comp)) {
		Comp->SetSimulatePhysics(Simulate);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GrabComponent->SetSimulatingParent->Cast to UPrimitiveComponent FAILED"))
	}
}

bool UGrabComponent::AttachParentToMotionController(UMotionControllerComponent* MotionController) {
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld,
																		  EAttachmentRule::KeepWorld,
																		  EAttachmentRule::KeepWorld,
																		  true);
	bool AttachmentResult = GetAttachParent()->AttachToComponent(MotionController, AttachmentRules);
	UE_CLOG( (!AttachmentResult), LogTemp, Warning, TEXT("Attaching %s to %s FAILED - Object is not grabbed."), *GetAttachParent()->GetName(), *MotionController->GetName())
	return AttachmentResult;
}