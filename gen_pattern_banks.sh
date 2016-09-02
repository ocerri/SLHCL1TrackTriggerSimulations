cd ..
amsim -B -i root://cmseos.fnal.gov//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt25_200M/stubs_tt25_34M.root \
-o PatternBanks/patternBank_tt25_$1_pt3_34M.root \
-v 2 -s $1 --minPt 3 \
&> log/patternBank_tt25_$1_pt3_34M.log

cd SLHCL1TrackTriggerSimulations
