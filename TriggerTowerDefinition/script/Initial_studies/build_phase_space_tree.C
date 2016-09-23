#include "TTStubReader.h"

#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _trigger_sector_map_h
  #define _trigger_sector_map_h
  #include "../data/trigger_sector_map.h"
#endif

// Int_t nbins_eta = 250;
// Int_t nbins_phi = 200;
// Int_t nbins_curv = 200;
// Int_t nbins_vz = 200;
// Double_t min_phiTT = 0.4;
// Double_t max_phiTT = 2*pigreco/3;
// Double_t max_vz = 25.;
// Double_t min_etaTT = -0.3;
// Double_t max_etaTT = 1.5;



Int_t is_in_TT(Int_t TT_number, Int_t module_number)
{
    for(Int_t i=0; i<(Int_t)trigger_sector_modules[TT_number].size(); i++)
    {
        if (module_number == trigger_sector_modules[TT_number][i]) return 1;
    }
    return 0;
}

void show_phase_space(Long64_t nentries = 5e6, Int_t number_TT=27, Int_t verbose = 2) {

    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/Demo_Emulator/stubs_tt27_300M_emu.root"; //tower 27 old
    TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";

    // Open the file
    std::cout << "Opening file..." << std::endl;

    TTStubReader reader;
    reader.init(filename);


    //Creating output file

    TString filename = Form("_root/tree_phase_space_%.0f.root",nentries/1e6);
    TFile *file = TFile::Open(filename,"recreate");

    float pt     ;
    float eta    ;
    float phi    ;
    float vx     ;
    float vy     ;
    float vz     ;
    int   charge ;

    TTree *T = new TTree("T","MC particle tree");
    // T->Branch("evento","OC_Event",&ev);
    T->Branch("ev_number", &ev_number, "ev_number/I");
    T->Branch("PID", &PID, "PID/I");
    T->Branch("status", &status, "status/I");
    T->Branch("Vprod_x",&Vprod[0], "Vprod_x/D");
    T->Branch("Vprod_y",&Vprod[1], "Vprod_y/D");
    T->Branch("Vprod_z",&Vprod[2], "Vprod_z/D");
    T->Branch("Vprod_t",&Vprod[3], "Vprod_t/D");
    T->Branch("Pprod_x",&Pprod[0], "Pprod_x/D");
    T->Branch("Pprod_y",&Pprod[1], "Pprod_y/D");
    T->Branch("Pprod_z",&Pprod[2], "Pprod_z/D");
    T->Branch("Pprod_t",&Pprod[3], "Pprod_t/D");

    // Get number of events
    if(nentries==-1) nentries = reader.getEntries();
    // Long64_t nentries = 100;

    std::cout << "Number of events: " << nentries << std::endl;

    // Loop over events
    // There is only one track per event
    for (Long64_t ievt = 0; ievt < nentries; ++ievt) {
        show_progress_bar(ievt, nentries);
        // Retrieve the event
        reader.getEntry(ievt);


        h_part_per_ev->SetBinContent(ievt+1,reader.vp_pt->size());

        // Get the variables
        // See TTStubReader.h for more info
        pt     = reader.vp_pt    ->front();
        eta    = reader.vp_eta   ->front();
        phi    = reader.vp_phi   ->front();
        vx     = reader.vp_vx    ->front();
        vy     = reader.vp_vy    ->front();
        vz     = reader.vp_vz    ->front();
        charge = reader.vp_charge->front();

        Double_t curv = charge/pt;

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

        Int_t N_TT_plane_hitted = 0;
        Int_t N_stub_layer[22] = {0};

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

            N_TT_plane_hitted += is_in_TT(number_TT,stub_modId);

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

        if(N_stub_layer[5]==1 && N_stub_layer[6]==1 && N_stub_layer[7]==1 && N_stub_layer[8]==1 && N_stub_layer[9]==1 && N_stub_layer[10]==1)
        {
          if(N_TT_plane_hitted==6)
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



}  // end analyze()
