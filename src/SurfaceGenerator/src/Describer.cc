/**
 * @brief Implementation of Describer class
 * @file Describer.cc
 * @author C.Gruener
 * @date 2023-06-01
 */

#include "SurfaceGenerator/include/Describer.hh"
#include <sstream>
#include <vector>
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "Randomize.hh"
#include "SurfaceGenerator/include/DescriberMessenger.hh"
#include "SurfaceGenerator/include/RectangleDivider.hh"
#include "SurfaceGenerator/include/Spike.hh"

Surface::Describer::Describer() noexcept
    : Describer(Surface::Describer::DescriberParameters{}){}

Surface::Describer::Describer(
    const Surface::Describer::DescriberParameters &params) noexcept
    : fParams(params), fMessenger(new DescriberMessenger(this)), fLogger("Describer") {
  fLogger.WriteInfo("initialized");
}
/**
 * @brief Generates the description of the rough surface in terms of G4Solids and transformation
 * @details generates a vector per
 */
void Surface::Describer::Generate() {
  fLogger.WriteDebugInfo("Generate description of surface");
  auto rectangle = GetRectangle();
  auto RectangleIter = rectangle.GetIterBegin();
  auto RectangleIterEnd = rectangle.GetIterEnd();
  while (RectangleIter != RectangleIterEnd) {
    Surface::RectangleDivider::Rectangle SingleRectangle = *RectangleIter;
    auto NewDescription = GetSpikeDescription(SingleRectangle);
    const auto NewTransformation = GetTransformation(SingleRectangle);
    AppendDescriptionAtPosition(NewDescription, NewTransformation);
    ++RectangleIter;
  }
}

G4Transform3D Surface::Describer::GetTransformation(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  const G4double X_Translate{(aRectangle.maxX + aRectangle.minX) / 2.};
  const G4double Y_Translate{(aRectangle.maxY + aRectangle.minY) / 2.};
  const G4ThreeVector translate{X_Translate, Y_Translate, 0};
  const G4RotationMatrix rotate{G4RotationMatrix()};
  const G4Transform3D transform{rotate, translate};
  return transform;
}

std::vector<Surface::SolidDescription> Surface::Describer::GetSpikeDescription(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  switch (fParams.spikeShape) {
    case SpikeShape::StandardPyramid:
      return GetStandardPyramid(aRectangle);
    case SpikeShape::UniformPyramid:
      return GetUniformPyramid(aRectangle);
    case SpikeShape::Bump:
      return GetBump(aRectangle);
    case SpikeShape::Peak:
      return GetPeak(aRectangle);
  }
}

void Surface::Describer::AppendDescriptionAtPosition(
    std::vector<Surface::SolidDescription> &aDescription,
    const G4Transform3D &aTransform) {
  for (auto &OldDescription : aDescription) {
    const G4ThreeVector newTranslation =
        OldDescription.Transform.getTranslation() + aTransform.getTranslation();
    const G4RotationMatrix newRotMatrix =
        OldDescription.Transform.getRotation() * aTransform.getRotation();
    const G4Transform3D newTransform =
        G4Transform3D{newRotMatrix, newTranslation};
    OldDescription.Transform = newTransform;
    fDescription.emplace_back(std::move(OldDescription));
  }
}

std::vector<Surface::SolidDescription> Surface::Describer::GetStandardPyramid(
    const Surface::RectangleDivider::Rectangle &aRectangle) const {
  const G4double Width_X{aRectangle.maxX - aRectangle.minX};
  const G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X / 2.,
                       Width_Y / 2., fParams.meanHeight, 1};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetUniformPyramid(
    const Surface::RectangleDivider::Rectangle &aRectangle) const {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  G4double height = G4RandGauss::shoot(fParams.meanHeight, fParams.heightDeviation);
  if (height <= 0.) {
    height = 1e-9; //minimum height is 1 nm
  }
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X / 2.,
                       Width_Y / 2., height, 1};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetBump(
    const Surface::RectangleDivider::Rectangle &aRectangle) const {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Bump, Width_X / 2.,
                       Width_Y / 2., fParams.meanHeight, fParams.nLayer};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetPeak(
    const Surface::RectangleDivider::Rectangle &aRectangle) const {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Peak, Width_X / 2.,
                       Width_Y / 2., fParams.meanHeight, fParams.nLayer};
  return Spike.GetSpikeDescription();
}

Surface::RectangleDivider Surface::Describer::GetRectangle() const {
  G4double SurfaceWidth_X{fParams.spikeWidth_X * fParams.nSpike_X};
  G4double SurfaceWidth_Y{fParams.spikeWidth_Y * fParams.nSpike_Y};
  Surface::RectangleDivider rectangle{1e-12, -SurfaceWidth_X, SurfaceWidth_X,
                                      -SurfaceWidth_Y, SurfaceWidth_Y};
  rectangle.SplitRectangleEven(fParams.nSpike_X, fParams.nSpike_Y);
  return rectangle;
}

std::vector<Surface::SolidDescription> Surface::Describer::GetSolidDescription()
    const {
  return fDescription;
}

G4String Surface::Describer::GetInfoDescription() const {
  std::stringstream stream;
  for (auto &description : fDescription) {
    G4String VolumeType;
    switch (description.VolumeType) {
      case Surface::SolidDescription::Solid::Box:
        VolumeType = "Box";
        break;
      case Surface::SolidDescription::Solid::Trd:
        VolumeType = "Trd";
        break;
    }
    stream << "Volume type: " << VolumeType << "\n";
    stream << "Volume parameter: ";
    for (auto &value : description.VolumeParameter) {
      stream << value << ", ";
    }
    stream << "\n";
    stream << "Outer surface: ";
    for (auto &value : description.OuterSurface) {
      stream << value << ", ";
    }
    stream << "\n";
  }

  return stream.str();
}
