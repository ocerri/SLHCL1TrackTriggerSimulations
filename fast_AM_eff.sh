cd ..
#first argument strip configuration eg: sf1_nz4_L5x2
SSConfig=$1
evtype="SingleMuonTest"
# evtype=$2
TT_number=25 ## tail -c 22 per TT26 che non ha una bank size full
EventType="${evtype}_"
NPU="0"
PU="PU${NPU}"
NTotEv=100000

echo "Input Parameters:"
echo "SSConfig: $SSConfig - EventType: $EventType - PU: $NPU - NTotEv: $NTotEv"

SampleSize=200

NBankPattern=`tail --lines=4 log/patternBank_tt${TT_number}_${SSConfig}_pt3_${SampleSize}M.log | grep 95 | tail -c 23 | cut -c -7`
echo "Patterns bank found - max patterns: $NBankPattern"

# pattern_file="root://cmseos:1094//store/user/l1upgrades/PatternBanks/Eta6_Phi8_vz15/patternBank_tt25_${SSConfig}_pt3_${SampleSize}M.root"
pattern_file="PatternBanks/patternBank_tt${TT_number}_${SSConfig}_pt3_${SampleSize}M.root"

inDir="/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/tt25_test"
# inDir="root://cmseos:1094//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/tt25_test"

roads_file="$inDir/roads_tt${TT_number}_$EventType${PU}_${1}.root"
track_file="$inDir/tracks_tt${TT_number}_$EventType${PU}_${1}.root"
echo $track_file

echo "Finding roads..."
echo `date`
echo
amsim -R -i SLHCL1TrackTriggerSimulations/Configuration/input/input_SingleMuonTest_tt25_large.txt \
-o $roads_file \
-b $pattern_file \
-n $NTotEv --maxMisses 1 -v 2 -s $SSConfig \
--maxPatterns $NBankPattern --tower $TT_number &> log/roads_tt${TT_number}_$EventType${PU}_${1}.log

echo "Fitting tracks..."
echo `date`
echo
amsim -T -i $roads_file \
-o $track_file \
-v 2 -f LTF &> log/tracks_tt${TT_number}_$EventType${PU}_${1}.log

echo "Running efficiency analysis:"

cd SLHCL1TrackTriggerSimulations/AMSimulation/res1

drawer_log_file=../../../log/drawer_$EventType${PU}_${SSConfig}

# echo "Blinding PT"
# echo `date`
# echo
# python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
# --inDir $inDir --blind_variable pt --tower $TT_number &> ${drawer_log_file}_pt.log

echo "Blinding eta*"
echo `date`
echo
python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir --blind_variable eta_star --tower $TT_number &> ${drawer_log_file}_eta_star.log

# echo "Blinding phi*"
# echo `date`
# echo
# python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
# --inDir $inDir --blind_variable phi_star --tower $TT_number &> ${drawer_log_file}_phi_star.log

# echo "Blinding vz"
# echo `date`
# echo
# python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
# --inDir $inDir --blind_variable vz --tower $TT_number &> ${drawer_log_file}_vz.log

cd ../..

echo "DONE"
