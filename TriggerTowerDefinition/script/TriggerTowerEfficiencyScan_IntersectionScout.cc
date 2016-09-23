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
#include "TGraphErrors.h"
#include "TLine.h"



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

vector<Double_t> integral_thr = {1.0, 0.99, 0.97, 0.95, 0.93};

TH1I* AnalyzeModuleStats(Int_t draw_canvas = 0, TString tag = "")
{
    cout << "Analyzing module statistics" << endl;


    TH1I *h_modules_sharing = new TH1I(("h_modules_sharing"+tag).Data(),"Tower modules request",171480,49999.5,221479.5);
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





    TH1I *h_modules_conn_distrib = new TH1I(("h_modules_conn_distrib"+tag).Data(),"Distribution of TT connection fo a module",9,-0.5,8.5);
    h_modules_conn_distrib->SetXTitle("Number of TT connected");
    h_modules_conn_distrib->SetYTitle("Number of modules");
    // h_modules_conn_distrib->SetStats(0);

    Int_t print_6_sharing = 0;

    UInt_t j=0;
    for(UInt_t i=0; i<(UInt_t)h_modules_sharing->GetNbinsX() && j<Tracker_modID_list_size ; i++)
    {
        UInt_t aux_modID = 50000+i;
        if(aux_modID == Tracker_modID_list[j])
        {
            Int_t aux = h_modules_sharing->GetBinContent(i+1);
            h_modules_conn_distrib->Fill(aux);
            j++;

            if(!print_6_sharing && aux == 6)
            {
                // print_6_sharing = 1;
                cout << aux_modID << " has "<< aux <<" connection" << endl;
                for(UInt_t itt=0; itt<48; itt++)
                {
                    if(TT[itt].is_module_inside(aux_modID))
                    {
                        cout << "TT" << itt << ": [" << TT[itt].Eta_number << ", " << TT[itt].Phi_number << "]"<< endl;
                    }
                }

                cout << endl;
            }
        }
    }
    if(draw_canvas)
    {
        TCanvas *c_modules_conn_distrib = new TCanvas("c_modules_conn_distrib","c_modules_conn_distrib",800,600);
        h_modules_conn_distrib->DrawCopy();
    }

    return h_modules_conn_distrib;
}

void TriggerTowerEfficiencyScan_IntersectionScout()
{
    cebefo_style();

    const UInt_t Ntot_step=4;

    TH1I *h_mod_conn[Ntot_step];
    Int_t colori[] = {1,2,4,6};

    TCanvas* c_out = new TCanvas("c_out","c_out",800,600);
    c_out->SetLogy();
    TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);
    for(UInt_t i=0; i<Ntot_step; i++)
    {
        for(UInt_t itt = 0; itt< 48; itt++)
        {
            TT[itt].LoadFromFiles(itt,15.);
            TT[itt].Generate_modulesID_VectorFromHisto(integral_thr[i]);
        }

        h_mod_conn[i] = AnalyzeModuleStats(0,Form("_thr%.2f",integral_thr[i]));
        h_mod_conn[i]->SetLineColor(colori[i]);
        h_mod_conn[i]->SetStats(0);
        h_mod_conn[i]->GetYaxis()->SetRangeUser(0.1,20000);
        h_mod_conn[i]->SetTitle(Form("Distribution of TT connection for a module - |v_{z}| < %.0f cm , P_{T} > %.0f GeV",TT[0].vz_lim,TT[0].PT_lim));

        h_mod_conn[i]->Draw(i==0?"":"Same");
        leg->AddEntry(h_mod_conn[i],Form("Thr = %.2f",integral_thr[i]),"l");
    }
    leg->Draw("same");

    // c_out->SaveAs(Form("_fig/09_TT_connection_varTHR_vz%.0f.pdf",TT[0].vz_lim));


}
