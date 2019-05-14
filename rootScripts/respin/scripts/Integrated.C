//////////////////////////////////////////////////////////////////////
//
// Integrated.C
//   Bryan Moffit - June 2005
// edited by Caryn Palatchi 3/2019 for japan
//
// edited by Tao Ye 5/2019 
//
//  Provides a means to determine the convergence (or lack of) of
//   a given parameter (branch of a tree) vs time.
//

void Integrated(TString treename, 
		TString parameter,
		TString usercut,
		UInt_t nbins) {

  // Check to make sure input arguments are okay.
  TTree* tree = (TTree*)gROOT->FindObject(treename);
  TPad* pad1 = new TPad("pad1","",0,0,1,1);
  pad1->SetGridy(1);
  pad1->Draw();  

  if(!tree) {
    cout << "Integrated: " << endl
	 << "\t Input tree does not exist." << endl;
    return 0;
  }
  if(!tree->FindBranch(parameter)) {
    cout << "Integrated: " << endl
	 << "\t Input parameter (" << parameter 
	 << ") does not exist in tree." << endl;
    return 0;
  }
  
  // Make sure nbins!=0
  if(nbins==0) return 0;

  // Grab the parameter entries from the tree
  
  Long64_t total_entries = tree->Draw(parameter,usercut,"goff");
  if(total_entries==-1) {
    cout << "Integrated: " << endl
	 << "\t Could not process.  Check variables within provided cut:" 
	 << endl
	 << "\t\t" << usercut << endl;
    return 0;
  }

  // Redraw, with higher fEstimate, if necessary
  if(total_entries>tree->GetEstimate()) {
    tree->SetEstimate(total_entries);
    tree->Draw(parameter,usercut,"goff");
  }

  // Determine the number of entries per bin (ent_per_bin)
  Double_t ent_per_bin = tree->GetSelectedRows()/nbins;

  TGraphErrors *graph = new TGraphErrors(nbins);

  // Go through each bin (accumulating statistics), 
  // and add it to the TGraphErrors

  TStatistic  *stat = new TStatistic();
  UInt_t current_bin = 0;
  UInt_t entries_in_bin = 0;
  for(UInt_t ient = 0; ient<tree->GetSelectedRows(); ient++) {
    if(entries_in_bin==ent_per_bin) {
      // Set current bin in TGraph, and move on to next bin
      graph->SetPoint(current_bin,(current_bin+1)*ent_per_bin,stat->GetMean());
      graph->SetPointError(current_bin,0,stat->GetMeanErr());
      current_bin++; entries_in_bin=0;
    }
    Double_t value = tree->GetV1()[ient];
    stat->Fill(value);
    entries_in_bin++;
  }
    
  // Set default titles for graph, x-axis, y-axis
  graph->SetTitle(parameter+" Integrated Convergence;Pattern Number ;"+parameter);

  graph->SetMarkerStyle(20);
  graph->Draw("AP");

}
