//Author: C.Gruener
//Date: 24-05-25
//File: PortalControl


#include "../include/PortalControl.hh"
#include "../../Service/include/Locator.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
#include <G4StepPoint.hh>
#include <G4VPhysicalVolume.hh>
#include "../include/VPortal.hh"

Surface::PortalControl::PortalControl():fPortalStore(Surface::Locator::GetPortalStore()){
}

void Surface::PortalControl::DoStep(G4Step *step){
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4StepStatus stepStatus = postStepPoint->GetStepStatus();
  if(stepStatus == G4StepStatus::fGeomBoundary){
    //postStep
    G4VPhysicalVolume *volume = postStepPoint->GetPhysicalVolume();
    if(fPortalStore.IsPortal(volume)){
      VPortal portal = fPortalStore.GetPortal(volume);
      portal.DoPortation(postStepPoint);
    }
    //preStep
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    volume = preStepPoint->GetPhysicalVolume();
    if(fPortalStore.IsPortal(volume)){
      VPortal portal = fPortalStore.GetPortal(volume);
      portal.DoPortation(postStepPoint);
  }
}
}
