//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_DESCRIBER
#define SURFACE_DESCRIBER

#include "RectangleDivider.hh"
#include "Storage.hh"
#include <G4Transform3D.hh>

namespace Surface {

class Describer {
  enum class Spikeform { StandardPyramide, UniformPyramide, Bump, Peak };

public:
  void Generate();

  void SetSpikeWidth_X(G4double);
  void SetSpikeWidth_Y(G4double);
  void SetSpikeNr_X(G4int);
  void SetSpikeNr_Y(G4int);
  void SetMeanHeight(G4double);
  void SetHeightDeviation(G4double);
  void SetSpikeform(Spikeform);

  std::vector<SolidDescription> GetSolidDescription() const;
  inline G4double GetSurfaceWidth_X() const {
    return fSpikeWidth_X * fNSpike_X;
  };
  inline G4double GetSurfaceWidth_Y() const {
    return fSpikeWidth_Y * fNSpike_Y;
  };
  inline G4double GetSpikeWidth_X() const { return fSpikeWidth_X; };
  inline G4double GetSpikeWidth_Y() const { return fSpikeWidth_Y; };
  inline G4int GetSpikeNr_X() const { return fNSpike_X; };
  inline G4int GetSpikeNr_Y() const { return fNSpike_Y; };
  inline G4double GetMeanHeight() const { return fMeanHeight; };
  inline G4double GetHeightDeviation() const { return fHeightDeviation; };

private:
  std::vector<Surface::SolidDescription>
  GetSpikeDescription(const Surface::RectangleDivider::Rectangle &);
  std::vector<Surface::SolidDescription>
  GetStandardPyramid(const Surface::RectangleDivider::Rectangle &);
  std::vector<Surface::SolidDescription>
  GetUniformPyramid(const Surface::RectangleDivider::Rectangle &);
  std::vector<Surface::SolidDescription>
  GetBump(const Surface::RectangleDivider::Rectangle &);
  std::vector<Surface::SolidDescription>
  GetPeak(const Surface::RectangleDivider::Rectangle &);
  G4Transform3D GetTransformation(const Surface::RectangleDivider::Rectangle &);
  Surface::RectangleDivider GetRectangle();
  void AppendDescriptionAtPosition(std::vector<SolidDescription> &,
                                   const G4Transform3D &);
  Spikeform fOptionSpikeform{Describer::Spikeform::StandardPyramide};
  G4double fSpikeWidth_X{1};
  G4double fSpikeWidth_Y{1};
  G4int fNSpike_X{1};
  G4int fNSpike_Y{1};
  G4double fMeanHeight{1};
  G4double fHeightDeviation{1};
  G4int fNLayer{1};
  std::vector<SolidDescription> fDescription;
};
} // namespace Surface
#endif
