#include "TTStubReader.h"

#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _TrackParametersToTT_h
  #define _TrackParametersToTT_h
  #include "TrackParametersToTT.C"
#endif

Int_t nbins_eta = 500;
Int_t nbins_phi = 360;
Int_t nbins_curv = 100;
Int_t nbins_vz = 100;
Double_t min_phiTT = 2*pigreco;
Double_t max_phiTT = -2*pigreco;
Double_t max_vz = 25.;
Double_t min_etaTT = -2.5;
Double_t max_etaTT = 2.5;


void ShowTriggerDefinitionMap(Long64_t nentries = 1e6, Int_t TT_number=27)
{
    cebefo_style();
    TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";

    // Open the file
    std::cout << "Opening file..." << std::endl;
    TTStubReader reader;
    reader.init(filename);
    if(nentries==-1) nentries = reader.getEntries();

    //Defining histos
    TH2D * h_part_inTT[2];
    h_part_inTT[0] = new TH2D(Form("h_part_inTT%d_curv_phi",TT_number),Form("Curvature-#phi distribution of particles in TT%d",TT_number),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
    h_part_inTT[0]->SetXTitle("#phi [rad]");
    h_part_inTT[0]->SetYTitle("q/P_{T} [1/GeV]");

    h_part_inTT[1] = new TH2D(Form("h_part_inTT%d_vz_eta",TT_number),Form("Vertex position-#eta distribution of particles in TT%d",TT_number),nbins_eta,min_etaTT,max_etaTT,nbins_vz,-max_vz,max_vz);
    h_part_inTT[1]->SetXTitle("#eta");
    h_part_inTT[1]->SetYTitle("Vertex Z position [cm]");



    std::cout << Form("Number of events: %.2fM", nentries/1e6) << std::endl;

    // Loop over events
    // There is only one track per event
    for (Long64_t ievt = 0; ievt < nentries; ++ievt) {
        show_progress_bar(ievt, nentries);
        // Retrieve the event
        reader.getEntry(ievt);


        // Get the variables
        // See TTStubReader.h for more info
        float pt     = reader.vp_pt    ->front();
        float eta    = reader.vp_eta   ->front();
        float phi    = reader.vp_phi   ->front();
        float vz     = reader.vp_vz    ->front();
        int   charge = reader.vp_charge->front();

        Double_t curv = charge/pt;
        Double_t rho = alpha_CMS*pt*charge;

        //Loop over stubs temporaty killed
        // Get number of stubs
        // unsigned nstubs = reader.vb_modId->size();
        //
        // Int_t N_TT_modules_hitted = 0;
        // Int_t N_stub_layer[22] = {0};
        // Int_t N_stub_tot = 0;
        //
        // for (unsigned istub=0; 0 && istub < nstubs; ++istub) {
        //
        //     // Get the variables
        //     // See TTStubReader.h for more info
        //     float    stub_z        = reader.vb_z       ->at(istub);
        //     float    stub_r        = reader.vb_r       ->at(istub);
        //     float    stub_eta      = reader.vb_eta     ->at(istub);
        //     float    stub_phi      = reader.vb_phi     ->at(istub);
        //     float    stub_coordx   = reader.vb_coordx  ->at(istub);
        //     float    stub_coordy   = reader.vb_coordy  ->at(istub);
        //     float    stub_trigBend = reader.vb_trigBend->at(istub);
        //     unsigned stub_modId    = reader.vb_modId   ->at(istub);
        //     int      stub_tpId     = reader.vb_tpId    ->at(istub);
        //
        //     N_stub_layer[stub_modId/10000]++;
        //     N_stub_tot++;
        //
        //     N_TT_modules_hitted += is_in_TT(number_TT,stub_modId);
        //
        // }  // end loop over stubs



        if(TT_number == TrackParametersToTT(phi,curv,eta,vz))
        {
          h_part_inTT[0]->Fill(phi,curv);
          h_part_inTT[1]->Fill(eta,vz);
        }


    }  // end loop over events


    TCanvas* c_part_inTT_vz_eta = new TCanvas("c_part_inTT_vz_eta","c_part_inTT_vz_eta",800,600);
    h_part_inTT[1]->Draw("colz");

    TCanvas* c_part_inTT_phi_curv = new TCanvas("c_part_inTT_phi_curv","c_part_inTT_phi_curv",800,600);
    h_part_inTT[0]->Draw("colz");

}
