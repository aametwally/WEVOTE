# What is WEVOTE? #
WEVOTE (WEighted VOting Taxonomic idEntification) is a method that classifies metagenome shotgun sequencing DNA reads based on an ensemble of existing methods using k-mer based, marker-based, and naive-similarity based approaches. Here is the full paper on PLOS ONE: http://journals.plos.org/plosone/article?id=10.1371/journal.pone.0163527


# How to run the WEVOTE Pipeline: #
1. install.sh
1. Install BLASTN, Kraken, TIPP, CLARK, and MetaPhlan on your machine.
1. Add the executables and database paths to wevote.cfg
1. wevote_pipeline.sh -i <input-query> -o <output-prefix> --db <path-to-taxonomy-DB> <options> 


# Implemented options: #
```
#!c++

-h|--help                     help flag
-i|--input <input-file>       input query
-o|--output <output-path>     Output prefix
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




# Example: #

```
$ ./wevote_pipeline.sh -i HiSeq_accuracy.fa -o HiSeqOutput --db WEVOTE_DB --clark --metaphlan --blastn --kraken --tipp --threads 16 -a 2
```

### WEVOTE Google group:###
https://groups.google.com/d/forum/wevote-users

## Please report any bugs & suggestions to: ametwa2@uic.edu ##
