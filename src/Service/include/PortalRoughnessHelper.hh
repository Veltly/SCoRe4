// Copyright [2024] C.Gruener
// Date: 24-07-18
// File: Helper class to build a Portal with multiple subworlds including a
// roughness

#ifndef SRC_SERVICE_INCLUDE_PORTALROUGHNESSHELPER_HH_
#define SRC_SERVICE_INCLUDE_PORTALROUGHNESSHELPER_HH_

#include <vector>

#include "Service/include/MultiportalHelper.hh"
#include "Service/include/RoughnessHelper.hh"

namespace Surface {

class PortalRoughnessHelper {
 public:
  void SetMultiportalHelper();
  void AddRoughness();

 private:
 private:
  MultiportalHelper fMultiportalHelper;
  std::vector<RoughnessHelper> fRoughnessVec;
};

}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_PORTALROUGHNESSHELPER_HH_
