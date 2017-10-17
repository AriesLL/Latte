designSize=$1
endII=$2
comment=$3
if [ $# -lt 4 ]
then
	echo "./run.sh designSize endII coment"
	exit
fi

if [ -d "${designSize}_${comment}" ]
then
	:
else
	mkdir ${designSize}_${comment};
fi

if [ -f "mm_${designSize}_${comment}.result" ]
then
	:
else
echo "designSize II total signal logic bram dsp" >> mm_${designSize}_${comment}.result
fi

if [ -f "mm_kernel_${designSize}_${comment}.result" ]
then
	:
else
echo "designSize II total signal logic bram dsp" >> mm_kernel_${designSize}_${comment}.result
fi

i=$designSize

while [ $i  -ge "$endII" ]
do
	echo $i;
	cp -rf testBroadcast_cpy_gold ${designSize}_${comment}/testBroadcast_${designSize}_${i}_${comment}; 
	cd ${designSize}_${comment}/testBroadcast_${designSize}_${i}_${comment};
	#ls run.sh -lrht;
	sh run.sh $designSize $i $comment;
	cd ../..;
	echo $designSize $i `./parsePWRmm.sh ${designSize}_${i}.${comment}_impl.pwr` >> mm_${designSize}_${comment}.result
	echo $designSize $i `./parsePWRkernel.sh ${designSize}_${i}.${comment}_impl.pwr` >> mm_kernel_${designSize}_${comment}.result
	i=$((i/2))
done
