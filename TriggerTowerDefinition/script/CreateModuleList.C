#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif


#include "../data/modID_list.h"
#include "../data/trigger_sector_map.h"



void CreateModuleList(Int_t TT_number = 27,UInt_t thr_hit = 0 ,Double_t vz_bound = 10., Double_t pt_bound = 3.)
{
    TFile* fin = TFile::Open( Form("_root/06_analysis_TTdef_10M_vz%.0f_PT%.0f.root", vz_bound, pt_bound ) );
    TH1I *h_modules_freq = (TH1I*) fin->Get(Form("h_modules_freq_TT%d",TT_number));

    TString tag = Form("_ThrHit%d",thr_hit);

    TString nameout = Form("_txt/mod_list_TT%d_fh_vz%.0f_PT%.0f",TT_number, vz_bound, pt_bound) + tag + ".txt";
    ofstream fout;
    fout.open(nameout.Data());
    cout << "Output file: " << nameout << endl;
    UInt_t kk = 0;

    UInt_t j=0;
    for(UInt_t i=0; i<(UInt_t)h_modules_freq->GetNbinsX() && j<Tracker_modID_list_size ; i++)
    {
        UInt_t aux_modID = 50000+i;
        if(aux_modID == Tracker_modID_list[j])
        {
            UInt_t aux = h_modules_freq->GetBinContent(i+1);
            j++;

            if(aux > thr_hit)
            {
                kk++;
                fout << aux_modID << endl;
            }
        }
    }

    fout.close();
    cout << "Modules written: " << kk << endl;
}


void CreateModuleList_old(Int_t TT_number = 27)
{
    TString nameout = Form("_txt/mod_list_TT%d_old.txt",TT_number);
    ofstream fout;
    fout.open(nameout.Data());
    cout << "Output file: " << nameout << endl;
    UInt_t kk = 0;

    for(UInt_t i=0; i<trigger_sector_modules[TT_number].size() ; i++)
    {

        kk++;
        fout << trigger_sector_modules[TT_number][i] << endl;
    }

    fout.close();
    cout << "Modules written: " << kk << endl;
}
