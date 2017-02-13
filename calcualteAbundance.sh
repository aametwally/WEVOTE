#!/bin/bash

prefix=`basename $1`
dirPath=`readlink -f $1 | xargs dirname`

taxonomyDB=$2

./ABUNDANCE -i $dirPath/$prefix"_WEVOTE.csv" -p $dirPath/$prefix -d $taxonomyDB



if [ ! -d $dirPath/$prefix"_profiles" ]; then
    mkdir $dirPath/$prefix"_profiles"
fi

awk '{if($4=="phylum") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Phylum"
awk '{if($4=="class") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Class"
awk '{if($4=="order") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Order"
awk '{if($4=="family") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Family"
awk '{if($4=="genus") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Genus"
awk '{if($4=="species") print $0}' $dirPath/$prefix"_Abundance.txt" > $dirPath/$prefix"_profiles"/$prefix"_Species"
