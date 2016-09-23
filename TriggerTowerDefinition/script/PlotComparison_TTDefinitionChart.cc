#include "incluse.h"
#include "../data/trigger_sector_map.h"
#include "TrackParametersToTT.C"

void PlotComparison_TTDefinitionChart()
{
    Int_t nentries = 2e6;
    cebefo_style();

    const Int_t pt_size = 2;
    const Int_t vz_size = 3;
    Double_t invPT_list[] = {1/2.,1/3.};
    Double_t vz_list[] = {7.,10.,15.};

    Int_t colors_list[]={2,1,4,5,6,7,9,28,46,8};

    TFile* fin[pt_size][vz_size];
    TH1I* h_modules_conn_distrib[pt_size][vz_size];
    TH1D* h_NofMod[pt_size][vz_size];
    TH1D* h_NofMod_weighted[pt_size][vz_size];


    TCanvas *c_modules_conn_distrib = new TCanvas("c_modules_conn_distrib","c_modules_conn_distrib",1200,600);
    c_modules_conn_distrib->Divide(3,1);
    TLegend *leg_conn[vz_size];

    TCanvas *c_NofMod = new TCanvas("c_NofMod","c_NofMod",1200,600);
    c_NofMod->Divide(3,1);
    TLegend *leg_NofMod[vz_size];


    TCanvas *c_NofMod_weighted = new TCanvas("c_NofMod_weighted","c_NofMod_weighted",1200,600);
    c_NofMod_weighted->Divide(3,1);
    TLegend *leg_NofMod_weighted[vz_size];


    TCanvas *c_modules_conn_distrib_cmp = new TCanvas("c_modules_conn_distrib_cmp","c_modules_conn_distrib_cmp",800,600);
    TLegend *leg_conn_cmp = new TLegend(0.55,0.5,0.95,0.7);

    TCanvas *c_NofMod_cmp = new TCanvas("c_NofMod_cmp","c_NofMod_cmp",800,600);
    TLegend *leg_NofMod_cmp = new TLegend(0.55,0.3,0.95,0.5);



    for(Int_t i=0; i<vz_size;i++)
    {
        leg_conn[i]  = new TLegend(0.55,0.5,0.95,0.7);
        leg_NofMod[i]  = new TLegend(0.55,0.3,0.95,0.5);
        leg_NofMod_weighted[i]  = new TLegend(0.59,0.74,0.99,0.94);
    }

    cout << "Everything booked correctly" << endl;

    for(Int_t ipt=0; ipt<pt_size; ipt++)
    {
        cout << "PT: " << 1/invPT_list[ipt] << endl;
        for(Int_t ivz=0; ivz<vz_size; ivz++)
        {
            cout << "vz: " << vz_list[ivz] << endl;

            fin[ipt][ivz] = TFile::Open( Form("_root/06_analysis_TTdef_%1.0fM_vz%.0f_PT%.0f.root", nentries/1e6, vz_list[ivz], 1/invPT_list[ipt]) );

            h_modules_conn_distrib[ipt][ivz] = (TH1I*) fin[ipt][ivz]->Get("h_modules_conn_distrib");
            h_modules_conn_distrib[ipt][ivz]->SetLineColor(colors_list[ivz]);
            h_modules_conn_distrib[ipt][ivz]->SetTitle(Form("Modules connection, |V_{z}| < %.0f cm",vz_list[ivz]));
            h_modules_conn_distrib[ipt][ivz]->SetLineStyle(1+2*(1-ipt));
            h_modules_conn_distrib[ipt][ivz]->SetLineWidth(2);
            h_modules_conn_distrib[ipt][ivz]->SetStats(0);
            h_modules_conn_distrib[ipt][ivz]->GetYaxis()->SetRangeUser(0,9e3);

            c_modules_conn_distrib->cd(ivz+1);
            h_modules_conn_distrib[ipt][ivz]->DrawCopy(ipt?"SAME":"");
            leg_conn[ivz]->AddEntry(h_modules_conn_distrib[ipt][ivz],Form("P_{T} > %.0f GeV", 1/invPT_list[ipt]),"l");


            if(ipt==1)
            {
              c_modules_conn_distrib_cmp->cd();
              Double_t integrale = h_modules_conn_distrib[ipt][ivz]->Integral();
              Double_t non_connessi = h_modules_conn_distrib[ipt][ivz]->GetBinContent(1);
              cout << "Connected modules: " << integrale << " - " << non_connessi << " = " << integrale -  non_connessi << endl;

              h_modules_conn_distrib[ipt][ivz]->SetTitle(Form("Modules connection"));
              h_modules_conn_distrib[ipt][ivz]->DrawCopy(ivz?"SAME":"");
              leg_conn_cmp->AddEntry(h_modules_conn_distrib[ipt][ivz],Form("|V_{z}| < %.0f cm, P_{T} > %.0f GeV",vz_list[ivz], 1/invPT_list[ipt]),"l");
            }


            h_NofMod[ipt][ivz] = (TH1D*) fin[ipt][ivz]->Get("h_NofMod");
            h_NofMod[ipt][ivz]->SetLineColor(colors_list[ivz]);
            h_NofMod[ipt][ivz]->SetFillColor(colors_list[ivz]);
            h_NofMod[ipt][ivz]->SetFillStyle(0);
            h_NofMod[ipt][ivz]->SetTitle(Form("Modules per tower, |V_{z}| < %.0f cm",vz_list[ivz]));
            h_NofMod[ipt][ivz]->SetLineStyle(1+2*(1-ipt));
            h_NofMod[ipt][ivz]->SetLineWidth(2);
            h_NofMod[ipt][ivz]->SetStats(0);
            h_NofMod[ipt][ivz]->GetYaxis()->SetRangeUser(0,700);


            c_NofMod->cd(ivz+1);
            h_NofMod[ipt][ivz]->DrawCopy(ipt?"SAME":"");
            leg_NofMod[ivz]->AddEntry(h_NofMod[ipt][ivz],Form("P_{T} > %.0f GeV", 1/invPT_list[ipt]),"l");


            if(ipt==0)
            {
              c_NofMod_cmp->cd();
              h_NofMod[ipt][ivz]->SetTitle(Form("Modules per tower"));
              h_NofMod[ipt][ivz]->DrawCopy(ivz?"SAME":"");
              leg_NofMod_cmp->AddEntry(h_NofMod[ipt][ivz],Form("|V_{z}| < %.0f cm, P_{T} > %.0f GeV",vz_list[ivz], 1/invPT_list[ipt]),"l");

              // cout << "NofMod_weighted..." << endl;

              c_NofMod_weighted->cd(ivz+1);
              h_NofMod_weighted[ipt][ivz] = (TH1D*) fin[ipt][ivz]->Get("h_NofMod_weighted");
              h_NofMod_weighted[ipt][ivz]->SetLineColor(colors_list[ivz]);
              h_NofMod_weighted[ipt][ivz]->SetTitle(Form("Modules per tower, |V_{z}| < %.0f cm",vz_list[ivz]));
              h_NofMod_weighted[ipt][ivz]->SetLineStyle(1);
              h_NofMod_weighted[ipt][ivz]->SetLineWidth(2);
              h_NofMod_weighted[ipt][ivz]->SetFillStyle(1);
              h_NofMod_weighted[ipt][ivz]->SetFillColor(colors_list[ivz]);
              h_NofMod_weighted[ipt][ivz]->SetStats(0);
              h_NofMod_weighted[ipt][ivz]->GetYaxis()->SetRangeUser(0,700);

              cout << "Weighted integral: " << h_NofMod_weighted[ipt][ivz]->Integral() << endl;

              h_NofMod[ipt][ivz]->SetTitle(Form("Modules per TT, |V_{z}| < %.0f cm, P_{T} > 3 GeV", vz_list[ivz]));
              h_NofMod[ipt][ivz]->SetFillStyle(3003);
              h_NofMod[ipt][ivz]->DrawCopy();
              h_NofMod_weighted[ipt][ivz]->DrawCopy("SAME HIST");
              leg_NofMod_weighted[ivz]->AddEntry(h_NofMod[ipt][ivz],Form("total connections"),"f");
              leg_NofMod_weighted[ivz]->AddEntry(h_NofMod_weighted[ipt][ivz],Form("direct connections"),"f");


            }


        }
    }

    // c_modules_conn_distrib->SetLogy();
    for(Int_t i=0; i<vz_size;i++)
    {
        c_modules_conn_distrib->cd(i+1);
        leg_conn[i]->Draw("SAME");

        c_NofMod->cd(i+1);
        leg_NofMod[i]->Draw("SAME");

        c_NofMod_weighted->cd(i+1);
        leg_NofMod_weighted[i]->Draw("SAME");
    }

    c_NofMod_cmp->cd();
    TH1D *h_NofMod_old = new TH1D("h_NofMod_old","Number of modules per TT old",48, -0.5,47.5);
    for(UInt_t ii=0; ii<48; ii++)
    {
        h_NofMod_old->SetBinContent(old2new_TTnumber(ii)+1,trigger_sector_modules[ii].size());
    }
    h_NofMod_old->SetLineColor(8);
    h_NofMod_old->Draw("Same");
    leg_NofMod_cmp->AddEntry(h_NofMod_old,"Old definition","l");
    leg_NofMod_cmp->Draw("Same");

    c_modules_conn_distrib_cmp->cd();
    leg_conn_cmp->Draw();

    cout << "Everything drawn correctly" << endl;


    // c_modules_conn_distrib->SaveAs("_fig/06_mod_conn_distrib_comp.pdf");
    // c_NofMod->SaveAs("_fig/06_NofMod_distrib_comp.pdf");
    // c_NofMod_weighted->SaveAs("_fig/06_NofMod_distrib_weighted.pdf");
    // c_NofMod_cmp->SaveAs("_fig/06_NofMod_distrib_comp3.pdf");
    // c_modules_conn_distrib_cmp->SaveAs("_fig/06_mod_conn_distrib_comp3.pdf");


}
