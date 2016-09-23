#include "TTStubReader.h"

#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _trigger_sector_map_h
  #define _trigger_sector_map_h
  #include "../data/trigger_sector_map.h"
#endif

Double_t alpha_CMS = -87.72;
Double_t R_star = 58.89;

Int_t nbins_eta = 250;
Int_t nbins_phi = 200;
Int_t nbins_curv = 200;
Int_t nbins_vz = 200;
Double_t min_phiTT = 0.4;
Double_t max_phiTT = 2*pigreco/3;
Double_t max_vz = 25.;
Double_t min_etaTT = -0.3;
Double_t max_etaTT = 1.5;



Int_t is_in_TT(Int_t TT_number, Int_t module_number)
{
    for(Int_t i=0; i<(Int_t)trigger_sector_modules[TT_number].size(); i++)
    {
        if (module_number == trigger_sector_modules[TT_number][i]) return 1;
    }
    return 0;
}

void show_phase_space_explorer(Long64_t nentries = 1e6, Int_t number_TT=27, Int_t verbose = 2) {
    Int_t save_root_file = 1;
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/Demo_Emulator/stubs_tt27_300M_emu.root"; //tower 27 old
    TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";

    // Open the file
    std::cout << "Opening file..." << std::endl;
    TTStubReader reader;
    reader.init(filename);
    if(nentries==-1) nentries = reader.getEntries();


    // Declaring histos
    TH1D * h_part_per_ev = new TH1D("h_part_per_ev","Particles per event",nentries,0,nentries);

    TH1D * h_stubs_per_ev = new TH1D("h_stubs_per_ev","Stubs per event",15,0,15);
    h_stubs_per_ev->SetXTitle("Stubs detected");
    h_stubs_per_ev->SetYTitle("Number of events");


    TH1D * h_part_phi = new TH1D("h_part_phi","Particles #phi distribution",360,-pigreco,pigreco);
    h_part_phi->SetXTitle("#phi [rad]");
    h_part_phi->SetYTitle("Entries");

    TH1D * h_part_phi_star = new TH1D("h_part_phi_star","Particles #phi* distribution",360,-pigreco,pigreco);
    h_part_phi_star->SetXTitle("#phi^{*} [rad]");
    h_part_phi_star->SetYTitle("Entries");


    TH1D * h_part_eta = new TH1D("h_part_eta","Particles #eta distribution",200,-3.4,3.4);
    h_part_eta->SetXTitle("#eta");
    h_part_eta->SetYTitle("Entries");

    TH1D * h_part_eta_star = new TH1D("h_part_eta_star","Particles #eta* distribution",300,-3.,3.);
    h_part_eta_star->SetXTitle("#eta^{*} [rad]");
    h_part_eta_star->SetYTitle("Entries");

    TH1D * h_part_pt = new TH1D("h_part_pt","Particles P_{T} distribution",300,1.5,40.);
    h_part_pt->SetXTitle("P_{T} [GeV]");
    h_part_pt->SetYTitle("Entries");

    TH1D * h_part_vz = new TH1D("h_part_vz","Particles vertex distribution",400,-25,25);
    h_part_vz->SetXTitle("vertex z [cm]");
    h_part_vz->SetYTitle("Entries");


    // Get number of events
    // Long64_t nentries = 100;

    std::cout << Form("Number of events: %.2fM", nentries/1e6) << std::endl;

    // Loop over events
    // There is only one track per event
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
        Double_t rho = alpha_CMS*pt*charge;

        Double_t phi_star = phi + asin(R_star/(2*rho));
        Double_t tg_theta = 1/sinh(eta);
        Double_t tg_theta_star = tg_theta/(1+ vz*tg_theta/R_star);
        Double_t eta_star = asinh(1/tg_theta_star);

        // Get number of stubs
        unsigned nstubs = reader.vb_modId->size();
        h_stubs_per_ev->Fill(nstubs);

        Int_t N_TT_modules_hitted = 0;
        Int_t N_stub_layer[22] = {0};
        Int_t N_stub_tot = 0;

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
            N_stub_tot++;

            N_TT_modules_hitted += is_in_TT(number_TT,stub_modId);

        }  // end loop over stubs

        if(N_TT_modules_hitted>=6)
        {

          h_part_phi->Fill(phi);
          h_part_phi_star->Fill(phi_star);
          h_part_eta->Fill(eta);
          h_part_eta_star->Fill(eta_star);
          h_part_pt->Fill(pt);
          h_part_vz->Fill(vz);
        }




    }  // end loop over events
    //
    // h_part_det_all[0]->Add(h_part_det_inTT[0],h_part_det_outTT[0]);
    // h_part_det_all[1]->Add(h_part_det_inTT[1],h_part_det_outTT[1]);




    if(save_root_file)
    {
      //Saving in files
      TFile *f_out = TFile::Open(Form("_root/02_PS_global_TT%2d_%1.0fM.root",number_TT, nentries/1e6),"recreate");

      //Printing Canvas
      // TCanvas* c_part_per_ev = new TCanvas("c_part_per_ev","c_part_per_ev",800,600);
      h_part_per_ev->Write();//Draw();
      // c_part_per_ev->SaveAs("_fig/00_part_per_ev.pdf");

      // TCanvas* c_stubs_per_ev = new TCanvas("c_stubs_per_ev","c_stubs_per_ev",800,600);
      h_stubs_per_ev->Write();//Draw();
      // c_stubs_per_ev->SaveAs("_fig/00_stubs_per_ev.pdf");


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

      h_part_phi_star->Write();

      h_part_eta_star->Write();
    }


    cout << "TT" << number_TT << " --- Number of particles: " << h_part_phi->GetEntries() << endl;
    cout << "phi* mean: " << h_part_phi_star->GetMean() << endl;
    cout << "eta* mean: " << h_part_eta_star->GetMean() << endl;



}  // end analyze()
