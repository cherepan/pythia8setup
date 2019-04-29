num=50
echo $num
for i in $(seq 1 $num);
do
echo "------------ Set_$i --------------"
nohup ./softQcd.exe run_card.cmnd  Res_$i &> nohup_log_$i.txt &
#tail nohup.txt 
echo "----------------------------------"

done;