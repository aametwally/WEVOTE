#!/bin/bash



wd=
prefix=
contigID_count_file=$sample"/assembly/"$dir"/ReadsPerContigSorted.txt" 
WEVOTE_output= 


awk '{ print $1 "\t" $NF}' $WEVOTE_output | awk -F '[_\t]' '{print $2 "\t" $NF}' | sort -k1,1 > $wd/$prefix"_WEVOTE_contigID_tax.txt"

join $wd/$prefix"_WEVOTE_contigID_tax.txt"  $contigID_taxa_count_file | cut -f2,3 | sed "s/ /,/g" > $wd/$prefix"_WEVOTE_contigID_Tax_ReadCount.csv"

./ABUNDANCE -i $wd/$prefix"_WEVOTE_contigID_Tax_ReadCount.csv" -p $wd/$prefix -d WEVOTE_DB/









