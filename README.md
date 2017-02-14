# What is WEVOTE?
WEVOTE (WEighted VOting Taxonomic idEntification) is a method that classifies metagenome shotgun sequencing DNA reads based on an ensemble of existing methods using k-mer based, marker-based, and naive-similarity based approaches. Here is the full paper: http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0163527



## Getting Started

### Prerequisites
* Install BLASTN, Kraken, TIPP, CLARK, and MetaPhlan on your machine.
* g++
* OpenMP



### Installing, Testing, and Running

Clone the project to your local repository:
```
git clone https://github.com/aametwally/WEVOTE.git
```


Change directory to WEVOTE, then build WEVOTE from scratch:
```
cd WEVOTE
./install.sh
```


Add the executables and databases paths to wevote.cfg
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

To execute the WEVOTE on FASTA sequence file, use:
```
./wevote_pipeline.sh -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options> 
```


### Example:
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


### WEVOTE Output Format
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


### How to generate taxonomic profile from WEVOTE output:

```
./calcAbundance.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
```



### Please report any bugs & suggestions to: ametwa2@uic.edu
