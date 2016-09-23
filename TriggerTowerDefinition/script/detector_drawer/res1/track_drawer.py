from ROOT import TEllipse, TVector3, TLine, TF1, TGraph
from math import cos,sin,tan,atan,pi,sinh,asin,sqrt,fabs

alpha_CMS = -87.72
max_radius_det = 115.
rad2deg = 180./pi

def computeRofZ(pt,eta,vx,vy,vz,z):
    Rin = sqrt(vx**2 + vy**2)
    R_curv = -alpha_CMS*pt
    tgtheta = 1/sinh(eta)
    return Rin + 2*R_curv*fabs(sin((z-vz)*tgtheta/(2*R_curv)))

def computeDeltaPhiofR(pt,r):
    R_curv = -alpha_CMS*pt
    return asin(r/(2*R_curv))

def points2phi(c, p): #need to take 2 TVector3
    dx = p[0]-c[0]
    if dx==0: return 90
    phi = atan( (p[1]-c[1])/dx )

    if dx<0:
        phi+= pi

    if phi<0:
        phi+= 2*pi

    phi *= 180./pi

    return phi


def DrawTrack_xy1(pt, eta, phi, vx, vy, vz, charge):

    print "phi0 = ",rad2deg*phi

    # print "Drawing.."
    z_end_barrel = 115.

    Vin = TVector3(vx,vy,vz)

    R_curv = -alpha_CMS*pt

    aux = TVector3(pt*cos(phi),pt*sin(phi),0)
    aux = aux * (1./aux.Mag())
    segno = 1 if (charge>0)  else -1

    ccr_displ = TVector3(segno*aux[1],-segno*aux[0],aux[2])

    ccr = Vin + R_curv*ccr_displ #center of curvature radius

    phi1 = points2phi(ccr, Vin)
    phi2 = 360.
    r_end_barrel = computeRofZ(pt,eta,vx,vy,vz,z_end_barrel)
    if r_end_barrel > max_radius_det :
        phi2 = phi1 - segno*rad2deg*120./R_curv
    else:
        phi2 = phi1 - segno*rad2deg*2*computeDeltaPhiofR(pt,r_end_barrel)

    if phi1>phi2:
        aux = phi1
        phi1=phi2
        phi2=aux

    if phi2-phi1>180:
        aux = phi2
        phi2=phi1
        phi1=aux-360

    print phi1, phi2
    particle_curve = TEllipse(ccr[0], ccr[1], R_curv, 0 , phi1, phi2)
    # print ccr[0]
    # print ccr[1]
    # print R_curv
    # print phi1
    # print phi2

    particle_curve.SetFillStyle(0)
    particle_curve.SetNoEdges()

    if charge < 0:
        particle_curve.SetLineColor(4)
        print "blu"
    else:
        particle_curve.SetLineColor(2)
        print "red"

    return particle_curve


def DrawTrack_xy2(pt, eta, phi, vx, vy, vz, charge):

    # print "Drawing.."
    z_begin_endcap = 130.
    z_end_endcap = 270.
    r_begin_endcap = computeRofZ(pt,eta,vx,vy,vz,z_begin_endcap)

    if r_begin_endcap < max_radius_det:
        Vin = TVector3(vx,vy,vz)

        R_curv = -alpha_CMS*pt

        aux = TVector3(pt*cos(phi),pt*sin(phi),0)
        aux = aux * (1./aux.Mag())
        segno = 1 if (charge>0)  else -1

        ccr_displ = TVector3(segno*aux[1],-segno*aux[0],aux[2])

        ccr = Vin + R_curv*ccr_displ #center of curvature radius

        phi1 = points2phi(ccr, Vin)
        phi2 = 360.
        r_end_endcap = computeRofZ(pt,eta,vx,vy,vz,z_end_endcap)
        if r_end_endcap > 115. :
            phi2 = phi1 - segno*rad2deg*120./R_curv
        else:
            phi2 = phi1 - segno*rad2deg*2*computeDeltaPhiofR(pt,r_end_endcap)

        phi1 = phi1 - segno*rad2deg*2*computeDeltaPhiofR(pt,r_begin_endcap)

        if phi1>phi2:
            aux = phi1
            phi1=phi2
            phi2=aux

        if phi2-phi1>180:
            aux = phi2
            phi2=phi1
            phi1=aux-360

        particle_curve = TEllipse(ccr[0], ccr[1], R_curv, 0 , phi1, phi2)
        # print ccr[0]
        # print ccr[1]
        # print R_curv
        # print phi1
        # print phi2

        particle_curve.SetFillStyle(0)
        particle_curve.SetNoEdges()

        if charge < 0:
            particle_curve.SetLineColor(4)
            print "blu"
        else:
            particle_curve.SetLineColor(2)
            print "red"

        return particle_curve

    else:
        return TEllipse(0, 0, 0, 0,0,0)



def DrawTrack_rz(pt, eta, phi, vx, vy, vz, charge):
        R_curv = -alpha_CMS*pt

        tgtheta = 1/sinh(eta)
        max_z = abs(2*max_radius_det/tgtheta) if tgtheta>0 else vz
        min_z = vz if tgtheta>0 else -abs(2*max_radius_det/tgtheta)

        # fun = TF1("fun","[0]*fabs(sin((x-[2])*[1]/[0])) + [3]",min_z,max_z)
        # fun.SetParameter(0, 2*R_curv)
        # fun.SetParameter(1, tgtheta)
        # fun.SetParameter(2, vz)
        # fun.SetParameter(3, sqrt(vx**2 + vy**2))

        def R_position_tmp(z):
            out = 2*R_curv*abs( sin( (z-vz)*tgtheta/(2*R_curv) ) ) + sqrt(vx**2 + vy**2)
            return out

        N_points = 50
        gr = TGraph(N_points)
        for i in range(N_points):
            z = min_z + i*(max_z-min_z)/float(N_points)
            gr.SetPoint(i,z,R_position_tmp(z))


        if charge < 0:
            gr.SetLineColor(4)
            print "blu"
        else:
            gr.SetLineColor(2)
            print "red"

        return gr
