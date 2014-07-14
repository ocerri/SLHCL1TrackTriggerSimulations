#ifndef AMSimulation_PatternGenerator_h_
#define AMSimulation_PatternGenerator_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperMath.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// SETTINGS: Superstrip size, # of fake superstrips, # of DC bits
// INPUT   : TTree with moduleId, hitId, sim info + trigger tower file
// OUTPUT  : Pattern bank

class PatternGenerator {
  public:
    // Constructor
    PatternGenerator(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits), nFakeSuperstrips_(po.nFakeSuperstrips),
      nEvents_(999999999), minFrequency_(1), verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(0 <= nDCBits_ && nDCBits_ <= 4);
        assert(0 <= nFakeSuperstrips_ && nFakeSuperstrips_ <= 3);

        chain_ = new TChain("ntupler/tree");

        layerMap_ = getLayerMergingMap(nLayers_);

        // Decide on the size of superstrip
        if (!po.subLadderVarSize.empty() && !po.subModuleVarSize.empty())
            superstripArbiter_ = new SuperstripArbiter(6, po.subLadderVarSize, po.subModuleVarSize);
        else
            superstripArbiter_ = new SuperstripArbiter(6, po.subLadderSize, po.subModuleSize);
    }

    // Destructor
    ~PatternGenerator() {
        delete superstripArbiter_;
    }


    // Setters
    //void setNLayers(int n)          { nLayers_ = n; }
    //void setNDCBits(int n)          { nDCBits_ = n; }
    //void setNFakeSuperstrips(int n) { nFakeSuperstrips_ = n; }

    void setNEvents(int n)          { if (n != -1)  nEvents_ = std::max(0, n); }
    void setMinFrequency(int n)     { minFrequency_ = std::max(1, minFrequency_); }
    void setVerbosity(int n)        { verbose_ = n; }

    // Getters
    int getNLayers()          const { return nLayers_; }
    int getNDCBits()          const { return nDCBits_; }
    int getNFakeSuperstrips() const { return nFakeSuperstrips_; }

    // Functions
    int readTriggerTowerFile(TString src);

    int readFile(TString src);

    int makePatterns();

    int writePatterns(TString out);

    // Main driver
    int run(TString out, TString src, TString layout);

  private:
    void addFakeSuperstrips(const std::vector<TTSuperstrip>& goodSuperstrips,
                            std::vector<TTSuperstrip>& goodAndFakeSuperstrips);

    void uniquifyPatterns();

    void uniquifyPatternsUsingStdMap();  // simpler, but need more memory (also slower)

    void erasePatternsNotWithinTriggerTowers();

    bool isNotWithinTriggerTower(const TTPattern& patt);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const int nLayers_;
    const int nDCBits_;
    const int nFakeSuperstrips_;

    // Program options
    int nEvents_;
    int minFrequency_;  // min frequency of a pattern to be valid
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<TTPattern> allPatterns_;

    std::map<unsigned, unsigned> layerMap_;  // defines layer merging

    std::map<unsigned, std::vector<unsigned> > triggerTowerMap_;  // key: towerId, value: moduleIds in the tower
    std::map<unsigned, std::vector<unsigned> > triggerTowerReverseMap_;  // key: moduleId, value: towerIds containing the module

    SuperstripArbiter* superstripArbiter_;
};

#endif
