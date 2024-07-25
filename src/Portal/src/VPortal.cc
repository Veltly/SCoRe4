// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "Portal/include/VPortal.hh"

#include <cstdlib>

#include "G4EventManager.hh"
#include "G4PathFinder.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "Service/include/Logger.hh"

Surface::VPortal::VPortal(const G4String &name, G4VPhysicalVolume *volume,
                          const PortalType type, const G4int verboseLvl)
    : fLogger("VPortal_" + name, verboseLvl),
      fName(name),
      fVolume(volume),
      fPortalType(type),
      fGlobalCoordSet(false) {
  fLogger.WriteDebugInfo("No global coord set for " + fName);
}

Surface::VPortal::VPortal(const G4String &name, G4VPhysicalVolume *volume,
                          const PortalType type, const G4int verboseLvl,
                          const G4ThreeVector &globalCoord)
    : fLogger("VPortal_" + name, verboseLvl),
      fName(name),
      fVolume(volume),
      fPortalType(type),
      fGlobalCoord(globalCoord),
      fGlobalCoordSet(true) {
  fLogger.WriteDebugInfo("Global coord of " + fName +
                         " is set to x: " + std::to_string(fGlobalCoord.x()) +
                         " y: " + std::to_string(fGlobalCoord.y()) +
                         " z: " + std::to_string(fGlobalCoord.z()));
}

G4ThreeVector Surface::VPortal::GetLocalCoordSystem() const {
  if (fGlobalCoordSet) {
    return fGlobalCoord;
  }
  fLogger.WriteError("Global coodinates not set for: " + fName);
  exit(EXIT_FAILURE);
}

void Surface::VPortal::SetGlobalCoord(const G4ThreeVector vec) {
  fGlobalCoord = vec;
  fGlobalCoordSet = true;
  Logger logger{"VPortal"};
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

void Surface::VPortal::UpdatePosition(G4Step *step,
                                      const G4ThreeVector &newPosition) {
  G4ThreeVector direction = step->GetPostStepPoint()->GetMomentumDirection();
  UpdatePositionMomentum(step, newPosition, direction);
}
void Surface::VPortal::UpdatePositionMomentum(
    G4Step *step, const G4ThreeVector &newPosition,
    const G4ThreeVector &newDirection) {
  //  G4EventManager *eventManager = G4EventManager::GetEventManager();
  //  G4TrackingManager *trackingManager = eventManager->GetTrackingManager();
  //  G4SteppingManager *steppingManager =
  //  trackingManager->GetSteppingManager();
  G4Navigator *navigator = G4TransportationManager::GetTransportationManager()
                               ->GetNavigatorForTracking();
  // G4Navigator *navigator = steppingManager->GetfNavigator();
  G4StepPoint *stepPoint = step->GetPostStepPoint();
  G4VTouchable *touchableStepPoint = stepPoint->GetTouchableHandle()();
  navigator->LocateGlobalPointAndUpdateTouchable(newPosition, newDirection,
                                                 touchableStepPoint, false);
  navigator->ComputeSafety(newPosition);

  G4PathFinder::GetInstance()->ReLocate(newPosition);
  G4PathFinder::GetInstance()->ComputeSafety(newPosition);

  G4Track *track = step->GetTrack();
  track->SetPosition(newPosition);
  track->SetMomentumDirection(newDirection);
  stepPoint->SetPosition(newPosition);
  stepPoint->SetMomentumDirection(
      newDirection);  // specified point in the global coordinate system.

  G4EventManager *evtm = G4EventManager::GetEventManager();
  G4TrackingManager *tckm = evtm->GetTrackingManager();
  G4VTrajectory *fpTrajectory = NULL;
  fpTrajectory = tckm->GimmeTrajectory();
  if (fpTrajectory) {
    fpTrajectory->AppendStep(step);
  }
}

void Surface::VPortal::SetVerbose(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}
