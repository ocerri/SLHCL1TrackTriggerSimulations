#ifndef _incluse_h
  #define _incluse_h
  #include "incluse.h"
#endif

#ifndef _TTStubReader_h
  #define _TTStubReader_h
  #include "TTStubReader.h"
#endif

#ifndef _TrackParametersToTT_h
  #define _TrackParametersToTT_h
  #include "TrackParametersToTT.C"
#endif

#ifndef _modID_list_h
  #define _modID_list_h
  #include "../data/modID_list.h"
#endif


#ifndef _OCTrack_
#define _OCTrack_

class OCTrack : public TObject
{
  public:
    Double_t phi;
    Double_t eta;
    Double_t vz;
    Double_t Pt;
    Double_t charge;

    Double_t curv;
    Double_t invPT;
    Double_t phi_star;
    Double_t eta_star;

    Int_t TT_location = -100;

    OCTrack(){};
    OCTrack(Double_t init_phi, Double_t init_eta, Double_t init_vz, Double_t init_Pt, Double_t init_charge)
    {
      phi = init_phi;
      eta = init_eta;
      vz = init_vz ;
      Pt = init_Pt ;
      charge = init_charge;

      curv = charge/Pt;
      invPT = curv;
    };

    ~OCTrack(){};

    Int_t GetTT_number()
    {
        if(TT_location==-100) TT_location = TrackParametersToTT(phi, invPT, eta, vz);
        return TT_location;
    };

    void SetParameters(Double_t init_phi, Double_t init_eta, Double_t init_vz, Double_t init_Pt, Double_t init_charge)
    {
      phi = init_phi;
      eta = init_eta;
      vz = init_vz ;
      Pt = init_Pt ;
      charge = init_charge;

      curv = charge/Pt;
      invPT = curv;
      TT_location = -100;
    };

    // ClassDef(OCTrack,1);

};

// ClassImp(OCTrack);

class OCLayer{
public:
    Int_t Layer_number;

    //coordinate 0 is phi
    //coordinate 1 is z for Layer<11 and R above
    Double_t min_coord[2]={0};
    Double_t max_coord[2]={0};

    TH1D *h_coord[2] = {nullptr, nullptr};

    Int_t clear[2] = {1,1};

    OCLayer(){};

    ~OCLayer(){};

    void Setup(Int_t init_Layer_number, Int_t TT_number)
    {
        Layer_number = init_Layer_number;

        TString ycoord = "z [cm]";
        if(Layer_number>10) ycoord = "R [cm]";

        TString name = Form("h_coord0_l%d_TT%d",Layer_number,TT_number);
        h_coord[0] = new TH1D(name,name,360*2,-pigreco,pigreco);
        h_coord[0]->SetXTitle("#phi [rad]");
        h_coord[0]->SetYTitle("Number of stubs");


        name = Form("h_coord1_l%d_TT%d",Layer_number,TT_number);
        if (Layer_number>10) h_coord[1] = new TH1D(name,name,1200,0,120);
        else h_coord[1] = new TH1D(name,name,2400,-120,120);

        h_coord[1]->SetXTitle(ycoord);
        h_coord[1]->SetYTitle("Number of stubs");

    }

    void Clear()
    {
        min_coord[0]=0;
        min_coord[1]=0;
        max_coord[0]=0;
        max_coord[1]=0;

        delete h_coord[0];
        delete h_coord[1];
        h_coord[0] = nullptr;
        h_coord[1] = nullptr;


        clear[0]=1;
        clear[1]=1;
    }

    void UpdateBoundaries(Int_t coord, Double_t x)
    {
        if(clear[coord])
        {
            min_coord[coord] = x;
            max_coord[coord] = x;
            clear[coord] = 0;
        }
        else
        {
            if (x<min_coord[coord]) min_coord[coord]=x;

            if (x>max_coord[coord]) max_coord[coord]=x;
        }
    }

    void FillLayer(Double_t phi, Double_t x)
    {
        h_coord[0]->Fill(phi);
        h_coord[1]->Fill(x);

        UpdateBoundaries(0,phi);
        UpdateBoundaries(1,x);
    }

    void PrintFileLimits(Int_t TT_number, ofstream *fout)
    {
        (*fout) << TT_number << " " << Layer_number << " " << min_coord[0] << " " << max_coord[0] << " " << min_coord[1] << " " << max_coord[1] << endl;
    }

    void PrintScreenLimits(Int_t TT_number)
    {
        cout << TT_number << " " << Layer_number << " " << min_coord[0] << " " << max_coord[0] << " " << min_coord[1] << " " << max_coord[1] << endl;
    }

    TString GetBoundariesString(Int_t TT_number)
    {
        TString out = Form("%d,%02d,%.4f,%.4f,%.4f,%.4f",TT_number, Layer_number,min_coord[0], max_coord[0], min_coord[1], max_coord[1]);
        return out;
    }

    Int_t GetEntries()
    {
        return h_coord[0]->GetEntries();
    }
};

class OCModule{
public:
  Int_t modID;
  Double_t Nstub;

  OCModule(Int_t mID, Double_t nhit)
  {
      modID = mID;
      Nstub = nhit;
  }

};

Bool_t LargerNofStubs(OCModule m1, OCModule m2)
{
    return (m1.Nstub > m2.Nstub);
}


class TTeff_results
{
public:
    Double_t eff5 = 0;
    Double_t eff6 = 0;
    Double_t deff5 = 0;
    Double_t deff6 = 0;
};

#endif

#ifndef _OCModulesList_
  #define _OCModulesList_
  class OCModulesList
  {
    public:
    vector<UInt_t> modulesID[23];


    Int_t WriteModList_fromVector(TString tag = "")
    {
        TString nameout = "_txt/tracks_list" + tag + "_modules.txt";
        ofstream fout;
        fout.open(nameout.Data());
        // cout << "Output file: " << nameout << endl;
        UInt_t kk = 0;
        for(UInt_t i=5; i<23; i++)
        {
            for(UInt_t j=0; j<modulesID[i].size() ; j++)
            {
                fout << modulesID[i][j] << endl;
                kk++;
            }
        }


        fout.close();
        // cout << "Modules written: " << kk << endl;

        return kk;
    }


    Int_t AddModule(UInt_t modID)
    {
        if(modID < 50000 || modID > 221479) return 0;

        UInt_t Nl = modID/10000;
        Int_t stop = 0;
        for(ULong_t i=0; i<modulesID[Nl].size() && !stop; i++)
        {

            if(modulesID[Nl][i] == modID)
            {
                stop=1;
            }
            else if(modulesID[Nl][i] > modID)
            {
                modulesID[Nl].push_back(modID);
                sort(modulesID[Nl].begin(), modulesID[Nl].end() );
                stop = 1;
            }
        }
        if(stop == 0 ) modulesID[Nl].push_back(modID);
        return 1;
    }

  };
#endif


#ifndef _OCTriggerTower_
#define _OCTriggerTower_

class OCTriggerTower : public TObject
{
  public:
    Int_t TT_number;

    Int_t Eta_number;
    Int_t Phi_number;

    Double_t vz_lim;
    Double_t PT_lim;

    //Particles that are in the phase space parameter
    TH2D *h_part_inTT_PC = nullptr;
    TH2D *h_part_inTT_VE = nullptr;

    // Particles that hit the modules of the TT
    TH2D *h_part_inMod_PC = nullptr;
    TH2D *h_part_inMod_VE = nullptr;

    // Particles sample
    TH2D *h_part_sample_PC = nullptr;
    TH2D *h_part_sample_VE = nullptr;

    vector<UInt_t> modulesID[23];
    TH1I *h_modules_freq = nullptr;
    UInt_t N_tot_modules;

    OCLayer layer[23];



    OCTriggerTower(){};

    OCTriggerTower(Int_t init_TT_number, Int_t nbins_eta = 500, Int_t nbins_phi = 360, Int_t nbins_curv = 100, Int_t nbins_vz = 100)
    {
        TT_number = init_TT_number;
        Eta_number = TTNumberToEtaNumber(TT_number);
        Phi_number = TTNumberToPhiNumber(TT_number);

        h_part_inTT_PC = new TH2D(Form("h_part_inTT%d_curv_phi",TT_number),Form("Curvature-#phi distribution of particles in TT%d",TT_number),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
        h_part_inTT_PC->SetXTitle("#phi [rad]");
        h_part_inTT_PC->SetYTitle("q/P_{T} [1/GeV]");

        h_part_inTT_VE = new TH2D(Form("h_part_inTT%d_vz_eta",TT_number),Form("Vertex position-#eta distribution of particles in TT%d",TT_number),nbins_eta,-2.5,2.5,nbins_vz,-25,25);
        h_part_inTT_VE->SetXTitle("#eta");
        h_part_inTT_VE->SetYTitle("Vertex Z position [cm]");

        h_modules_freq = new TH1I(Form("h_modules_freq_TT%d",TT_number),Form("Stubs per modules TT%d",TT_number),171480,49999.5,221479.5);

    };

    ~OCTriggerTower(){};

    void SetupTriggerTower(Int_t init_TT_number, Double_t init_vz_lim = vz_max_TTdef, Double_t init_PT_lim = 1/invPT_max_TTdef, Int_t nbins_eta = 500, Int_t nbins_phi = 360, Int_t nbins_curv = 100, Int_t nbins_vz = 100)
    {
        cout << "Setting up TT" << init_TT_number << endl;
        TT_number = init_TT_number;
        Eta_number = TTNumberToEtaNumber(TT_number);
        Phi_number = TTNumberToPhiNumber(TT_number);

        vz_lim = init_vz_lim;
        PT_lim = init_PT_lim;

        delete h_part_inTT_PC;
        h_part_inTT_PC = new TH2D(Form("h_part_inTT%d_curv_phi",TT_number),Form("Curvature-#phi distribution of particles in TT%d",TT_number),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
        h_part_inTT_PC->SetXTitle("#phi [rad]");
        h_part_inTT_PC->SetYTitle("q/P_{T} [1/GeV]");

        delete h_part_inTT_VE;
        h_part_inTT_VE = new TH2D(Form("h_part_inTT%d_vz_eta",TT_number),Form("Vertex position-#eta distribution of particles in TT%d",TT_number),nbins_eta,-2.5,2.5,nbins_vz,-25,25);
        h_part_inTT_VE->SetXTitle("#eta");
        h_part_inTT_VE->SetYTitle("Vertex Z position [cm]");

        delete h_modules_freq;
        h_modules_freq = new TH1I(Form("h_modules_freq_TT%d",TT_number),Form("Stubs per modules TT%d",TT_number),171480,49999.5,221479.5);

        for(UInt_t i=0; i<23; i++)
        {
            modulesID[i].clear();
            layer[i].Clear();
            layer[i].Setup(i, TT_number);
        }
    };

    Int_t is_module_inside(UInt_t modID)
    {
        if(modID < 50000 || modID > 221479) return 0;

        UInt_t Nl = modID/10000;
        for(ULong_t i=0; i<modulesID[Nl].size(); i++)
        {
            if(modulesID[Nl][i] == modID)
            {
                return 1;
            }
            else if(modulesID[Nl][i] > modID)
            {
                return 0;
            }
        }
        return 0;
    }

    Double_t GetModuleFrequency(Int_t modId)
    {
          if(modId>=50000 && modId <= 221480)
          {
              UInt_t index = modId - 50000;
              return h_modules_freq->GetBinContent(index+1);
          }
          else return 0;
    }

    Int_t is_in_barrel()
    {
        if (Eta_number == 3 || Eta_number == 4) return 1;
        else return 0;
    }

    Int_t is_in_intermediate()
    {
        if (Eta_number == 2 || Eta_number == 5) return 1;
        else return 0;
    }

    Int_t is_in_endcap()
    {
        if (Eta_number == 1 || Eta_number == 6) return 1;
        else return 0;
    }

    Int_t RemoveModule(UInt_t modID)
    {
        if(modID < 50000 || modID > 221479) return 0;

        UInt_t Nl = modID/10000;
        Int_t stop = 0;
        Int_t removed = 0;

        for(ULong_t i=0; i<modulesID[Nl].size() && !stop; i++)
        {

            if(modulesID[Nl][i] == modID)
            {
                modulesID[Nl].erase(modulesID[Nl].begin() + i) ;
                stop=1;
                removed = 1;
            }
            else if(modulesID[Nl][i] > modID)
            {
                stop = 1;
            }
        }

        return removed;
    }

    Int_t AddModule(UInt_t modID)
    {
        if(modID < 50000 || modID > 221479) return 0;

        UInt_t Nl = modID/10000;
        Int_t stop = 0;
        for(ULong_t i=0; i<modulesID[Nl].size() && !stop; i++)
        {

            if(modulesID[Nl][i] == modID)
            {
                stop=1;
            }
            else if(modulesID[Nl][i] > modID)
            {
                modulesID[Nl].push_back(modID);
                sort(modulesID[Nl].begin(), modulesID[Nl].end() );
                stop = 1;
            }
        }
        if(stop == 0 ) modulesID[Nl].push_back(modID);
        return 1;
    }

    Int_t is_track_inside(OCTrack t)
    {
        if(t.GetTT_number() == TT_number) return 1;
        else return 0;
    };

    Int_t GetN_tot_modules()
    {
        N_tot_modules=0;
        for(UInt_t i = 0; i<23; i++)
        {
            N_tot_modules += modulesID[i].size();
        }
        return N_tot_modules;
    }

    vector<UInt_t> GetSharedModules(OCTriggerTower TT_2)
    {
        vector<UInt_t> smod; //Shared modules

        for(UInt_t il=5; il<23; il++)
        {
            for(UInt_t im=0; im<modulesID[il].size(); im++)
            {
                Int_t tmp_modID = modulesID[il][im];
                if(TT_2.is_module_inside(tmp_modID)) smod.push_back(tmp_modID);
            }
        }

        return smod;
    }


    TTeff_results GetEfficiecy(Long64_t nentries = 1e6, Int_t save_tracks = 0, TString tag="", Int_t max_written_tracks = 3)
    {
        TString filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";
        // Open the file
        // std::cout << "Opening file..." << std::endl;
        TTStubReader reader;
        reader.init(filename);
        if(nentries==-1) nentries = reader.getEntries();

        ULong_t denominator=0;
        ULong_t numerator[]={0,0};

        //---Track saver parameters--------------------
        TString nameout = Form("_txt/tracks_list_TT%d",TT_number) + tag + Form("_%d.txt",max_written_tracks);
        ofstream fout;
        Int_t NTracksWritten = 0;
        OCModulesList mod_list;

        if(save_tracks)
        {
          fout.open(nameout.Data());
          fout << "#PT eta phi vx vy vz charge" << endl;
          // cout << "file opened: " << nameout.Data() << endl;
        }
        //----------------------------------

        for (Long64_t ievt = 0; ievt < nentries; ++ievt)
        {
            show_progress_bar(ievt, nentries);
            // Retrieve the event
            reader.getEntry(ievt);


            // Get the variables
            // See TTStubReader.h for more info
            float Pt     = reader.vp_pt    ->front();
            float eta    = reader.vp_eta   ->front();
            float phi    = reader.vp_phi   ->front();
            float vz     = reader.vp_vz    ->front();
            float vx     = reader.vp_vx    ->front();
            float vy     = reader.vp_vy    ->front();
            int   charge = reader.vp_charge->front();

            OCTrack track(phi, eta, vz, Pt, charge);
            Int_t aux_TT_number = track.GetTT_number();

            unsigned nstubs = reader.vb_modId->size();

            if(aux_TT_number==TT_number)// && nstubs>=5)
            {
                denominator++;

                // Int_t N_TT_modules_hitted = 0;
                Int_t N_stub_layer[23] = {0};
                // Int_t N_stub_tot = 0;
                //
                for (unsigned istub=0; istub < nstubs; ++istub) {
                    unsigned stub_modId    = reader.vb_modId   ->at(istub);
                    if(is_module_inside(stub_modId)) N_stub_layer[stub_modId/10000]++;
                }  // end loop over stubs

                Int_t hitted_layer=0;
                for (UInt_t kk=5; kk < 23; ++kk) {
                    if(N_stub_layer[kk]>0) hitted_layer++;
                }

                if(hitted_layer >= 5) numerator[0]++;
                // else if(save_tracks && NTracksWritten<max_written_tracks)
                // {
                //     fout << Pt << " " << eta << " " << phi << " " << vx << " " << vy << " " << vz << " " << charge << endl;
                //     NTracksWritten++;
                //
                //     for (unsigned istub=0; istub < nstubs; ++istub) {
                //
                //
                //         unsigned stub_modId    = reader.vb_modId   ->at(istub);
                //         mod_list.AddModule(stub_modId);
                //
                //     }  // end loop over stubs
                // }

                if(hitted_layer >= 6) numerator[1]++;
                else if(save_tracks && NTracksWritten<max_written_tracks)
                {
                    fout << Pt << " " << eta << " " << phi << " " << vx << " " << vy << " " << vz << " " << charge << endl;
                    NTracksWritten++;

                    for (unsigned istub=0; istub < nstubs; ++istub) {


                        unsigned stub_modId    = reader.vb_modId   ->at(istub);
                        mod_list.AddModule(stub_modId);

                    }  // end loop over stubs
                }

            }


        }  // end loop over events

        if(save_tracks)
        {
            fout.close();
            mod_list.WriteModList_fromVector(Form("_TT%d",TT_number)+tag+Form("_%d",max_written_tracks));
            cout << "Number of tracks written: " << NTracksWritten << endl;
        }

        TTeff_results out;

        out.eff5 = (Double_t)numerator[0]/(Double_t)denominator;
        out.eff6 = (Double_t)numerator[1]/(Double_t)denominator;

        out.deff5 = sqrt(numerator[0]*(1-out.eff5))/(Double_t)denominator;
        out.deff6 = sqrt(numerator[1]*(1-out.eff6))/(Double_t)denominator;

        cout << "eff5 = " << numerator[0] << "/" << denominator << " = " << out.eff5 << " +/- " << out.deff5 << endl;

        return out;
    }

    Int_t WriteModList_fromVector(TString tag = "")
    {
        TString nameout = Form("_txt/mod_list_TT%d_fv_vz%.0f_PT%.0f",TT_number,vz_lim, PT_lim) + tag + ".txt";
        ofstream fout;
        fout.open(nameout.Data());
        // cout << "Output file: " << nameout << endl;
        UInt_t kk = 0;
        for(UInt_t i=5; i<23; i++)
        {
            for(UInt_t j=0; j<modulesID[i].size() ; j++)
            {
                fout << modulesID[i][j] << endl;
                kk++;
            }
        }


        fout.close();
        // cout << "Modules written: " << kk << endl;

        return kk;
    }

    Int_t WriteModuleList_fromHisto(UInt_t thr_hit = 0)
    {
        TString tag = Form("_ThrHit%d",thr_hit);

        TString nameout = Form("_txt/mod_list_fh_TT%d",TT_number) + tag + ".txt";
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

        return kk;
    }

    Int_t LoadTrainingTracks(Long64_t nentries = 2e6, TString filename = "")
    {
        if(filename == "") filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";
        else if(filename == "local") filename = "/Users/olmo/Dropbox/16_07_FERMILAB_summer_school/CMStrigger/_root/10_stubs_tt25w_10k.root";

        // Open the file
        std::cout << "Opening file..." << flush;
        TTStubReader reader;
        reader.init(filename);
        if(nentries==-1 || nentries > reader.getEntries()) nentries = reader.getEntries();
        cout << nentries << " entries to be read" << endl;


        for (Long64_t ievt = 0; ievt < nentries; ++ievt)
        {
            show_progress_bar(ievt, nentries);
            // Retrieve the event
            reader.getEntry(ievt);


            // Get the variables
            // See TTStubReader.h for more info
            float Pt     = reader.vp_pt    ->front();
            float eta    = reader.vp_eta   ->front();
            float phi    = reader.vp_phi   ->front();
            float vz     = reader.vp_vz    ->front();
            int   charge = reader.vp_charge->front();

            OCTrack track(phi, eta, vz, Pt, charge);
            Int_t aux_TT_number = track.GetTT_number();
            h_part_inTT_PC->GetEntries();
            // cout << "TT number: " << aux_TT_number << endl;

            unsigned nstubs = reader.vb_modId->size();

            if( aux_TT_number == TT_number )
            {
                h_part_inTT_PC->Fill(track.phi,track.curv);
                h_part_inTT_VE->Fill(track.eta,track.vz);


                for (unsigned istub=0; istub < nstubs; ++istub) {
                    // cout << "stub loop" << istub << endl;
                    unsigned stub_modId    = reader.vb_modId   ->at(istub);

                    AddModule(stub_modId);
                    h_modules_freq->Fill(stub_modId);

                    Int_t aux_NLayer = stub_modId/10000;

                    float    stub_z        = reader.vb_z       ->at(istub);
                    float    stub_r        = reader.vb_r       ->at(istub);
                    float    stub_phi      = reader.vb_phi     ->at(istub);

                    Double_t coord1 = (aux_NLayer>10)? stub_r : stub_z;
                    layer[aux_NLayer].FillLayer(stub_phi, coord1);

                }  // end loop over stubs
            }


        }  // end loop over events


        return h_part_inTT_PC->GetEntries();
    }

    Int_t PhaseSpaceHittingTracks(Long64_t nentries = 2e6, Int_t NStubLim =5, Int_t nbins_eta = 500, Int_t nbins_phi = 360, Int_t nbins_curv = 100, Int_t nbins_vz = 100)
    {
        // TString filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";
        // TString filename = "/Users/olmo/Dropbox/16_07_FERMILAB_summer_school/CMStrigger/_root/10_stubs_tt25w_10k.root";
        TString filename = "/Users/olmo/Dropbox/16_07_FERMILAB_summer_school/CMStrigger/_root/10_stubs_tt25w_10k_shrinked.root";


        // Open the file
        std::cout << "Opening file..." << flush;
        TTStubReader reader;
        reader.init(filename);
        if(nentries==-1 || nentries > reader.getEntries()) nentries = reader.getEntries();
        cout << nentries << " entries to be read" << endl;

        //Booking histos
        delete h_part_inMod_PC;
        h_part_inMod_PC = new TH2D(Form("h_part_inMod%d_curv_phi",TT_number),Form("Curvature-#phi distribution of particles in TT%d",TT_number),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
        h_part_inMod_PC->SetXTitle("#phi [rad]");
        h_part_inMod_PC->SetYTitle("q/P_{T} [1/GeV]");

        delete h_part_inMod_VE;
        h_part_inMod_VE = new TH2D(Form("h_part_inMod%d_vz_eta",TT_number),Form("Vertex position-#eta distribution of particles in TT%d",TT_number),nbins_eta,-2.5,2.5,nbins_vz,-25,25);
        h_part_inMod_VE->SetXTitle("#eta");
        h_part_inMod_VE->SetYTitle("Vertex Z position [cm]");


        delete h_part_sample_PC;
        h_part_sample_PC = new TH2D(Form("h_part_sample%d_curv_phi",TT_number),Form("Curvature-#phi distribution of particles in TT%d",TT_number),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
        h_part_sample_PC->SetXTitle("#phi [rad]");
        h_part_sample_PC->SetYTitle("q/P_{T} [1/GeV]");

        delete h_part_sample_VE;
        h_part_sample_VE = new TH2D(Form("h_part_sample%d_vz_eta",TT_number),Form("Vertex position-#eta distribution of particles in TT%d",TT_number),nbins_eta,-2.5,2.5,nbins_vz,-25,25);
        h_part_sample_VE->SetXTitle("#eta");
        h_part_sample_VE->SetYTitle("Vertex Z position [cm]");


        for (Long64_t ievt = 0; ievt < nentries; ++ievt)
        {
            show_progress_bar(ievt, nentries);
            // Retrieve the event
            reader.getEntry(ievt);


            // Get the variables
            // See TTStubReader.h for more info
            float Pt     = reader.vp_pt    ->front();
            float eta    = reader.vp_eta   ->front();
            float phi    = reader.vp_phi   ->front();
            float vz     = reader.vp_vz    ->front();
            int   charge = reader.vp_charge->front();

            OCTrack track(phi, eta, vz, Pt, charge);

            h_part_sample_PC->Fill(track.phi,track.curv);
            h_part_sample_VE->Fill(track.eta,track.vz);

            Int_t aux_TT_number = track.GetTT_number();

            unsigned nstubs = reader.vb_modId->size();


            // Int_t N_TT_modules_hitted = 0;
            Int_t N_stub_layer[23] = {0};
            // Int_t N_stub_tot = 0;
            //
            for (unsigned istub=0; istub < nstubs; ++istub) {
                unsigned stub_modId    = reader.vb_modId   ->at(istub);
                if(is_module_inside(stub_modId)) N_stub_layer[stub_modId/10000]++;
            }  // end loop over stubs

            Int_t hitted_layer=0;
            for (unsigned kk=5; kk < 23; ++kk) {
                if(N_stub_layer[kk]>0) hitted_layer++;
            }

            if(hitted_layer>=NStubLim)
            {
                h_part_inMod_PC->Fill(track.phi,track.curv);
                h_part_inMod_VE->Fill(track.eta,track.vz);
            }


        }  // end loop over events


        return h_part_inMod_PC->GetEntries();
    }

    void DrawPrePostComparison(Long64_t nentries = 2e6, Int_t save_file=0, Int_t NStubLim=5)
    {
        PhaseSpaceHittingTracks(nentries, NStubLim);
        cout << "Drawing Training/Acceptance comparison" << endl;

        TCanvas *c_cmp_PC = new TCanvas(Form("c_cmp_PC_TT%d",TT_number),Form("c_cmp_PC_TT%d",TT_number),800,600);
        c_cmp_PC->SetGrid();
        TLegend* leg_PC = new TLegend(0.75,0.12,0.98,0.3);

        h_part_sample_PC->SetMarkerColor(18);
        h_part_sample_PC->SetFillColor(18);
        h_part_sample_PC->SetLineWidth(0);
        h_part_sample_PC->SetStats(0);
        h_part_sample_PC->DrawCopy();
        leg_PC->AddEntry(h_part_sample_PC,"Sample extention","f");

        h_part_inMod_PC->SetMarkerColor(2);
        h_part_inMod_PC->SetFillColor(2);
        h_part_inMod_PC->SetLineWidth(0);
        h_part_inMod_PC->SetStats(0);
        h_part_inMod_PC->DrawCopy("SAME");
        leg_PC->AddEntry(h_part_inMod_PC,"Tower acceptance","f");

        h_part_inTT_PC->SetMarkerColor(1);
        h_part_inTT_PC->SetFillColor(1);
        h_part_inTT_PC->SetLineWidth(0);
        h_part_inTT_PC->SetStats(0);
        h_part_inTT_PC->DrawCopy("SAME");
        leg_PC->AddEntry(h_part_inTT_PC,"Training Sample","f");

        leg_PC->Draw("SAME");

        TLegend* notes = new TLegend(0.2,0.83,0.6,0.9,Form("|V_{z}| < %.0f, P_{T} > %.0f, Sample %.0fM", vz_lim, PT_lim, nentries/1.e6));
        notes->Draw("SAME");


        TCanvas *c_cmp_VE = new TCanvas(Form("c_cmp_VE_TT%d",TT_number),Form("c_cmp_VE_TT%d",TT_number),800,600);
        c_cmp_VE->SetGrid();
        TLegend* leg_VE = new TLegend(0.75,0.12,0.98,0.3);


        h_part_sample_VE->SetMarkerColor(18);
        h_part_sample_VE->SetFillColor(18);
        h_part_sample_VE->SetLineWidth(0);
        h_part_sample_VE->SetStats(0);
        h_part_sample_VE->DrawCopy();
        leg_VE->AddEntry(h_part_sample_VE,"Sample extention","f");

        h_part_inMod_VE->SetMarkerColor(2);
        h_part_inMod_VE->SetFillColor(2);
        h_part_inMod_VE->SetLineWidth(0);
        h_part_inMod_VE->SetStats(0);
        h_part_inMod_VE->DrawCopy("SAME");
        leg_VE->AddEntry(h_part_inMod_VE,"Reco tracks","f");

        h_part_inTT_VE->SetMarkerColor(1);
        h_part_inTT_VE->SetFillColor(1);
        h_part_inTT_VE->SetLineWidth(0);
        h_part_inTT_VE->SetStats(0);
        h_part_inTT_VE->DrawCopy("SAME");
        leg_VE->AddEntry(h_part_inTT_VE,"Definition","f");

        leg_VE->Draw("SAME");
        notes->Draw("SAME");

        if(save_file)
        {
            c_cmp_PC->SaveAs(Form("_fig/07_cmp_PC_TT%d_vz%.0f_PT%.0f.png",TT_number,vz_lim, PT_lim));
            c_cmp_VE->SaveAs(Form("_fig/07_cmp_VE_TT%d_vz%.0f_PT%.0f.png",TT_number,vz_lim, PT_lim));
        }
    }


    void ClearModulesID()
    {
        for(UInt_t i=0; i<23; i++)
        {
            modulesID[i].clear();
        }
    }

    Double_t GetIntegralLayer(Int_t Nl = 5)
    {
        Int_t min_bin = 10000*(Nl-5) +1;
        Int_t max_bin = min_bin + 10000 - 2;

        return h_modules_freq->Integral(min_bin,max_bin);
    }



    Int_t Generate_modulesID_VectorFromHisto(Double_t thr_acc = 1.)
    {
        ClearModulesID();

        vector<OCModule> modList[23];

        // create the modules list structure
        UInt_t j=0;
        for(UInt_t i=0; i<(UInt_t)h_modules_freq->GetNbinsX() && j<Tracker_modID_list_size ; i++)
        {
            UInt_t aux_modID = 50000+i;
            if(aux_modID == Tracker_modID_list[j])
            {
                UInt_t aux = h_modules_freq->GetBinContent(i+1);
                j++;

                OCModule aux_mod(aux_modID,aux);
                modList[aux_modID/10000].push_back(aux_mod);
            }
        }

        UInt_t kk=0;
        for(UInt_t i=5; i<23; i++)
        {
            Double_t integration_limit = thr_acc*GetIntegralLayer(i);

            sort(modList[i].begin(), modList[i].end(), LargerNofStubs);

            Double_t sum_stubs = 0;
            UInt_t jj=0;
            while(sum_stubs < integration_limit)
            {
                sum_stubs += modList[i][jj].Nstub;
                AddModule(modList[i][jj].modID);
                jj++;

                kk++;
            }
        }

        return kk;
    }

    void LoadFromFiles(Int_t init_TT_number=25, Double_t init_vz = 15., Double_t init_pt = 3., Int_t version=10, Double_t nentries = 1e7)
    {
      if(init_TT_number!=-1)
      {
          TT_number = init_TT_number;
          Eta_number = TTNumberToEtaNumber(TT_number);
          Phi_number = TTNumberToPhiNumber(TT_number);
          vz_lim = init_vz;
          PT_lim = init_pt;

          vz_max_TTdef = vz_lim;
          invPT_max_TTdef = 1/PT_lim;
      }

      TFile *f_in = TFile::Open(Form("_root/%02d_analysis_TTdef_%1.0fM_vz%.0f_PT%.0f.root",version, nentries/1e6, vz_lim, PT_lim));
      h_modules_freq = (TH1I*) f_in->Get(Form("h_modules_freq_TT%d",TT_number));

      cout << "histo loaded" << endl;

      Generate_modulesID_VectorFromHisto();

      cout << "modulesID created" << endl;

    }


    void PrintLayersBoundaries(UInt_t imin =5, UInt_t imax = 23)
    {
        cout << "Layer Boundaries:" << endl;
        for(UInt_t i=imin; i<imax; i++)
        {
            if(layer[i].GetEntries()>0) layer[i].PrintScreenLimits(TT_number);
        }
    }

    void SaveLayersBoundaries(TString tag = "", UInt_t imin =5, UInt_t imax = 23)
    {
        TString nameout = Form("_txt/LayerBoundaries_TT%d_vz%.0f_PT%.0f",TT_number,vz_lim, PT_lim) + tag + ".txt";
        ofstream *fout = new ofstream();
        fout->open(nameout.Data());

        for(UInt_t i=imin; i<imax; i++)
        {
            if(layer[i].GetEntries()>0) layer[i].PrintFileLimits(TT_number, fout);
        }

        fout->close();
    }
    // ClassDef(OCTriggerTower,1);
};


// ClassImp(OCTriggerTower);

#endif
