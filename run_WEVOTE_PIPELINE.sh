#!/bin/bash


###******************************************************************###
###********                    WEVOTE                        ********###
###********     WEighted VOting Taxonomic IdEntification     ********###
###********                                                  ********###
###********     Copyright by Ahmed A. Metwally (C) 2016      ********###
###********        Finn-Perkins' Lab & Dai's Lab, UIC        ********###
###********                                                  ********###
###********  This code is released under GNU GPLv3 License   ********###
###********                                                  ********###
###********       Please report bugs & suggestions to:       ********###
###********                 <ametwa2@uic.edu>                ********###
###******************************************************************###

######## parse each tool parameters from wevote.cfg
eval $(grep "^blastDB=" wevote.cfg)
eval $(grep "^krakenDB=" wevote.cfg)
eval $(grep "^clarkDB=" wevote.cfg)
eval $(grep "^taxonomyDB=" wevote.cfg)
eval $(grep "^clarkPath=" wevote.cfg)
eval $(grep "^krakenPath=" wevote.cfg)
eval $(grep "^blastnPath=" wevote.cfg)
eval $(grep "^metaphlanPath=" wevote.cfg)
eval $(grep "^tippPath=" wevote.cfg)




## parse commandline arguments
Usage="\nUsage: $0 -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options>

Implemented options:
-h|--help                  \t help flag
-i|--input <input-file>    \t input query
-o|--output <output-path>  \t Output prefix
--db <taxonomy_db>         \t taxonomy database path
--threads <num-threads>    \t Number of threads
-s <score>                 \t Score threshold
-a <num-of-agreed>         \t Minimum number of tools agreed tools on WEVOTE decision
-k <penalty>               \t Score penalty for disagreement
--kraken                   \t Run Kraken
--blastn                   \t Run BLASTN
--tipp                     \t Run TIPP
--clark                    \t Run CLARK
--metaphlan                \t Run MetaPhlAn
-c|--classfy               \t Start the pipeline from the classification step. i.e., skip running individual tools
"

my_dir=`dirname $0`
query=""
outputPrefix=""
blastnFlag=0
krakenFlag=0
tippFlag=0
clarkFlag=0
metaphlanFlag=0
minScore=0
minNumAgreed=0
eval=0
threads=1
penalty=2
c=0


if [ $# -lt 2 ]; then
echo -e "$Usage"
exit
fi


while [[ $# > 0 ]]
do
key="$1"

case $key in

    -h|--help)
		echo -e "$Usage"
		;;
    -i|--input)
		query=$2
		queryPath=`readlink -f $query | xargs dirname`
		queryName=`basename $query`
		query=$queryPath/$queryName	
		prefix=`basename ${query%.*}`
		dirPath=`pwd`
		echo "Query= "$query
		shift
		;;    
    -o|--output)
		outputPrefix=$2
		echo "Output prefix = "$outputPrefix
		shift
		;;
    --db)
		taxonomyDB=$2
		echo "Taxonomy Database located at = "$taxonomyDB
		shift
		;;
    --threads)
		threads=$2
		shift
		echo "Num of threads = "$threads 
		;;     
    -s)
		minScore=$2
		echo "WEVOTE score threshold = "$minScore
		shift
		;;   
    -a)
		minNumAgreed=$2
		echo "WEVOTE min num of agreed tools threshold = "$minNumAgreed
		shift
		;; 
    -k)
		penalty=$2
		echo "Penalty = "$penalty 
		shift
		;;
    --kraken)
		krakenFlag=1
		;;
    --blastn)
		blastnFlag=1
		;;
    --tipp)
		tippFlag=1
		;;
    --clark)
		clarkFlag=1
		;;
    --metaphlan)
		metaphlanFlag=1
		;;                                                			
    -c|--classfy)
		c=1
		;;
    *)
		echo "Sorry " $1 " is an invalid option was received."
		echo -e "$Usage"
		exit
		;;
esac
shift
done



if [ $outputPrefix!="" ]; then

	prefix=`basename ${outputPrefix%.*}`
	dirPath=`readlink -f $outputPrefix | xargs dirname`
fi


if [ $c != 1 ]; then

	if [ ! -d $dirPath/$prefix ]; then
		mkdir $dirPath/$prefix
	fi


	######## Run BLASTN	    
	if [ $blastnFlag -eq 1 ]; then 
	echo "BLASTN in progress ........"
	START=$(date +%s)
	$blastnPath/blastn -db $blastDB -query $query -out $dirPath/$prefix/$prefix"_OutputBLASTN" -outfmt "6 qseqid sseqid sgi staxids length qstart qend sstart send pident evalue score bitscore stitle" -num_threads $threads -perc_identity 90 -max_target_seqs 1 -evalue 1e-5 -best_hit_score_edge 0.05 -best_hit_overhang 0.25
	END=$(date +%s)
	DIFF=`expr $(($END - $START)) / 60`
	echo -e `date` "\tTax Identification for "$prefix" using BLASTN executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"
	cut -f1,4 $dirPath/$prefix/$prefix"_OutputBLASTN" | sort -k1,1 -u > $dirPath/$prefix/$prefix"_read_tax_blastn"	
	fi



	######## Run Kraken
	if [ $krakenFlag -eq 1 ]; then
	echo "Kraken in progress ........"
	START=$(date +%s)
	$krakenPath/kraken --db $krakenDB --threads $threads --fasta-input --output $dirPath/$prefix/$prefix"_OutputKraken" $query
	END=$(date +%s)
	DIFF=`expr $(($END - $START)) / 60`
	echo -e `date` "\tTax Identification for "$prefix" using Kraken executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"
	cut -f2,3 $dirPath/$prefix/$prefix"_OutputKraken"  > $dirPath/$prefix/$prefix"_read_tax_kraken"
	fi



	####### Run TIPP
	if [ $tippFlag -eq 1 ]; then
	echo "TIPP in progress ........"
	START=$(date +%s)
	$tippPath"run_abundance.py" -f $query -c ~/.sepp/tipp.config -d $dirPath/$prefix/$prefix"_out" -x $threads > $dirPath/$prefix/$prefix"_OutputTIPP"
	END=$(date +%s)
	DIFF=`expr $(($END - $START)) / 60`
	echo -e `date` "\tTax Identification for "$prefix" using TIPP executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"
	sed '1d' $dirPath/$prefix/$prefix"_out"/markers/all.classification > $dirPath/$prefix/temp
	awk '{if ($2!="NA") print $1 "\t" $2; else if ($3!="NA") print $1 "\t" $3; else if ($4!="NA") print $1 "\t" $4; else if ($5!="NA") print $1 "\t" $5; else if ($6!="NA") print $1 "\t" $6; else if ($7!="NA") print $1 "\t" $7}' $dirPath/$prefix/temp > $dirPath/$prefix/$prefix"_read_tax_tipp"
	rm $dirPath/$prefix/temp
	fi



	################# Run CLARK	
	if [ $clarkFlag -eq 1 ]; then
	echo "CLARK in progress ........"
	START=$(date +%s)
	cd $clarkPath
	./set_targets.sh $clarkDB bacteria viruses
	./classify_metagenome.sh -O $query -R $dirPath/$prefix/$prefix"_OutputCLARK" -n $threads
	cd $dirPath
	END=$(date +%s)
	DIFF=`expr $(($END - $START)) / 60`
	
	echo -e `date` "\tTax Identification for "$prefix" using CLARK executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"
	sed 1d $dirPath/$prefix/$prefix"_OutputCLARK.csv" | sed "s/,/\t/g" | cut -f 1,3 | awk '{if ($2!="NA") print $1 "\t" $2}' > $dirPath/$prefix/$prefix"_read_tax_clark"
	fi



	################### Run MetaPlAn
	if [ $metaphlanFlag -eq 1 ]; then
	echo "MetaPhlAn in progress ........"
	START=$(date +%s)
	if [ -f $dirPath/$prefix/$prefix".bt2out" ];then
	rm $dirPath/$prefix/$prefix".bt2out"
	fi

	python $metaphlanPath/metaphlan.py $query --bowtie2db  $metaphlanPath/bowtie2db/mpa --bt2_ps sensitive-local --bowtie2out $dirPath/$prefix/$prefix".bt2out" --input_type multifasta  --nproc $threads > $dirPath/$prefix/$prefix"_OutputMetaPhlAn"
	END=$(date +%s)
	DIFF=`expr $(($END - $START)) / 60`
	echo -e `date` "\tTax Identification for "$prefix" using MetaPhlAn executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"
	awk '{print $2 "\t" $1}' $dirPath/$prefix/$prefix".bt2out" | sort -k1,1 > $dirPath/$prefix/$prefix".bt2out_sorted"
	join $dirPath/$prefix/$prefix".bt2out_sorted" $metaphlanPath/MphId_taxID.txt -a1 | sed "s/ /\t/g" | awk '{print $2 "\t" $3}' > $dirPath/$prefix/$prefix"_read_tax_metaphlan"
	fi


		
	######## Combine the N tools output for all reads
	grep "^>" $query | sed "s/>//g" | cut -f1 -d " " | sort > $dirPath/$prefix/$prefix"_readsHeader"
	for file in $dirPath/$prefix/$prefix"_read_tax"*
	do
		sort -k1,1 $file > $file"_sorted" 
		join $dirPath/$prefix/$prefix"_readsHeader" $file"_sorted" -a1 | sed 's/ /,/g' | sed  '/,/! s/$/,0/' | sed "s/,/\t/g" > $file"_joined"
		
		name=`echo $file | sed "s/_read_tax//g" | sed "s/_joined//g"`


		sed "s/\t/,/g" $file"_joined" > $name".csv"

	
	done

	cp $dirPath/$prefix/$prefix"_readsHeader" $dirPath/$prefix/$prefix"_ensemble"
	

	for file in $dirPath/$prefix/$prefix"_read_tax_"*"joined"
	do
		cut -f2 $file > $dirPath/$prefix/temp
		paste $dirPath/$prefix/$prefix"_ensemble" $dirPath/$prefix/temp > $dirPath/$prefix/temp_agg 
		mv $dirPath/$prefix/temp_agg $dirPath/$prefix/$prefix"_ensemble"
	done


	

	
	rm $dirPath/$prefix/$prefix"_readsHeader"  $dirPath/$prefix/$prefix*"_sorted" $dirPath/$prefix/temp
	


#rm $dirPath/$prefix/$prefix*"_joined"
	

	sed "s/\t/,/g" $dirPath/$prefix/$prefix"_ensemble" > $dirPath/$prefix/$prefix"_ensemble.csv"
fi



### Manipulate the NCBI taxonomy database 
if [[ ! -f $taxonomyDB/names_wevote.dmp || ! -f $taxonomyDB/nodes_wevote.dmp ]]; then	
	echo "Downloading NCBI Taxonomy Database in progress ........"
	$my_dir/downloadTaxonomyDB.sh $taxonomyDB
fi



####### Call WEVOTE classification algorithm
START=$(date +%s)
	echo "WEVOTE classification algorithm in progress ........"

$my_dir/bin/WEVOTE -i $dirPath/$prefix/$prefix"_ensemble.csv" -d $taxonomyDB -p $dirPath/$prefix/$prefix -n $threads -k $penalty -a $minNumAgreed -s $minScore
END=$(date +%s)
DIFF=`expr $(($END - $START)) / 60`
echo -e `date` "\tTax Identification for "$prefix" using WEVOTE executed in = "$DIFF" min" >> $dirPath/$prefix/$prefix"_Log"



