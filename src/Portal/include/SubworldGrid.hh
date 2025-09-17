/**
 * @brief Implements SubworldGrid and HelperFillSubworldGrid template classes
 * @author C.Gruener
 * @date 2024-06-11
 * @file SubworldGrid.hh
 */

#ifndef SRC_PORTAL_INCLUDE_SUBWORLDGRID_HH
#define SRC_PORTAL_INCLUDE_SUBWORLDGRID_HH

#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

#include "Randomize.hh"
#include "Service/include/Logger.hh"

namespace Surface {
/**
 * @brief Implementation of SubworldGrid template
 * @details Provides a grid structure with pointers to objects T.
 * Stores position in grid.
 * @tparam T template object
 */
template <class T>
class SubworldGrid {
 public:

  SubworldGrid(const G4int sizeX, const G4int sizeY, const VerboseLevel verboseLvl = VerboseLevel::Default)
      : fColumnSize(sizeY),
        fMaxX(sizeX),
        fMaxY(sizeY),
        fCurrentX(-1),
        fCurrentY(-1),
        fLogger("SubworldGrid", verboseLvl) {
    // col major order
    fGrid = new T *[sizeX * sizeY];
    fLogger.WriteDebugInfo("SubworldGrid of size " +
                           std::to_string(sizeX * sizeY) + " initialized");
  }

  ~SubworldGrid() {
    delete[] fGrid;
  }  // only destructs the array of pointers but not the Portals, they are
     // handled in the PortalStore

  void SetSubworld(const G4int x, const G4int y, T *subworld) {
    fLogger.WriteDebugInfo("Set Subworld " + subworld->GetName() + " at X: " +
                           std::to_string(x) + " Y: " + std::to_string(y) +
                           " at id: " + std::to_string(x * fColumnSize + y));
    fGrid[x * fColumnSize + y] = subworld;
  }

  T *GetSubworld(const G4int x, const G4int y) const {
    return fGrid[x * fColumnSize + y];
  }

  T *GetSubworld() const { return GetSubworld(fCurrentX, fCurrentY); }

  inline G4int MaxX() const { return fMaxX; }
  inline G4int MaxY() const { return fMaxY; }
  inline G4int CurrentPosX() const { return fCurrentX; }
  inline G4int CurrentPosY() const { return fCurrentY; }

  inline void SetCurrentX(const G4int x) { fCurrentX = x; }
  inline void SetCurrentY(const G4int y) { fCurrentY = y; }

  inline void IncrX() { ++fCurrentX; }
  inline void DecrX() { --fCurrentX; }
  inline void IncrY() { ++fCurrentY; }
  inline void DecrY() { --fCurrentY; }

  std::stringstream StreamGrid(const G4int minX, const G4int maxX,
                               const G4int minY, const G4int maxY) const {
    std::stringstream ss;
    auto symbols = GetLegend();
    for (G4int x = minX; x < maxX; ++x) {
      for (G4int y = minY; y < maxY; ++y) {
        T *subworld = fGrid[x * fColumnSize + y];
        ss << symbols[subworld] << " ";
      }
      ss << "\n";
    }
    return ss;
  }

  void PrintGrid(const G4int minX, const G4int maxX, const G4int minY,
                 const G4int maxY) const {
    G4cout << StreamGrid(minX, maxX, minY, maxY).str() << G4endl;
  }

  std::set<T *> GetUniqueSubworlds() const {
    std::set<T *> uniqueSubworlds;
    const size_t N = fMaxX * fMaxY;
    for (size_t i = 0; i < N; ++i) {
      fLogger.WriteDebugInfo("Added " + fGrid[i]->GetName() + " to with id: " +
                             std::to_string(i) + " to unique set");
      uniqueSubworlds.insert(fGrid[i]);
    }
    return uniqueSubworlds;
  }

  std::stringstream StreamUniqueSubworlds() const {
    std::stringstream ss;
    ss << "Unique subworlds in grid\n";
    std::set<T *> unique = GetUniqueSubworlds();
    for (T *ele : unique) {
      ss << ele->GetName() << "\n";
    }
    return ss;
  }

  void PrintUniqueSubworlds() {
    G4cout << StreamUniqueSubworlds().str() << G4endl;
  }

  std::stringstream StreamStatistic() const {
    std::map<T *, G4int> counter;
    const size_t N = fMaxX * fMaxY;
    for (size_t i = 0; i < N; ++i) {
      counter[fGrid[i]] += 1;
    }
    const G4int sum = std::accumulate(
        std::begin(counter), std::end(counter), 0.,
        [](const G4double value,
           const typename std::map<T *, G4int>::value_type &ele) {
          return value + ele.second;
        });

    const G4double invertedSum = 100. / static_cast<G4double>(sum);

    std::stringstream ss;
    ss << "Used subworlds in % / total\n";
    ss << "\n";

    for (auto ele : counter) {
      ss << std::setw(20) << ele.first->GetName() << " : " << std::setw(10)
         << ele.second * invertedSum << " % " << std::setw(10) << ele.second
         << "\n";
    }

    ss << "\n";
    ss << "Total subworlds: " << sum << "\n";
    return ss;
  }

  void PrintStatistic() const { G4cout << StreamStatistic().str() << G4endl; }

  std::stringstream StreamLegend() const {
    std::stringstream ss;
    ss << "Legend: \n";
    for (auto &ele : GetLegend()) {
      ss << "    " << ele.second << " : " << ele.first->GetName() << "\n";
    }
    return ss;
  }

  void PrintLegend() { G4cout << StreamLegend().str() << G4endl; }

 private:
  std::map<T *, char> GetLegend() const {
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/**
 * @brief Helper class to fill SubworldGrid randomly with instances of T based on
 * a density
 * @tparam T template value
 */
template <class T>
class HelperFillSubworldGrid {
 public:
  explicit HelperFillSubworldGrid(const VerboseLevel verboseLvl = VerboseLevel::Default)
      : fLogger("HelperFillSubworldGrid", verboseLvl) {}

  void AddAvailableSubworld(T *subworld, const G4double density) {
    fLogger.WriteDetailInfo(std::stringstream()
                            << "Added " << subworld->GetName()
                            << " with density " << density
                            << " to helper class");
    fAvailableSubworlds.push_back(subworld);
    fDensity.push_back(density);
  }

  void SetGridInSubworlds(SubworldGrid<T> *grid) {
    for (auto *subworld : fAvailableSubworlds) {
      subworld->SetGrid(grid);
    }
    fLogger.WriteDetailInfo("Link Grid to Subworlds");
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
    fLogger.WriteDetailInfo(std::stringstream()
                            << "Start filling Grid of size:\n"
                            << "Nx : " << gridMaxX << "\n"
                            << "Ny : " << gridMaxY << "\n");
    for (G4int x = 0; x < gridMaxX; ++x) {
      for (G4int y = 0; y < gridMaxY; ++y) {
        const G4double random = G4UniformRand();
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
  std::vector<T *> fAvailableSubworlds;  // available subworlds for filling grid
  std::vector<G4double> fDensity;  // Expected density of subworld in grid. Value between 0 and 1
  Logger fLogger;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_SUBWORLDGRID_HH
