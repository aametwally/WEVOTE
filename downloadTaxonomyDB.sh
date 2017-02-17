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

if [ $# -ne 1 ]; then
	echo "Usage: $0 <Directory: directory to store taxonomy data> "
	echo "Note: if the chosen directory is not empty, then its content will be erased."
exit

fi


if [ ! -d $1 ]; then
	mkdir $1
fi

cd $1

echo "Downloading in progress ... "
wget ftp://ftp.ncbi.nih.gov/pub/taxonomy/taxdump.tar.gz

# Extrat downloaded data
if [ -s taxdump.tar.gz ]; then
	echo "Uncompressing files... "
	tar -zxf taxdump.tar.gz
	if [ ! -s names.dmp ] && [ ! -s nodes.dmp ]; then
		echo "The downloaded file doesnot has names.dmp and nodes.dmp data!"
		exit
	fi
	echo "Format the taxonomy database for WEVOTE"
        sed "s/ /_/g" names.dmp | awk '{ if($0 ~ "scientific_name") print $0}' > names_wevote.dmp
        sed "s/ /_/g" nodes.dmp > nodes_wevote.dmp
else
	echo "Failed to download taxonomy data!"
	exit
fi

