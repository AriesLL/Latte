echo $1 #design size
echo $2 #II 
echo $3 #comment
sh script0.sh $1 $2
vivado_hls mm.tcl
sh script1.sh
sh script2.sh $1 $2.$3
vivado -mode batch -source power_current.tcl
