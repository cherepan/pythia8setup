#! /bin/bash
echo 'Starting Job'

export workdir=$PWD
echo $workdir
cd /home/vladimircherepanov/pythia/pythia8setup; source  install_env_Apr_27_2019;
cd $workdir
export TMPDIR ="/tmp"

mkdir $TMPDIR/runDir
cp -r * $TMPDIR/runDir
cd  $TMPDIR/runDir
$workdir/softQcd.exe run_card.cmnd CondorTest

cp -r * $workdir
rm -r   $TMPDIR/runDir
echo 'Completed Job'
cd $workDir


