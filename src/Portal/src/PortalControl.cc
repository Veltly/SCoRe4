// Author: C.Gruener
// Date: 24-05-25
// File: PortalControl

#include "../include/PortalControl.hh"
#include "../../Service/include/Locator.hh"
#include "../../Service/include/Logger.hh"
#include "../include/VPortal.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
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
  }
  if (checkPortation && !fJustPorted) {
    fLogger.WriteDebugInfo("Boundary reached!");
    // postStep
    fLogger.WriteDebugInfo("Volume of postStepPoint is: " +
                           postStepPoint->GetPhysicalVolume()->GetName());
    fLogger.WriteDebugInfo(
        "Volume of preStepPoint is: " +
        step->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    if (EnterPortalCheck(step)) {
      EnterPortal(step);
    } else if (LeavePortalCheck(step)) {
      LeavePortal(step);
    }
  }
  std::stringstream stream;
  G4StepPoint *test = step->GetPostStepPoint();
  G4ThreeVector post = test->GetPosition();
  stream << "PostStepPoint: x: " << post.x() << " y: " << post.y()
         << " z: " << post.z();
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
  case (PortalType::SimplePortal):
    SimplePortal *simplePortal = static_cast<SimplePortal *>(portal);
    fLogger.WriteDebugInfo("Using SimplePortal " + simplePortal->GetName());
    simplePortal->DoPortation(step);
  }
  fJustPorted = true;
}

G4bool Surface::PortalControl::EnterPortalCheck(const G4Step *step) {
  G4VPhysicalVolume *preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume *postVolume = step->GetPostStepPoint()->GetPhysicalVolume();
  if (fPortalStore.IsNotPortal(postVolume)) {
    return false;
  }
  G4LogicalVolume *motherOfPreLogVolume = preVolume->GetMotherLogical();
  G4LogicalVolume *postLogVolume = postVolume->GetLogicalVolume();
  if (motherOfPreLogVolume == postLogVolume) {
    return false;
  }
  return true;
}

G4bool Surface::PortalControl::LeavePortalCheck(const G4Step *step) {
  G4VPhysicalVolume *preVolume = step->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume *postVolume = step->GetPostStepPoint()->GetPhysicalVolume();
  if (fPortalStore.IsNotPortal(preVolume)) {
    return false;
  }
  G4LogicalVolume *preLogVolume = preVolume->GetLogicalVolume();
  G4LogicalVolume *motherOfPostLogVolume = postVolume->GetMotherLogical();
  if (preLogVolume == motherOfPostLogVolume) {
    return false;
  }
  return true;
}

void Surface::PortalControl::EnterPortal(const G4Step *step) {
  G4VPhysicalVolume *volume = step->GetPostStepPoint()->GetPhysicalVolume();
  DoPortation(step, volume);
}

void Surface::PortalControl::LeavePortal(const G4Step *step) {
  G4VPhysicalVolume *volume = step->GetPreStepPoint()->GetPhysicalVolume();
  DoPortation(step, volume);
}
