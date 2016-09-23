#include <iostream>

#ifndef _TriggerTowerClass_h
  #define _TriggerTowerClass_h
  #include "TriggerTowerClass.h"
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

void TracksFinderAndParameterWriter(Long64_t nentries = 10e6, Int_t max_written_tracks = 10e6, TString tag="_6_time_shared_modules") {

    //TString filename = "root://cmsxrootd-site.fnal.gov//store/user/l1upgrades/SLHC/GEN/Demo_Emulator/stubs_tt27_300M_emu.root";
    // TString filename = "root://xrootd2.ihepa.ufl.edu//store/user/jiafulow/L1TrackTrigger/6_2_0_SLHC25p3/Demo_Emulator/stubs_tt27_300M_emu.root";
    // TString filename = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/Demo_Emulator/stubs_tt27_300M_emu.root";
    TString filename = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim_SLHC25p3_naper/tt48/stubs_tt48_50M.0.root";


    // Open the file
    std::cout << "Opening file..." << std::endl;

    TTStubReader reader;
    reader.init(filename);

    // TString nameout = Form("_txt/track_list_%ld",time(nullptr)) + tag + ".txt";
    TString nameout = "_txt/tracks_list" + tag + ".txt";

    ofstream fout;
    fout.open(nameout.Data());
    fout << "#PT eta phi vx vy vz charge" << endl;
    Int_t NTracksWritten = 0;

    std::cout << "Number of events: " << nentries << std::endl;

    OCModulesList mod_list;
    Int_t nbins_eta = 500;
    Int_t nbins_phi = 360;
    Int_t nbins_curv = 100;
    Int_t nbins_vz = 100;

    TH2D* h_part_PC = new TH2D(Form("h_part_curv_phi"),Form("Curvature-#phi distribution of particles"),nbins_phi,-pigreco,pigreco,nbins_curv,-1.,1.);
    h_part_PC->SetXTitle("#phi [rad]");
    h_part_PC->SetYTitle("q/P_{T} [1/GeV]");

    TH2D* h_part_VE = new TH2D(Form("h_part_vz_eta"),Form("Vertex position-#eta distribution of particles"),nbins_eta,-2.5,2.5,nbins_vz,-25,25);
    h_part_VE->SetXTitle("#eta");
    h_part_VE->SetYTitle("Vertex Z position [cm]");
    // Loop over events
    // There is only one track per event
    for (Long64_t ievt = 0; NTracksWritten < max_written_tracks && ievt < nentries; ++ievt) {
        // Retrieve the event
        show_progress_bar(ievt, nentries);

        reader.getEntry(ievt);

        // Get the variables
        // See TTStubReader.h for more info
        float pt     = reader.vp_pt    ->front();
        float eta    = reader.vp_eta   ->front();
        float phi    = reader.vp_phi   ->front();
        float vx     = reader.vp_vx    ->front();
        float vy     = reader.vp_vy    ->front();
        float vz     = reader.vp_vz    ->front();
        int   charge = reader.vp_charge->front();


        unsigned nstubs = reader.vb_modId->size();


        // if (nstubs<=5 && fabs(eta)<1. && fabs(eta)>0.9)
        // if (fabs(eta)<.7 && phi<1.01*pigreco/4 && phi>.99*pigreco/4 && pt>3)
        if(fabs(vz)>10 && fabs(vz)<15)
        {
            UInt_t mod_looking4 = 51429;
            Int_t mod_found = 0;
            for (unsigned istub=0; istub < nstubs; ++istub) {


                unsigned stub_modId    = reader.vb_modId   ->at(istub);
                if (stub_modId == mod_looking4) mod_found=1;

            }  // end loop over stubs

            if(mod_found)
            {
                fout << pt << " " << eta << " " << phi << " " << vx << " " << vy << " " << vz << " " << charge << endl;
                NTracksWritten++;
                h_part_VE->Fill(eta,vz);
                h_part_PC->Fill(phi,charge/pt);


                for (unsigned istub=0; istub < nstubs; ++istub) {


                    unsigned stub_modId    = reader.vb_modId   ->at(istub);
                    mod_list.AddModule(stub_modId);

                }  // end loop over stubs
            }
        }

        // std::cout << std::endl;

    }  // end loop over events
    fout.close();
    mod_list.WriteModList_fromVector(tag);

    cout << "Number of tracks written: " << NTracksWritten << endl;
    TCanvas *c_PC = new TCanvas("c_PC","c_PC",800,600);
    c_PC->SetGrid();
    h_part_PC->Draw("Cont2");
    c_PC->SaveAs("_fig/10_tracks_6_share_MOD_PC.pdf");


    TCanvas *c_VE = new TCanvas("c_VE","c_VE",800,600);
    c_VE->SetGrid();
    h_part_VE->Draw("Cont2");

    c_VE->SaveAs("_fig/10_tracks_6_share_MOD_VE.pdf");

}  // end analyze()
