//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr  2 11:40:42 2015 by ROOT version 5.34/26
// from TTree tree/
// found on file: roads.root
//////////////////////////////////////////////////////////

#ifndef roadsStubs_h
#define roadsStubs_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <iostream>
//#include <tuple>

using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

struct patternS {
	double meanPt   ;
	double rmsPt    ;
	double meanPhi  ;
	double rmsPhi   ;
	double frequency;
	int    chargeSum;
	std::vector <unsigned int > superstripIds;

//	bool operator<(const patternS& a) const
//	{
//		return frequency< a.frequency;
//	}
	bool operator<(const patternS& b) const
	{
		//		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[2],a.superstripIds[3],a.superstripIds[4],a.superstripIds[5])
		//		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
		return std::tie(  superstripIds[0],  superstripIds[1],  superstripIds[2],  superstripIds[3],  superstripIds[4],  superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
	}
	bool cmp (const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[2],a.superstripIds[3],a.superstripIds[4],a.superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
	}
	static bool cmp0(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[1],a.superstripIds[2],a.superstripIds[3],a.superstripIds[4],a.superstripIds[5])
		<      std::tie(b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
	}
	static bool cmp1(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[2],a.superstripIds[3],a.superstripIds[4],a.superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
	}
	static bool cmp2(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[3],a.superstripIds[4],a.superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[3],b.superstripIds[4],b.superstripIds[5]);
	}
	static bool cmp3(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[2],a.superstripIds[4],a.superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[4],b.superstripIds[5]);
	}
	static bool cmp4(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[2],a.superstripIds[3],a.superstripIds[5])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[5]);
	}
	static bool cmp5(const patternS& a, const patternS& b)
	{
		return std::tie(a.superstripIds[0],a.superstripIds[1],a.superstripIds[2],a.superstripIds[3],a.superstripIds[4])
		<      std::tie(b.superstripIds[0],b.superstripIds[1],b.superstripIds[2],b.superstripIds[3],b.superstripIds[4]);
	}

};


class roadsStubs {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<bool>    *TTStubs_barrel;
   vector<bool>    *TTStubs_isCombinatoric;
   vector<bool>    *TTStubs_isGenuine;
   vector<bool>    *TTStubs_isUnknown;
   vector<bool>    *TTStubs_psmodule;
   vector<bool>    *trkParts_intime;
   vector<bool>    *trkParts_primary;
   vector<bool>    *trkParts_signal;
   Float_t         gen_trueNPV;
   Float_t         gen_weightMC;
   Float_t         gen_weightPDF;
   Float_t         gen_weightPU;
   vector<float>   *gen_puZpositions;
   vector<float>   *genParts_E;
   vector<float>   *genParts_M;
   vector<float>   *genParts_eta;
   vector<float>   *genParts_phi;
   vector<float>   *genParts_pt;
   vector<float>   *genParts_px;
   vector<float>   *genParts_py;
   vector<float>   *genParts_pz;
   vector<float>   *genParts_vx;
   vector<float>   *genParts_vy;
   vector<float>   *genParts_vz;
   vector<float>   *simTracks_charge;
   vector<float>   *simTracks_eta;
   vector<float>   *simTracks_phi;
   vector<float>   *simTracks_pt;
   vector<float>   *simTracks_px;
   vector<float>   *simTracks_py;
   vector<float>   *simTracks_pz;
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_dirx;
   vector<float>   *TTStubs_diry;
   vector<float>   *TTStubs_dirz;
   vector<float>   *TTStubs_eta;
   vector<float>   *TTStubs_phi;
   vector<float>   *TTStubs_r;
   vector<float>   *TTStubs_roughPt;
   vector<float>   *TTStubs_separation;
   vector<float>   *TTStubs_simEta;
   vector<float>   *TTStubs_simPhi;
   vector<float>   *TTStubs_simPt;
   vector<float>   *TTStubs_trigBend;
   vector<float>   *TTStubs_trigDist;
   vector<float>   *TTStubs_trigOffset;
   vector<float>   *TTStubs_trigPos;
   vector<float>   *TTStubs_x;
   vector<float>   *TTStubs_y;
   vector<float>   *TTStubs_z;
   vector<float>   *trkParts_eta;
   vector<float>   *trkParts_phi;
   vector<float>   *trkParts_pt;
   vector<float>   *trkParts_px;
   vector<float>   *trkParts_py;
   vector<float>   *trkParts_pz;
   vector<float>   *trkParts_vx;
   vector<float>   *trkParts_vy;
   vector<float>   *trkParts_vz;
   vector<vector<float> > *TTStubs_hitXs;
   vector<vector<float> > *TTStubs_hitYs;
   vector<vector<float> > *TTStubs_hitZs;
   Int_t           bx;
   Int_t           orbit;
   Int_t           gen_nPV;
   vector<int>     *gen_puNtrks;
   vector<int>     *genParts_charge;
   vector<int>     *genParts_pdgId;
   vector<int>     *genParts_status;
   vector<int>     *simTracks_genpId;
   vector<int>     *simTracks_pdgId;
   vector<int>     *simTracks_vtxId;
   vector<int>     *TTStubs_detWindow;
   vector<int>     *TTStubs_pdgId;
   vector<int>     *TTStubs_tpId;
   vector<int>     *TTStubs_trkId;
   vector<int>     *trkParts_charge;
   vector<int>     *trkParts_genpId;
   vector<int>     *trkParts_nhits;
   vector<int>     *trkParts_ntkhits;
   vector<int>     *trkParts_ntklayers;
   vector<int>     *trkParts_pdgId;
   vector<int>     *trkParts_trkId;
   vector<vector<int> > *TTStubs_hitChans;
   vector<vector<int> > *TTStubs_hitTrkIds;
   UInt_t          event;
   UInt_t          lumi;
   UInt_t          run;
   UInt_t          genParts_size;
   UInt_t          simTracks_size;
   UInt_t          TTStubs_size;
   UInt_t          trkParts_size;
   vector<unsigned int> *gen_randomSeeds;
   vector<unsigned int> *simTracks_evtId;
   vector<unsigned int> *simTracks_trkId;
   vector<unsigned int> *TTStubs_clusId0;
   vector<unsigned int> *TTStubs_clusId1;
   vector<unsigned int> *TTStubs_clusWidth;
   vector<unsigned int> *TTStubs_geoId0;
   vector<unsigned int> *TTStubs_geoId1;
   vector<unsigned int> *TTStubs_modId;
   vector<unsigned int> *TTStubs_nhits;
   vector<unsigned int> *TTStubs_stackId;
   vector<unsigned int> *trkParts_evtId;
   vector<vector<unsigned int> > *TTStubs_hitEvtIds;
   vector<vector<unsigned int> > *trkParts_trkIds;
   vector<unsigned int> *AMTTRoads_patternRef;
   vector<unsigned int> *AMTTRoads_tower;
   vector<unsigned int> *AMTTRoads_nstubs;
   vector<vector<unsigned int> > *AMTTRoads_superstripIds;
   vector<vector<vector<unsigned int> > > *AMTTRoads_stubRefs;

   // List of branches
   TBranch        *b_TTStubs_barrel;   //!
   TBranch        *b_TTStubs_isCombinatoric;   //!
   TBranch        *b_TTStubs_isGenuine;   //!
   TBranch        *b_TTStubs_isUnknown;   //!
   TBranch        *b_TTStubs_psmodule;   //!
   TBranch        *b_trkParts_intime;   //!
   TBranch        *b_trkParts_primary;   //!
   TBranch        *b_trkParts_signal;   //!
   TBranch        *b_gen_trueNPV;   //!
   TBranch        *b_gen_weightMC;   //!
   TBranch        *b_gen_weightPDF;   //!
   TBranch        *b_gen_weightPU;   //!
   TBranch        *b_gen_puZpositions;   //!
   TBranch        *b_genParts_E;   //!
   TBranch        *b_genParts_M;   //!
   TBranch        *b_genParts_eta;   //!
   TBranch        *b_genParts_phi;   //!
   TBranch        *b_genParts_pt;   //!
   TBranch        *b_genParts_px;   //!
   TBranch        *b_genParts_py;   //!
   TBranch        *b_genParts_pz;   //!
   TBranch        *b_genParts_vx;   //!
   TBranch        *b_genParts_vy;   //!
   TBranch        *b_genParts_vz;   //!
   TBranch        *b_simTracks_charge;   //!
   TBranch        *b_simTracks_eta;   //!
   TBranch        *b_simTracks_phi;   //!
   TBranch        *b_simTracks_pt;   //!
   TBranch        *b_simTracks_px;   //!
   TBranch        *b_simTracks_py;   //!
   TBranch        *b_simTracks_pz;   //!
   TBranch        *b_TTStubs_coordx;   //!
   TBranch        *b_TTStubs_coordy;   //!
   TBranch        *b_TTStubs_dirx;   //!
   TBranch        *b_TTStubs_diry;   //!
   TBranch        *b_TTStubs_dirz;   //!
   TBranch        *b_TTStubs_eta;   //!
   TBranch        *b_TTStubs_phi;   //!
   TBranch        *b_TTStubs_r;   //!
   TBranch        *b_TTStubs_roughPt;   //!
   TBranch        *b_TTStubs_separation;   //!
   TBranch        *b_TTStubs_simEta;   //!
   TBranch        *b_TTStubs_simPhi;   //!
   TBranch        *b_TTStubs_simPt;   //!
   TBranch        *b_TTStubs_trigBend;   //!
   TBranch        *b_TTStubs_trigDist;   //!
   TBranch        *b_TTStubs_trigOffset;   //!
   TBranch        *b_TTStubs_trigPos;   //!
   TBranch        *b_TTStubs_x;   //!
   TBranch        *b_TTStubs_y;   //!
   TBranch        *b_TTStubs_z;   //!
   TBranch        *b_trkParts_eta;   //!
   TBranch        *b_trkParts_phi;   //!
   TBranch        *b_trkParts_pt;   //!
   TBranch        *b_trkParts_px;   //!
   TBranch        *b_trkParts_py;   //!
   TBranch        *b_trkParts_pz;   //!
   TBranch        *b_trkParts_vx;   //!
   TBranch        *b_trkParts_vy;   //!
   TBranch        *b_trkParts_vz;   //!
   TBranch        *b_TTStubs_hitXs;   //!
   TBranch        *b_TTStubs_hitYs;   //!
   TBranch        *b_TTStubs_hitZs;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_gen_nPV;   //!
   TBranch        *b_gen_puNtrks;   //!
   TBranch        *b_genParts_charge;   //!
   TBranch        *b_genParts_pdgId;   //!
   TBranch        *b_genParts_status;   //!
   TBranch        *b_simTracks_genpId;   //!
   TBranch        *b_simTracks_pdgId;   //!
   TBranch        *b_simTracks_vtxId;   //!
   TBranch        *b_TTStubs_detWindow;   //!
   TBranch        *b_TTStubs_pdgId;   //!
   TBranch        *b_TTStubs_tpId;   //!
   TBranch        *b_TTStubs_trkId;   //!
   TBranch        *b_trkParts_charge;   //!
   TBranch        *b_trkParts_genpId;   //!
   TBranch        *b_trkParts_nhits;   //!
   TBranch        *b_trkParts_ntkhits;   //!
   TBranch        *b_trkParts_ntklayers;   //!
   TBranch        *b_trkParts_pdgId;   //!
   TBranch        *b_trkParts_trkId;   //!
   TBranch        *b_TTStubs_hitChans;   //!
   TBranch        *b_TTStubs_hitTrkIds;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_run;   //!
   TBranch        *b_genParts_size;   //!
   TBranch        *b_simTracks_size;   //!
   TBranch        *b_TTStubs_size;   //!
   TBranch        *b_trkParts_size;   //!
   TBranch        *b_gen_randomSeeds;   //!
   TBranch        *b_simTracks_evtId;   //!
   TBranch        *b_simTracks_trkId;   //!
   TBranch        *b_TTStubs_clusId0;   //!
   TBranch        *b_TTStubs_clusId1;   //!
   TBranch        *b_TTStubs_clusWidth;   //!
   TBranch        *b_TTStubs_geoId0;   //!
   TBranch        *b_TTStubs_geoId1;   //!
   TBranch        *b_TTStubs_modId;   //!
   TBranch        *b_TTStubs_nhits;   //!
   TBranch        *b_TTStubs_stackId;   //!
   TBranch        *b_trkParts_evtId;   //!
   TBranch        *b_TTStubs_hitEvtIds;   //!
   TBranch        *b_trkParts_trkIds;   //!
   TBranch        *b_AMTTRoads_patternRef;   //!
   TBranch        *b_AMTTRoads_tower;   //!
   TBranch        *b_AMTTRoads_nstubs;   //!
   TBranch        *b_AMTTRoads_superstripIds;   //!
   TBranch        *b_AMTTRoads_stubRefs;   //!

   roadsStubs(TString sTree, TTree* tree);
   virtual ~roadsStubs();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef roadsStubs_cxx
roadsStubs::roadsStubs(TString sTree, TTree* tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	if (sTree != "") {
		sTree=TString("/data/rossin/EOS/")+sTree;
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(sTree);
      if (!f || !f->IsOpen()) {
         f = new TFile(sTree);
      }
      TDirectory * dir = (TDirectory*)f->Get(sTree+":/ntupler");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

roadsStubs::~roadsStubs()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t roadsStubs::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t roadsStubs::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void roadsStubs::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

	   // Set object pointer
	   TTStubs_barrel = 0;
	   TTStubs_isCombinatoric = 0;
	   TTStubs_isGenuine = 0;
	   TTStubs_isUnknown = 0;
	   TTStubs_psmodule = 0;
	   trkParts_intime = 0;
	   trkParts_primary = 0;
	   trkParts_signal = 0;
	   gen_puZpositions = 0;
	   genParts_E = 0;
	   genParts_M = 0;
	   genParts_eta = 0;
	   genParts_phi = 0;
	   genParts_pt = 0;
	   genParts_px = 0;
	   genParts_py = 0;
	   genParts_pz = 0;
	   genParts_vx = 0;
	   genParts_vy = 0;
	   genParts_vz = 0;
	   simTracks_charge = 0;
	   simTracks_eta = 0;
	   simTracks_phi = 0;
	   simTracks_pt = 0;
	   simTracks_px = 0;
	   simTracks_py = 0;
	   simTracks_pz = 0;
	   TTStubs_coordx = 0;
	   TTStubs_coordy = 0;
	   TTStubs_dirx = 0;
	   TTStubs_diry = 0;
	   TTStubs_dirz = 0;
	   TTStubs_eta = 0;
	   TTStubs_phi = 0;
	   TTStubs_r = 0;
	   TTStubs_roughPt = 0;
	   TTStubs_separation = 0;
	   TTStubs_simEta = 0;
	   TTStubs_simPhi = 0;
	   TTStubs_simPt = 0;
	   TTStubs_trigBend = 0;
	   TTStubs_trigDist = 0;
	   TTStubs_trigOffset = 0;
	   TTStubs_trigPos = 0;
	   TTStubs_x = 0;
	   TTStubs_y = 0;
	   TTStubs_z = 0;
	   trkParts_eta = 0;
	   trkParts_phi = 0;
	   trkParts_pt = 0;
	   trkParts_px = 0;
	   trkParts_py = 0;
	   trkParts_pz = 0;
	   trkParts_vx = 0;
	   trkParts_vy = 0;
	   trkParts_vz = 0;
	   TTStubs_hitXs = 0;
	   TTStubs_hitYs = 0;
	   TTStubs_hitZs = 0;
	   gen_puNtrks = 0;
	   genParts_charge = 0;
	   genParts_pdgId = 0;
	   genParts_status = 0;
	   simTracks_genpId = 0;
	   simTracks_pdgId = 0;
	   simTracks_vtxId = 0;
	   TTStubs_detWindow = 0;
	   TTStubs_pdgId = 0;
	   TTStubs_tpId = 0;
	   TTStubs_trkId = 0;
	   trkParts_charge = 0;
	   trkParts_genpId = 0;
	   trkParts_nhits = 0;
	   trkParts_ntkhits = 0;
	   trkParts_ntklayers = 0;
	   trkParts_pdgId = 0;
	   trkParts_trkId = 0;
	   TTStubs_hitChans = 0;
	   TTStubs_hitTrkIds = 0;
	   gen_randomSeeds = 0;
	   simTracks_evtId = 0;
	   simTracks_trkId = 0;
	   TTStubs_clusId0 = 0;
	   TTStubs_clusId1 = 0;
	   TTStubs_clusWidth = 0;
	   TTStubs_geoId0 = 0;
	   TTStubs_geoId1 = 0;
	   TTStubs_modId = 0;
	   TTStubs_nhits = 0;
	   TTStubs_stackId = 0;
	   trkParts_evtId = 0;
	   TTStubs_hitEvtIds = 0;
	   trkParts_trkIds = 0;
	   AMTTRoads_patternRef = 0;
	   AMTTRoads_tower = 0;
	   AMTTRoads_nstubs = 0;
	   AMTTRoads_superstripIds = 0;
	   AMTTRoads_stubRefs = 0;
	   // Set branch addresses and branch pointers
	   if (!tree) return;
	   fChain = tree;
	   fCurrent = -1;
	   fChain->SetMakeClass(1);

	   fChain->SetBranchAddress("TTStubs_barrel", &TTStubs_barrel, &b_TTStubs_barrel);
	   fChain->SetBranchAddress("TTStubs_isCombinatoric", &TTStubs_isCombinatoric, &b_TTStubs_isCombinatoric);
	   fChain->SetBranchAddress("TTStubs_isGenuine", &TTStubs_isGenuine, &b_TTStubs_isGenuine);
	   fChain->SetBranchAddress("TTStubs_isUnknown", &TTStubs_isUnknown, &b_TTStubs_isUnknown);
	   fChain->SetBranchAddress("TTStubs_psmodule", &TTStubs_psmodule, &b_TTStubs_psmodule);
	   fChain->SetBranchAddress("trkParts_intime", &trkParts_intime, &b_trkParts_intime);
	   fChain->SetBranchAddress("trkParts_primary", &trkParts_primary, &b_trkParts_primary);
	   fChain->SetBranchAddress("trkParts_signal", &trkParts_signal, &b_trkParts_signal);
	   fChain->SetBranchAddress("gen_trueNPV", &gen_trueNPV, &b_gen_trueNPV);
	   fChain->SetBranchAddress("gen_weightMC", &gen_weightMC, &b_gen_weightMC);
	   fChain->SetBranchAddress("gen_weightPDF", &gen_weightPDF, &b_gen_weightPDF);
	   fChain->SetBranchAddress("gen_weightPU", &gen_weightPU, &b_gen_weightPU);
	   fChain->SetBranchAddress("gen_puZpositions", &gen_puZpositions, &b_gen_puZpositions);
	   fChain->SetBranchAddress("genParts_E", &genParts_E, &b_genParts_E);
	   fChain->SetBranchAddress("genParts_M", &genParts_M, &b_genParts_M);
	   fChain->SetBranchAddress("genParts_eta", &genParts_eta, &b_genParts_eta);
	   fChain->SetBranchAddress("genParts_phi", &genParts_phi, &b_genParts_phi);
	   fChain->SetBranchAddress("genParts_pt", &genParts_pt, &b_genParts_pt);
	   fChain->SetBranchAddress("genParts_px", &genParts_px, &b_genParts_px);
	   fChain->SetBranchAddress("genParts_py", &genParts_py, &b_genParts_py);
	   fChain->SetBranchAddress("genParts_pz", &genParts_pz, &b_genParts_pz);
	   fChain->SetBranchAddress("genParts_vx", &genParts_vx, &b_genParts_vx);
	   fChain->SetBranchAddress("genParts_vy", &genParts_vy, &b_genParts_vy);
	   fChain->SetBranchAddress("genParts_vz", &genParts_vz, &b_genParts_vz);
	   fChain->SetBranchAddress("simTracks_charge", &simTracks_charge, &b_simTracks_charge);
	   fChain->SetBranchAddress("simTracks_eta", &simTracks_eta, &b_simTracks_eta);
	   fChain->SetBranchAddress("simTracks_phi", &simTracks_phi, &b_simTracks_phi);
	   fChain->SetBranchAddress("simTracks_pt", &simTracks_pt, &b_simTracks_pt);
	   fChain->SetBranchAddress("simTracks_px", &simTracks_px, &b_simTracks_px);
	   fChain->SetBranchAddress("simTracks_py", &simTracks_py, &b_simTracks_py);
	   fChain->SetBranchAddress("simTracks_pz", &simTracks_pz, &b_simTracks_pz);
	   fChain->SetBranchAddress("TTStubs_coordx", &TTStubs_coordx, &b_TTStubs_coordx);
	   fChain->SetBranchAddress("TTStubs_coordy", &TTStubs_coordy, &b_TTStubs_coordy);
	   fChain->SetBranchAddress("TTStubs_dirx", &TTStubs_dirx, &b_TTStubs_dirx);
	   fChain->SetBranchAddress("TTStubs_diry", &TTStubs_diry, &b_TTStubs_diry);
	   fChain->SetBranchAddress("TTStubs_dirz", &TTStubs_dirz, &b_TTStubs_dirz);
	   fChain->SetBranchAddress("TTStubs_eta", &TTStubs_eta, &b_TTStubs_eta);
	   fChain->SetBranchAddress("TTStubs_phi", &TTStubs_phi, &b_TTStubs_phi);
	   fChain->SetBranchAddress("TTStubs_r", &TTStubs_r, &b_TTStubs_r);
	   fChain->SetBranchAddress("TTStubs_roughPt", &TTStubs_roughPt, &b_TTStubs_roughPt);
	   fChain->SetBranchAddress("TTStubs_separation", &TTStubs_separation, &b_TTStubs_separation);
	   fChain->SetBranchAddress("TTStubs_simEta", &TTStubs_simEta, &b_TTStubs_simEta);
	   fChain->SetBranchAddress("TTStubs_simPhi", &TTStubs_simPhi, &b_TTStubs_simPhi);
	   fChain->SetBranchAddress("TTStubs_simPt", &TTStubs_simPt, &b_TTStubs_simPt);
	   fChain->SetBranchAddress("TTStubs_trigBend", &TTStubs_trigBend, &b_TTStubs_trigBend);
	   fChain->SetBranchAddress("TTStubs_trigDist", &TTStubs_trigDist, &b_TTStubs_trigDist);
	   fChain->SetBranchAddress("TTStubs_trigOffset", &TTStubs_trigOffset, &b_TTStubs_trigOffset);
	   fChain->SetBranchAddress("TTStubs_trigPos", &TTStubs_trigPos, &b_TTStubs_trigPos);
	   fChain->SetBranchAddress("TTStubs_x", &TTStubs_x, &b_TTStubs_x);
	   fChain->SetBranchAddress("TTStubs_y", &TTStubs_y, &b_TTStubs_y);
	   fChain->SetBranchAddress("TTStubs_z", &TTStubs_z, &b_TTStubs_z);
	   fChain->SetBranchAddress("trkParts_eta", &trkParts_eta, &b_trkParts_eta);
	   fChain->SetBranchAddress("trkParts_phi", &trkParts_phi, &b_trkParts_phi);
	   fChain->SetBranchAddress("trkParts_pt", &trkParts_pt, &b_trkParts_pt);
	   fChain->SetBranchAddress("trkParts_px", &trkParts_px, &b_trkParts_px);
	   fChain->SetBranchAddress("trkParts_py", &trkParts_py, &b_trkParts_py);
	   fChain->SetBranchAddress("trkParts_pz", &trkParts_pz, &b_trkParts_pz);
	   fChain->SetBranchAddress("trkParts_vx", &trkParts_vx, &b_trkParts_vx);
	   fChain->SetBranchAddress("trkParts_vy", &trkParts_vy, &b_trkParts_vy);
	   fChain->SetBranchAddress("trkParts_vz", &trkParts_vz, &b_trkParts_vz);
	   fChain->SetBranchAddress("TTStubs_hitXs", &TTStubs_hitXs, &b_TTStubs_hitXs);
	   fChain->SetBranchAddress("TTStubs_hitYs", &TTStubs_hitYs, &b_TTStubs_hitYs);
	   fChain->SetBranchAddress("TTStubs_hitZs", &TTStubs_hitZs, &b_TTStubs_hitZs);
	   fChain->SetBranchAddress("bx", &bx, &b_bx);
	   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
	   fChain->SetBranchAddress("gen_nPV", &gen_nPV, &b_gen_nPV);
	   fChain->SetBranchAddress("gen_puNtrks", &gen_puNtrks, &b_gen_puNtrks);
	   fChain->SetBranchAddress("genParts_charge", &genParts_charge, &b_genParts_charge);
	   fChain->SetBranchAddress("genParts_pdgId", &genParts_pdgId, &b_genParts_pdgId);
	   fChain->SetBranchAddress("genParts_status", &genParts_status, &b_genParts_status);
	   fChain->SetBranchAddress("simTracks_genpId", &simTracks_genpId, &b_simTracks_genpId);
	   fChain->SetBranchAddress("simTracks_pdgId", &simTracks_pdgId, &b_simTracks_pdgId);
	   fChain->SetBranchAddress("simTracks_vtxId", &simTracks_vtxId, &b_simTracks_vtxId);
	   fChain->SetBranchAddress("TTStubs_detWindow", &TTStubs_detWindow, &b_TTStubs_detWindow);
	   fChain->SetBranchAddress("TTStubs_pdgId", &TTStubs_pdgId, &b_TTStubs_pdgId);
	   fChain->SetBranchAddress("TTStubs_tpId", &TTStubs_tpId, &b_TTStubs_tpId);
	   fChain->SetBranchAddress("TTStubs_trkId", &TTStubs_trkId, &b_TTStubs_trkId);
	   fChain->SetBranchAddress("trkParts_charge", &trkParts_charge, &b_trkParts_charge);
	   fChain->SetBranchAddress("trkParts_genpId", &trkParts_genpId, &b_trkParts_genpId);
	   fChain->SetBranchAddress("trkParts_nhits", &trkParts_nhits, &b_trkParts_nhits);
	   fChain->SetBranchAddress("trkParts_ntkhits", &trkParts_ntkhits, &b_trkParts_ntkhits);
	   fChain->SetBranchAddress("trkParts_ntklayers", &trkParts_ntklayers, &b_trkParts_ntklayers);
	   fChain->SetBranchAddress("trkParts_pdgId", &trkParts_pdgId, &b_trkParts_pdgId);
	   fChain->SetBranchAddress("trkParts_trkId", &trkParts_trkId, &b_trkParts_trkId);
	   fChain->SetBranchAddress("TTStubs_hitChans", &TTStubs_hitChans, &b_TTStubs_hitChans);
	   fChain->SetBranchAddress("TTStubs_hitTrkIds", &TTStubs_hitTrkIds, &b_TTStubs_hitTrkIds);
	   fChain->SetBranchAddress("event", &event, &b_event);
	   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
	   fChain->SetBranchAddress("run", &run, &b_run);
	   fChain->SetBranchAddress("genParts_size", &genParts_size, &b_genParts_size);
	   fChain->SetBranchAddress("simTracks_size", &simTracks_size, &b_simTracks_size);
	   fChain->SetBranchAddress("TTStubs_size", &TTStubs_size, &b_TTStubs_size);
	   fChain->SetBranchAddress("trkParts_size", &trkParts_size, &b_trkParts_size);
	   fChain->SetBranchAddress("gen_randomSeeds", &gen_randomSeeds, &b_gen_randomSeeds);
	   fChain->SetBranchAddress("simTracks_evtId", &simTracks_evtId, &b_simTracks_evtId);
	   fChain->SetBranchAddress("simTracks_trkId", &simTracks_trkId, &b_simTracks_trkId);
	   fChain->SetBranchAddress("TTStubs_clusId0", &TTStubs_clusId0, &b_TTStubs_clusId0);
	   fChain->SetBranchAddress("TTStubs_clusId1", &TTStubs_clusId1, &b_TTStubs_clusId1);
	   fChain->SetBranchAddress("TTStubs_clusWidth", &TTStubs_clusWidth, &b_TTStubs_clusWidth);
	   fChain->SetBranchAddress("TTStubs_geoId0", &TTStubs_geoId0, &b_TTStubs_geoId0);
	   fChain->SetBranchAddress("TTStubs_geoId1", &TTStubs_geoId1, &b_TTStubs_geoId1);
	   fChain->SetBranchAddress("TTStubs_modId", &TTStubs_modId, &b_TTStubs_modId);
	   fChain->SetBranchAddress("TTStubs_nhits", &TTStubs_nhits, &b_TTStubs_nhits);
	   fChain->SetBranchAddress("TTStubs_stackId", &TTStubs_stackId, &b_TTStubs_stackId);
	   fChain->SetBranchAddress("trkParts_evtId", &trkParts_evtId, &b_trkParts_evtId);
	   fChain->SetBranchAddress("TTStubs_hitEvtIds", &TTStubs_hitEvtIds, &b_TTStubs_hitEvtIds);
	   fChain->SetBranchAddress("trkParts_trkIds", &trkParts_trkIds, &b_trkParts_trkIds);
	   fChain->SetBranchAddress("AMTTRoads_patternRef", &AMTTRoads_patternRef, &b_AMTTRoads_patternRef);
	   fChain->SetBranchAddress("AMTTRoads_tower", &AMTTRoads_tower, &b_AMTTRoads_tower);
	   fChain->SetBranchAddress("AMTTRoads_nstubs", &AMTTRoads_nstubs, &b_AMTTRoads_nstubs);
	   fChain->SetBranchAddress("AMTTRoads_superstripIds", &AMTTRoads_superstripIds, &b_AMTTRoads_superstripIds);
	   fChain->SetBranchAddress("AMTTRoads_stubRefs", &AMTTRoads_stubRefs, &b_AMTTRoads_stubRefs);
   Notify();
}

Bool_t roadsStubs::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void roadsStubs::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t roadsStubs::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef roadsStubs_cxx
