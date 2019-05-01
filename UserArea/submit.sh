#!/bin/bash
# short.sh: a short discovery job
printf "Start time: "; /bin/date
printf "Job is running on node: "; /bin/hostname
printf "Job running as user: "; /usr/bin/id
printf "Job is running in directory: "; /bin/pwd
echo
printf $1,$2,$3,'\n'
echo "\n Working hard..."
ls 
pwd
/home/vladimircherepanov/pythia/pythia8setup/install/tauola++/1.1.5/pythia8/176/UserArea/softQcd.exe run_card.cmnd output
echo "Science complete!"
