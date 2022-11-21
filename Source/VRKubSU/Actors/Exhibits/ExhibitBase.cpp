// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Exhibits/ExhibitBase.h"

// Sets default values
AExhibitBase::AExhibitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Components setup
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = DefaultSceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

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

void AExhibitBase::ShowInfo() {

}