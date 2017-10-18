# automation to copy input folder, 
# generate output, and result folder
repo=$1
designSize=$2
endII=$3
comment=$repo

# repo for the output project folder
repoOut="../EnergyOut/${repo}_out"

# repo for the output result folder
resultOut="../ResultOut"

# check input parameters
if [ $# -lt 3 ]
then
	echo "./run.sh repo designSize endII"
	exit
fi

# check repos
if [ ! -d "$resultOut" ]
then
	mkdir -p $resultOut;
fi

if [ ! -d "${repoOut}/${designSize}_${comment}" ]
then
	mkdir -p ${repoOut}/${designSize};
fi

# check results
if [ ! -f "${resultOut}/mm_${designSize}_${comment}.result" ]
then
echo "designSize II total signal logic bram dsp" >> ${resultOut}/${repo}_${designSize}.result
fi

if [ ! -f "mm_kernel_${designSize}_${comment}.result" ]
then
echo "designSize II total signal logic bram dsp" >> ${resultOut}/${repo}_kernel_${designSize}.result
fi

i=$designSize



while [ $i  -ge "$endII" ]
do
	echo $i;
	cp -rf $repo ${repoOut}/${designSize}/${repo}_${designSize}_${i}; 
	cd ${repoOut}/${designSize}/${repo}_${designSize}_${i};
	#ls run.sh -lrht;
	sh run.sh $designSize $i $comment;
	cd ../../..;
	echo $designSize $i `./parsePWRmm.sh ${designSize}_${i}.${comment}_impl.pwr` >> ${resultOut}/${repo}_${designSize}.result
	echo $designSize $i `./parsePWRkernel.sh ${designSize}_${i}.${comment}_impl.pwr` >> ${resultOut}/${repo}_kernel_${designSize}.result
	i=$((i/2))
done
