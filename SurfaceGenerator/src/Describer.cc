//
//
//
//
//	author: C.Gruener
//
//

#include "../include/Describer.hh"
#include "../include/DescriberMessenger.hh"
#include "../include/RectangleDivider.hh"
#include "../include/Spike.hh"
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <Randomize.hh>
#include <sstream>
#include <vector>

Surface::Describer::Describer() noexcept
    : fMessenger(new DescriberMessenger(this)) {
  fLogger.WriteInfo("initialized");
};

void Surface::Describer::Generate() {
  fLogger.WriteDebugInfo("calling Generate");
  auto rectangle = GetRectangle();
  auto RectangleIter = rectangle.GetIterBegin();
  auto RectangleIterEnd = rectangle.GetIterEnd();
  while (RectangleIter != RectangleIterEnd) {
    Surface::RectangleDivider::Rectangle SingleRectangle = *RectangleIter;
    auto NewDescription = GetSpikeDescription(SingleRectangle);
    auto NewTransformation = GetTransformation(SingleRectangle);
    AppendDescriptionAtPosition(NewDescription, NewTransformation);
    ++RectangleIter;
  }
}

G4Transform3D Surface::Describer::GetTransformation(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  G4double X_Translate{(aRectangle.maxX + aRectangle.minX) / 2.};
  G4double Y_Translate{(aRectangle.maxY + aRectangle.minY) / 2.};
  G4ThreeVector translate{X_Translate, Y_Translate, 0};
  G4RotationMatrix rotate{G4RotationMatrix()};
  G4Transform3D transform{rotate, translate};
  return transform;
}

std::vector<Surface::SolidDescription> Surface::Describer::GetSpikeDescription(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  switch (fOptionSpikeform) {
  case Spikeform::StandardPyramide:
    return GetStandardPyramid(aRectangle);
  case Spikeform::UniformPyramide:
    return GetUniformPyramid(aRectangle);
  case Spikeform::Bump:
    return GetBump(aRectangle);
  case Spikeform::Peak:
    return GetPeak(aRectangle);
  }
}

void Surface::Describer::AppendDescriptionAtPosition(
    std::vector<Surface::SolidDescription> &aDescription,
    const G4Transform3D &aTransform) {
  for (auto &OldDescription : aDescription) {
    G4ThreeVector newTranslation =
        OldDescription.Transform.getTranslation() + aTransform.getTranslation();
    G4RotationMatrix newRotMatrix =
        OldDescription.Transform.getRotation() * aTransform.getRotation();
    G4Transform3D newTransform = G4Transform3D{newRotMatrix, newTranslation};
    OldDescription.Transform = newTransform;
    fDescription.emplace_back(std::move(OldDescription));
  }
}

std::vector<Surface::SolidDescription> Surface::Describer::GetStandardPyramid(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X/2., Width_Y/2.,
                       fMeanHeight, 1};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetUniformPyramid(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  G4double height = G4RandGauss::shoot(fMeanHeight, fHeightDeviation);
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X/2., Width_Y/2.,
                       height, 1};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetBump(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X/2., Width_Y/2.,
                       fMeanHeight, 1};
  return Spike.GetSpikeDescription();
}

std::vector<Surface::SolidDescription> Surface::Describer::GetPeak(
    const Surface::RectangleDivider::Rectangle &aRectangle) {
  G4double Width_X{aRectangle.maxX - aRectangle.minX};
  G4double Width_Y{aRectangle.maxY - aRectangle.minY};
  Surface::Spike Spike{Surface::Spike::Spikeform::Pyramid, Width_X/2., Width_Y/2.,
                       fMeanHeight, 1};
  return Spike.GetSpikeDescription();
}

Surface::RectangleDivider Surface::Describer::GetRectangle() {
  G4double SurfaceWidth_X{fSpikeWidth_X * fNSpike_X};
  G4double SurfaceWidth_Y{fSpikeWidth_Y * fNSpike_Y};
  Surface::RectangleDivider rectangle{1e-12, -SurfaceWidth_X, SurfaceWidth_X,
                                      -SurfaceWidth_Y, SurfaceWidth_Y};
  rectangle.SplitRectangleEven(fNSpike_X, fNSpike_Y);
  return rectangle;
}

void Surface::Describer::SetSpikeWidth_X(G4double aWidth) {
  fSpikeWidth_X = aWidth;
}

void Surface::Describer::SetSpikeWidth_Y(G4double aWidth) {
  fSpikeWidth_Y = aWidth;
}

void Surface::Describer::SetNrSpike_X(G4int aN) { fNSpike_X = aN; }

void Surface::Describer::SetNrSpike_Y(G4int aN) { fNSpike_Y = aN; }

void Surface::Describer::SetMeanHeight(G4double aHeight) {
  fMeanHeight = aHeight;
}

void Surface::Describer::SetHeightDeviation(G4double aDeviation) {
  fHeightDeviation = aDeviation;
}

void Surface::Describer::SetSpikeform(
    Surface::Describer::Spikeform aSpikeform) {
  fOptionSpikeform = aSpikeform;
}

std::vector<Surface::SolidDescription>
Surface::Describer::GetSolidDescription() const {
  return fDescription;
}

G4String Surface::Describer::GetInfoDescription() const {
  std::stringstream stream;
  for (auto &description : fDescription) {
    G4String VolumeType;
    switch (description.Volumetype) {
      case Surface::SolidDescription::Solid::Box:
        VolumeType = "Box";
      break;
      case Surface::SolidDescription::Solid::Trd:
      VolumeType = "Trd";
      break;
    }
    stream << "VolumeType: " << VolumeType << "\n";
    stream << "Volumeparameter: ";
    for(auto& value:description.Volumeparameter){
      stream << value << ", ";
    }
    stream << "\n";
    stream << "Outer surface: ";
    for(auto& value:description.OuterSurface){
      stream << value << ", ";
    }
    stream << "\n";
  }
  
  return stream.str();
}
