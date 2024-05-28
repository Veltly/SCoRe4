// Author: C.Gruener
// Date: 24-05-025
// File: SimplePortal

#include "../include/SimplePortal.hh"
#include "Portal/include/VPortal.hh"
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>

void Surface::SimplePortal::DoPortation(G4StepPoint *point) {
  G4ThreeVector prePosition = point->GetPosition();
  fLogger.WriteDebugInfo(std::stringstream()
                         << "PrePosition: x: " << prePosition.x() << " y: "
                         << prePosition.y() << " z: " << prePosition.z());
  G4ThreeVector tmpPosition = TransformToLocalCoordinate(prePosition);
  tmpPosition = TransformBetweenPortals(tmpPosition);
  G4ThreeVector postPosition =
      fOtherPortal->TransformToGlobalCoordinate(tmpPosition);
  fLogger.WriteDebugInfo(std::stringstream()
                         << "PostPosition: x: " << postPosition.x() << " y: "
                         << postPosition.y() << " z: " << postPosition.z());
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

void Surface::SimplePortal::SetVerbose(G4int verbose) {
  fLogger.SetVerboseLvl(verbose);
}
