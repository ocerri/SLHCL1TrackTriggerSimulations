#ifndef _TTStubReader_h
  #define _TTStubReader_h
  #include "TTStubReader.h"
#endif

#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _TrackParametersToTT_h
  #define _TrackParametersToTT_h
  #include "TrackParametersToTT.C"
#endif

#ifndef _TriggerTowerClass_h
  #define _TriggerTowerClass_h
  #include "TriggerTowerClass.h"
#endif

#include "../data/modID_list.h"

// #include "TClass.h"
// #include "TROOT.h"
// #include "TRint.h"
// gROOT->ProcessLine(".L script/TriggerTowerClass.h+");


Int_t nbins_eta = 500;
Int_t nbins_phi = 360;
Int_t nbins_curv = 100;
Int_t nbins_vz = 100;
Double_t min_phiTT = 2*pigreco;
Double_t max_phiTT = -2*pigreco;
Double_t max_vz = 25.;
Double_t min_etaTT = -2.5;
Double_t max_etaTT = 2.5;

Int_t colors_list[]={2,3,4,5,6,7,9,28,46,8};

OCTriggerTower TT[48];
OCTriggerTower TT_out;
Long64_t nentries = 10e6;
TString stubs_input_file = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";

void LoadOCTriggerTowerFromSim(Double_t invPt_limit = 0, Double_t vz_limit = 0, Int_t save_mod_list = 0)
{
    if(invPt_limit > 0 ) invPT_max_TTdef = invPt_limit;
    if(vz_limit > 0 ) vz_max_TTdef = vz_limit;
    cebefo_style();
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";
    TString filename = stubs_input_file;
    // Open the file
    std::cout << "Opening file..." << std::endl;
    TTStubReader reader;
    reader.init(filename);
    if(nentries==-1) nentries = reader.getEntries();

    //Building TT class
    for(Int_t ii=0; ii<48; ii++)
    {
        TT[ii].SetupTriggerTower(ii, vz_max_TTdef, 1/invPT_max_TTdef);
    }
    TT_out.SetupTriggerTower(-1, vz_max_TTdef, 1/invPT_max_TTdef);


    std::cout << Form("Number of events: %.2fM", nentries/1e6) << std::endl;

    // Loop over events
    // There is only one track per event
    for (Long64_t ievt = 0; ievt < nentries; ++ievt) {
        show_progress_bar(ievt, nentries);
        // Retrieve the event
        reader.getEntry(ievt);


        // Get the variables
        OCTrack track(reader.vp_phi->front(), reader.vp_eta->front(), reader.vp_vz->front(), reader.vp_pt->front() , reader.vp_charge->front());
        Int_t aux_TT_number = track.GetTT_number();

        if(aux_TT_number==-1)
        {
            TT_out.h_part_inTT_PC->Fill(track.phi,track.curv);
            TT_out.h_part_inTT_VE->Fill(track.eta,track.vz);
            // cout << "Non TT track found" << endl;
        }
        else
        {
            TT[aux_TT_number].h_part_inTT_PC->Fill(track.phi,track.curv);
            TT[aux_TT_number].h_part_inTT_VE->Fill(track.eta,track.vz);
        }

        // Get number of stubs
        unsigned nstubs = reader.vb_modId->size();

        for (unsigned istub=0; istub < nstubs; ++istub) {
            // Get the variables
            // See TTStubReader.h for more info
            // cout << "stub loop" << endl;
            unsigned stub_modId    = reader.vb_modId   ->at(istub);

            if(aux_TT_number==-1)
            {
                // cout << "module to be added: " << stub_modId << endl;
                TT_out.AddModule(stub_modId);
                TT_out.h_modules_freq->Fill(stub_modId);
                // cout << "module added" << endl;
            }
            else
            {
                TT[aux_TT_number].AddModule(stub_modId);
                TT[aux_TT_number].h_modules_freq->Fill(stub_modId);

                Int_t aux_NLayer = stub_modId/10000;

                float    stub_z        = reader.vb_z       ->at(istub);
                float    stub_r        = reader.vb_r       ->at(istub);
                float    stub_phi      = reader.vb_phi     ->at(istub);

                Double_t coord1 = (aux_NLayer>10)? stub_r : stub_z;
                TT[aux_TT_number].layer[aux_NLayer].FillLayer(stub_phi, coord1);
            }
            // N_stub_layer[stub_modId/10000]++;
            // N_stub_tot++;


        }  // end loop over stubs
        // cout << "next ev..." << endl << endl;


    }  // end loop over events

    cout << "Trigger tower definition loaded" << endl << endl;

    for(UInt_t i=0; save_mod_list && i<48; i++)
    {
        // TFile *f_out = TFile::Open(Form("_root/06_TT%ddef_%1.0fM_vz%.0f_PT%.0f.root",i, nentries/1e6, vz_max_TTdef, 1/invPT_max_TTdef),"recreate");
        //
        // TT[i].Write();
        //
        // f_out->Close();
        TT[i].WriteModList_fromVector();
    }
}


void AnalyzeModuleStats(Int_t draw_canvas = 0)
{
    cout << "Analyzing module statistics" << endl;

    TH1D* h_NofMod = new TH1D("h_NofMod","Number of modules per TT",48, -0.5,47.5);
    h_NofMod->SetXTitle("Trigger Tower number");
    h_NofMod->SetYTitle("Number of modules");
    h_NofMod->SetStats(0);

    for(UInt_t i=0; i<48; i++)
    {
        h_NofMod->SetBinContent(i+1, TT[i].GetN_tot_modules());
    }



    TH2D *h_NofSMod = new TH2D("h_NofSMod","Quantity of shared modules for each couple",48, -0.5,47.5,48, -0.5,47.5);
    h_NofSMod->SetXTitle("TT number");
    h_NofSMod->SetYTitle("TT number");
    h_NofSMod->SetZTitle("Number of modules");
    h_NofSMod->SetStats(0);

    for(UInt_t i=0; i<48; i++)
    {
        for(UInt_t j=i+1; j<48; j++)
        {
            // cout << "{"<<i<<","<<j<<"}: ";
            Double_t aux = TT[i].GetSharedModules(TT[j]).size();
            // cout << aux;
            h_NofSMod->SetBinContent( h_NofSMod->GetBin(i+1,j+1) , aux );
            // cout << " ...done"<< endl;
        }
    }

    if(draw_canvas)
    {
        TCanvas *c_NofSMod = new TCanvas("c_NofSMod","c_NofSMod",800,600);
        h_NofSMod->DrawCopy("colz");
    }


    TH1I *h_modules_sharing = new TH1I("h_modules_sharing_analysis","Tower modules request",171480,49999.5,221479.5);
    h_modules_sharing->SetXTitle("Module ID");
    h_modules_sharing->SetYTitle("Number of towers containing the module");
    h_modules_sharing->SetStats(0);
    for(UInt_t i=0; i<48; i++)
    {
        for(UInt_t j=5; j<23; j++)
        {
            for(UInt_t k=0; k<TT[i].modulesID[j].size(); k++)
            {
                h_modules_sharing->Fill(TT[i].modulesID[j][k]);
            }
        }
    }
    if(draw_canvas)
    {
        TCanvas *c_modules_sharing = new TCanvas("c_modules_sharing","c_modules_sharing",800,600);
        h_modules_sharing->DrawCopy();
    }


    // TH1D* h_NofMod_weighted = new TH1D("h_NofMod_weighted","Number of direct modules per TT", 48, -0.5, 47.5);
    // h_NofMod_weighted->SetXTitle("Trigger Tower number");
    // h_NofMod_weighted->SetYTitle("Number of direct modules");
    // h_NofMod_weighted->SetStats(0);
    // h_NofMod_weighted->SetLineStyle(7);
    // for(UInt_t i=0; i<(UInt_t)h_modules_sharing->GetNbinsX() ; i++)
    // {
    //     if(h_modules_sharing->GetBinContent(i+1) > 0)
    //     {
    //         UInt_t aux_modID = 50000+i;
    //         Double_t weight = 1/h_modules_sharing->GetBinContent(i+1);
    //         for(UInt_t j=0; j<48; j++)
    //         {
    //             if(TT[j].is_module_inside(aux_modID))
    //             {
    //                 h_NofMod_weighted->Fill(j,weight);
    //             }
    //         }
    //     }
    // }
    // if(draw_canvas)
    // {
    //     TCanvas *c_NofMod = new TCanvas("c_NofMod","c_NofMod",800,600);
    //     h_NofMod->GetYaxis()->SetRangeUser(0,700);
    //     h_NofMod->DrawCopy();
    //     h_NofMod_weighted->DrawCopy("SAME");
    // }





    TH1I *h_modules_conn_distrib = new TH1I("h_modules_conn_distrib","Distribution of TT connection fo a module",9,-0.5,8.5);
    h_modules_conn_distrib->SetXTitle("Number of TT connected");
    h_modules_conn_distrib->SetYTitle("Number of modules");
    // h_modules_conn_distrib->SetStats(0);
    UInt_t j=0;
    for(UInt_t i=0; i<(UInt_t)h_modules_sharing->GetNbinsX() && j<Tracker_modID_list_size ; i++)
    {
        UInt_t aux_modID = 50000+i;
        if(aux_modID == Tracker_modID_list[j])
        {
            Int_t aux = h_modules_sharing->GetBinContent(i+1);
            h_modules_conn_distrib->Fill(aux);
            j++;
        }
    }
    if(draw_canvas)
    {
        TCanvas *c_modules_conn_distrib = new TCanvas("c_modules_conn_distrib","c_modules_conn_distrib",800,600);
        h_modules_conn_distrib->DrawCopy();
    }


    TFile *f_out = TFile::Open(Form("_root/14_analysis_TTdef_%1.0fM_vz%.0f_PT%.0f_Rsphi%d_Rseta%d.root", nentries/1e6, vz_max_TTdef, 1/invPT_max_TTdef,(Int_t)R_star_phi,(Int_t)R_star_eta),"recreate");
    h_modules_sharing->Write();
    h_NofSMod->Write();
    h_NofMod->Write();
    h_modules_conn_distrib->Write();
    // h_NofMod_weighted->Write();
    for(UInt_t i=0; i<48; i++)
    {
        TT[i].h_modules_freq->Write();
    }

    f_out->Close();
    delete f_out;
    delete h_modules_sharing;
    delete h_NofSMod;
    delete h_NofMod;
    delete h_modules_conn_distrib;
    // delete h_NofMod_weighted;
}

void WriteTriggerSectorMap_CSVfile(Int_t LoadTTdef=0, Double_t init_vz = 15., Double_t init_pt = 3., Int_t version=10, Double_t nentries = 10e6, Double_t init_eff = 1., TString tag="")
{
    if(LoadTTdef)
    {
        for(Int_t i=0; i<48;i++)
        {
            // cout << "TT"<< i<< endl;
            TT[i].LoadFromFiles(i, init_vz , init_pt , version, nentries);
            TT[i].Generate_modulesID_VectorFromHisto(init_eff);
        }
    }

    Double_t vz_lim = TT[0].vz_lim;
    Double_t PT_lim = TT[0].PT_lim;

    TString eff_str = "";
    if(init_eff<1.) eff_str = Form("_eff%.02f",init_eff);

    tag = Form("_vz%.0f_pt%.0f",vz_lim,PT_lim) + eff_str + tag;


    TString nameout = Form("data/trigger_sector_map") + tag + "_modules.csv";
    ofstream fout;
    fout.open(nameout.Data());
    cout << "Output file: " << nameout << endl;

    fout << "eta_idx, phi_idx, module_list" << endl;

    // cout << TT[25].modulesID[5].size() << endl;

    for(UInt_t itt=0; itt<48; itt++)
    {
        fout << Form("%d,%d", TT[itt].Eta_number,TT[itt].Phi_number);
        for(UInt_t il=0; il<23; il++)
        {
            for(UInt_t kk=0; kk<TT[itt].modulesID[il].size(); kk++)
            {
                fout << "," << TT[itt].modulesID[il][kk];
            }
        }
        fout << endl;
    }

    fout.close();
}


void WriteTriggerSectorMap_Boundaries_CSVfile(Int_t LoadTTdef=0, Double_t init_vz = 15., Double_t init_pt = 3., Int_t version=10, Double_t nentries = 10e6, TString tag="")
{
    if(LoadTTdef)
    {
        LoadOCTriggerTowerFromSim(1/init_pt,init_vz);
    }

    Double_t vz_lim = TT[0].vz_lim;
    Double_t PT_lim = TT[0].PT_lim;



    tag = Form("_vz%.0f_pt%.0f",vz_lim,PT_lim) + tag;


    TString nameout = Form("data/trigger_sector_map") + tag + "_boundaries.csv";
    ofstream fout;
    fout.open(nameout.Data());
    cout << "Output file: " << nameout << endl;

    fout << "TT,Layer,Phi_min,Phi_max,coord1_min,coord1_max" << endl;

    // cout << TT[25].modulesID[5].size() << endl;

    for(UInt_t itt=0; itt<48; itt++)
    {
        for(UInt_t il=0; il<23; il++)
        {
            if(!TT[itt].layer[il].clear[0] && !TT[itt].layer[il].clear[1])
            {
                TString aux_out = TT[itt].layer[il].GetBoundariesString(itt);
                fout << aux_out.Data() << endl;
            }
        }
    }

    fout.close();
}


TH1I* Build_h_modules_sharing()
{
    Int_t time_stamp = (Int_t)time(nullptr);
    TH1I *h_modules_sharing = new TH1I(Form("h_modules_sharing_%d", time_stamp),"Tower modules request",171480,49999.5,221479.5);
    h_modules_sharing->SetXTitle("Module ID");
    h_modules_sharing->SetYTitle("Number of towers containing the module");
    h_modules_sharing->SetStats(0);
    for(UInt_t i=0; i<48; i++)
    {
        for(UInt_t j=5; j<23; j++)
        {
            for(UInt_t k=0; k<TT[i].modulesID[j].size(); k++)
            {
                h_modules_sharing->Fill(TT[i].modulesID[j][k]);
            }
        }
    }
    return h_modules_sharing;
}

void PurgeMoreThan4SharedModules()
{
    TH1I *h_modules_sharing = Build_h_modules_sharing();
    for(UInt_t i=0; i<171480; i++)
    {
        if(h_modules_sharing->GetBinContent(i+1) > 4)
        {
            UInt_t aux_modId = i+50000;

            cout << endl << "Module " << aux_modId << " has " << h_modules_sharing->GetBinContent(i+1) << " TT connected:" << endl;

            vector<UInt_t> freq;
            vector<UInt_t> TT_id;


            for(UInt_t j=0; j<48; j++)
            {
                if(TT[j].is_module_inside(aux_modId))
                {
                    freq.push_back( TT[j].GetModuleFrequency(aux_modId) );
                    TT_id.push_back(j);
                    cout << j  << "(" << freq.back() << ")" << " " << flush;
                }
            }
            cout << endl;

            while(freq.size()>4)
            {
                UInt_t kk = index_min_elem(freq);
                freq.erase(freq.begin() + kk);

                cout << "Removing from TT" << TT_id[kk] << "..." << flush;
                TT[ TT_id[kk] ].RemoveModule(aux_modId);
                if(TT[ TT_id[kk] ].is_module_inside(aux_modId)) {cout << "ERROR" << endl;}
                else {cout << "done" << endl;}
                TT_id.erase(TT_id.begin() + kk);
            }

        }
    }
}


OCTriggerTower t;
void TT_phase_space_visualization(UInt_t TT_number = 25)
{
    t.SetupTriggerTower(TT_number);

    t.LoadFromFiles(TT_number);

    t.LoadTrainingTracks();
    t.DrawPrePostComparison();
}

void TTLayerLimits(UInt_t TT_number = 25)
{
    t.SetupTriggerTower(TT_number);

    t.LoadTrainingTracks(1e5,"local");

    t.PrintLayersBoundaries(0);
}


void TrialPurgingMultipleMod()
{
    LoadOCTriggerTowerFromSim();

    TTeff_results aux_eff = TT[8].GetEfficiecy();

    PurgeMoreThan4SharedModules();

    aux_eff = TT[8].GetEfficiecy();
}


void GenerateTTDefinition(Int_t init_nentries = 20e6, Double_t init_PT = 3., Double_t init_vz = 15., Double_t init_Rs_phi = R_star_phi_std, Double_t init_Rs_eta = R_star_eta_std)
{
    nentries = init_nentries;
    R_star_eta = init_Rs_eta;
    R_star_phi = init_Rs_phi;

    cout << "Generating a new definition of TT using full simulation approach" << endl;

    cout << "Configurations: " << endl;
    cout  << "Number of entries: " << nentries << endl
          << "PT limit: " << init_PT << " GeV" << endl
          << "Vz acceptance: " << init_vz << " cm" << endl
          << "R*_phi: " << R_star_phi << " cm" << endl
          << "R*_eta: " << R_star_eta << " cm" << endl
          << endl;

    TString tag = Form("_vz%.0f_pt%.0f",init_vz,init_PT);
    TString nameout = Form("data/trigger_sector_map") + tag + "_config.csv";
    ofstream fout;
    fout.open(nameout.Data());
    fout << "Configurations: " << endl;
    fout  << "Number of entries: " << nentries << endl
          << "PT limit: " << init_PT << " GeV" << endl
          << "Vz acceptance: " << init_vz << " cm" << endl
          << "R*_phi: " << R_star_phi << " cm" << endl
          << "R*_eta: " << R_star_eta << " cm" << endl
          << endl;
    fout.close();

    cout << "---------------- TT generation from Muons -----------------" << endl;
    cout << endl << "Input file:" << stubs_input_file.Data() << endl;

    LoadOCTriggerTowerFromSim(1/init_PT, init_vz);

    cout << "Purging more than 4 shared modules..." << endl;
    PurgeMoreThan4SharedModules();

    cout << "Writing map CSV files" << endl;
    WriteTriggerSectorMap_CSVfile(0);

    cout << "Writing boundaries CSV files" << endl;
    WriteTriggerSectorMap_Boundaries_CSVfile(0);

}

void Comparizon_Newdef_vs_naif()
{
  cout << "------------------ Comparizon_Newdef_vs_naif -------------------------" << endl;
  cout << "Building and analyzing the new definition"<< endl;
  LoadOCTriggerTowerFromSim(invPT_max_TTdef, vz_max_TTdef);
  PurgeMoreThan4SharedModules();
  AnalyzeModuleStats();

  cout << "Building and analyzing the naif approach" << endl;
  R_star_eta=-1;
  R_star_phi=-1;
  LoadOCTriggerTowerFromSim(invPT_max_TTdef, vz_max_TTdef);
  PurgeMoreThan4SharedModules();
  AnalyzeModuleStats();

}

void TriggerTowerDefinitionChart()
{
    Comparizon_Newdef_vs_naif();
    // Double_t invPT_list[] = {1/2.,1/3.};
    // Double_t vz_list[] = {7.,10.,15.};
    // for(Int_t i=0; i<2; i++)
    // {
    //     for(Int_t j=0; j<3; j++)
    //     {
    //       cout << "-------- PT:" << 1/invPT_list[i] << " --- Vz: " << vz_list[j] << "--------------" << endl;
    //       LoadOCTriggerTowerFromSim(invPT_list[i],vz_list[j],1);
    //       AnalyzeModuleStats(0);
    //       cout << endl<<endl;
    //     }
    // }
    // R_star_eta = 60.;
    // R_star_phi = 90.;
    // LoadOCTriggerTowerFromSim(1/3., 15);
    // TT[25].DrawPrePostComparison(nentries,1);
    // TT[33].DrawPrePostComparison(nentries,1);
    // TT[41].DrawPrePostComparison(nentries,1);

    // TT[27].GetN_tot_modules();
    // TT[27].GetEfficiecy();
    // AnalyzeModuleStats();
    // WriteTriggerSectorMap_CSVfile();

    // for(Int_t i=0; i<48;i++)
    // {
    //     // cout << "TT"<< i<< endl;
    //     TT[i].LoadFromFiles(i);
    //     TT[i].Generate_modulesID_VectorFromHisto(1.);
    //     if(TT[i].is_module_inside(102702))
    //     {
    //         cout << "Found in: " <<i << endl;
    //     }
    // }

    // t.LoadFromFiles(25);
    //
    // t.PhaseSpaceTrainingTracks();
    // t.DrawPrePostComparison();
    // t.Generate_modulesID_VectorFromHisto(1.);
    // t.GetEfficiecy();
}
