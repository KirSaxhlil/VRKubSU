// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Pawns/TeleportRing.h"

// Sets default values
ATeleportRing::ATeleportRing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeleportRing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportRing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

