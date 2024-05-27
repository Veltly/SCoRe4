// Author: C.Gruener
// Date: 24-05-24
// File: SimplePortal

// Simple Portal combines two Boxes
#ifndef SIMPLE_PORTAL_HH
#define SIMPLE_PORTAL_HH
#include "VPortal.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class SimplePortal : public VPortal {
private:
  G4ThreeVector TransformBetweenPortals(G4ThreeVector &vec);

public:
  void DoPortation(G4StepPoint *point);
  void SetOtherPortal(SimplePortal *otherPortal);

private:
  SimplePortal *fOtherPortal;
};

} // namespace Surface
#endif // SIMPLE_PORTAL_HH
