#!/bin/bash

#PBS -l nodes=1:ppn=16
#PBS -l walltime=1:00:00:00
#PBS -d ./
#PBS -m abe
#PBS -M ametwa2@uic.edu
#PBS -N test_wevote_bigContig_local



../WEVOTE_optimize/run_WEVOTE_PIPELINE.sh -i test_contigs.fa -o TEST --db WEVOTE_DBwww/ --blastn --metaphlan --threads 16


../WEVOTE_optimize/run_ABUNDANCE.sh -i Test_Utilities_Allergy_WEVOTE_Details.txt -p ABU/Hadoooo --db WEVOTE_DB/ --seqcount ReadsPerContigSorted.txt




#WEVOTE:
#./WEVOTE -i $dirPath/$prefix/$prefix"_ensemble.csv" -d $taxonomyDB -p $dirPath/$prefix/$prefix -n $threads -k $penalty -a $minNumAgreed -s $minScore
#
#
#ABUNDANCE:
#./ABUNDANCE -i $prefix"_WEVOTE_taxID_count.csv" -p $prefix -d $taxonomyDB
