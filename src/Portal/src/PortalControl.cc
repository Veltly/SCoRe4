// Copyright [2024] C.Gruener
// Date: 24-05-25
// File: PortalControl

#include "Portal/include/PortalControl.hh"

#include <sstream>

#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PeriodicPortal.hh"
#include "Portal/include/SimplePortal.hh"
#include "Portal/include/VPortal.hh"
#include "Service/include/Locator.hh"
#include "Service/include/Logger.hh"

Surface::PortalControl::PortalControl()
    : fPortalStore(Surface::Locator::GetPortalStore()),
      fLogger("PortalControl") {
  fLogger.WriteInfo("PortalControl initialized");
}

Surface::PortalControl::PortalControl(const G4int verboseLvl)
    : fPortalStore(Surface::Locator::GetPortalStore()),
      fLogger("PortalControl", verboseLvl) {
  fLogger.WriteInfo("PortalControl initialized");
}

// Usefull function because using SteppingAction.hh step is const
void Surface::PortalControl::DoStep(const G4Step *step) {
  DoStep(const_cast<G4Step *>(step));
}

void Surface::PortalControl::DoStep(G4Step *step) {
  const G4StepPoint *postStepPoint = step->GetPostStepPoint();
  const G4StepPoint *preStepPoint = step->GetPreStepPoint();

  const G4VPhysicalVolume *prePhysVol = preStepPoint->GetPhysicalVolume();
  const G4VPhysicalVolume *postPhysVol = postStepPoint->GetPhysicalVolume();

  G4bool checkPortation = false;

  if (postPhysVol != nullptr) {
    const G4String preVolName = prePhysVol->GetName();
    const G4String postVolName = postPhysVol->GetName();
    if (preVolName != postVolName) {
      checkPortation = true;
    }
    // if preVolName is not postVolName -> there was a volume change-> check if
    // a portal is involved
  }
  // if (checkPortation && !fJustPorted) {
  if (checkPortation) {
    fLogger.WriteDebugInfo(
        "Boundary reached!\n"
        "Volume of preStepPoint is: " +
            step->GetPreStepPoint()->GetPhysicalVolume()->GetName() + " at ",
        preStepPoint->GetPosition());

    if (EnterPortalCheck(step)) {
      UsePortal(step);
    }
  }

  std::stringstream stream;
  const G4StepPoint *postStep = step->GetPostStepPoint();
  if (postStep->GetPhysicalVolume() != nullptr) {
    stream << " Volume is: " << postStep->GetPhysicalVolume()->GetName()
           << " at ";
  }
  fLogger.WriteDebugInfo(stream.str(), postStep->GetPosition());
}

void Surface::PortalControl::SetVerbose(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}

void Surface::PortalControl::DoPortation(G4Step *step,
                                         const G4VPhysicalVolume *volume) {
  VPortal *portal = fPortalStore.GetPortal(volume);
  const PortalType type = portal->GetPortalType();
  switch (type) {
    case PortalType::SimplePortal: {
      SimplePortal *simplePortal = static_cast<SimplePortal *>(portal);
      fLogger.WriteDebugInfo("Using SimplePortal " + simplePortal->GetName());
      simplePortal->DoPortation(step);
      break;
    }
    case PortalType::PeriodicPortal: {
      PeriodicPortal *periodicPortal = static_cast<PeriodicPortal *>(portal);
      fLogger.WriteDebugInfo("Using PeriodicPortal " +
                             periodicPortal->GetName());
      periodicPortal->DoPortation(step);
      break;
    }
    case PortalType::MultipleSubworld: {
      MultipleSubworld *multipleSubworld =
          static_cast<MultipleSubworld *>(portal);
      fLogger.WriteDebugInfo("Using MultipleSubworld " +
                             multipleSubworld->GetName());
      multipleSubworld->DoPortation(step);
      break;
    }
  }
  fJustPorted = true;
}

G4bool Surface::PortalControl::EnterPortalCheck(const G4Step *step) {
  const G4VPhysicalVolume *postVolume =
      step->GetPostStepPoint()->GetPhysicalVolume();
  if (fPortalStore.IsTrigger(postVolume)) {
    return true;
  }
  return false;
}

void Surface::PortalControl::UsePortal(G4Step *step) {
  const G4VPhysicalVolume *trigger =
      step->GetPostStepPoint()->GetPhysicalVolume();
  const G4int triggerId = fPortalStore.FindTrigger(trigger);
  const G4VPhysicalVolume *volume = fPortalStore.at(triggerId)->GetVolume();
  DoPortation(step, volume);
}

G4bool Surface::PortalControl::IsVolumeInsidePortal(
    const G4VPhysicalVolume *volume) const {
  G4LogicalVolume *motherLog = volume->GetMotherLogical();
  if (motherLog == nullptr) return false;
  return fPortalStore.IsLogPortal(motherLog);
}
