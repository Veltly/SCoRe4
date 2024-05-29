// Author: C.Gruener
// Date: 24-05-025
// File: SimplePortal

#include "../include/SimplePortal.hh"
#include "ParticleGenerator/include/SurfaceSource.hh"
#include "Portal/include/VPortal.hh"
#include <G4EventManager.hh>
#include <G4Navigator.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4SteppingManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4TouchableHandle.hh>
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
  G4VPhysicalVolume *volume = GetVolume();
  G4VPhysicalVolume *otherVolume = fOtherPortal->GetVolume();
  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4double volumeZ = pMax.z() - pMin.z();
  otherVolume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4double otherVolumeZ = pMax.z() - pMin.z();
  G4double vecZ = vec.z();
  transformedVec.setZ(vecZ / volumeZ * otherVolumeZ);

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
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4ThreeVector prePosition = track->GetPosition();
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
  UpdateTouchable(track, postStepPoint, postPosition);
  track->SetPosition(postPosition);
  postStepPoint->SetPosition(postPosition);
}

void Surface::SimplePortal::UpdateTouchable(G4Track *track,
                                            G4StepPoint *stepPoint,
                                            G4ThreeVector &newPosition) {
  G4EventManager *eventManager = G4EventManager::GetEventManager();
  G4TrackingManager *trackingManager = eventManager->GetTrackingManager();
  G4SteppingManager *steppingManager = trackingManager->GetSteppingManager();
  G4Navigator *navigator = steppingManager->GetfNavigator();
  G4TouchableHandle tHandle = track->GetTouchableHandle();
  G4VTouchable *touchableTrack = tHandle();
  G4ThreeVector direction = track->GetMomentumDirection();
  navigator->LocateGlobalPointAndUpdateTouchable(newPosition, direction,
                                                 touchableTrack, false);

  // G4VTouchable *touchableStepPoint = stepPoint->GetTouchableHandle()();
  // navigator->LocateGlobalPointAndUpdateTouchable(newPosition, direction,
  //                                                touchableStepPoint, false);
  // stepPoint->SetTouchableHandle(tHandle);
}
