cd ..
#first argument strip configuration eg: sf1_nz4_L5x2
SSConfig=$1
evtype="SingleMuonTest"
# evtype=$2
EventType="${evtype}_"
NPU="0"
PU="PU${NPU}"
NTotEv=200000

echo "Input Parameters:"
echo "SSConfig: $SSConfig - EventType: $EventType - PU: $NPU - NTotEv: $NTotEv"

SampleSize=68

NBankPattern=`tail --lines=4 log/patternBank_tt25_${SSConfig}_pt3_${SampleSize}M.log | grep 95 | tail -c 23 | cut -c -7`
echo "Patterns bank found - max patterns: $NBankPattern"

# pattern_file="root://cmseos:1094//store/user/l1upgrades/PatternBanks/Eta6_Phi8_vz15/patternBank_tt25_${SSConfig}_pt3_${SampleSize}M.root"
pattern_file="PatternBanks/patternBank_tt25_${SSConfig}_pt3_${SampleSize}M.root"

inDir="/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/tt25_test"
# inDir="root://cmseos:1094//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/tt25_test"

roads_file="$inDir/roads_$EventType${PU}_${1}_6oo6.root"
track_file="$inDir/tracks_$EventType${PU}_${1}_6oo6.root"
echo $track_file

# echo "Finding roads..."
# echo `date`
# echo
# amsim -R -i SLHCL1TrackTriggerSimulations/Configuration/input/input_SingleMuonTest_tt25_large.txt \
# -o $roads_file \
# -b $pattern_file \
# -n $NTotEv -v 2 -s $SSConfig --maxPatterns $NBankPattern &> log/roads_$EventType${PU}_${1}_6oo6.log
#
# echo "Fitting tracks..."
# echo `date`
# echo
# amsim -T -i $roads_file \
# -o $track_file \
# -v 2 -f LTF &> log/tracks_$EventType${PU}_${1}_6oo6.log

echo "Running efficiency analysis:"

cd SLHCL1TrackTriggerSimulations/AMSimulation/res1

drawer_log_file=../../../log/drawer_$EventType${PU}_${SSConfig}

echo "Blinding PT"
echo `date`
echo
python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir --blind_variable pt --m6oo6 1 &> ${drawer_log_file}_6oo6_pt.log

echo "Blinding eta*"
echo `date`
echo
python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir --blind_variable eta_star --m6oo6 1 &> ${drawer_log_file}_6oo6_eta_star.log

echo "Blinding phi*"
echo `date`
echo
python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir --blind_variable phi_star --m6oo6 1 &> ${drawer_log_file}_6oo6_phi_star.log

echo "Blinding vz"
echo `date`
echo
python drawer_FOM_driver.py --task 2 --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir --blind_variable vz --m6oo6 1 &> ${drawer_log_file}_6oo6_vz.log

cd ../..

echo "DONE"
