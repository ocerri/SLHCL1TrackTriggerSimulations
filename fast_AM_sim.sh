cd ..
#first argument strip configuration eg: sf1_nz4_L5x2
SSConfig=$1
evtype="TTbar"
# evtype=$2
EventType="${evtype}_"
NPU="140"
PU="PU140"
NTotEv=1000

NBankPattern=`tail --lines=4 log/patternBank_tt25_${SSConfig}_pt3_34M.log | grep 95 | tail -c 23 | cut -c -7`
echo "Patterns bank found - max patterns: $NBankPattern"

# pattern_file="root://cmseos:1094//store/user/l1upgrades/PatternBanks/Eta6_Phi8_vz15/patternBank_tt25_${SSConfig}_pt3_34M.root"
pattern_file="PatternBanks/patternBank_tt25_${SSConfig}_pt3_34M.root"
roads_file="root://cmseos:1094//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/tt25_test/roads_$EventType${PU}_${1}.root"
track_file="root://cmseos:1094//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/tt25_test/tracks_$EventType${PU}_${1}.root"
echo $track_file

echo "Finding roads..."
amsim -R -i SLHCL1TrackTriggerSimulations/Configuration/input/input_$EventType$PU.txt \
-o $roads_file \
-b $pattern_file \
-n $NTotEv --maxMisses 1 -v 2 -s $SSConfig --maxPatterns $NBankPattern &> log/roads_$EventType${PU}_${1}.log

echo "Fitting tracks..."
amsim -T -i $roads_file \
-o $track_file \
-v 2 -f LTF &> log/tracks_$EventType${PU}_${1}.log

echo "Running roads analysis..."
drawer_log_file=../../../log/drawer_$EventType${PU}_${SSConfig}.log
cd SLHCL1TrackTriggerSimulations/AMSimulation/res1
python drawer_FOM_driver.py --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
&> ../../../log/drawer_$EventType${PU}_${SSConfig}.log

NRoads95pc=`more $drawer_log_file | grep SpecialOut | grep 95 | tail -c 5`
NRoadsMean=`tail --lines=2 $drawer_log_file | grep Mean | tail -c -16`
echo -e "$evtype\t$NPU\t$SSConfig\t$NBankPattern\t$NRoadsMean\t$NRoads95pc" >> SLHCL1TrackTriggerSimulations/AM_Sim_fast_stat.txt

cd ../..
