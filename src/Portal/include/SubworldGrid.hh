// Author: C.Gruener
// Date: 24-06-11
// File: SubworldGrid
// Info: Header only

#ifndef SUBWORLDGRID_HH
#define SUBWORLDGRID_HH

#include "../../Service/include/Logger.hh"
#include "G4Box.hh"
#include "Randomize.hh"
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>

namespace Surface {

template <class T> class SubworldGrid {
public:
  SubworldGrid(const G4int sizeX, const G4int sizeY, G4int verbose = 3)
      : fColumnSize(sizeY), fMaxX(sizeX), fMaxY(sizeY), fCurrentX(-1),
        fCurrentY(-1), fLogger(Logger{"SubworldGrid", verbose}) {
    // col major order
    fGrid = new T *[sizeX * sizeY];
    fLogger.WriteDebugInfo("SubworldGrid of size " +
                           std::to_string(sizeX * sizeY) + " initialized");
  }
  ~SubworldGrid() {
    delete[] fGrid;
  } // only destructs the array of pointers but not the Portals, they are
    // handled in the PortalStore

  void SetSubworld(const G4int x, const G4int y, T *subworld) {
    fLogger.WriteDebugInfo("Set Subworld " + subworld->GetName() + " at X: " +
                           std::to_string(x) + " Y: " + std::to_string(y) +
                           " at id: " + std::to_string(x * fColumnSize + y));
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
                 const G4int maxY) {
    std::stringstream ss;
    auto symbols = GetLegend();
    for (size_t x = minX; x < maxX; ++x) {
      for (size_t y = minY; y < maxY; ++y) {
        T *subworld = fGrid[x * fColumnSize + y];
        ss << symbols[subworld] << " ";
      }
      ss << "\n";
    }
    G4cout << ss.str() << G4endl;
  };

  const std::set<T *> GetUniqueSubworlds() {
    std::set<T *> uniqueSubworlds;
    const size_t N = fMaxX * fMaxY;
    for (size_t i = 0; i < N; ++i) {
      fLogger.WriteDebugInfo("Added " + fGrid[i]->GetName() + " to with id: " +
                             std::to_string(i) + " to unique set");
      uniqueSubworlds.insert(fGrid[i]);
    }
    return uniqueSubworlds;
  }

  void PrintUniqueSubworlds() {
    fLogger.WriteDebugInfo("Unique subworlds in grid");
    std::set<T *> unique = GetUniqueSubworlds();
    for (T *ele : unique) {
      fLogger.WriteDebugInfo(ele->GetName());
    }
  }

  void PrintStatistic() {
    std::map<T *, G4int> counter;
    const size_t N = fMaxX * fMaxY;
    for (size_t i = 0; i < N; ++i) {
      counter[fGrid[i]] += 1;
    }

    const G4double invertedSum =
        100. / std::accumulate(
                   std::begin(counter), std::end(counter), 0.,
                   [](const G4double value,
                      const typename std::map<T *, G4int>::value_type &ele) {
                     return value + ele.second;
                   });
    std::stringstream ss;
    ss << "Used subworlds in %\n";
    for (auto ele : counter) {
      ss << "---> " << ele.first->GetName() << " : " << ele.second * invertedSum
         << "% \n";
    }
    fLogger.WriteInfo(ss.str());
  };

  void PrintLegend() {
    std::stringstream ss;
    ss << "Legend: \n";
    for (auto &ele : GetLegend()) {
      ss << "    " << ele.second << " : " << ele.first->GetName() << "\n";
    }
    fLogger.WriteInfo(ss.str());
  }

private:
  std::map<T *, char> GetLegend() {
    std::set<T *> uniqueSubworlds = GetUniqueSubworlds();

    static char symbols[] = {'+', '#', '$', '%', '&', '*', 'O', 'H', '@', '='};
    G4int counter = 0;
    std::map<T *, char> legend;
    for (T *subworld : uniqueSubworlds) {
      legend[subworld] = symbols[counter];
      ++counter;
    }
    return legend;
  }

private:
  const G4int fColumnSize;
  const G4int fMaxX;
  const G4int fMaxY;
  G4int fCurrentX;
  G4int fCurrentY;
  Logger fLogger;

  T **fGrid;
};

template <class T> class HelperFillSubworldGrid {
public:
  HelperFillSubworldGrid(G4int verbose = 3)
      : fLogger(Logger{"HelperFillSubworldGrid", verbose}){};

  void AddAvailableSubworld(T *subworld, const G4double density) {
    std::stringstream ss;
    ss << "Added " << subworld->GetName() << " with density " << density
       << " to helper class";
    fLogger.WriteDebugInfo(ss.str());
    fAvailableSubworlds.push_back(subworld);
    fDensity.push_back(density);
  }

  void SetGridInSubworlds(SubworldGrid<T> *grid) {
    for (auto *subworld : fAvailableSubworlds) {
      subworld->SetGrid(grid);
    }
  }

  void FillGrid(SubworldGrid<T> *grid) {
    G4double sumOfProb{0};
    std::vector<G4double> probability;
    for (G4double &val : fDensity) {
      sumOfProb += val;
      probability.push_back(sumOfProb);
    }
    for (G4double &val : fDensity) {
      val = val / sumOfProb;
    }
    SetGridInSubworlds(grid);

    const G4int gridMaxX = grid->MaxX();
    const G4int gridMaxY = grid->MaxY();
    for (size_t x = 0; x < gridMaxX; ++x) {
      for (size_t y = 0; y < gridMaxY; ++y) {
        G4double random = G4UniformRand();
        for (size_t i = 0; i < probability.size(); ++i) {
          if (random <= probability.at(i)) {
            grid->SetSubworld(x, y, fAvailableSubworlds[i]);
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
  Logger fLogger;
};
} // namespace Surface
#endif // VPORTAL_HH
