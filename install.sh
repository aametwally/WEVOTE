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


VERSION="1.5.0"

if [[ ! -d bin ]];then
mkdir bin

fi
make -C src
make -C src install
