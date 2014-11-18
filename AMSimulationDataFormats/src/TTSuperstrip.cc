#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include <iostream>


namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss) {
    o << "id: " << ss.id << " bit: " << ss.bit << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTHit& hit) {
    o << "r: " << hit.r << " phi: " << hit.phi << " z: " << hit.z
      << " r_err: " << hit.rError << " phi_err: " << hit.phiError << " z_err: " << hit.zError
      << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const std::vector<TTHit>& hits) {
    for (std::vector<TTHit>::const_iterator it=hits.begin(); it!=hits.end(); ++it) {
        o << *it << std::endl;
    }
    return o;
}

}  // namespace slhcl1tt
