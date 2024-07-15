// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "Service/include/Locator.hh"

#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"

Surface::FacetStore Surface::Locator::fFacetStore;
Surface::Logger Surface::Locator::fLogger{"StandardLogger", 3};
Surface::PortalStore Surface::Locator::fPortalStore;
