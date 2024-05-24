//Author: C.Gruener
//Date: 24-05-24
//File: Surface.cc

#include "../include/SimpleSurface.hh"
#include "../include/Generator.hh"
#include "Service/include/G4Voxelizer_Green.hh"
#include "SurfaceGenerator/include/Assembler.hh"
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4MultiUnion.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>

G4LogicalVolume *Surface::SimpleSurface::GetRoughSurface(){
  SurfaceGenerator generator = SurfaceGenerator();
  auto describer = generator.GetDescriber();
  describer.SetNrSpike_X(spikeNrX);
  describer.SetNrSpike_Y(spikeNrY);
  describer.SetSpikeWidth_X(0.5 * sizeX/spikeNrX);
  describer.SetSpikeWidth_Y(0.5 * sizeY/spikeNrY);
  describer.SetMeanHeight(meanHeight);
  describer.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);
  generator.GenerateSurface();
  G4MultiUnion *munionSolid = static_cast<G4MultiUnion*>(generator.GetSolid());

  G4Box *basisSolid = new G4Box("Surface::Basis", 0.5 * sizeX, 0.5 * sizeY, 0.5 * basisSizeZ);
  G4ThreeVector basisTranslation{0,0,-0.5 * basisSizeZ};
  G4Transform3D basisTransform{G4RotationMatrix(), basisTranslation};
  munionSolid->AddNode(*basisSolid, basisTransform);

  G4Voxelizer_Green &voxelizer = (G4Voxelizer_Green&)(munionSolid->GetVoxels());
  voxelizer.SetMaxBoundary({10,10,2});
  voxelizer.Voxelize(munionSolid);

  G4LogicalVolume *munionLogic = new G4LogicalVolume(munionSolid, roughnessMaterial, "Surface::Patch");


  G4ThreeVector boundingMin, boundingMax;
  munionSolid->BoundingLimits(boundingMin, boundingMax);
  G4double subworldSizeZ = boundingMax.z() - boundingMin.z();
  G4Box *subworldSolid = new G4Box("Surface::Subworld" , 0.5 * sizeX, 0.5 * sizeY, 0.5 * subworldSizeZ);
  G4LogicalVolume *subworldLogic = new G4LogicalVolume(subworldSolid, subworldMaterial, "Surface::Subworld"); 

  G4ThreeVector roughnessPlacement = {0,0, - 0.5 * subworldSizeZ + basisSizeZ};
  new G4PVPlacement(0,
                  roughnessPlacement,
                  munionLogic,
                  "Surface::Roughness",
                  subworldLogic,
                  false,
                  0,
                  0);


  return subworldLogic;
};

G4LogicalVolume *Surface::SimpleSurface::GetSurface(){};

Surface::SimpleSurface::SimpleSurface(){};
