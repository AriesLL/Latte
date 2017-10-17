pwrPath=/curr/peipei/xilinx
fileName=$1

totalPower=`head -n 200 ${pwrPath}/$fileName | grep "| mm " | cut -d '|' -f 3`
signalPower=`head -n 200 ${pwrPath}/$fileName | grep "| mm " | cut -d '|' -f 9`
logicPower=`head -n 200 ${pwrPath}/$fileName | grep "| mm " | cut -d '|' -f 11`
bramPower=`head -n 200 ${pwrPath}/$fileName | grep "| mm " | cut -d '|' -f 13`
dspPower=`head -n 200 ${pwrPath}/$fileName | grep "| mm " | cut -d '|' -f 15`

echo $totalPower $signalPower $logicPower $bramPower $dspPower
