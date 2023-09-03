//
//
//
//
//      Author: cgruner
//

#ifndef SURFACE_FACETSTORE
#define SURFACE_FACETSTORE

#include <G4ThreeVector.hh>
#include <vector>
#include <G4TriangularFacet.hh>

namespace Surface {

/**
 *Holds a Store for Facets. All Facets together represent the surface of a
 *geometry placed in the global volume. It is possible to choose random points
 *on this surface which are normally distributed.
 */
class FacetStore {
private:
  struct FacetEdges{
    G4String edgeAB, edgeBC, edgeCA;
  };

public:
  FacetStore();
  void CloseFacetStore();
  G4ThreeVector GetRandomPoint() const;
  void AppendToFacetVector(const G4TriangularFacet&);
  void DrawFacets();
  inline G4bool GetIsStoreClosed() const { return fClosed;};

private:
  void CalculateFacetProbability();
  FacetEdges GetFacetLines(const G4TriangularFacet&);
  std::vector<G4TriangularFacet> fFacetVector;
  std::vector<G4double> fFacetProbability;
  G4bool fClosed{false};
};
} //namespace Surface
#endif // SURFACE_FACETSTORE
