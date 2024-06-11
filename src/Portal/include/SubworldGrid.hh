// Author: C.Gruener
// Date: 24-06-11
// File: SubworldGrid
// Info: Header only

#ifndef SUBWORLDGRID_HH
#define SUBWORLDGRID_HH

#include "G4Box.hh"

namespace Surface {

template <class T> class SubworldGrid {
public:
  SubworldGrid(const G4int sizeX, const G4int sizeY)
      : fColumnSize(sizeX), fMaxX(sizeX), fMaxY(sizeY), fCurrentX(-1),
        fCurrentY(-1) {
    // col major order
    fGrid = new T *[sizeX * sizeY];
  }
  ~SubworldGrid() {
    delete[] fGrid;
  } // only destructs the array of pointers but not the Portals, they are
    // handled in the PortalStore

  void SetSubworld(const G4int x, const G4int y, T *subworld) {
    fGrid[x * fColumnSize + y] = subworld;
  };

  T *GetSubworld(const G4int x, const G4int y) const {
    return fGrid[x * fColumnSize + y];
  };

  T *GetSubworld() const { return GetSubworld(fCurrentX, fCurrentY); };

  inline G4int MaxX() const { return fMaxX; }
  inline G4int MaxY() const { return fMaxY; }
  inline G4int CurrentPosX() const { return fCurrentX; };
  inline G4int CurrentPosY() const { return fCurrentY; };

  inline void SetCurrentX(const G4int x) { fCurrentX = x; }
  inline void SetCurrentY(const G4int y) { fCurrentY = y; }

  inline void IncrX() { ++fCurrentX; }
  inline void DecrX() { --fCurrentX; }
  inline void IncrY() { ++fCurrentY; }
  inline void DecrY() { --fCurrentY; }

private:
  const G4int fColumnSize;
  const G4int fMaxX;
  const G4int fMaxY;
  G4int fCurrentX;
  G4int fCurrentY;

  T **fGrid;
};

template <class T> class HelperFillSubworldGrid {
public:
  HelperFillSubworldGrid(){};
  void AddAvailableSubworld(T *subworld, const G4double density) {
    fAvailableSubworlds.push_back(subworld);
    fDensity.push_back(density);
  }

  void FillGrid(SubworldGrid<T> *grid){};

private:
  std::vector<T *> fAvailableSubworlds; // available subworlds for filling grid
  std::vector<G4double>
      fDensity; // Expected density of subworld in grid. Value between 0 and 1
};
} // namespace Surface
#endif // VPORTAL_HH
