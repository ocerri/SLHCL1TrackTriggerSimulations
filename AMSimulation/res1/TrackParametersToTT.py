from math import pi,cos,sin,tan,atan,pi,sinh,asin,sqrt,fabs,asinh

alpha_CMS = -87.72

pigreco = pi

OCeta_max_TTdef = 2.2
OCvz_max_TTdef = 15
OCinvPT_max_TTdef = 1/3.
OCR_star_eta = 60.
OCR_star_phi = 90.

def OCEtaToEta_star(eta, vz, invPT):
   rho = alpha_CMS/invPT

   tg_theta_star = OCR_star_eta/( vz + 2*rho*sinh(eta)*asin( OCR_star_eta/(2*rho) ) )

   eta_star = asinh(1/tg_theta_star)
   return eta_star

def OCPhiToPhi_star(phi, invPT):
    rho = alpha_CMS/invPT
    phi_star = phi + asin(OCR_star_phi/(2*rho))
    return phi_star

def OCTrackParametersToTT(phi, invPT, eta, vz):
    N_phi_divisions = 8
    N_eta_divisions = 6

    eta_star = OCEtaToEta_star(eta,vz,invPT)

    if fabs(eta_star)>OCeta_max_TTdef or fabs(vz)>OCvz_max_TTdef or fabs(invPT) > OCinvPT_max_TTdef:
        return -1
    else:
        phi_star = OCPhiToPhi_star(phi,invPT)

        while phi_star>2*pigreco:
            phi_star -= 2*pigreco

        while phi_star<0:
            phi_star += 2*pigreco

        N_phi = phi_star / (2*pigreco/N_phi_divisions)


        N_eta = (eta_star + OCeta_max_TTdef) / (2*OCeta_max_TTdef/N_eta_divisions)

        return 8*N_eta + N_phi
