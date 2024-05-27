#include "../include/Locator.hh"
#include "../../Portal/include/PortalStore.hh"
#include "../include/Logger.hh"

Surface::FacetStore Surface::Locator::fFacetStore;
Surface::Logger Surface::Locator::fLogger{"StandardLogger", 3};
Surface::PortalStore Surface::Locator::fPortalStore;
