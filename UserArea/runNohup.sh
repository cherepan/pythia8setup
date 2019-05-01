num=100
echo $num
for i in $(seq 1 $num);
do
echo "------------ Set_$i --------------"
nohup ./softQcd.exe run_card.cmnd  Result_$i &> nohup_log_$i.txt &
#tail nohup.txt 
echo "----------------------------------"

done;