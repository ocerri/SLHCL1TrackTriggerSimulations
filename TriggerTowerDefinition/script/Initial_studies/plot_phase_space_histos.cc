#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

void plot_phase_space_histos()
{
    Int_t look4phi = 0;
    Int_t look4eta = !look4phi;

    TString tag;
    if(look4phi)
    {
      tag = "phi_curv";
    }
    else if(look4eta)
    {
      tag = "vz_eta";
    }

    cebefo_style();

    TFile *factual = TFile::Open("_root/00_phase_space_5M.root");
    TH2D* h1 = (TH2D*) factual->Get("h_part_det_all_vz_eta");
    TH2D* h2 = (TH2D*) factual->Get("h_part_det_inTT_vz_eta");
    TH2D* h3 = (TH2D*) factual->Get("h_part_det_outTT_vz_eta");


    TCanvas *canvas = new TCanvas("canvas","canvas",800,600);

    h1->SetStats(0);
    // h1->DrawCopy("colz");
    // canvas->SaveAs("_fig/00_actual_sample_vz_eta_all.pdf");

    h2->SetStats(0);
    // h2->DrawCopy("colz");
    // canvas->SaveAs("_fig/00_actual_sample_vz_eta_in.pdf");

    TH2D *htmp = (TH2D*)h3->Clone("htmp");
    if(look4phi)  htmp->SetTitle("Curvature-#phi distribution");
    else if(look4eta) htmp->SetTitle("Vertex z-#eta distribution");

    htmp->SetStats(0);
    htmp->SetMarkerColor(2);
    // htmp->DrawCopy();
    // h2->DrawCopy("Same");
    // canvas->SaveAs("_fig/00_actual_sample_vz_eta_overlap.png");


    // TFile *fnew = TFile::Open("_root/02_phase_space_global_5M.root");
    TFile *fnew = TFile::Open("_root/02_phase_space_global_49M.root");
    TH2D * h4 = (TH2D*) fnew->Get("h_part_det_inTT_vz_eta");
    h4->SetStats(0);
    // h4->DrawCopy("colz");
    // canvas->SaveAs("_fig/02_global_sample_vz_eta_in.pdf");

    TH2D * h5 = (TH2D*) fnew->Get("h_part_det_all_vz_eta");
    h5->SetStats(0);
    // h5->DrawCopy("colz");
    // canvas->SaveAs("_fig/02_global_sample_vz_eta_all.pdf");



//--------------------------------Summary plot-------------------------------------------
    TLegend* leg;
    if(look4phi)
    {
      leg = new TLegend(0.7331078,0.1426087,0.9899749,0.4730435);//phi
    }
    else if(look4eta)
    {
      leg = new TLegend(0.7418797,0.5478261,0.9974937,0.9304348);//eta
    }

    h4->SetMarkerColor(8);
    h4->SetLineWidth(0);
    h4->SetFillColor(8);
    h4->SetTitle("Phase space distributions");
    h4->DrawCopy();
    leg->AddEntry(h4,"Full in TT","f");
    h3->SetMarkerColor(2);
    h3->SetLineWidth(0);
    h3->SetFillColor(2);
    h3->DrawCopy("Same");
    leg->AddEntry(h3,"Standard out TT","f");
    h2->SetFillColor(1);
    h2->SetLineWidth(0);
    h2->DrawCopy("Same");
    leg->AddEntry(h2,"Standard in TT","f");

    if(look4phi)
    {
        Double_t R_star = 58.89;
        Double_t alpha = 87.72;
        Double_t pt_min = 2;
        Double_t Dphi_max = asin( R_star/(2*alpha*pt_min) );// + pigreco/4;

        Double_t phi_star[] = {pigreco/4. , pigreco/2.};
        TF1 *fu_att[2];
        for(Int_t ii=0; ii<2; ii++)
        {
            fu_att[ii] = new TF1(Form("fu_att_%d",ii),Form("2.979*sin(x - %.4f)", phi_star[ii]), phi_star[ii]-Dphi_max, phi_star[ii]+Dphi_max);
            fu_att[ii]->SetLineColor(6);
            fu_att[ii]->SetLineWidth(3);
            fu_att[ii]->SetLineStyle(7);
            fu_att[ii]->DrawCopy("Same");
        }
        leg->AddEntry(fu_att[0],"#phi* #in [#pi/4,#pi/2]","l");

        phi_star[0] = 0.71;
        phi_star[1] = 1.64;
        for(Int_t ii=0; ii<2; ii++)
        {
            fu_att[ii] = new TF1(Form("fu_real_%d",ii),Form("2.979*sin(x - %.4f)", phi_star[ii]), phi_star[ii]-Dphi_max, phi_star[ii]+Dphi_max);
            fu_att[ii]->SetLineColor(4);
            fu_att[ii]->SetLineWidth(3);
            fu_att[ii]->SetLineStyle(7);
            fu_att[ii]->DrawCopy("Same");
        }
        leg->AddEntry(fu_att[0],"#phi* #in [0.71,1.64]","l");
    }


    leg->Draw("Same");
    canvas->SaveAs("_fig/00_actual_sample_vz_eta_summary.png");

}
