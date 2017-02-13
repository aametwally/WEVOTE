./ABUNDANCE -i MCAS000063_S1_L001_R1_001_wevote.csv -p S1_tax -d WEVOTE_DB/


awk 'BEGIN { FS = "," }; {print "read,"$1}' MCAS000063_S1_Tax_Reads.csv > test.csv

./ABUNDANCE -i test.csv -p test_tax -d WEVOTE_DB/



grep "Eukaryota" -v MKAN003343_S3_Complete_Tax_with_counts.csv | awk 'BEGIN {FS=","}; {if($3!="") print $0}' |

