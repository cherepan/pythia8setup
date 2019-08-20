#! /bin/bash
echo 'Starting Job' 
export workdir="/afs/cern.ch/work/c/cherepan/pythia/pythia8setup/install/tauola++/1.1.5/pythia8/176/UserArea/"
#export X509_USER_PROXY="/afs/cern.ch/work/c/cherepan/T3M/Setup/Tools/ControlScripts/proxy/x509up_u54841"
#export X509_USER_PROXY=/afs/cern.ch/work/c/cherepan/T3M/Setup/Tools/ControlScripts/proxy/x509up_u54841
export X509_USER_PROXY="/afs/cern.ch/user/c/cherepan/x509up_u54841"
export X509_USER_PROXY=/afs/cern.ch/user/c/cherepan/x509up_u54841
cd /afs/cern.ch/work/c/cherepan/pythia/pythia8setup/; source install_env_May_31_2019;
cd /afs/cern.ch/work/c/cherepan/pythia/pythia8setup/install/tauola++/1.1.5/pythia8/176/UserArea
mkdir $TMPDIR/workdirCondor
cp -r * $TMPDIR/workdirCondor
cd  $TMPDIR/workdirCondor
#echo $1
/afs/cern.ch/work/c/cherepan/pythia/pythia8setup/install/tauola++/1.1.5/pythia8/176/UserArea/minbias.exe run_card.cmnd MinBias_03_06_Run_4_$1
cp -r *  /afs/cern.ch/work/c/cherepan/pythia/pythia8setup/install/tauola++/1.1.5/pythia8/176/UserArea
echo 'Completed Job' 

