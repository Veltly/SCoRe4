// Author: C.Gruener
// Date: 24-05-25
// File: PortalControl

#include "../include/PortalControl.hh"
#include "../../Service/include/Locator.hh"
#include "../include/VPortal.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "Portal/include/SimplePortal.hh"
#include <G4StepPoint.hh>
#include <G4VPhysicalVolume.hh>

Surface::PortalControl::PortalControl()
    : fPortalStore(Surface::Locator::GetPortalStore()) {}

void Surface::PortalControl::DoStep(const G4Step *step) {
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4StepStatus stepStatus = postStepPoint->GetStepStatus();
  G4cout << "StepStatus is: " << stepStatus << "\n";
  if (stepStatus == G4StepStatus::fGeomBoundary) {
    // postStep
    G4VPhysicalVolume *volume = postStepPoint->GetPhysicalVolume();
    if (fPortalStore.IsPortal(volume)) {
      DoPortation(postStepPoint, volume);
    }
    // preStep
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    volume = preStepPoint->GetPhysicalVolume();
    if (fPortalStore.IsPortal(volume)) {
      DoPortation(postStepPoint, volume);
    }
  }
}

void Surface::PortalControl::DoPortation(G4StepPoint *stepPoint,
                                         G4VPhysicalVolume *volume) {
  VPortal *portal = fPortalStore.GetPortal(volume);
  PortalType type = portal->GetPortalType();
  switch (type) {
  case (PortalType::SimplePortal):
    SimplePortal *simplePortal = static_cast<SimplePortal *>(portal);
    G4cout << "static cast working!\n";
    G4cout << "Name of Portal is " << simplePortal->GetName() << "\n";
    simplePortal->DoPortation(stepPoint);
  }
}
