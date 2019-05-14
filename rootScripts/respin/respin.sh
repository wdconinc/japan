#! /bin/tcsh

# switch to ROOT 6 and run JAPAN
source ./setup_japan.tcsh;
./auto_japan.sh $1;

#switch to root 5 and run postpan
source /apps/root/5.34.36/setroot_CUE ;
./auto_postpan.sh $1 

# switch to ROOT 6 for vectorized TObject supprot
# and generate summary output
source ./setup_japan.tcsh;
./auto_summary.sh $1 
