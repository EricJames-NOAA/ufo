/*
 * (C) Copyright 2017-2018 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "ObsGnssroTLAD.h"

#include <ostream>
#include <string>

#include <boost/scoped_ptr.hpp>

#include "oops/base/Variables.h"
#include "oops/util/Logger.h"
#include "ioda/ObsSpace.h"
#include "ioda/ObsVector.h"
#include "ufo/GeoVaLs.h"
#include "ufo/ObsBias.h"
#include "ufo/ObsBiasIncrement.h"

namespace ufo {

// -----------------------------------------------------------------------------
static LinearObsOperatorMaker<ObsGnssroRefTLAD> makerGnssroRefTL_("GnssroRef");
// -----------------------------------------------------------------------------

ObsGnssroRefTLAD::ObsGnssroRefTLAD(const ioda::ObsSpace & odb, const eckit::Configuration & config)
  : keyOperGnssroRef_(0), varin_(), odb_(odb)
{
  const eckit::Configuration * configc = &config;
  ufo_gnssro_tlad_setup_f90(keyOperGnssroRef_, &configc);
const std::vector<std::string> vv{"temperature", "humidity_mixing_ratio", "air_pressure","geopotential_height"};
  varin_.reset(new oops::Variables(vv));
  oops::Log::trace() << "ObsGnssroRefTLAD created" << std::endl;
}

// -----------------------------------------------------------------------------

ObsGnssroRefTLAD::~ObsGnssroRefTLAD() {
  ufo_gnssro_tlad_delete_f90(keyOperGnssroRef_);
  oops::Log::trace() << "ObsGnssroRefTLAD destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGnssroRefTLAD::setTrajectory(const GeoVaLs & geovals, const ObsBias & bias) {
  ufo_gnssro_tlad_settraj_f90(keyOperGnssroRef_, geovals.toFortran(), odb_.toFortran());
}

// -----------------------------------------------------------------------------

void ObsGnssroRefTLAD::simulateObsTL(const GeoVaLs & geovals, ioda::ObsVector & ovec,
                                      const ObsBiasIncrement & bias) const {
  ufo_gnssro_ref_tlad_tl_f90(keyOperGnssroRef_, geovals.toFortran(), odb_.toFortran(), ovec.toFortran());
}

// -----------------------------------------------------------------------------

void ObsGnssroRefTLAD::simulateObsAD(GeoVaLs & geovals, const ioda::ObsVector & ovec,
                                      ObsBiasIncrement & bias) const {
  ufo_gnssro_ref_tlad_ad_f90(keyOperGnssroRef_, geovals.toFortran(), odb_.toFortran(), ovec.toFortran());
}

// -----------------------------------------------------------------------------

void ObsGnssroRefTLAD::print(std::ostream & os) const {
  os << "ObsGnssroRefTLAD::print not implemented" << std::endl;
}

// -----------------------------------------------------------------------------

}  // namespace ufo