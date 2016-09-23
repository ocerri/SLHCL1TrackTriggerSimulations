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
  // #define R_star_eta_std 58.89
  #define R_star_eta_std 60.
  // #define R_star_phi_std 58.89
  #define R_star_phi_std 90.
#endif

Double_t R_star_eta = R_star_eta_std;
Double_t R_star_phi = R_star_phi_std;

Double_t eta_max_TTdef = 2.2;
Double_t vz_max_TTdef = 15;
Double_t invPT_max_TTdef = 1/3.;


Double_t EtaToEta_star(Double_t eta, Double_t vz, Double_t invPT)//Correggi! LA traiettoria e' alicoidale non una retta mannaggia
{
  if(R_star_eta == -1) return eta;
  Double_t rho = alpha_CMS/invPT;

  Double_t tg_theta_star = R_star_eta/( vz + 2*rho*sinh(eta)*asin( R_star_eta/(2*rho) ) );

  // Double_t tg_theta = 1/sinh(eta);
  // Double_t tg_theta_star = tg_theta/(1+ vz*tg_theta/R_star);
  Double_t eta_star = asinh(1/tg_theta_star);
  return eta_star;
}

Double_t PhiToPhi_star(Double_t phi, Double_t invPT)
{
    if(R_star_phi==-1) return phi;
    Double_t rho = alpha_CMS/invPT;
    Double_t phi_star = phi + asin(R_star_phi/(2*rho));
    return phi_star;
}

Int_t TTNumberToPhiNumber(Int_t TT_number)
{
    if(TT_number>=0 && TT_number<=48) return 1+TT_number%8;
    else return -1;
}

Int_t TTNumberToEtaNumber(Int_t TT_number)
{
    if(TT_number>=0 && TT_number<=48) return 1+TT_number/8;
    else return -1;
}

Int_t DivNumberToTTNumber(Int_t eta_div, Int_t phi_div){
  if(eta_div<1 || eta_div>6 || phi_div<1 || phi_div>8) return -1;
  return 8*(eta_div-1)+ phi_div-1;
}

Int_t TrackParametersToTT(Double_t phi, Double_t invPT, Double_t eta, Double_t vz)
{
    //invPT is q/PT
    Double_t N_phi_divisions = 8;
    Double_t N_eta_divisions = 6;

    Double_t eta_star = EtaToEta_star(eta,vz,invPT);

    if(fabs(eta_star)>eta_max_TTdef || fabs(vz)>vz_max_TTdef || fabs(invPT) > invPT_max_TTdef)//Il controllo deve stare su eta_star probabilmente
    {
        return -1;
    }
    else
    {
        Double_t phi_star = PhiToPhi_star(phi,invPT);

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

        // UInt_t N_phi = (phi_star + pigreco/2) / (2*pigreco/N_phi_divisions);
        UInt_t N_phi = phi_star / (2*pigreco/N_phi_divisions);


        UInt_t N_eta = (eta_star + eta_max_TTdef) / (2*eta_max_TTdef/N_eta_divisions);
        // UInt_t N_eta = 3*((eta>0)?1:0) + fabs(eta)/(2*eta_max_TTdef/N_eta_divisions); //equivalent

        return 8*N_eta + N_phi;
    }

}

Double_t calcIdealPhi(Double_t simPhi, Double_t simChargeOverPt, Double_t r) {
    static const Double_t mPtFactor = 0.3*3.8*1e-2/2.0;
    //return simPhi - mPtFactor * r * simChargeOverPt;
    return simPhi - asin(mPtFactor * r * simChargeOverPt);
}

Double_t calcIdealZ(Double_t simVz, Double_t simCotTheta, Double_t simChargeOverPt, Double_t r=R_star_eta) {
    static const Double_t mPtFactor = 0.3*3.8*1e-2/2.0;
    //return simVz + r * simCotTheta;
    return simVz + (1.0 / (mPtFactor * simChargeOverPt) * asin(mPtFactor * r * simChargeOverPt)) * simCotTheta;
}

Double_t EtaToEta_star_JF(Double_t eta, Double_t vz, Double_t invPT)
{
    Double_t z_star = calcIdealZ(vz,sinh(eta),invPT);
    Double_t eta_star = asinh(z_star/R_star_eta);

    return eta_star;
}


Int_t old2new_TTnumber(Int_t old)
{
    Int_t eta = TTNumberToEtaNumber(old);
    Int_t phi = TTNumberToPhiNumber(old);

    phi = 1+(phi+6)%8;

    return DivNumberToTTNumber(eta,phi);

}
