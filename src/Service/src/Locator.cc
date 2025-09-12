/**
 * @brief Instantiates global FacetStore and Portal Store
 * @author C.Gruener
 * @date 2023-06-01
 * @file Locator.cc
 */

#include "Service/include/Locator.hh"

#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"

Surface::FacetStore Surface::Locator::fFacetStore{"Locator"};
Surface::PortalStore Surface::Locator::fPortalStore;