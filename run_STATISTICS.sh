#!/bin/bash

## Run the R evaluations cript:
module load apps/R-3.2.0
Rscript ./WEVOTE_Statistics.R $1 $2
