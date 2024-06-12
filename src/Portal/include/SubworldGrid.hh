// Author: C.Gruener
// Date: 24-06-11
// File: SubworldGrid
// Info: Header only

#ifndef SUBWORLDGRID_HH
#define SUBWORLDGRID_HH

#include "G4Box.hh"
#include "Randomize.hh"
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>

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

  void PrintGrid(const G4int minX, const G4int maxX, const G4int minY,
                 const G4int maxY) const {
    std::stringstream ss;
    const auto symbols = GetLegend();
    for (size_t x = minX; x < maxX; ++x) {
      for (size_t y = minY; y < maxY; ++y) {
        ss << symbols[fGrid[x][y]] << " ";
      }
      ss << "\n";
    }
    G4cout << ss.str() << G4endl;
  };

  void PrintStatistic() const {
    std::set<T *> uniquesubworlds;
    for (auto &ele : fGrid) {
      uniquesubworlds.add(ele);
    }
    std::map<T *, G4int> counter;
    for (auto &subworld : uniquesubworlds) {
      counter[subworld] += 1;
    }

    const G4int invertedSum =
        100. / std::accumulate(
                   std::begin(counter), std::end(counter), 0.,
                   [](const G4double value,
                      const typename std::map<T *, G4int>::value_type &ele) {
                     return value + ele.second;
                   });

    std::stringstream ss;
    ss << "Used subworlds in %\n";
    for (auto &ele : counter) {
      ss << ele.first.GetName() << " : " << ele.second() * invertedSum
         << "% \n";
    }
    G4cout << ss.str() << G4endl;
  };

  void PrintLegend() {
    std::stringstream ss;
    for (auto &ele : GetLegend()) {
      ss << ele.second() << " : " << ele.first.GetName() << "\n";
    }
    G4cout << ss.str() << G4endl;
  }

private:
  std::map<T *, char> GetLegend() {
    std::set<T *> uniquesubworlds;
    for (auto &ele : fGrid) {
      uniquesubworlds.add(ele);
    }

    static char symbols[] = {'+', '#', '$', '%', '&', '*', 'O', 'H', '@', '='};
    G4int counter = 0;
    std::map<T *, char> legend;
    for (auto &subworld : uniquesubworlds) {
      legend[subworld] = symbols[counter];
      ++counter;
    }
    return legend;
  }

  char Symbol(const T *subworld, const std::set<std::string> uniqueSet) const {
    static char symbols[] = {'+', '#', '$', '%', '&', '*', 'O', 'H', '@', '='};
  }

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

  void FillGrid(SubworldGrid<T> &grid) {
    auto sumOfProbabilities =
        fDensity / std::accumulate(fDensity.cbegin(), fDensity.cend(),
                                   decltype(fDensity)::value_type(0));
    auto probability = std::inclusive_scan(fDensity.cbegin(), fDensity.cend(),
                                           decltype(fDensity)::value_type(0));
    probability /= sumOfProbabilities;
    const G4int gridMaxX = grid.MaxX();
    const G4int gridMaxY = grid.MaxY();
    for (size_t x = 0; x < gridMaxX; ++x) {
      for (size_t y = 0; y < gridMaxY; ++y) {
        G4double random = G4UniformRand();
        for (size_t i = 0; i < probability.size(); ++i) {
          if (random <= probability.at(i)) {
            grid.SetSubworld(x, y, fAvailableSubworlds[i]);
            break;
          }
        }
      }
    }
  }

private:
  std::vector<T *> fAvailableSubworlds; // available subworlds for filling grid
  std::vector<G4double>
      fDensity; // Expected density of subworld in grid. Value between 0 and 1
};
} // namespace Surface
#endif // VPORTAL_HH
