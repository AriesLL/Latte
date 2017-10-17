#change the simulation file
sim='/solution1/sim/'
sim_V="${sim}verilog"
echo $sim_V
CUR_PATH=$(pwd)$sim
echo $CUR_PATH
TRF_PATH=${CUR_PATH//\//\\\/}
my_REG="s/\.\.\//$TRF_PATH/g" 
echo $my_REG
sed -i -- $my_REG solution1/sim/verilog/*.v 
