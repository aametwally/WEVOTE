#!/bin/bash

## Run the R evaluations script:
# example: ./run_STATISTICS.sh <path-to-AbundanceFiles-dir> <output-prefix>
module load apps/R-3.2.0

Rscript ./WEVOTE_Statistics.R $1 $2
