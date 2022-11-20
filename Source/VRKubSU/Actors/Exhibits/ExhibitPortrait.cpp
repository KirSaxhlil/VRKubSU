// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Exhibits/ExhibitPortrait.h"

AExhibitPortrait::AExhibitPortrait() {
	// Setting an image of portrait

	FVector BoundsMin, BoundsMax;
	Mesh->GetLocalBounds(BoundsMin, BoundsMax);
	WidgetTitle->SetRelativeLocation(BoundsMax);
	WidgetMain->SetRelativeLocation(BoundsMin);
}

void AExhibitPortrait::ShowInfo() {
	// activate showing animation in widget
	// starting audio
}