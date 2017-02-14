# What is WEVOTE?
WEVOTE (WEighted VOting Taxonomic idEntification) is a method that classifies metagenome shotgun sequencing DNA reads based on an ensemble of existing methods using k-mer based, marker-based, and naive-similarity based approaches. Here is the full paper: http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0163527



## Getting Started

### Prerequisites
* Install BLASTN, Kraken, TIPP, CLARK, and MetaPhlan on your machine.
* g++
* openMP



### Installing, Testing, and Running

Clone the project to your local repository:
```
git clone https://github.com/aametwally/WEVOTE.git
```


Then, change directory to WEVOTE. Then build WEVOTE from scratch:
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

To execute the WEVOTE on fasta sequence file, use:
```
./wevote_pipeline.sh -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options> 
```


### Example:
```
$ ./wevote_pipeline.sh -i HiSeq_accuracy.fa -o HiSeqOutput --db WEVOTE_DB --clark --metaphlan --blastn --kraken --tipp --threads 16 -a 2
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

### How to generate taxonomic profile from WEVOTE output:

```
./calcAbundance.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
```



### Please report any bugs & suggestions to: ametwa2@uic.edu
