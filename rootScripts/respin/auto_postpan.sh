#! /bin/sh

while IFS= read -r line; do
    runnum=$line
    ./redana -r $runnum -c conf/pair_regression.conf;
    ./redana -r $runnum -c conf/mul_regression.conf;
done < $1
