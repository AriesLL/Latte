#change the mm.cpp file
echo $1
echo $2
actualSize=$(($1))
echo "input/output size is $actualSize and design size is $1, II is $2"
factor=$(expr $1 / $2)
echo "PE number is $factor"

sed -- "s/myFactor/$factor/g" mm_template.cpp > mm1.cpp
sed -- "s/designSize/$1/g" mm1.cpp > mm.cpp


sed -- "s/actualSize/$actualSize/g" mm_template.h > mm.h

