#!/bin/bash

#PBS -l nodes=1:ppn=16
#PBS -l walltime=30:00
#PBS -d ./
#PBS -m abe
#PBS -M ametwa2@uic.edu
#PBS -N test_wevote_extreme_4



./wevote_pipeline.sh -i test_contigs.fa -o test_contigs_output_extreme_3tools_5 --db WEVOTE_DB/ --kraken --blastn --clark --metaphlan --threads 16



#WEVOTE:
#./wevote_pipeline.sh -i test_contigs.fa -o test_contigs_output_extreme_3tools_4 --db WEVOTE_DB/ --kraken --blastn --clark --threads 16
#./WEVOTE -i $dirPath/$prefix/$prefix"_ensemble.csv" -d $taxonomyDB -p $dirPath/$prefix/$prefix -n $threads -k $penalty -a $minNumAgreed -s $minScore
#
#
#ABUNDANCE:
#./calcAbundance.sh -i test_contigs_output_extreme_3tools_4/test_contigs_output_extreme_3tools_4_WEVOTE_Details.txt -p test_contigs_output_extreme_3tools_4/Ahmed_Contig --db WEVOTE_DB/ --seqcount ReadsPerContigSorted.txt
#./ABUNDANCE -i $prefix"_WEVOTE_taxID_count.csv" -p $prefix -d $taxonomyDB
