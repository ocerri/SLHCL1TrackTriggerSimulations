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

void TriggerTowerEfficiencyScan()
{
    Int_t save_missing_tracks = 0;
    cebefo_style();

    const UInt_t Ntot_TT=3;
    OCTriggerTower TT[Ntot_TT];


    for(UInt_t itt = 0; itt< Ntot_TT; itt++)
    {
        Int_t TT_number = DivNumberToTTNumber(itt+4,2);
        TT[itt].LoadFromFiles(TT_number);

        vector<Double_t> eff[2];
        vector<Double_t> deff[2];

        vector<Double_t> NofMod;

        UInt_t N_tot_points = save_missing_tracks? 1 : integral_thr.size();
        for(UInt_t k=0; k< N_tot_points; k++)
        {
            Double_t aux_NofMod = TT[itt].Generate_modulesID_VectorFromHisto(integral_thr[k]);
            TTeff_results aux_eff;
            if (!save_missing_tracks) aux_eff = TT[itt].GetEfficiecy();
            else aux_eff = TT[itt].GetEfficiecy(1e6,1,"_miss",3);

            eff[0].push_back(aux_eff.eff5);
            deff[0].push_back(aux_eff.deff5);

            eff[1].push_back(aux_eff.eff6);
            deff[1].push_back(aux_eff.deff6);
            NofMod.push_back(aux_NofMod);
        }

        TGraphErrors* gr_eff[2];
        TLegend* leg = new TLegend(0.65,0.4,0.95,0.6);

        for(UInt_t i=0; i<2; i++)
        {
            gr_eff[i] = new TGraphErrors(eff[i].size(),&NofMod[0],&(eff[i][0]),0,&(deff[i][0]));
            gr_eff[i]->GetYaxis()->SetTitle("Efficiency");
            gr_eff[i]->GetXaxis()->SetTitle("Total number of modules");
            gr_eff[i]->SetName(Form("gr_eff%d_TT%d",i+5,TT_number));
            gr_eff[i]->SetTitle(Form("TT [%d,%d] - |V_{z}| < %.0f cm, P_{T} > %.0f GeV",TTNumberToEtaNumber(TT_number),TTNumberToPhiNumber(TT_number),TT[itt].vz_lim,TT[itt].PT_lim));

            gr_eff[i]->SetMarkerStyle(4);
            gr_eff[i]->SetMarkerColor(2*(1+i));
            gr_eff[i]->SetLineColor(2*(1+i));

            leg->AddEntry(gr_eff[i],Form("Requiring %d stubs",5+i),"lep");
        }

        Double_t aux[] = {NofMod[NofMod.size()-1]-10, NofMod[0]+10};
        TLine top_line(aux[0] , 1 , aux[1],1);
        top_line.SetLineColor(6);
        top_line.SetLineStyle(7);
        top_line.SetLineWidth(2);

        TCanvas *c_eff = new TCanvas(Form("c_eff_TT%d",TT_number),Form("c_eff_TT%d",TT_number),800,600);
        c_eff->SetGrid();
        gr_eff[0]->GetYaxis()->SetRangeUser(eff[1][eff[1].size()-1]-0.1,1.05);
        gr_eff[0]->Draw("APL");
        gr_eff[1]->Draw("PL");
        top_line.Draw();
        leg->Draw("SAME");
        top_line.DrawClone();

        if(!save_missing_tracks) c_eff->SaveAs(Form("_fig/07_effVSNofMod_TT%d.pdf",TT_number));


    }
}
