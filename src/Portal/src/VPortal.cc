// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "../include/VPortal.hh"
#include "../../Service/include/Logger.hh"
#include <G4EventManager.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4VPhysicalVolume.hh>
#include <cstdlib>

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume,
                          PortalType type)
    : fName(name), fVolume(volume), fPortalType(type), fGlobalCoordSet(false) {

  Logger logger{"VPortal", 3};
  logger.WriteDebugInfo("No global coord set for " + fName);
};

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume,
                          PortalType type, G4ThreeVector &globalCoord)
    : fName(name), fVolume(volume), fPortalType(type),
      fGlobalCoord(globalCoord), fGlobalCoordSet(true) {
  Logger logger{"VPortal", 3};
  logger.WriteDebugInfo("Global coord of " + fName +
                        " is set to x: " + std::to_string(fGlobalCoord.x()) +
                        " y: " + std::to_string(fGlobalCoord.y()) +
                        " z: " + std::to_string(fGlobalCoord.z()));
};

G4ThreeVector Surface::VPortal::GetLocalCoordSystem(G4VPhysicalVolume *volume) {
  // To do: Implement routine to find global coordinate for given volume
  exit(EXIT_FAILURE);
}

G4ThreeVector Surface::VPortal::GetLocalCoordSystem() {
  if (fGlobalCoordSet) {
    return fGlobalCoord;
  }
  return GetLocalCoordSystem(fVolume);
}

void Surface::VPortal::SetGlobalCoord(G4ThreeVector vec) {
  fGlobalCoord = vec;
  fGlobalCoordSet = true;
  Logger logger{"VPortal", 3};
  logger.WriteDebugInfo("Global coord of " + fName +
                        " is set to x: " + std::to_string(fGlobalCoord.x()) +
                        " y: " + std::to_string(fGlobalCoord.y()) +
                        " z: " + std::to_string(fGlobalCoord.z()));
}

void Surface::VPortal::TransformToLocalCoordinate(G4ThreeVector &vec) {
  G4ThreeVector coordinateOfVolume = GetLocalCoordSystem();
  vec -= coordinateOfVolume;
}

void Surface::VPortal::TransformToGlobalCoordinate(G4ThreeVector &vec) {
  G4ThreeVector coordinateOfVolume = GetLocalCoordSystem();
  vec += coordinateOfVolume;
}

void Surface::VPortal::SetTrigger(G4VPhysicalVolume *volume) {
  fTrigger = volume;
}

void Surface::VPortal::UpdatePosition(const G4Step *step,
                                      G4ThreeVector &newPosition) {
  G4ThreeVector direction = step->GetPostStepPoint()->GetMomentumDirection();
  UpdatePositionMomentum(step, newPosition, direction);
}
void Surface::VPortal::UpdatePositionMomentum(const G4Step *step,
                                              G4ThreeVector &newPosition,
                                              G4ThreeVector &newDirection) {
  G4EventManager *eventManager = G4EventManager::GetEventManager();
  G4TrackingManager *trackingManager = eventManager->GetTrackingManager();
  G4SteppingManager *steppingManager = trackingManager->GetSteppingManager();
  G4Navigator *navigator = steppingManager->GetfNavigator();
  G4StepPoint *stepPoint = step->GetPostStepPoint();
  G4VTouchable *touchableStepPoint = stepPoint->GetTouchableHandle()();
  navigator->LocateGlobalPointAndUpdateTouchable(newPosition, newDirection,
                                                 touchableStepPoint, false);
  G4Track *track = step->GetTrack();
  track->SetPosition(newPosition);
  track->SetMomentumDirection(newDirection);
  stepPoint->SetPosition(newPosition);
  stepPoint->SetMomentumDirection(newDirection);
}
