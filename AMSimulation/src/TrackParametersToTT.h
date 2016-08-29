#include <cstdio>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef pigreco
  #define pigreco 3.141592653589793
#endif

#ifndef alpha_CMS
  #define alpha_CMS -87.72
#endif

#ifndef R_star_std_
  #define R_star_std_
  #define OCR_star_eta_std 60.
  #define OCR_star_phi_std 90.

  #define OCeta_max_TTdef 2.2
  #define OCvz_max_TTdef 15
  #define OCinvPT_max_TTdef 1/3.

  #define OCR_star_eta 60.
  #define OCR_star_phi 90.
#endif


#ifndef _TrackParametersToTT_fun
#define _TrackParametersToTT_fun

  double OCEtaToEta_star(double eta, double vz, double invPT);

  double OCPhiToPhi_star(double phi, double invPT);


  int OCTrackParametersToTT(double phi, double invPT, double eta, double vz);

#endif
