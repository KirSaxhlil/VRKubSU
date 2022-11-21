


#include "VRKubSU/Actors/Exhibits/ExhibitCup.h"

AExhibitCup::AExhibitCup() {

}

void AExhibitCup::ShowInfo() {

}

void AExhibitCup::ReturnPosition() {
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
	Mesh->SetRelativeRotation(FRotator(0, 0, 0));
}