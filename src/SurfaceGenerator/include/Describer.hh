/**
 * @brief definition of class Describer
 * @file Describer.hh
 * @date 2023-06-01
 * @author C.Gruener
 */
#ifndef SRC_SURFACE_GENERATOR_INCLUDE_DESCRIBER_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_DESCRIBER_HH_

#include <vector>
#include "../../Service/include/Logger.hh"
#include "G4Transform3D.hh"
#include "SurfaceGenerator/include/RectangleDivider.hh"
#include "SurfaceGenerator/include/Storage.hh"

using Rectangle = Surface::RectangleDivider::Rectangle;

namespace Surface {
class DescriberMessenger;
/**
 * @brief The Describer class is used to define a rough surface based on parameters.
 * These parameters are then translated to G4Solid descriptions and their position.
 * @details The
 */
class Describer {
 public:
  enum class SpikeShape { StandardPyramid, UniformPyramid, Bump, Peak };

  struct DescriberParameters {
    G4double spikeWidth_X{1};
    G4double spikeWidth_Y{1};
    G4int nSpike_X{1};
    G4int nSpike_Y{1};
    G4double meanHeight{1};
    G4double heightDeviation{1};
    G4int nLayer{1};
    Describer::SpikeShape spikeShape{Describer::SpikeShape::StandardPyramid};
  };

 public:
  Describer() noexcept;
  explicit Describer(const DescriberParameters &params) noexcept;
  void Generate();

  //Setter
  void SetParameters(const DescriberParameters &params){fParams = params;}
  void SetSpikeWidth_X(G4double width){fParams.spikeWidth_X = width;}
  void SetSpikeWidth_Y(G4double width){fParams.spikeWidth_Y = width;}
  void SetNrSpike_X(G4int n){fParams.nSpike_X = n;}
  void SetNrSpike_Y(G4int n){fParams.nSpike_Y = n;}
  void SetMeanHeight(G4double height){fParams.meanHeight = height;}
  void SetHeightDeviation(G4double deviation){fParams.heightDeviation = deviation;}
  void SetSpikeform(SpikeShape shape){fParams.spikeShape = shape;}
  void SetNLayer(G4int n){fParams.nLayer = n;}

  //Getter
  G4String GetInfoDescription() const;
  std::vector<SolidDescription> GetSolidDescription() const;
  G4double GetSurfaceWidth_X() const { return fParams.spikeWidth_X * fParams.nSpike_X;}
  G4double GetSurfaceWidth_Y() const { return fParams.spikeWidth_Y * fParams.nSpike_Y;}
  G4double GetSpikeWidth_X() const { return fParams.spikeWidth_X; }
  G4double GetSpikeWidth_Y() const { return fParams.spikeWidth_Y; }
  G4int GetSpikeNr_X() const { return fParams.nSpike_X; }
  G4int GetSpikeNr_Y() const { return fParams.nSpike_Y; }
  G4double GetMeanHeight() const { return fParams.meanHeight; }
  G4double GetHeightDeviation() const { return fParams.heightDeviation; }
  G4int GetNLayer() const { return fParams.nLayer; }
  SpikeShape GetSpikeShape() const {return fParams.spikeShape; }

 private:
  std::vector<SolidDescription> GetSpikeDescription(const Rectangle &);
  std::vector<SolidDescription> GetStandardPyramid(const Rectangle &) const;
  std::vector<SolidDescription> GetUniformPyramid(const Rectangle &) const;
  std::vector<SolidDescription> GetBump(const Rectangle &) const;
  std::vector<SolidDescription> GetPeak(const Rectangle &) const;
  static G4Transform3D GetTransformation(const Rectangle &);
  Surface::RectangleDivider GetRectangle() const;
  void AppendDescriptionAtPosition(std::vector<SolidDescription> &,
                                   const G4Transform3D &);

  DescriberParameters fParams;
  std::vector<SolidDescription> fDescription;
  Surface::DescriberMessenger *fMessenger;
  Surface::Logger fLogger;
};
}
#endif
