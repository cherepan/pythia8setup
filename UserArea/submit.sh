#!/bin/bash
# short.sh: a short discovery job
printf "Start time: "; /bin/date
printf "Job is running on node: "; /bin/hostname
printf "Job running as user: "; /usr/bin/id
printf "Job is running in directory: "; /bin/pwd
echo
printf $1,$2,$3,'\n'
echo "\n Working hard..."
./softQcd.exe $1 $2
echo "Science complete!"
