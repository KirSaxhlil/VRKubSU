// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NiagaraComponent.h"

#include "TeleportRing.generated.h"

UCLASS()
class VRKUBSU_API ATeleportRing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportRing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	USceneComponent* DefaultSceneRoot;
	UNiagaraComponent* NS_Ring;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
