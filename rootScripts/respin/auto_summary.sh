#! /bin/sh

while IFS= read -r line; do
    runnum=$line
    rm -f ./summary_plots/run$runnum/run$runnum\_all.pdf ;
    root -b -q -l './scripts/PlotSummary.C('$runnum')';
    root -b -q -l './scripts/CheckRegression.C('$runnum',"mul")';
    root -b -q -l './scripts/CheckRegression.C('$runnum',"pr")';
    root -b -q -l './scripts/CheckPairSAM.C('$runnum',"mul")';
    root -b -q -l './scripts/CheckPairSAM.C('$runnum',"pr")';
    root -b -q -l './scripts/GetBeamNoise.C('$runnum',"mul")';
    root -b -q -l './scripts/GetBeamNoise.C('$runnum',"pr")';

    pdfunite $(ls -rt ./summary_plots/run$runnum/run$runnum*.pdf) \
	./summary_plots/run$runnum/run$runnum\_all.pdf ;
done < $1


