cd ..

SampleSize=68
# inDir="root://cmseos.fnal.gov//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt25_200M"
inDir="SingleMuonStubsSample"


amsim -B -i $inDir/stubs_tt25_${SampleSize}M.root \
-o PatternBanks/patternBank_tt25_$1_pt3_${SampleSize}M.root \
-v 2 -s $1 --minPt 3 \
&> log/patternBank_tt25_$1_pt3_${SampleSize}M.log

cd SLHCL1TrackTriggerSimulations
