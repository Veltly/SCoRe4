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
  if (checkPortation) {
    fLogger.WriteDebugInfo("Boundary reached!");
    G4bool IsPorted = false;
    // postStep
    G4VPhysicalVolume *volume = postStepPoint->GetPhysicalVolume();
    fLogger.WriteDebugInfo("Volume of postStepPoint is: " + volume->GetName());
    fLogger.WriteDebugInfo(
        "Volume of preStepPoint is: " +
        step->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    if (fPortalStore.IsPortal(volume)) {
      // DoPortation(postStepPoint, volume);
      DoPortation(step, volume);
      IsPorted = true;
    }
    // preStep
    volume = preStepPoint->GetPhysicalVolume();
    if (!IsPorted && fPortalStore.IsPortal(volume)) {
      // DoPortation(postStepPoint, volume);
      DoPortation(step, volume);
    }
  }
  std::stringstream stream;
  G4StepPoint *test = step->GetPostStepPoint();
  G4ThreeVector post = test->GetPosition();
  stream << "PostStepPoint: x: " << post.x() << " y: " << post.y()
         << " z: " << post.z();
  fLogger.WriteDebugInfo(stream.str());
}

void Surface::PortalControl::DoPortation(G4StepPoint *stepPoint,
                                         G4VPhysicalVolume *volume) {
  VPortal *portal = fPortalStore.GetPortal(volume);
  PortalType type = portal->GetPortalType();
  switch (type) {
  case (PortalType::SimplePortal):
    SimplePortal *simplePortal = static_cast<SimplePortal *>(portal);
    fLogger.WriteDebugInfo("Using SimplePortal " + simplePortal->GetName());
    simplePortal->DoPortation(stepPoint);
  }
  fJustPorted = true;
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
