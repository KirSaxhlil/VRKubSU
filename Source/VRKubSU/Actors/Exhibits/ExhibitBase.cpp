// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Exhibits/ExhibitBase.h"

// Sets default values
AExhibitBase::AExhibitBase():
	WidgetScale(FVector(1,1,1)),
	InfoShowed(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components setup
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	DefaultSceneRoot->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube.Cube")).Object);
	Mesh->SetupAttachment(DefaultSceneRoot);

	WidgetTitle = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetTitle"));
	WidgetTitle->SetupAttachment(Mesh);

	WidgetMain = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetMain"));
	WidgetMain->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AExhibitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExhibitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AExhibitBase::GetDistanceToCamera() {
	AVRPawn* pawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (pawn) {
		return FVector::Dist(Mesh->GetComponentLocation(), pawn->Camera->GetComponentLocation());
	}
	else {
		return 0.0f;
	}
}