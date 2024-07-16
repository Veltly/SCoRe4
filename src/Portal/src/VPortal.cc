// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "Portal/include/VPortal.hh"

#include <cstdlib>

#include "G4EventManager.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "Service/include/Logger.hh"

Surface::VPortal::VPortal(const G4String &name, G4VPhysicalVolume *volume,
                          const PortalType type)
    : fName(name),
      fLogger("VPortal_" + name),
      fVolume(volume),
      fPortalType(type),
      fGlobalCoordSet(false) {
  fLogger.WriteDebugInfo("No global coord set for " + fName);
}

Surface::VPortal::VPortal(const G4String &name, G4VPhysicalVolume *volume,
                          PortalType type, const G4ThreeVector &globalCoord)
    : fName(name),
      fLogger("VPortal_" + name),
      fVolume(volume),
      fPortalType(type),
      fGlobalCoord(globalCoord),
      fGlobalCoordSet(true) {
  fLogger.WriteDebugInfo("Global coord of " + fName +
                         " is set to x: " + std::to_string(fGlobalCoord.x()) +
                         " y: " + std::to_string(fGlobalCoord.y()) +
                         " z: " + std::to_string(fGlobalCoord.z()));
}

G4ThreeVector Surface::VPortal::GetLocalCoordSystem() {
  if (fGlobalCoordSet) {
    return fGlobalCoord;
  }
  fLogger.WriteError("Global coodinates not set for: " + fName);
  exit(EXIT_FAILURE);
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
