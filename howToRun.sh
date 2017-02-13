./ABUNDANCE_CONTIGS -i test_ID_tax_count.csv -p test_ID_tax_count -d WEVOTE_DB/





cut -f1,4 MKAN003341_S1_45_blastOutput_Filtered.blast | awk -F '[_\t]' '{print $2 "\t" $NF}' | sort -k1,1 > contigID_tax.txt

join contigID_tax.txt ReadsPerContigSorted.txt | sed "s/ /\t/g" > contigID_Tax_ReadCount.txt

cut -f1 MKAN003341_S1_45_blastOutput_Filtered.blast | awk -F '[_\t]' '{print $2 "\t" $0}' | sort -k1,1 > contigID_contigheader.txt

join contigID_contigheader.txt contigID_Tax_ReadCount.txt | cut -f2,3,4 -d " " | sed "s/ /,/g" > contigheader_Tax_readCount.csv
./ABUNDANCE_CONTIGS -i contigheader_Tax_readCount.csv -p FINAL_TAxonomy -d WEVOTE_DB/
