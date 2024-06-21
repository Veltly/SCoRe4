// Author: C.Gruener
// Date: 24-05-025
// File: SimplePortal

#include "../include/SimplePortal.hh"
#include "../../ParticleGenerator/include/SurfaceSource.hh"
#include "../include/VPortal.hh"
#include <G4EventManager.hh>
#include <G4Navigator.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4SteppingManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4TouchableHandle.hh>
#include <G4VPhysicalVolume.hh>

G4ThreeVector
Surface::SimplePortal::TransformBetweenPortals(G4ThreeVector &vec) {
  G4ThreeVector transformedVec = vec;
  G4VPhysicalVolume *volume = GetVolume();
  G4VPhysicalVolume *otherVolume = fOtherPortal->GetVolume();
  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4ThreeVector volumeDistance = pMax - pMin;
  otherVolume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4ThreeVector otherVolumeDistance = pMax - pMin;
  transformedVec.setX(vec.x() / volumeDistance.x() * otherVolumeDistance.x());
  transformedVec.setY(vec.y() / volumeDistance.y() * otherVolumeDistance.y());
  transformedVec.setZ(vec.z() / volumeDistance.z() * otherVolumeDistance.z());

  // Implement Transformation here
  return transformedVec;
};

void Surface::SimplePortal::SetOtherPortal(Surface::SimplePortal *otherPortal) {
  fOtherPortal = otherPortal;
}

void Surface::SimplePortal::SetVerbose(G4int verbose) {
  fLogger.SetVerboseLvl(verbose);
}

void Surface::SimplePortal::DoPortation(const G4Step *step) {
  G4Track *track = step->GetTrack();
  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4ThreeVector prePosition = preStepPoint->GetPosition();
  fLogger.WriteDebugInfo(std::stringstream()
                         << "PrePosition: x: " << prePosition.x() << " y: "
                         << prePosition.y() << " z: " << prePosition.z());
  TransformToLocalCoordinate(prePosition);
  // Transformation of position
  TransformBetweenPortals(prePosition);
  fOtherPortal->TransformToGlobalCoordinate(prePosition);

  fLogger.WriteDebugInfo(std::stringstream()
                         << "PostPosition: x: " << prePosition.x() << " y: "
                         << prePosition.y() << " z: " << prePosition.z());
  G4ThreeVector direction = track->GetMomentumDirection();
  UpdatePosition(step, prePosition);
}
