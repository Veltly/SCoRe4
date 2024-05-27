// Author: C.Gruener
// Date: 24-05-025
// File: SimplePortal

#include "../include/SimplePortal.hh"
#include <G4ThreeVector.hh>

void Surface::SimplePortal::DoPortation(G4StepPoint *point) {
  G4ThreeVector prePosition = point->GetPosition();
  G4ThreeVector tmpPosition = TransformToLocalCoordinate(prePosition);
  tmpPosition = TransformBetweenPortals(tmpPosition);
  G4ThreeVector postPosition =
      fOtherPortal->TransformToGlobalCoordinate(tmpPosition);
  point->SetPosition(postPosition);
}

G4ThreeVector
Surface::SimplePortal::TransformBetweenPortals(G4ThreeVector &vec) {
  G4ThreeVector transformedVec = vec;
  // Implement Transformation here
  return transformedVec;
};

void Surface::SimplePortal::SetOtherPortal(Surface::SimplePortal *otherPortal) {
  fOtherPortal = otherPortal;
}
