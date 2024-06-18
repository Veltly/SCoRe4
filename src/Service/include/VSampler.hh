// Author: C.Gruener
// Date: 24-06-18
// File: VSampler

#ifndef MULTIPLESUBWORLDSAMPLER_HH
#define MULTIPLESUBWORLDSAMPLER_HH
#include "../../Service/include/Logger.hh"
#include "Randomize.hh"
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

namespace Surface {
template <class T> class VSampler {
private:
  void PrepareProbability() {
    if (fIsClosed) {
      return;
    }
    G4double counter{0};
    for (G4double &val : fProbability) {
      counter += val;
    }
    G4double prob{0};
    for (G4double &val : fProbability) {
      prob += val / counter;
      val = prob;
    }
    fIsClosed = true;
  }

public:
  VSampler(const G4String name = "VSampler", const G4int verbose = 3)
      : fIsClosed(false), fLogger({name, verbose}), fName(name){};

  void AppendValue(const T value) {
    if (!fIsClosed) {
      fValues.push_back(value);
    } else {
      fLogger.WriteError(
          "AppendValue(..) not possible, Sampler already closed");
      exit(EXIT_FAILURE);
    }
  }

  void AppendProbability(const G4double prob) {
    if (!fIsClosed) {
      fProbability.push_back(prob);
    } else {
      fLogger.WriteError("AppendProbability(" + std::to_string(prob) +
                         ") not possible, Sampler already closed");
      exit(EXIT_FAILURE);
    }
  }

  T GetRandom() {
    if (!fIsClosed) {
      PrepareProbability();
    }
    G4double random = G4UniformRand();
    for (size_t i = 0; i < fProbability.size(); ++i) {
      if (random <= fProbability.at(i)) {
        return fValues.at(i);
      }
    }
    fLogger.WriteError("GetRandom() returned nothing!");
    exit(EXIT_FAILURE);
  }

  void PrintSampler() {
    if (!fIsClosed) {
      fLogger.WriteError("Sampler not finished, can not be printed");
      return;
    }
    std::stringstream ss;
    ss << "Printing Sampler: " << fName << "\n";
    for (size_t i = 0; i < fProbability.size(); ++i) {
      ss << "Probability: " << fProbability.at(i) << "\n";
    }
    fLogger.WriteInfo(ss.str());
  }

private:
  std::vector<G4double> fProbability;
  std::vector<T> fValues;
  G4bool fIsClosed;
  Logger fLogger;
  const G4String fName;
};
} // namespace Surface
#endif // MULTIPLESUBWORLDSAMPLER_HH
