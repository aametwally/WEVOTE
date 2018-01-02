# WEVOTE Bootcamp for Finn-Perkins lab members
This tutorial is intended for beginner Linux and WEVOTE users, and will detail the steps required to install and run WEVOTE on the UIC Extreme cluster using a precompiled WEVOTE_PACKAGE. If you have not installed and setup your Linux virtual machine and Extreme account, see this [tutorial](https://github.com/aametwally/FP_MetaGenomics/blob/master/Tutorial_InstallLINUX.md). WEVOTE is under active research development. Please report any bugs/suggestions to Ahmed Metwally (ametwa2@uic.edu).

## WEVOTE Installation
Although WEVOTE normally requires that each tool be downloaded and compiled, and the databases for these tools be built, this process has already been completed in the WEVOTE_PACKAGE folder transferred to you after your Extreme account setup was completed. Thus, your WEVOTE installation mainly consists of configuration steps to ensure all file paths are correct.

### Environment Variables
In order to execute commands without having to type the path to the executable, Linux keeps track of an environment variable called PATH. In order to run WEVOTE scripts from any folder within your Extreme account you need to add WEVOTE’s main folder to your PATH. You can use the command export to add environment variables. However, these will be reset when you logout. To make them permanent, you will need to create a .profile file with all your environment variables. This is a special filetype that will be loaded each time you login. Within your Extreme account home directory:
```
vim ~/.profile
```

Then copy the following into your .profile:
```
export PATH=$PATH:~/WEVOTE_PACKAGE/WEVOTE
```

You can add additional PATH variables or additional environment variables by adding more export commands to .profile. To append more executable paths to PATH, you can just add a : between paths. 
Example(Don't copy paste this):
```
export PATH=$PATH:~/WEVOTE_PACKAGE/WEVOTE:~/blast+/bin:~/centrifuge/scripts
```

While you are in .profile, you can add one more setting to make your Extreme account a little easier to navigate. You can change the shell prompt from -bash-4.1$ to display your netid and current folder instead by copying this into .profile:
```
# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
```

This loads a system configuration file called bashrc that has been preset. Be sure to save .profile and exit vim.


### WEVOTE Configuration
Navigate to the WEVOTE directory in WEVOTE_PACKAGE and open wevote.cfg for editing:
```
cd ~/WEVOTE_PACKAGE/WEVOTE
vim wevote.cfg
```

Add the paths for WEVOTE’s tools (note that ~/ notation does work correctly when executed as a part of a script in the cluster):
```
blastnPath="<PATH to WEVOTE_PACKAGE>/blast"
blastDB="<PATH to WEVOTE_PACKAGE>/blastDB/nt"
krakenPath="<PATH to WEVOTE_PACKAGE>/kraken"
krakenDB="<PATH to WEVOTE_PACKAGE>/krakenDB"
clarkPath="<PATH to WEVOTE_PACKAGE>/clark"
clarkDB="<PATH to WEVOTE_PACKAGE>/clarkDB"
metaphlanPath="<PATH to WEVOTE_PACKAGE>/metaphlan"
tippPath=""
```

For the sake of our tutorial and assuming that the WEVOTE_PACKAGE is located on your home directory on Extreme, your wevote.cfg should loook like this:
```
blastnPath="~/WEVOTE_PACKAGE/blast"
blastDB="~/WEVOTE_PACKAGE/blastDB/nt"
krakenPath="~/WEVOTE_PACKAGE/kraken"
krakenDB="~/WEVOTE_PACKAGE/krakenDB"
clarkPath="~/WEVOTE_PACKAGE/clark"
clarkDB="~/WEVOTE_PACKAGE/clarkDB"
metaphlanPath="~/WEVOTE_PACKAGE/metaphlan"
tippPath=""
```

Copy wevote.cfg to location of your FASTA input files. For the sake of our examples, we will put the wevote.cfg in the home directory. If you will be working with many FASTA files, it would be a good idea to place them all in a seperate folder with a wevote.cfg file so you don't need to repeat this step for each new file.
```
cp wevote.cfg ~/

or

cp wevote.cfg <PATH to FASTA files directory>
```

_Note: FASTA files should always be in the following format:_
```
>read name/info
read
>read name/info
read
...

e.g.

>B_cereus_MiSeq.88
TTCCTTTGTAACTCCGTATAGAATGTCCTACAACCCCAAGAGGCAAGCCTCTTGGTTTGGGCTATGTTCCGTTTCGCTCGCCGCTACTCAGGAAATCGCATTTGCTTTCTCTTCCTCCAGGTACTTAGATGTTTCAGTTCCCTGGGTCTGTCTTCCTTACCCTATGTATTCAGATAAGGATACCATACCATTACGTATGGTGGGTTTCCCCATTCGGAAATCTTCGGATCAAAGCTTACTTACAGCTCCCC
>B_cereus_MiSeq.682
AAATGTATTAGCGAACAATATCGAAAATGCGAAAG
>B_cereus_MiSeq.2899
AAGGAGATGGCGTGTTGTAAGAAACTGTACCATTAACAGAATCACGAACTGGTGCAGAGTATACTTCACCGTCTGGAATGTTTAAATGACCTGAGCATTTAATTGCTGGAATGTCTTTAATAGAGAATGTTAAGTCAGTTCCAGGACCAGTTAGGCGCACTTTATCTGTTTTATTCATTAATGTAACAAGGCTATCCATCGCCTTATCCATTTTACCGTAGTCTAAGTTACAAACTTCGAAGTAGAAGTCT
```

## Running WEVOTE
In order to take advantage of the Extreme cluster’s resources, WEVOTE should not be run directly from the shell. A shell script containing all relevant commands and info needs to be written and submitted to the Extreme cluster as a job for automatic approval and resource distribution. It is recommended that you familiarize yourself with the basics of [shell scripting](http://linuxcommand.org/lc3_writing_shell_scripts.php) and [specific commands](http://rc.uic.edu/resources/technical-documentation/) for the cluster. 

### Prepare the Shell Script

To start making a WEVOTE script, create a new file using vim in a directory where you would like to store your shell scripts. For our examples, we name this script WEVOTERUN.sh and we will use the directory ~/TestWEVOTE (you can enter "mkdir Scripts" in your home directory to make this).
```
cd
mkdir TestWEVOTE
cd TestWEVOTE
vim WEVOTERUN
```

Similar to when we set our virtual machine’s terminal to use bash as its shell, the first line of every shell script designates the shell that will be used to carry out commands.
```
#!/bin/bash
```

Next, we need to define options for the Extreme cluster management system using #PBS commands. There are additional PBS commands listed in the technical documentation for Extreme, but the important ones for successfully running WEVOTE are listed below:
```
#PBS -l nodes=16 
#PBS -l partition=ALL
#PBS -l walltime=24:00:00
#PBS -M <your-netid>@uic.edu
#PBS -m abe
#PBS -N WEVOTERUN
#PBS -V
#PBS -o ~/TestWEVOTE/WEVOTERUN.out
```

PBS -l commands describe the resources required for the job, with nodes being the number of computing nodes requested, partition meaning what part of Extreme to run on (leave this setting alone), and walltime indicating how much time the program is allotted to run before it is terminated in ds:hrs:mins:secs.

_Note that shorter wall time jobs are given priority. Setting this to 12 or 24 hours is more than enough for FASTA files with far more than 20k reads. For our example, we will set it to 20 minutes since we will only analyze 100 reads._ 

PBS -M sets an email to receive notifications about the status of the job.

PBS -m be tells Extreme to notify the user when the program begins and ends. You can change the setting to “abe” if you would also like a notification when the program aborts. 

PBS -N names the job, making it easier to parse errors and outputs later.

PBS -V passes all environment variables from your Extreme account to the job. This is vital if your script relies on your environment variables but is optional if you wrote out the full path for everything in your script.

PBS -o sets a file to be used for the stdout of the script, i.e. any notifications or prompts from the shell as it executes commands.

### Metaphlan Python Modules
The UIC Extreme account does not load all available tools for each user by default. To use Metaphlan, you need to add one more line after the #PBS commands that loads numpy, a necessary python module for Metaphlan:
```
module load tools/numpy-1.8.1-intel-python2.7.6
module load apps/bowtie2-2.2.9
```

### Setting Up WEVOTE in the Shell Script
Now that your shell script settings are ready, you can begin writing the actual script for running WEVOTE. These commands will essentially run in the same environment as commands you type into your Extreme bash shell. All syntax is the same.

Assuming you’ve added the path to WEVOTE in your PATH environment variable, you will only need one major command:
```
run_WEVOTE_PIPELINE.sh -i <input-file> -o <output-foldername> --db <path to WEVOTE_DB> <options>
```

Options:
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


Now we can add the example command that actually runs WEVOTE (Note that threads should be adjusted based on the number of nodes you request. Each node usually can run 16 threads.):
```
run_WEVOTE_PIPELINE.sh -i 100readtest.fa -o wevote_output --db ~/WEVOTE_PACKAGE/WEVOTE_DB --clark --metaphlan --blastn --kraken --threads 256 -a 2
```

After this command executes, you can add one more line to let the script indicate that it finished running successfully. This message will be saved to stdout.
```
echo ‘Done’
```

In summary, your script for running WEVOTE on a file called 100readtest.fa that saved in a directory "TestWEVOTE" should look like this:
```
#!/bin/bash
#PBS -l nodes=1 
#PBS -l partition=ALL
#PBS -l walltime=00:20:00
#PBS -M <your-netid>@uic.edu
#PBS -m abe
#PBS -N WEVOTERUN
#PBS -V
#PBS -o ~/TestWEVOTE/WEVOTERUN.out

module load tools/numpy-1.8.1-intel-python2.7.6
module load apps/bowtie2-2.2.9

run_WEVOTE_PIPELINE.sh -i 100readtest.fa -o wevote_output --db ~/WEVOTE_PACKAGE/WEVOTE_DB --clark --metaphlan --blastn --kraken --threads 16 -a 2

echo 'Done'
```

Save the file and exit vim.

### Running WEVOTE on the Cluster
To run your new script on the cluster:
```
qsub -q batch WEVOTERUN.sh
```

_Note: If you run WEVOTE multiple times in the same output folder, errors may occur. Always designated a new folder for each run, or delete the folder contents._

Successful job submission will return a job ID number. You can track your job using two main commands. 

checkjob shows details of a particular job.
```
checkjob <job ID>
```

showq gives an overview of all jobs running and in the queue.
```
showq
```

### WEVOTE classification Output Format:
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


## How to generate the abundance profile from WEVOTE output:
WEVOTE supports calculating the abundance for the reads or contigs profiling. This can be written into the script that executes WEVOTE or simply entered from the terminal. To execute the the Abundance script on WEVOTE output, use:
```
run_ABUNDANCE.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
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

### Abundance example:
After successfully running our example WEVOTE script above, you can use the following commands to run WEVOTE's abundance analysis on the output. The input for abundance is always the file with WEVOTE_details.txt as a suffix. In our case, this file should be in the folder wevote_output, which we designated in the script for running WEVOTE.

```
cd wevote_output
run_ABUNDANCE.sh -i wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db ~/WEVOTE_PACKAGE/WEVOTE_DB
```

### Abundance example assuming the sequences are from contigs:
In this case, each sequence in the FASTA file that WEVOTE analyzed is a contig. To calculate the actual number of reads mapped to every taxon, we need to have a mapping file that has the information of how many reads are used to assemble each contig. This file should be a tab-delimited file with two fields. The first field has the contig-ID and the second field has the number of reads that used to assemble the corresponding contig. Here is an example of how to call the script for contigs profiling:
```
run_ABUNDANCE.sh -i wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db ~/WEVOTE_PACKAGE/WEVOTE_DB --seqcount contig_reads.txt
```

### Abundance profiling output format:
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

## How to Analyze Abundance Output Using R
An additional shell script is available for use after running abundance: run_STATISTICS.sh. This script will take the abundance output and calculate summary statistcs using R. These stats can then be
exported to the WEVOTE Visualization script for automatic graphic generation.

To run the script, you need to cd to the WEVOTE directory:
```
cd ~/WEVOTE_PACKAGE/WEVOTE
```

Then run the script:
```
run_STATISTICS.sh <path-to-wevote-output-folder> <output prefix>
```
Example using output from previous example steps:
```
run_STATISTICS.sh ~/wevote_output 'statistics'
```

This will create two files with the prefix statistics. The statistics_countMatrix.csv contains counts of each taxonID classified, and the statistcs_OTUMatrix.csv contains full taxonomy name info for each taxonID classified.

## How to run all WEVOTE components in one script

It is possible to add lines for running WEVOTE abundance and statistics components in your primary WEVOTERUN shell script.
```
cd <path-to-wevote-output>
run_ABUNDANCE.sh -i <input-file> -p <output-prefix> --db <path-to-taxonomy-DB> <options>
cd <path-to-WEVOTE-program-directory>
run_STATISTICS.sh <path-to-wevote-output-directory> <statistics-output-prefix>
```
Using our previous examples, this would be:
```
cd ~/wevote_output
run_ABUNDANCE.sh -i wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db ~/WEVOTE_PACKAGE/WEVOTE_DB
cd ~/WEVOTE_PACKAGE/WEVOTE
run_STATISTICS.sh ~/wevote_output statistics
```

The full script for our example would be:
```
#!/bin/bash
#PBS -l nodes=1 
#PBS -l partition=ALL
#PBS -l walltime=00:20:00
#PBS -M <your-netid>@uic.edu
#PBS -m abe
#PBS -N WEVOTERUN
#PBS -V
#PBS -o ~/TestWEVOTE/WEVOTERUN.out

module load tools/numpy-1.8.1-intel-python2.7.6
module load apps/bowtie2-2.2.9


run_WEVOTE_PIPELINE.sh -i 100readtest.fa -o wevote_output --db ~/WEVOTE_PACKAGE/WEVOTE_DB --clark --metaphlan --blastn --kraken --threads 16 -a 2

cd wevote_output
run_ABUNDANCE.sh -i wevote_output_WEVOTE_Details.txt -p test_wevote_abundance --db ~/WEVOTE_PACKAGE/WEVOTE_DB
cd ~/WEVOTE_PACKAGE/WEVOTE
run_STATISTICS.sh ~/TestWEVOTE/wevote_output statistics

echo 'Done'
```

_Note: run_STATISTICS.sh must be run from the WEVOTE program directory for the R script to work properly._

### Troubleshooting
After your job is complete, it will generate files that can help with troubleshooting. These files are labeled with the suffix .o[JOBID] or .e[JOBID] and stored in the folder designated by PBS -o.

Check these files for clues to script errors if the output of WEVOTE is not generated or not what was expected.



