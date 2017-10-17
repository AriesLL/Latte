#change the power_current.tcl file
echo $1
echo $2
CUR_PATH=$(pwd)
echo $CUR_PATH
TRF_PATH=${CUR_PATH//\//\\\/}
echo $TRF_PATH
my_REG="s/testFolder/$TRF_PATH/g" 
echo $my_REG
#
sed -- $my_REG  power_template.tcl > power_1.tcl

sed -- "s/testFlow/$1\_$2/g" power_1.tcl > power_current.tcl

