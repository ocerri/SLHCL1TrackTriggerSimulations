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

#ifndef R_star_std
  // #define OCR_star_eta_std 58.89
  #define OCR_star_eta_std 60.
  // #define OCR_star_phi_std 58.89
  #define OCR_star_phi_std 90.
#endif



#ifndef _TrackParametersToTT_fun
#define _TrackParametersToTT_fun


double OCR_star_eta = OCR_star_eta_std;
double OCR_star_phi = OCR_star_phi_std;

double OCeta_max_TTdef = 2.2;
double OCvz_max_TTdef = 15;
double OCinvPT_max_TTdef = 1/3.;

double OCEtaToEta_star(double eta, double vz, double invPT)//Correggi! LA traiettoria e' alicoidale non una retta mannaggia
{
  double rho = alpha_CMS/invPT;

  double tg_theta_star = OCR_star_eta/( vz + 2*rho*sinh(eta)*asin( OCR_star_eta/(2*rho) ) );

  // double tg_theta = 1/sinh(eta);
  // double tg_theta_star = tg_theta/(1+ vz*tg_theta/R_star);
  double eta_star = asinh(1/tg_theta_star);
  return eta_star;
}

double OCPhiToPhi_star(double phi, double invPT)
{
    double rho = alpha_CMS/invPT;
    double phi_star = phi + asin(OCR_star_phi/(2*rho));
    return phi_star;
}

// int TTNumberToPhiNumber(int TT_number)
// {
//     if(TT_number>=0 && TT_number<=48) return 1+TT_number%8;
//     else return -1;
// }
//
// int TTNumberToEtaNumber(int TT_number)
// {
//     if(TT_number>=0 && TT_number<=48) return 1+TT_number/8;
//     else return -1;
// }
//
// int DivNumberToTTNumber(int eta_div, int phi_div){
//   if(eta_div<1 || eta_div>6 || phi_div<1 || phi_div>8) return -1;
//   return 8*(eta_div-1)+ phi_div-1;
// }

int OCTrackParametersToTT(double phi, double invPT, double eta, double vz)
{
    //invPT is q/PT
    double N_phi_divisions = 8;
    double N_eta_divisions = 6;

    double eta_star = OCEtaToEta_star(eta,vz,invPT);

    if(fabs(eta_star)>OCeta_max_TTdef || fabs(vz)>OCvz_max_TTdef || fabs(invPT) > OCinvPT_max_TTdef)//Il controllo deve stare su eta_star probabilmente
    {
        return -1;
    }
    else
    {
        double phi_star = OCPhiToPhi_star(phi,invPT);

        while(phi_star>2*pigreco)
        {
          phi_star -= 2*pigreco;
        }

        // while(phi_star<-pigreco/2)
        // {
        //   phi_star += 2*pigreco;
        // }
        while(phi_star<0)
        {
          phi_star += 2*pigreco;
        }

        // Uint N_phi = (phi_star + pigreco/2) / (2*pigreco/N_phi_divisions);
        int N_phi = phi_star / (2*pigreco/N_phi_divisions);


        int N_eta = (eta_star + OCeta_max_TTdef) / (2*OCeta_max_TTdef/N_eta_divisions);
        // Uint N_eta = 3*((eta>0)?1:0) + fabs(eta)/(2*OCeta_max_TTdef/N_eta_divisions); //equivalent

        return 8*N_eta + N_phi;
    }

}

#endif
// double calcIdealPhi(double simPhi, double simChargeOverPt, double r) {
//     static const double mPtFactor = 0.3*3.8*1e-2/2.0;
//     //return simPhi - mPtFactor * r * simChargeOverPt;
//     return simPhi - asin(mPtFactor * r * simChargeOverPt);
// }
//
// double calcIdealZ(double simVz, double simCotTheta, double simChargeOverPt, double r=OCR_star_eta) {
//     static const double mPtFactor = 0.3*3.8*1e-2/2.0;
//     //return simVz + r * simCotTheta;
//     return simVz + (1.0 / (mPtFactor * simChargeOverPt) * asin(mPtFactor * r * simChargeOverPt)) * simCotTheta;
// }
//
// double EtaToEta_star_JF(double eta, double vz, double invPT)
// {
//     double z_star = calcIdealZ(vz,sinh(eta),invPT);
//     double eta_star = asinh(z_star/OCR_star_eta);
//
//     return eta_star;
// }
//
//
// int old2new_TTnumber(int old)
// {
//     int eta = TTNumberToEtaNumber(old);
//     int phi = TTNumberToPhiNumber(old);
//
//     phi = 1+(phi+6)%8;
//
//     return DivNumberToTTNumber(eta,phi);
//
// }
