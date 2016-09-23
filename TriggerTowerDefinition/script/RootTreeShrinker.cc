#ifndef _TriggerTowerClass_h
  #define _TriggerTowerClass_h
  #include "TriggerTowerClass.h"
#endif

void RootTreeShrinker(TString fold="_root/10_stubs_tt25w_10k.root" , TString fnew = "")
{

  //  gSystem->Load("$ROOTSYS/test/libEvent");

   //Get old file, old tree and set top branch address

   TFile *oldfile = new TFile(fold);
   TTree *oldtree = (TTree*)oldfile->Get("T");
   Long64_t nentries = oldtree->GetEntries();
   Event *event   = 0;
   oldtree->SetBranchAddress("event",&event);

   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile("small.root","recreate");
   TTree *newtree = oldtree->CloneTree(0);

   for (Long64_t i=0;i<nentries; i++) {
      oldtree->GetEntry(i);
      if (event->GetNtrack() > 605) newtree->Fill();
      event->Clear();
   }
   newtree->Print();
   newtree->AutoSave();
   delete oldfile;
   delete newfile;
}
