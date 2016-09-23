#include "TTStubReader.h"
#include "incluse.h"
#include <iostream>

void ModulesPUWeight(Long64_t nentries = 1e4) {
    cebefo_style();

    TString filename = "/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/Configuration/input/input_Neutrino_PU140_20150408.txt";

    TH1D *h_modules_frequency = new TH1D("h_modules_frequency","Modules stub frequency",171480,49999.5,221479.5);
    h_modules_frequency->SetXTitle("Module ID");
    h_modules_frequency->SetYTitle(Form("Number of stub in %lld events of PU140",nentries));
    h_modules_frequency->SetStats(0);

    // Open the file
    std::cout << "Opening file..." << std::endl;

    TTStubReader reader;
    reader.init(filename);

    // Get number of events
    //Long64_t nentries = reader.getEntries();
    // Long64_t nentries = 100;

    std::cout << "Number of events: " << nentries << std::endl;

    // Loop over events

    for (Long64_t ievt = 0; ievt < nentries; ++ievt) {
        show_progress_bar(ievt,nentries);
        // Retrieve the event
        reader.getEntry(ievt);

        // Get the variables
        // See TTStubReader.h for more info
        unsigned npart = reader.vp_pt->size();

        // float pt     = reader.vp_pt    ->front();
        // float eta    = reader.vp_eta   ->front();
        // float phi    = reader.vp_phi   ->front();
        // float vx     = reader.vp_vx    ->front();
        // float vy     = reader.vp_vy    ->front();
        // float vz     = reader.vp_vz    ->front();
        // int   charge = reader.vp_charge->front();
        //
        // std::cout << "ievt: " << ievt
        //           << " muon pT: " << pt
        //           << " eta: " << eta
        //           << " phi: " << phi
        //           << " vz: "  << vz
        //           << std::endl;

        // Get number of stubs
        unsigned nstubs = reader.vb_modId->size();
        // cout << "ievt: " << ievt << ", npart: " << npart << ", nstubs: " << nstubs <<endl;

        // Loop over stubs
        for (unsigned istub=0; istub < nstubs; ++istub) {

            // Get the variables
            // See TTStubReader.h for more info
            // float    stub_z        = reader.vb_z       ->at(istub);
            // float    stub_r        = reader.vb_r       ->at(istub);
            // float    stub_eta      = reader.vb_eta     ->at(istub);
            // float    stub_phi      = reader.vb_phi     ->at(istub);
            // float    stub_coordx   = reader.vb_coordx  ->at(istub);
            // float    stub_coordy   = reader.vb_coordy  ->at(istub);
            // float    stub_trigBend = reader.vb_trigBend->at(istub);
            unsigned stub_modId    = reader.vb_modId   ->at(istub);
            // int      stub_tpId     = reader.vb_tpId    ->at(istub);
            //
            // std::cout << ".. istub: " << istub
            //           << " module ID: " << stub_modId
            //           << " local phi: " << stub_coordx
            //           << " z: " << stub_coordy
            //           << " global phi: " << stub_phi
            //           << " z: " << stub_z
            //           << " r: " << stub_r
            //           << std::endl;
            h_modules_frequency->Fill(stub_modId);

        }  // end loop over stubs

        // std::cout << std::endl;

    }  // end loop over events

    TCanvas *c_mod_freq = new TCanvas("c_mod_freq","c_mod_freq",800,600);
    h_modules_frequency->Draw();

    TString nameout = "_root/13_ModulesPUWeight_PU140.root";
    cout << "OUTPUT: " << nameout.Data() << endl;
    TFile *f_out = TFile::Open(nameout,"recreate");

    h_modules_frequency->Write();
    c_mod_freq->Write();

    f_out->Close();

    c_mod_freq->SaveAs("_fig/13_ModulesPUWeight_PU140.root");

}  // end analyze()
