// Author: C.Gruener
// Date: 24-05-25
// File: PortalControl

#include "../include/PortalControl.hh"
#include "../../Service/include/Locator.hh"
#include "../../Service/include/Logger.hh"
#include "../include/VPortal.hh"
#include "G4Step.hh"
#include "Portal/include/PeriodicPortal.hh"
#include "Portal/include/SimplePortal.hh"
#include <G4LogicalVolume.hh>
#include <G4StepPoint.hh>
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>
#include <sstream>

Surface::PortalControl::PortalControl(G4int verbose)
    : fPortalStore(Surface::Locator::GetPortalStore()),
      fLogger({"PortalControl", verbose}) {
  fLogger.WriteInfo("PortalControl initialized");
}

void Surface::PortalControl::DoStep(const G4Step *step) {
  if (fJustPorted) {
    fJustPorted = false;
    return;
  }
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4VPhysicalVolume *prePhysVol = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume *postPhysVol = postStepPoint->GetPhysicalVolume();
  G4bool checkPortation = false;
  if (postPhysVol != nullptr) {
    G4String preVolName = prePhysVol->GetName();
    G4String postVolName = postPhysVol->GetName();
    if (preVolName != postVolName)
      checkPortation = true;
    // if preVolName is not postVolName -> there was a volume change-> check if
    // a portal is involved
  }
  // if (checkPortation && !fJustPorted) {
  if (checkPortation) {
    fLogger.WriteDebugInfo("Boundary reached!");
    // postStep
    fLogger.WriteDebugInfo(
        "Volume of preStepPoint is: " +
        step->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    fLogger.WriteDebugInfo("Volume of postStepPoint is: " +
                           postStepPoint->GetPhysicalVolume()->GetName());
    if (EnterPortalCheck(step)) {
      UsePortal(step);
    }
  }
  std::stringstream stream;
  G4StepPoint *test = step->GetPostStepPoint();
  G4ThreeVector post = test->GetPosition();
  stream << "PostStepPoint: x: " << post.x() << " y: " << post.y()
         << " z: " << post.z();
  if (test->GetPhysicalVolume() != nullptr) {
    stream << " Volume is: " << test->GetPhysicalVolume()->GetName();
  }
  fLogger.WriteDebugInfo(stream.str());
}

void Surface::PortalControl::SetVerbose(G4int verbose) {
  fLogger.SetVerboseLvl(verbose);
}

void Surface::PortalControl::DoPortation(const G4Step *step,
                                         G4VPhysicalVolume *volume) {
  VPortal *portal = fPortalStore.GetPortal(volume);
  PortalType type = portal->GetPortalType();
  switch (type) {
  case PortalType::SimplePortal: {
    SimplePortal *simplePortal = static_cast<SimplePortal *>(portal);
    fLogger.WriteDebugInfo("Using SimplePortal " + simplePortal->GetName());
    simplePortal->DoPortation(step);
    break;
  }
  case PortalType::PeriodicPortal: {
    PeriodicPortal *periodicPortal = static_cast<PeriodicPortal *>(portal);
    fLogger.WriteDebugInfo("Using PeriodicPortal " + periodicPortal->GetName());
    periodicPortal->DoPortation(step);
    break;
  }
  }
  fJustPorted = true;
}

G4bool Surface::PortalControl::EnterPortalCheck(const G4Step *step) {
  G4VPhysicalVolume *postVolume = step->GetPostStepPoint()->GetPhysicalVolume();
  if (fPortalStore.IsTrigger(postVolume)) {
    return true;
  }
  return false;
}

void Surface::PortalControl::UsePortal(const G4Step *step) {
  G4VPhysicalVolume *trigger = step->GetPostStepPoint()->GetPhysicalVolume();
  G4int triggerId = fPortalStore.FindTrigger(trigger);
  G4VPhysicalVolume *volume = fPortalStore.at(triggerId)->GetVolume();
  DoPortation(step, volume);
}

G4bool Surface::PortalControl::IsVolumeInsidePortal(
    const G4VPhysicalVolume *volume) const {
  G4LogicalVolume *motherLog = volume->GetMotherLogical();
  if (motherLog == nullptr)
    return false;
  return fPortalStore.IsLogPortal(motherLog);
}
