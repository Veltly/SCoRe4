// Author: C.Gruener
// Date: 24-05-24
// File: SimplePortal

#ifndef SIMPLE_PORTAL_HH
#define SIMPLE_PORTAL_HH
#include "VPortal.hh"

namespace Surface {

class SimplePortal : public VPortal {

public:
  void DoPortation(G4StepPoint *point);

private:
  SimplePortal *otherPortal;
};

} // namespace Surface
#endif // SIMPLE_PORTAL_HH
