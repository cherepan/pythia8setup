#! /bin/bash
echo 'Starting Job'

export workdir=$PWD

echo $1

#source  install_env_Apr_27_2019;
softQcd.exe run_card.cmnd CondorTest2
echo 'Completed Job'



