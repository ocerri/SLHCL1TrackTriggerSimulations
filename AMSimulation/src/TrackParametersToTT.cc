#include <cstdio>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "TrackParametersToTT.h"


double OCEtaToEta_star(double eta, double vz, double invPT)
{
  double rho = alpha_CMS/invPT;

  double tg_theta_star = OCR_star_eta/( vz + 2*rho*sinh(eta)*asin( OCR_star_eta/(2*rho) ) );

  double eta_star = asinh(1/tg_theta_star);
  return eta_star;
}

double OCPhiToPhi_star(double phi, double invPT)
{
    double rho = alpha_CMS/invPT;
    double phi_star = phi + asin(OCR_star_phi/(2*rho));
    return phi_star;
}

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

        while(phi_star<0)
        {
          phi_star += 2*pigreco;
        }

        int N_phi = phi_star / (2*pigreco/N_phi_divisions);


        int N_eta = (eta_star + OCeta_max_TTdef) / (2*OCeta_max_TTdef/N_eta_divisions);

        return 8*N_eta + N_phi;
    }

}
