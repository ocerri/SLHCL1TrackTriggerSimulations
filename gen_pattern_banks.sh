cd ..

SampleSize=276
# inDir="root://cmseos.fnal.gov//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt25_200M"
# inDir="SingleMuonStubsSample"
inDir="/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/Configuration/input"


# amsim -B -i $inDir/stubs_tt$2_${SampleSize}M.root \
amsim -B -i $inDir/input_stubs_tt$2_${SampleSize}M.txt \
-o PatternBanks/patternBank_tt$2_$1_pt3_200M.root \
-v 2 -s $1 --minPt 3 --tower $2 --speedup 1 \
&> log/patternBank_tt$2_$1_pt3_${SampleSize}M.log
# -o PatternBanks/patternBank_tt$2_$1_pt3_${SampleSize}M.root \
#ATTENZIONE cambiare anche PatternGenerator.cc looking for ATTENZIONE
#Poi ricompilare

cd SLHCL1TrackTriggerSimulations
