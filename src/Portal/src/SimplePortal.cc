// Copyright [2024] C.Gruener
// Date: 24-05-025
// File: SimplePortal

#include "Portal/include/SimplePortal.hh"

#include "G4Navigator.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "ParticleGenerator/include/SurfaceSource.hh"

G4ThreeVector Surface::SimplePortal::TransformBetweenPortals(
    const G4ThreeVector &vec) {
  const G4VPhysicalVolume *volume = GetVolume();
  const G4VPhysicalVolume *otherVolume = fOtherPortal->GetVolume();

  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector volumeDistance = pMax - pMin;

  otherVolume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMin;

  G4ThreeVector transformedVec = vec;
  transformedVec.setX(vec.x() / volumeDistance.x() * otherVolumeDistance.x());
  transformedVec.setY(vec.y() / volumeDistance.y() * otherVolumeDistance.y());
  transformedVec.setZ(vec.z() / volumeDistance.z() * otherVolumeDistance.z());

  return transformedVec;
}

void Surface::SimplePortal::SetOtherPortal(Surface::SimplePortal *otherPortal) {
  fOtherPortal = otherPortal;
}

void Surface::SimplePortal::SetVerbose(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}

void Surface::SimplePortal::DoPortation(G4Step *step) {
  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4ThreeVector prePosition = preStepPoint->GetPosition();

  fLogger.WriteDebugInfo("PrePosition: ", prePosition);
  TransformToLocalCoordinate(prePosition);
  // Transformation of position
  TransformBetweenPortals(prePosition);
  fOtherPortal->TransformToGlobalCoordinate(prePosition);
  fLogger.WriteDebugInfo("PostPosition: ", prePosition);

  UpdatePosition(step, prePosition);
}
