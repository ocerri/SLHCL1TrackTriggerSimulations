cd ..
#first argument strip configuration eg: sf1_nz4_L5x2
SSConfig=$1
evtype="TTbar"
# evtype=$2
EventType="${evtype}_"
NPU="140"
PU="PU140"
NTotEv=2000

echo "Input Parameters:"
echo "SSConfig: $SSConfig - EventType: $EventType - PU: $NPU - NTotEv: $NTotEv"

# inDir="root://cmseos:1094//store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/tt25_test"
inDir="/uscms/home/ocerri/nobackup/CMSTrigger/CMSSW_6_2_0_SLHC25_patch3/src/tt25_test"

SampleSize=68

NBankPattern=`tail --lines=4 log/patternBank_tt25_${SSConfig}_pt3_${SampleSize}M.log | grep 95 | tail -c 23 | cut -c -7`
echo "Patterns bank found - max patterns: $NBankPattern"


echo "Reading roads analysis..."

cd SLHCL1TrackTriggerSimulations/AMSimulation/res1

drawer_log_file=../../../log/drawer_$EventType${PU}_${SSConfig}.log

python drawer_FOM_driver.py --npatterns $NBankPattern --ss $SSConfig --pu $NPU --EventType $EventType \
--inDir $inDir &> $drawer_log_file

NRoads95pc=`more $drawer_log_file | grep SpecialOut | grep 95 | tail -c 6`
NRoadsMean=`tail --lines=2 $drawer_log_file | grep Mean | tail -c -16`

cd ../..

echo -e "$evtype\t$NPU\t$SSConfig\t$NBankPattern\t$NRoadsMean\t$NRoads95pc"
echo -e "$evtype\t$NPU\t$SSConfig\t$NBankPattern\t$NRoadsMean\t$NRoads95pc" >> AM_Sim_fast_stat.txt
