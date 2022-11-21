// Fill out your copyright notice in the Description page of Project Settings.


#include "VRKubSU/Actors/Exhibits/ExhibitStatic.h"

AExhibitStatic::AExhibitStatic()
{
	WidgetTitle->SetRelativeLocation(InfoPosition);
	WidgetMain->SetRelativeLocation(InfoPosition - FVector(0, 0, 10));
}

void AExhibitStatic::ShowInfo() {

}