#!/usr/bin/env Rscript
require("plyr")
rm(list=ls())

dir = ""
prefix = "WEVOTE"
args = commandArgs(trailingOnly=TRUE)
# test if there is at least one argument: if not, return an error
if (length(args)==0) {
  stop("At least one argument must be supplied (Directory that has the WEVOTE ABUNDANCE files)", call.=FALSE)
} else if (length(args)==1) {
  # input directory file
  dir = args[1]
  cat("Input Directory = ", dir, "\n")
  cat("Output Prefix = ", prefix, "\n")
} else if (length(args)==2) {
  dir = args[1]
  prefix = args[2]
  cat("Input Directory = ", dir, "\n")
  cat("Output Prefix = ", prefix, "\n")
} else{
  stop("You have entered wrong number of arguments", call.=FALSE)

}



#### change directory to the directory where the files exist
setwd(dir)



#### Prepare Taxanomy matrix
PrepareTaxonomy <- function(output.prefix = "WEVOTE")
{
  curfilesc = NULL
  filenamesc = NULL
  filenames = list.files(pattern="*_Abundance.csv", full.names=FALSE)
  curfiles <- lapply(filenames,function(i){
    read.csv(i, header=TRUE)[ ,-2]
  })
  
  taxaTable = join_all(curfiles,by="taxon", type = "full")
  colnames(taxaTable) = c("taxon", "Superkingdom", "Kingdom", "Phylum", "Class", "Order", "Family", "Genus", "Species")
  rownames(taxaTable) = taxaTable[,1];
  taxaTable = taxaTable[,-1]
  length(unique(sort((taxaTable[,1]))))
  write.csv(taxaTable, file = paste(output.prefix, "_OTUMatrix.csv", sep=""))
}



#### Prepare Count matrix
PrepareCount <- function(output.prefix = "WEVOTE")
{
  curfilesc = NULL
  filenamesc = NULL
  filenames = list.files(pattern="*_Abundance.csv", full.names=FALSE)
  curfiles <- lapply(filenames,function(i){
    read.csv(i, header=TRUE)[ ,1:2]
  })
  
  for (i in 1:length(filenames))
  {
    colnames(curfiles[[i]]) =  c("taxon",sapply(strsplit(filenames[i],split="\\_V"),"[",1)) 
  }
  
  countData = join_all(curfiles, by="taxon", type = "full")
  rownames(countData) = countData[,1]; 
  countData = countData[,-1]
  countData[is.na(countData)] = 0
  write.csv(countData, file = paste(output.prefix, "_countMatrix.csv", sep=""))
}




PrepareTaxonomy(prefix)
PrepareCount(prefix)
