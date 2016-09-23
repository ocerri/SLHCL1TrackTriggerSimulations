#include "TTStubReader.h"

#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _trigger_sector_map_h
  #define _trigger_sector_map_h
  #include "../data/trigger_sector_map_10_vz15_pt3.h"
#endif

#ifndef _TrackParametersToTT_h
  #define _TrackParametersToTT_h
  #include "TrackParametersToTT.C"
#endif

Int_t nbins_eta = 250;
Int_t nbins_phi = 200;
Int_t nbins_curv = 200;
Int_t nbins_vz = 200;
Double_t min_phiTT = 0.1;
Double_t max_phiTT = 2*pigreco/3;
Double_t max_vz = 25.;
Double_t min_etaTT = -0.5;
Double_t max_etaTT = 1.5;



Int_t is_in_TT(Int_t TT_number, Int_t module_number)
{
    for(Int_t i=0; i<(Int_t)trigger_sector_modules[TT_number].size(); i++)
    {
        if (module_number == trigger_sector_modules[TT_number][i]) return 1;
    }
    return 0;
}

void show_phase_space(Int_t TT_number=25, Long64_t nentries = 1e6, Int_t verbose = 2) {

    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/Demo_Emulator/stubs_tt27_300M_emu.root"; //tower 27 old
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt25_200M/stubs_tt25_1k.root";
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt25_200M/stubs_tt25_200M.root";
    TString filename = "/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/Configuration/input/input_SingleMuon_tt25_largeS.txt";
    // TString filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt25_large/SingleMuon_tt25_large_ntuple.1.root";

    // Open the file
    std::cout << "Opening file..." << std::endl;
    TTStubReader reader;
    reader.init(filename);
    if(nentries==-1 || reader.getEntries() < nentries) nentries = reader.getEntries();



    // Declaring histos
    TH1D * h_part_per_ev = new TH1D("h_part_per_ev","Particles per event",nentries,0,nentries);

    TH1D * h_stubs_per_ev = new TH1D("h_stubs_per_ev","Stubs per event",15,0,15);
    h_stubs_per_ev->SetXTitle("Stubs detected");
    h_stubs_per_ev->SetYTitle("Number of events");

    TH1D * h_layer_hitted = new TH1D("h_layer_hitted","Layer hitted per particle",15,0,15);
    h_layer_hitted->SetXTitle("Number of different layer hitted");
    h_layer_hitted->SetYTitle("Number of events");


    TH1D * h_part_phi = new TH1D("h_part_phi","Particles #phi distribution",360,-pigreco,pigreco);
    h_part_phi->SetXTitle("#phi [rad]");
    h_part_phi->SetYTitle("Entries");


    TH1D * h_part_eta = new TH1D("h_part_eta","Particles #eta distribution",200,-3.4,3.4);
    h_part_eta->SetXTitle("#eta");
    h_part_eta->SetYTitle("Entries");

    TH1D * h_part_pt = new TH1D("h_part_pt","Particles P_{T} distribution",300,1.5,40.);
    h_part_pt->SetXTitle("P_{T} [GeV]");
    h_part_pt->SetYTitle("Entries");

    TH1D * h_part_vz = new TH1D("h_part_vz","Particles vertex distribution",400,-25,25);
    h_part_vz->SetXTitle("vertex z [cm]");
    h_part_vz->SetYTitle("Entries");


    TH2D * h_part_det_inTT[2];
    h_part_det_inTT[0] = new TH2D("h_part_det_inTT_phi_curv","Curvature-#phi distribution of particles in TT",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.);
    h_part_det_inTT[0]->SetXTitle("#phi [rad]");
    h_part_det_inTT[0]->SetYTitle("q/P_{T} [1/GeV]");

    h_part_det_inTT[1] = new TH2D("h_part_det_inTT_vz_eta","Vertex position-#eta distribution of particles in TT",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-max_vz,max_vz);
    h_part_det_inTT[1]->SetXTitle("#eta");
    h_part_det_inTT[1]->SetYTitle("Vertex Z position [cm]");


    TH2D * h_part_det_outTT[2];
    h_part_det_outTT[0] = new TH2D("h_part_det_outTT_phi_curv","Curvature-#phi distribution of particles out TT",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.);
    h_part_det_outTT[0]->SetXTitle("#phi [rad]");
    h_part_det_outTT[0]->SetYTitle("q/P_{T} [1/GeV]");

    h_part_det_outTT[1] = new TH2D("h_part_det_outTT_vz_eta","Vertex position-#eta distribution of particles out TT",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-max_vz,max_vz);
    h_part_det_outTT[1]->SetXTitle("#eta");
    h_part_det_outTT[1]->SetYTitle("Vertex Z position [cm]");

    TH2D * h_part_det_all[2];
    h_part_det_all[0] = new TH2D("h_part_det_all_phi_curv","Curvature-#phi distribution of all particles",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.);
    h_part_det_all[0]->SetXTitle("#phi [rad]");
    h_part_det_all[0]->SetYTitle("q/P_{T} [1/GeV]");

    h_part_det_all[1] = new TH2D("h_part_det_all_vz_eta","Vertex position-#eta distribution of all particles",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-max_vz,max_vz);
    h_part_det_all[1]->SetXTitle("#eta");
    h_part_det_all[1]->SetYTitle("Vertex Z position [cm]");


    TH2D * h_part_undet[2];
    h_part_undet[0] = new TH2D("h_part_undet_phi_curv","Curvature-#phi distribution of discarded particles",nbins_phi,min_phiTT,max_phiTT,nbins_curv,-1.,1.);
    h_part_undet[0]->SetXTitle("#phi [rad]");
    h_part_undet[0]->SetYTitle("q/P_{T} [1/GeV]");

    h_part_undet[1] = new TH2D("h_part_undet_vz_eta","Vertex position-#eta distribution of undetected particles",nbins_eta,min_etaTT,max_etaTT,nbins_vz,-max_vz,max_vz);
    h_part_undet[1]->SetXTitle("#eta");
    h_part_undet[1]->SetYTitle("Vertex Z position [cm]");


    std::cout << Form("Number of events: %.2fM", nentries/1e6) << std::endl;


    for (Long64_t ievt = 0; ievt < nentries; ++ievt) {
        show_progress_bar(ievt, nentries);
        // Retrieve the event
        reader.getEntry(ievt);


        h_part_per_ev->SetBinContent(ievt+1,reader.vp_pt->size());

        // Get the variables
        // See TTStubReader.h for more info
        float pt     = reader.vp_pt    ->front();
        float eta    = reader.vp_eta   ->front();
        float phi    = reader.vp_phi   ->front();
        float vx     = reader.vp_vx    ->front();
        float vy     = reader.vp_vy    ->front();
        float vz     = reader.vp_vz    ->front();
        int   charge = reader.vp_charge->front();

        Double_t curv = charge/pt;
        Int_t aux_TT_number = TrackParametersToTT(phi, curv , eta, vz);

        h_part_phi->Fill(phi);
        h_part_eta->Fill(eta);
        h_part_pt->Fill(pt);
        h_part_vz->Fill(vz);


        if(verbose > 2)
        {
            std::cout << std::endl;
            std::cout << "ievt: " << ievt
                      << " muon pT: " << pt
                      << " eta: " << eta
                      << " phi: " << phi
                      << " vz: "  << vz
                      << " Npart: " << reader.vp_pt->size()
                      << std::endl;
        }

        // Get number of stubs
        unsigned nstubs = reader.vb_modId->size();
        h_stubs_per_ev->Fill(nstubs);

        Int_t N_stub_layer[23] = {0};

        for (unsigned istub=0; istub < nstubs; ++istub) {

            // Get the variables
            // See TTStubReader.h for more info
            float    stub_z        = reader.vb_z       ->at(istub);
            float    stub_r        = reader.vb_r       ->at(istub);
            float    stub_eta      = reader.vb_eta     ->at(istub);
            float    stub_phi      = reader.vb_phi     ->at(istub);
            float    stub_coordx   = reader.vb_coordx  ->at(istub);
            float    stub_coordy   = reader.vb_coordy  ->at(istub);
            float    stub_trigBend = reader.vb_trigBend->at(istub);
            unsigned stub_modId    = reader.vb_modId   ->at(istub);
            int      stub_tpId     = reader.vb_tpId    ->at(istub);

            N_stub_layer[stub_modId/10000]++;


            if(verbose > 2)
            {
              std::cout << ".. istub: " << istub
                        << " module ID: " << stub_modId
                        << " local phi: " << stub_coordx
                        << " z: " << stub_coordy
                        << " global phi: " << stub_phi
                        << " z: " << stub_z
                        << " r: " << stub_r
                        << std::endl;
            }

        }  // end loop over stubs

        Int_t N_hitted_layer=0;
        for(UInt_t kk=0; kk<23; kk++)
        {
            if(N_stub_layer[kk]>0) N_hitted_layer++;
        }

        h_layer_hitted->Fill(N_hitted_layer);

        if( N_hitted_layer>=5 )
        {
          if(TT_number == aux_TT_number)
          {
            h_part_det_inTT[0]->Fill(phi,curv);
            h_part_det_inTT[1]->Fill(eta,vz);
          }
          else
          {
            h_part_det_outTT[0]->Fill(phi,curv);
            h_part_det_outTT[1]->Fill(eta,vz);
          }
        }
        else
        {
          h_part_undet[0]->Fill(phi,curv);
          h_part_undet[1]->Fill(eta,vz);
        }



    }  // end loop over events

    h_part_det_all[0]->Add(h_part_det_inTT[0],h_part_det_outTT[0]);
    h_part_det_all[1]->Add(h_part_det_inTT[1],h_part_det_outTT[1]);





    //Saving in files
    Int_t last_sl = filename.Last('/');
    Int_t last_dot = filename.Last('.');
    TString namein = filename(last_sl+1, last_dot-1-last_sl);
    TString nameout = "_root/11_PhaseSpaceAnalysis_" + namein + ".root";
    cout << "OUTPUT: " << nameout.Data() << endl;
    TFile *f_out = TFile::Open(nameout,"recreate");

    //Printing Canvas
    // TCanvas* c_part_per_ev = new TCanvas("c_part_per_ev","c_part_per_ev",800,600);
    h_part_per_ev->Write();//Draw();
    // c_part_per_ev->SaveAs("_fig/00_part_per_ev.pdf");

    // TCanvas* c_stubs_per_ev = new TCanvas("c_stubs_per_ev","c_stubs_per_ev",800,600);
    h_stubs_per_ev->Write();//Draw();
    // c_stubs_per_ev->SaveAs("_fig/00_stubs_per_ev.pdf");

    h_layer_hitted->Write();


    // TCanvas* c_part_phi = new TCanvas("c_part_phi","c_part_phi",800,600);
    h_part_phi->Write();//Draw();
    // c_part_phi->SaveAs("_fig/00_part_phi.pdf");


    // TCanvas* c_part_eta = new TCanvas("c_part_eta","c_part_eta",800,600);
    h_part_eta->Write();//Draw();
    // c_part_eta->SaveAs("_fig/00_part_eta.pdf");


    // TCanvas* c_part_pt = new TCanvas("c_part_pt","c_part_pt",800,600);
    h_part_pt->Write();//Draw();
    // c_part_pt->SaveAs("_fig/00_part_pt.pdf");


    // TCanvas* c_part_vz = new TCanvas("c_part_vz","c_part_vz",800,600);
    h_part_vz->Write();//Draw();
    // c_part_vz->SaveAs("_fig/00_part_vz.pdf");

    h_part_det_all[0]->Write();
    h_part_det_all[1]->Write();

    h_part_det_inTT[0]->Write();
    h_part_det_inTT[1]->Write();

    h_part_det_outTT[0]->Write();
    h_part_det_outTT[1]->Write();

    h_part_undet[0]->Write();
    h_part_undet[1]->Write();

}  // end analyze()
