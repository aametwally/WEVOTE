# What is WEVOTE?
WEVOTE (WEighted VOting Taxonomic idEntification) is a method that classifies metagenome shotgun sequencing DNA reads based on an ensemble of existing methods using k-mer based, marker-based, and naive-similarity based approaches. The performance evaluation based on fourteen simulated microbiome datasets consistently demonstrates that WEVOTE achieves a high level of sensitivity and precision compared to the individual methods across different taxonomic levels. The major advantage of the WEVOTE pipeline is that the user can make the choice of which tools to use in order to explore the trade-off between sensitivity, precision, time, and memory. The WEVOTE architecture is flexible so that additional taxonomic tools can be easily added, or the current tools can be replaced by improved ones. Moreover, the score assigned to the taxon for each read indicates the confidence level of the assignment. This information is especially useful for the assessment of false positive annotations at a particular taxonomic level. The classification score given by WEVOTE can be used for any downstream analysis that requires the high confidence of the annotated sequences. Here is the full paper: http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0163527


Although the current version of WEVOTE only supports five tools, the voting scheme in our framework is flexible and allows for the inclusion or removal of different methods. The current supported tools are:
* Kraken: Wood, Derrick E., and Steven L. Salzberg. "Kraken: ultrafast metagenomic sequence classification using exact alignments." Genome biology 15.3 (2014): R46.
* Clark: Ounit, Rachid, et al. "CLARK: fast and accurate classification of metagenomic and genomic sequences using discriminative k-mers." BMC genomics 16.1 (2015): 236.
* MetaPhlAn: Segata, Nicola, et al. "Metagenomic microbial community profiling using unique clade-specific marker genes." Nature methods 9.8 (2012): 811-814.
* TIPP: Nguyen, N. P., Mirarab, S., Liu, B., Pop, M., & Warnow, T. (2014). TIPP: taxonomic identification and phylogenetic profiling. Bioinformatics, 30(24), 3548-3555.
* BLASTN: Altschul, Stephen F., et al. "Basic local alignment search tool." Journal of molecular biology 215.3 (1990): 403-410.


## Getting Started

### Prerequisites
* BLASTN, Kraken, TIPP, CLARK, and MetaPhlan installed on the machine.
* g++
* OpenMP: for multithreading execution.
* Machine with at least 75 GB to run Kraken and Clark. You may ingore this prerequisites if you will not use kraken or clark.
* R: for generating summary statitcs, graphs, and messaging the data to be compatible with Phyloseq package.


### Installing, Testing, and Running

#### Clone the project to your local repository:
```
git clone https://github.com/aametwally/WEVOTE.git
```


#### Change directory to WEVOTE, then build WEVOTE from scratch:
```
cd WEVOTE
./install.sh
```


#### Add the path of each individual tool's executable and database to wevote.cfg
```
taxonomyDB=""
blastnPath=""
blastDB=""
krakenPath=""
krakenDB=""
clarkPath=""
clarkDB=""
metaphlanPath=""
tippPath=""
```

#### To execute the WEVOTE on FASTA sequence file, use:
```
./wevote_pipeline.sh -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options> 
```


#### Example:
```
./wevote_pipeline.sh -i HiSeq_accuracy.fa -o HiSeqOutput --db WEVOTE_DB --clark --metaphlan --blastn --kraken --tipp --threads 16 -a 2
```


### Implemented options:
```
-h|--help                     help flag
-i|--input <input-file>       input query
-o|--output <output-prefix>   Output prefix
--db <taxonomy_db>            taxonomy database path
--threads <num-threads>       number of threads 
-s <score>                    Score threshold
-a <num-of-agreed>            Minimum number of tools agreed tools on WEVOTE decision	
-k <penalty>                  Score penalty for disagreement
--kraken                      Run Kraken
--blastn                      Run BLASTN
--tipp                        Run TIPP
--clark                       Run CLARK
--metaphlan                   Run MetaPhlAn
-c|--classfy                  Start the pipeline from the classification step. i.e., skip running individual tools
```


### WEVOTE classification Output Format:
Each sequence classified by WEVOTE results in a single line of output. Output lines have tab-delimited fields; from left to right, they are:
* The sequence ID, obtained from the FASTA header.
* The number of tools that have classified the sequence.
* The number of tools that agreed on WEVOTE decision.
* Classification score.
* Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #1.
* Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #2.
* Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #3.
* Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #4.
* Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #5.
* The last field is the taxonomy ID assigned to the sequence by WEVOTE. This is 0 if the sequence is unclassified by WEVOTE.


### How to generate abundance profile from WEVOTE output:
WEVOTE supports calculating the abundance for the reads or contigs profiling. To execute the the Abundance script on WEVOTE output, use:
```
./calcAbundance.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
```


### Abundance profiling implemented options: 
```
-h|--help                  	 help flag
-i|--input <input-file>    	 input query
-p|--prefix <output-prefix>  Output prefix
--db <taxonomy_db>         	 taxonomy database path
--threads <num-threads>    	 Number of threads
--seqcount <contig-reads-count-file>		 File that contains how many reads are used to assemble each contig
```

### Abundance profiling output format:
Each line of the Abundance file has 10 fileds. Output lines have comma-delimted fields; from left to right, they are:
1. Taxon: taxonomy ID
1. Count: number of reads classified to the taxon in the first field
1. Superkingdom: the name of the superkingdom corresponding to the taxonomy id of the first field. This field is left empty if no defined superkingdom for this taxon
1. Kingdom: the name of the kingdom corresponding to the taxonomy id of the first field. This field is left empty if no defined kingdom for this taxon
1. Phylum: the name of the phylum corresponding to the taxonomy id of the first field. This field is left empty if no defined phylum for this taxon
1. Class: the name of the class corresponding to the taxonomy id of the first field. This field is left empty if no defined class for this taxon
1. Order: the name of the order corresponding to the taxonomy id of the first field. This field is left empty if no defined order for this taxon
1. Family: the name of the family corresponding to the taxonomy id of the first field. This field is left empty if no defined family for this taxon
1. Genus: the name of the genus corresponding to the taxonomy id of the first field. This field is left empty if no defined genus for this taxon
1. Species: the name of the species corresponding to the taxonomy id of the first field. This field is left empty if no defined species for this taxon

#### Please report any bugs & suggestions to: ametwa2@uic.edu
