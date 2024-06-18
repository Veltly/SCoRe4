// Author: C.Gruener
// Date: 24-06-18
// File: MultiSubworldSampler

#ifndef MULTISUBWORLDSAMPLER_HH
#define MULTISUBWORLDSAMPLER_HH

#include "../../Service/include/VSampler.hh"

namespace Surface {

struct Coord {
  G4int x;
  G4int y;
};

class MultiSubworldSampler : public VSampler<Coord> {
private:
public:
private:
};

} // namespace Surface

#endif // MULTISUBWORLDSAMPLER
