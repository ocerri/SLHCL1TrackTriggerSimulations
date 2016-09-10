#!/usr/bin/env python

from rootdrawing import *
from ROOT import *
from TrackParametersToTT import *
from cebefo_style import *
import tempfile

donotdelete=[]

col = TColor.GetColor("#BA0000")
fcol = TColor.GetColor("#E58080")
col_res = TColor.GetColor("#008000")

nbins_eta = 250
nbins_phi = 200
nbins_curv = 200
nbins_vz = 200
min_phiTT = 0.1
max_phiTT = 2*pigreco/3
min_etaTT = -0.5
max_etaTT = 1.5

class OCOptionsIn:
    def __init__(self):
        self.infile = ""
        self.nentries = 1e5


# ______________________________________________________________________________
def use_TEfficiency():
    del ROOT.TEfficiency
    gROOT.ProcessLine(".L ./fixes/TEfficiency.cxx+")
    #gSystem.Load("./fixes/TEfficiency_cxx.so")

    global TEfficiency
    TEfficiency = ROOT.TEfficiency


# ______________________________________________________________________________
def drawer_book(options):
    histos = {}

    hname = "TrainigSample_PC"
    histos[hname] = TH2D(hname,"Curvature-#phi distribution of Training Sample Particles",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.)
    histos[hname].SetXTitle("#phi [rad]")
    histos[hname].SetYTitle("q/P_{T} [1/GeV]")
    histos[hname].SetMarkerColor(1)
    histos[hname].SetMarkerStyle(7)
    histos[hname].SetStats(0)

    hname = "TrainigSample_VE"
    histos[hname] = TH2D(hname,"Vertex position-#eta distribution of Training Sample Particles",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-25,25)
    histos[hname].SetXTitle("#eta")
    histos[hname].SetYTitle("Vertex Z position [cm]")
    histos[hname].SetMarkerColor(1)
    histos[hname].SetMarkerStyle(7)
    histos[hname].SetStats(0)


    hname = "Acceptance_PC"
    histos[hname] = TH2D(hname,"Curvature-#phi distribution of particles inside acceptance",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.)
    histos[hname].SetXTitle("#phi [rad]")
    histos[hname].SetYTitle("q/P_{T} [1/GeV]")
    histos[hname].SetMarkerColor(2)
    histos[hname].SetMarkerStyle(7)
    histos[hname].SetStats(0)



    hname = "Acceptance_VE"
    histos[hname] = TH2D(hname,"Vertex position-#eta distribution of particles inside acceptance",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-25,25)
    histos[hname].SetXTitle("#eta")
    histos[hname].SetYTitle("Vertex Z position [cm]")
    histos[hname].SetMarkerColor(2)
    histos[hname].SetMarkerStyle(7)
    histos[hname].SetStats(0)


    donotdelete.append(histos)
    return histos

def repr_cppvector(v):
    s = ",".join([str(x) for x in v])
    s = "[" + s + "]"
    return s

def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("trkParts_pt"     , 1)
    tree.SetBranchStatus("trkParts_eta"    , 1)
    tree.SetBranchStatus("trkParts_phi"    , 1)
    #tree.SetBranchStatus("trkParts_vx"     , 1)
    #tree.SetBranchStatus("trkParts_vy"     , 1)
    tree.SetBranchStatus("trkParts_vz"     , 1)
    tree.SetBranchStatus("trkParts_charge" , 1)
    tree.SetBranchStatus("trkParts_primary", 1)
    tree.SetBranchStatus("trkParts_signal" , 1)
    tree.SetBranchStatus("trkParts_pdgId"  , 1)
    #tree.SetBranchStatus("AMTTRoads_patternRef" , 1)
    #tree.SetBranchStatus("AMTTRoads_stubRefs"   , 1)
    #tree.SetBranchStatus("AMTTTracks_invPt"     , 1)
    #tree.SetBranchStatus("AMTTTracks_phi0"      , 1)
    #tree.SetBranchStatus("AMTTTracks_cottheta"  , 1)
    #tree.SetBranchStatus("AMTTTracks_z0"        , 1)
    tree.SetBranchStatus("AMTTTracks_pt"        , 1)
    tree.SetBranchStatus("AMTTTracks_eta"       , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"      , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"      , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"   , 1)
    tree.SetBranchStatus("AMTTTracks_tpId"      , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef", 1)
    tree.SetBranchStatus("AMTTTracks_stubRefs"  , 1)

    # Loop over events

    # options.verbose=1 #DEBUG line

    for ievt, evt in enumerate(tree):

        if (ievt == options.nentries):  break

        if   (ievt % 10000 == 0               ):  print "Processing event: %i" % ievt

        nparts_all = evt.trkParts_primary.size()
        trkparts = {}

        for ipart in xrange(nparts_all):

            charge  = evt.trkParts_charge [ipart]
            primary = evt.trkParts_primary[ipart]
            signal  = evt.trkParts_signal [ipart]

            if not (charge!=0 and primary):
                continue


            pt      = evt.trkParts_pt     [ipart]
            eta     = evt.trkParts_eta    [ipart]
            phi     = evt.trkParts_phi    [ipart]
            vz      = evt.trkParts_vz     [ipart]
            pdgId   = evt.trkParts_pdgId  [ipart]

            aux_TT = OCTrackParametersToTT(phi, charge/pt , eta, vz)


            trkparts[ipart] = (pt, eta, phi, vz, charge, pdgId, aux_TT)
            if options.verbose:  print ievt, "part ", ipart, trkparts[ipart]


        ntracks_all = evt.AMTTTracks_patternRef.size()
        trkparts_trigger = {}
        trkparts_trigger_vars = {}

        for itrack in xrange(ntracks_all):
            trigger = False

            synTpId  = evt.AMTTTracks_synTpId[itrack]
            if synTpId == 1:
                trigger = True

            patternRef = evt.AMTTTracks_patternRef[itrack]
            if not (patternRef < options.npatterns):
                trigger = False

            chi2     = evt.AMTTTracks_chi2[itrack]
            ndof     = evt.AMTTTracks_ndof[itrack]
            chi2Red  = chi2/ndof

            if not (chi2Red < options.maxChi2):
                trigger = False

            track_pt   = evt.AMTTTracks_pt      [itrack]
            track_eta  = evt.AMTTTracks_eta     [itrack]

            if trigger:
                tpId = evt.AMTTTracks_tpId[itrack]
                trkparts_trigger[tpId] = True
                trkparts_trigger_vars[tpId] = (track_pt, track_eta)

                if options.verbose and (tpId in trkparts):  print ievt, "track", itrack, track_pt, track_eta, repr_cppvector(evt.AMTTTracks_stubRefs[itrack])

        #for k, v in trkparts_trigger.iteritems():
        #    assert k in trkparts

        # ______________________________________________________________________
        # N-1 conditions

        for k, v in trkparts.iteritems():
            pt, eta, phi, vz, charge, pdgId, aux_TT = v
            trigger = k in trkparts_trigger

            if trigger:
                histos["Acceptance_VE"].Fill(eta,vz)
                histos["Acceptance_PC"].Fill(phi,charge/pt)

                if aux_TT==25:
                    histos["TrainigSample_VE"].Fill(eta,vz)
                    histos["TrainigSample_PC"].Fill(phi,charge/pt)


    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    cebefo_style()

    canvas_VE = TCanvas("canvas_VE","canvas_VE",800,600)
    donotdelete.append(canvas_VE)
    histos["Acceptance_VE"].Draw()
    histos["TrainigSample_VE"].Draw("SAME")
    # canvas.SaveAs

    canvas_PC = TCanvas("canvas_PC","canvas_PC",800,600)
    donotdelete.append(canvas_PC)
    histos["Acceptance_PC"].Draw()
    histos["TrainigSample_PC"].Draw("SAME")

    nameout = options.outdir+"13_TrainingVSAcc_"+options.SSConfig

    canvas_PC.SaveAs(nameout+"_PC.pdf")
    canvas_VE.SaveAs(nameout+"_VE.pdf")

    fout = TFile()
    fout.Open(nameout+".root","recreate")
    histos["Acceptance_VE"].Write()
    histos["TrainigSample_VE"].Write()
    histos["Acceptance_PC"].Write()
    histos["TrainigSample_PC"].Write()
    canvas_PC.Write()
    canvas_VE.Write()
    fout.Close()

    return



# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit(setgStyle=0)
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    gStyle.SetNdivisions(510, "Y")
    gStyle.SetEndErrorSize(0)
    use_TEfficiency()

    # Process
    histos = drawer_book(options)
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':
    NPatterns = {}
    NPatterns["sf1_nz4_L5x2"] = 1454700
    NPatterns["sf1_nz6_L5x2"] = 3282100
    NPatterns["sf1_nz8_L5x2"] = 6431800



    options = OCOptionsIn()
    options.SSConfig = "sf1_nz8_L5x2"
    options.nentries = 1e6
    options.infile = "/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/tt25_test/tracks_SingleMuonTest_PU0_"+options.SSConfig+".root"
    options.maxChi2 = 5
    options.verbose = 0
    options.npatterns = NPatterns[options.SSConfig]
    options.outdir = "~/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/AM_analysis_output/"

    if options.infile.endswith(".root"):
        # Create a temporary file with one line
        with tempfile.NamedTemporaryFile() as infile:
            infile.write(options.infile)
            infile.flush()
            options.tfilecoll = TFileCollection("fc", "", infile.name)
    else:
        options.tfilecoll = TFileCollection("fc", "", options.infile)
    # Call the main function
    main(options)
