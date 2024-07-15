// Copyright [2024] C.Gruener
// Date: 23-06-01
// File: Describer to hold all information needed to build a rough surface
// object

#ifndef SRC_SURFACEGENERATOR_INCLUDE_DESCRIBER_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_DESCRIBER_HH_

#include <vector>

#include "../../Service/include/Logger.hh"
#include "G4Transform3D.hh"
#include "SurfaceGenerator/include/RectangleDivider.hh"
#include "SurfaceGenerator/include/Storage.hh"
using Rectangle = Surface::RectangleDivider::Rectangle;

namespace Surface {
class DescriberMessenger;

class Describer {
 public:
  enum class Spikeform { StandardPyramide, UniformPyramide, Bump, Peak };

 public:
  Describer() noexcept;
  void Generate();

  void SetSpikeWidth_X(G4double);
  void SetSpikeWidth_Y(G4double);
  void SetNrSpike_X(G4int);
  void SetNrSpike_Y(G4int);
  void SetMeanHeight(G4double);
  void SetHeightDeviation(G4double);
  void SetSpikeform(Spikeform);
  void SetNLayer(G4int);
  G4String GetInfoDescription() const;
  std::vector<SolidDescription> GetSolidDescription() const;
  inline G4double GetSurfaceWidth_X() const {
    return fSpikeWidth_X * fNSpike_X;
  }
  inline G4double GetSurfaceWidth_Y() const {
    return fSpikeWidth_Y * fNSpike_Y;
  }
  inline G4double GetSpikeWidth_X() const { return fSpikeWidth_X; }
  inline G4double GetSpikeWidth_Y() const { return fSpikeWidth_Y; }
  inline G4int GetSpikeNr_X() const { return fNSpike_X; }
  inline G4int GetSpikeNr_Y() const { return fNSpike_Y; }
  inline G4double GetMeanHeight() const { return fMeanHeight; }
  inline G4double GetHeightDeviation() const { return fHeightDeviation; }
  inline G4int GetNLayer() const { return fNLayer; }

 private:
  std::vector<SolidDescription> GetSpikeDescription(const Rectangle &);
  std::vector<SolidDescription> GetStandardPyramid(const Rectangle &);
  std::vector<SolidDescription> GetUniformPyramid(const Rectangle &);
  std::vector<SolidDescription> GetBump(const Rectangle &);
  std::vector<SolidDescription> GetPeak(const Rectangle &);
  G4Transform3D GetTransformation(const Rectangle &);
  Surface::RectangleDivider GetRectangle();
  void AppendDescriptionAtPosition(std::vector<SolidDescription> &,
                                   const G4Transform3D &);

  Surface::DescriberMessenger *fMessenger;
  Spikeform fOptionSpikeform{Spikeform::StandardPyramide};
  G4double fSpikeWidth_X{1};
  G4double fSpikeWidth_Y{1};
  G4int fNSpike_X{1};
  G4int fNSpike_Y{1};
  G4double fMeanHeight{1};
  G4double fHeightDeviation{1};
  G4int fNLayer{1};
  std::vector<SolidDescription> fDescription;
  Surface::Logger fLogger;
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_DESCRIBER_HH_
