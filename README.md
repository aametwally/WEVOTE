# What is WEVOTE?
WEVOTE (WEighted VOting Taxonomic idEntification) is a method that classifies metagenome shotgun sequencing DNA reads based on an ensemble of existing methods using k-mer based, marker-based, and naive-similarity based approaches. The performance evaluation based on fourteen simulated microbiome datasets consistently demonstrates that WEVOTE achieves a high level of sensitivity and precision compared to the individual methods across different taxonomic levels. The major advantage of the WEVOTE pipeline is that the user can make the choice of which tools to use in order to explore the trade-off between sensitivity, precision, time, and memory. The WEVOTE architecture is flexible so that additional taxonomic tools can be easily added, or the current tools can be replaced by improved ones. Moreover, the score assigned to the taxon for each read indicates the confidence level of the assignment. This information is especially useful for the assessment of false positive annotations at a particular taxonomic level. The classification score given by WEVOTE can be used for any downstream analysis that requires the high confidence of the annotated sequences. 

### Publication:
Metwally, Ahmed A., Yang Dai, Patricia W. Finn, and David L. Perkins. "WEVOTE: Weighted Voting Taxonomic Identification Method of Microbial Sequences." PloS one 11, no. 9 (2016): e0163527.
http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0163527

###  Note
Although the current version of WEVOTE only supports five tools, the voting scheme in our framework is flexible and allows for the inclusion or removal of different methods. The currently supported tools are:
* Kraken: Wood, Derrick E., and Steven L. Salzberg. "Kraken: ultrafast metagenomic sequence classification using exact alignments." Genome biology 15.3 (2014): R46.
* Clark: Ounit, Rachid, et al. "CLARK: fast and accurate classification of metagenomic and genomic sequences using discriminative k-mers." BMC genomics 16.1 (2015): 236.
* MetaPhlAn: Segata, Nicola, et al. "Metagenomic microbial community profiling using unique clade-specific marker genes." Nature methods 9.8 (2012): 811-814.
* TIPP: Nguyen, N. P., Mirarab, S., Liu, B., Pop, M., & Warnow, T. (2014). TIPP: taxonomic identification and phylogenetic profiling. Bioinformatics, 30(24), 3548-3555.
* BLASTN: Altschul, Stephen F., et al. "Basic local alignment search tool." Journal of molecular biology 215.3 (1990): 403-410.

</br>


# Getting Started
This section details steps for installing and running WEVOTE. Current WEVOTE version only support Linux. If you experience difficulty installing or running the software, please contact (Ahmed Metwally: ametwa2@uic.edu).

### Prerequisites

* BLASTN, Kraken, TIPP, CLARK, and MetaPhlan installed on the machine. 
* g++ 
* OpenMP: for multithreading execution. 
* A machine with at least 75 GB to run Kraken and Clark. You may ignore this prerequisite if you do not use kraken or clark. 
* R: for generating summary statistics, graphs, and messaging the data to be compatible with Phyloseq package. 

</br>

## Installing, Testing, and Running

#### Clone the project to your local repository:
```
git clone https://github.com/aametwally/WEVOTE.git
```


#### Change directory to WEVOTE, then build WEVOTE from scratch:
```
cd WEVOTE
./install.sh
```

#### Add WEVOTE path to the PATH environmental variable
```
export PATH=$PATH:<path-to-WEVOTE>
```
Alternatively, you can add the to the ~/.bashrc file. 


#### Copy WEVOTE configuration file (wevote.cfg) to the directory where your FASTA file exists:
```
cp wevote.cfg [directory-where-FASTA-file-exists]
cd [directory-where-FASTA-file-exists]
```


#### In the directory where your the FASTA file exists, edit wevote.cfg to add the path of each individual tool's executable and database between the double quotes:
```
blastnPath=""
blastDB=""
krakenPath=""
krakenDB=""
clarkPath=""
clarkDB=""
metaphlanPath=""
tippPath=""
```
</br>
## Running WEVOTE on FASTA sequence file:
```
run_WEVOTE_PIPELINE.sh -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options> 
```


#### Implemented options:
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


#### Example:
```
run_WEVOTE_PIPELINE.sh -i test_wevote.fa -o test_wevote_output --db WEVOTE_DB --clark --metaphlan --blastn --kraken --tipp --threads 16 -a 2
```


#### WEVOTE classification Output Format:
Each sequence classified by WEVOTE results in a single line of output. Output lines have tab-delimited fields; from left to right, they are:


1. The sequence ID, obtained from the FASTA header. 
2. The total number of tools used in WEVOTE (N). 
2. The number of tools that were able to classify the sequence (C).  
3. The number of tools that agreed on WEVOTE decision (A).  
4. Classification score (S).  
5. Taxonomy ID used to classify the sequence by tool #1. This is 0 if the sequence is unclassified by tool #1. 
6. Taxonomy ID used to classify the sequence by tool #2. This is 0 if the sequence is unclassified by tool #2. 
7. Taxonomy ID used to classify the sequence by tool #3. This is 0 if the sequence is unclassified by tool #3. 
8. Taxonomy ID used to classify the sequence by tool #4. This is 0 if the sequence is unclassified by tool #4. 
9. Taxonomy ID used to classify the sequence by tool #5. This is 0 if the sequence is unclassified by tool #5. 
10. The last field is the taxonomy ID assigned to the sequence by WEVOTE. This is 0 if the sequence is unclassified by WEVOTE. 


</br>
## How to generate abundance profile from WEVOTE output:
WEVOTE supports calculating the abundance for the reads or contigs profiling. To execute the the Abundance script on WEVOTE output, use:
```
run_ABUNDANCE.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
```

#### Abundance profiling implemented options: 
```
-h|--help                  	 help flag
-i|--input <input-file>    	 input query
-p|--prefix <output-prefix>  Output prefix
--db <taxonomy_db>         	 taxonomy database path
--threads <num-threads>    	 Number of threads
--seqcount <contig-reads-count-file>		 File that contains how many reads are used to assemble each contig
```

#### Abundance example:
```
run_ABUNDANCE.sh -i test_wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db WEVOTE_DB
```

#### Abundance example assuming the sequences are from contigs:
In this case, each sequence in the FASTA file that WEVOTE analyzed is a contig. To calculate the actual number of reads mapped to every taxon, we need to have a mapping file that has the information of how many reads are used to assemble each contig. This file should be a tab-delimited file with two fields. The first field has the contig-ID and the second field has the number of reads that used to assemble the corresponding contig. Here is an example of how to call the script for contigs profiling:
```
run_ABUNDANCE.sh -i test_wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db WEVOTE_DB --seqcount contig_reads.txt
```

#### Abundance profiling output format:
The abundance ouput file is a comma-seprated file that has 10 fields; from left to right, they are:


1. Taxon: taxonomy ID. 
2. Count: number of reads classified to the taxon in the first field. 
3. Superkingdom: the name of the superkingdom corresponding to the taxonomy id of the first field. This field is left empty if no defined superkingdom for this taxon. 
4. Kingdom: the name of the kingdom corresponding to the taxonomy id of the first field. This field is left empty if no defined kingdom for this taxon. 
5. Phylum: the name of the phylum corresponding to the taxonomy id of the first field. This field is left empty if no defined phylum for this taxon. 
6. Class: the name of the class corresponding to the taxonomy id of the first field. This field is left empty if no defined class for this taxon. 
7. Order: the name of the order corresponding to the taxonomy id of the first field. This field is left empty if no defined order for this taxon. 
8. Family: the name of the family corresponding to the taxonomy id of the first field. This field is left empty if no defined family for this taxon. 
9. Genus: the name of the genus corresponding to the taxonomy id of the first field. This field is left empty if no defined genus for this taxon. 
10. Species: the name of the species corresponding to the taxonomy id of the first field. This field is left empty if no defined species for this taxon. 

</br></br>

### Bugs and Suggestions
WEVOTE is under active research development. Please report any bugs/suggestions to Ahmed Metwally (ametwa2@uic.edu).
