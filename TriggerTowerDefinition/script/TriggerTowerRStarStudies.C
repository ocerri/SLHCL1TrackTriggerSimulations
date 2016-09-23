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
#include "TLine.h"
#include "TLatex.h"



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
Int_t save_fig = 1;

TFile *f_in = TFile::Open("_root/13_ModulesPUWeight_PU140.root");
TH1D *h_modules_frequency = (TH1D*) f_in->Get("h_modules_frequency");

Double_t GetModuleWeight(Int_t modId)
{
    Int_t bin_number = modId-50000+1;
    return h_modules_frequency->GetBinContent(bin_number);
}

void LoadOCTriggerTowerFromSim(Double_t invPt_limit = 0, Double_t vz_limit = 0, Int_t save_mod_list = 0)
{
    if(invPt_limit > 0 ) invPT_max_TTdef = invPt_limit;
    if(vz_limit > 0 ) vz_max_TTdef = vz_limit;
    cebefo_style();
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt48_50M/stubs_tt48_50M.0.root";
    TString filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";
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
        //
        // Int_t N_TT_modules_hitted = 0;
        // Int_t N_stub_layer[22] = {0};
        // Int_t N_stub_tot = 0;
        //
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


    TH1I *h_modules_sharing = new TH1I("h_modules_sharing","Tower modules request",171480,49999.5,221479.5);
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


    TH1D* h_NofMod_weighted = new TH1D("h_NofMod_weighted","Number of direct modules per TT", 48, -0.5, 47.5);
    h_NofMod_weighted->SetXTitle("Trigger Tower number");
    h_NofMod_weighted->SetYTitle("Number of direct modules");
    h_NofMod_weighted->SetStats(0);
    h_NofMod_weighted->SetLineStyle(7);
    for(UInt_t i=0; i<(UInt_t)h_modules_sharing->GetNbinsX() ; i++)
    {
        if(h_modules_sharing->GetBinContent(i+1) > 0)
        {
            UInt_t aux_modID = 50000+i;
            Double_t weight = 1/h_modules_sharing->GetBinContent(i+1);
            for(UInt_t j=0; j<48; j++)
            {
                if(TT[j].is_module_inside(aux_modID))
                {
                    h_NofMod_weighted->Fill(j,weight);
                }
            }
        }
    }
    if(draw_canvas)
    {
        TCanvas *c_NofMod = new TCanvas("c_NofMod","c_NofMod",800,600);
        h_NofMod->GetYaxis()->SetRangeUser(0,700);
        h_NofMod->DrawCopy();
        h_NofMod_weighted->DrawCopy("SAME");
    }





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


    TFile *f_out = TFile::Open(Form("_root/06_analysis_TTdef_%1.0fM_vz%.0f_PT%.0f.root", nentries/1e6, vz_max_TTdef, 1/invPT_max_TTdef),"recreate");
    h_modules_sharing->Write();
    h_NofSMod->Write();
    h_NofMod->Write();
    h_modules_conn_distrib->Write();
    h_NofMod_weighted->Write();
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
    delete h_NofMod_weighted;
}

void ScanRStarPhi(Int_t N_step_RS_phi = 6, Double_t vz_limit = 10., Int_t EnableWeights = 1)
{
    Int_t colori[] = {1,2,4};

    Double_t RS_phi_min = 0.;
    Double_t RS_phi_max = 110.;

    TGraph *gr_RS_phi[3];
    TString gr_tag[3] = {"barrel","intermediate","endcap"};
    for(UInt_t i=0; i<3; i++)
    {
        gr_RS_phi[i] = new TGraph(N_step_RS_phi);
        gr_RS_phi[i]->SetLineColor(colori[i]);
        gr_RS_phi[i]->SetMarkerColor(colori[i]);
        gr_RS_phi[i]->SetMarkerStyle(4);

    }

    for(Int_t i=0; i<1+N_step_RS_phi; i++)
    {
        R_star_eta = R_star_eta_std;
        R_star_phi = RS_phi_min + i*(RS_phi_max - RS_phi_min)/N_step_RS_phi ;
        Double_t N_modules[3] = {0};

        LoadOCTriggerTowerFromSim(0,vz_limit);

        for(Int_t iTT=0; iTT<48; iTT++)
        {
            Double_t TowerWeight = 0;
            if(EnableWeights)
            {
                for(UInt_t klay=5; klay<23; klay++)
                {
                    for(UInt_t kmod =0; kmod<TT[iTT].modulesID[klay].size(); kmod++)
                    {
                        TowerWeight += GetModuleWeight(TT[iTT].modulesID[klay][kmod]);
                    }
                }
            }
            else TowerWeight = TT[iTT].GetN_tot_modules();

            if(TT[iTT].is_in_barrel()) N_modules[0] += TowerWeight;
            else if(TT[iTT].is_in_intermediate()) N_modules[1] += TowerWeight;
            else if(TT[iTT].is_in_endcap()) N_modules[2] += TowerWeight;
        }


        for(UInt_t k=0; k<3; k++)
        {
            gr_RS_phi[k]->SetPoint(i,R_star_phi,N_modules[k]);
            cout << N_modules[k] << " " <<R_star_phi << endl;
        }
    }

    TLegend *leg_phi = new TLegend(0.78,0.7,0.98,0.9);
    TCanvas *c_gr_phi = new TCanvas("c_gr_phi","c_gr_phi",800,600);
    gr_RS_phi[0]->GetYaxis()->SetRangeUser(5000,10000);
    for(UInt_t k=0; k<3; k++)
    {
        leg_phi->AddEntry(gr_RS_phi[k], gr_tag[k], "lp");
        gr_RS_phi[k]->Draw(k==0?"APL":"PL");
    }

    TLine *ln = new TLine(R_star_phi_std,5000,R_star_phi_std,10000);
    ln->SetLineColor(6);
    ln->SetLineStyle(7);
    ln->SetLineWidth(2);
    ln->Draw("SAME");
    leg_phi->AddEntry(ln, "Actual value", "l");
    leg_phi->Draw();

    TLatex latex;
    latex.SetTextSize(0.05);
    latex.SetTextAlign(13);  //align at top
    latex.DrawLatex(30,9000,Form("R*_{#eta} = %.2f", R_star_eta_std));


    for(UInt_t i=0; i<3; i++)
    {
        TString title = Form("Connection required for different R*_{#phi} - %.1fM sample",nentries/1e6);
        if(EnableWeights) title = "Weighted " + title;
        gr_RS_phi[i]->SetTitle(title);
        gr_RS_phi[i]->GetXaxis()->SetTitle("R*_{#phi} [cm]");

        TString ylabel = "Total number of connection";
        if(EnableWeights) ylabel+= " weighted";
        gr_RS_phi[i]->GetYaxis()->SetTitle(ylabel);

        gr_RS_phi[i]->GetYaxis()->SetTitleOffset(1.31);

    }

    if(save_fig)
    {
        TString nameout = "_fig/";
        if(EnableWeights) nameout += Form("13_RS_phi_scan_WEIGHTED_%.1fM_RSeta%.0f_vz%.0f",nentries/1e6,R_star_eta_std,vz_limit);
        else nameout += Form("07_RS_phi_scan_%.1fM_RSeta%.0f_vz%.0f",nentries/1e6,R_star_eta_std,vz_limit);
        c_gr_phi->SaveAs((nameout+".root").Data());
    }


}



void ScanRStarEta(Int_t N_step_RS_eta = 6, Double_t vz_limit = 10., Int_t EnableWeights = 1)
{
    Int_t colori[] = {1,2,4};

    // Double_t RS_eta_min = 10.;
    Double_t RS_eta_min = 10.;

    Double_t RS_eta_max = 110.;

    TGraph *gr_RS_eta[3];
    TString gr_tag[3] = {"barrel","intermediate","endcap"};
    for(UInt_t i=0; i<3; i++)
    {
        gr_RS_eta[i] = new TGraph(N_step_RS_eta);
        gr_RS_eta[i]->SetLineColor(colori[i]);
        gr_RS_eta[i]->SetMarkerColor(colori[i]);
        gr_RS_eta[i]->SetMarkerStyle(4);

    }

    for(Int_t i=0; i<1+N_step_RS_eta; i++)
    {
        R_star_phi = R_star_phi_std;
        R_star_eta = RS_eta_min + i*(RS_eta_max - RS_eta_min)/N_step_RS_eta ;
        Double_t N_modules[3] = {0};

        LoadOCTriggerTowerFromSim(0,vz_limit);

        for(Int_t iTT=0; iTT<48; iTT++)
        {
            Double_t TowerWeight = 0;
            if(EnableWeights)
            {
                for(UInt_t klay=5; klay<23; klay++)
                {
                    for(UInt_t kmod =0; kmod<TT[iTT].modulesID[klay].size(); kmod++)
                    {
                        TowerWeight += GetModuleWeight(TT[iTT].modulesID[klay][kmod]);
                    }
                }
            }
            else TowerWeight = TT[iTT].GetN_tot_modules();

            if(TT[iTT].is_in_barrel()) N_modules[0] += TowerWeight;
            else if(TT[iTT].is_in_intermediate()) N_modules[1] += TowerWeight;
            else if(TT[iTT].is_in_endcap()) N_modules[2] += TowerWeight;
        }


        for(UInt_t k=0; k<3; k++)
        {
            gr_RS_eta[k]->SetPoint(i,R_star_eta,N_modules[k]);
            cout << N_modules[k] << " " <<R_star_eta << endl;
        }
    }

    TLegend *leg_eta = new TLegend(0.78,0.7,0.98,0.9);
    TCanvas *c_gr_eta = new TCanvas("c_gr_eta","c_gr_eta",800,600);
    gr_RS_eta[0]->GetYaxis()->SetRangeUser(5000,12000);
    for(UInt_t k=0; k<3; k++)
    {
        leg_eta->AddEntry(gr_RS_eta[k], gr_tag[k], "lp");
        gr_RS_eta[k]->Draw(k==0?"APL":"PL");
    }
    TLine *ln = new TLine(R_star_eta_std,5000,R_star_eta_std,12000);
    ln->Draw("SAME");
    ln->SetLineColor(6);
    ln->SetLineStyle(7);
    ln->SetLineWidth(2);
    leg_eta->AddEntry(ln, "Actual value", "l");
    leg_eta->Draw();

    TLatex latex;
    latex.SetTextSize(0.05);
    latex.SetTextAlign(13);  //align at top
    latex.DrawLatex(30,11000,Form("R*_{#phi} = %.2f", R_star_phi_std));

    for(UInt_t i=0; i<3; i++)
    {
        TString title = Form("Connection required for different R*_{#eta} - %.1fM sample",nentries/1e6);
        if(EnableWeights) title = "Weighted " + title;
        gr_RS_eta[i]->SetTitle(title);
        TString ylabel = "Total number of connection";
        if(EnableWeights) ylabel+= " weighted";
        gr_RS_eta[i]->GetYaxis()->SetTitle(ylabel);

        gr_RS_eta[i]->GetXaxis()->SetTitle("R*_{#eta} [cm]");
        gr_RS_eta[i]->GetYaxis()->SetTitleOffset(1.31);

    }


    if(save_fig)
    {
        TString nameout = "_fig/";
        if(EnableWeights) nameout += Form("13_RS_eta_scan_WEIGHTED_%.1fM_RSphi%.0f_vz%.0f",nentries/1e6,R_star_phi_std,vz_limit);
        else nameout += Form("07_RS_eta_scan_%.1fM_RSphi%.0f_vz%.0f",nentries/1e6,R_star_phi_std,vz_limit);
        c_gr_eta->SaveAs((nameout+".root").Data());
    }

}


OCTriggerTower t;
void TriggerTowerRStarStudies()
{
    h_modules_frequency->Scale(1.5/1e4);
    cebefo_style();
    nentries = 5e6;
    ScanRStarPhi(15,15.,1);
    ScanRStarEta(15,15.,1);

}
