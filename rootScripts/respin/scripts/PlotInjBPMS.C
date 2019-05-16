// Injector BPMs Plot Macro 
// Source from Caryn Palatchi
// Adapted by Tao Ye for JAPAN 
// Last Update : 05-2019

#include "device_list.h"

//FIXME : IHWPstatus should be able to obtain from slow tree. -TY

void PlotInjBPMS(vector<const char*> &vBPM , 
		 Int_t IHWPstatus, TString ucut);  // Generic function
void PlotInjBPMS(Int_t IHWPstatus, TString ucut); // interface to panguin
void PlotInjBPMS();// interface to summary plots

void PlotInjBPMS(){
  TCanvas *c4 = new TCanvas("c4","c4",2400,1200);
  c4->cd();
  PlotInjBPMS(1,"ErrorFlag==0");
  plot_title = Form("run%d_injector_BPM_dxdy.png",run_number);
  c4->SaveAs(output_path+plot_title);
}

void PlotInjBPMS( Int_t IHWPstatus, TString ucut){
  PlotInjBPMS(vInjBPM,1,"ErrorFlag==0");
} 

void PlotInjBPMS(vector<const char*> &vBPM , 
		 Int_t IHWPstatus, TString ucut){

  //collect data points from first run file
  TTree* evt_tree = (TTree*)gROOT->FindObject("evt");
  TTree* pair_tree = (TTree*)gROOT->FindObject("pr");
  TTree* mul_tree = (TTree*)gROOT->FindObject("mul");

  Int_t ndeti = vBPM.size();
  Double_t* Aq= new Double_t[ndeti];
  Double_t* eAq= new Double_t[ndeti];
  Double_t* rmsAq= new Double_t[ndeti];
  Double_t* Dx= new Double_t[ndeti];
  Double_t* Dy= new Double_t[ndeti];
  Double_t* eDx= new Double_t[ndeti];
  Double_t* eDy= new Double_t[ndeti];
  Double_t* rmsDx= new Double_t[ndeti];
  Double_t* rmsDy= new Double_t[ndeti];
  Double_t* zero= new Double_t[ndeti];

  TPad *pad1= new TPad("pad1","pad1",0,0,1,1);
  pad1->Divide(2,2);
  pad1->Draw();
  TH1D* thishist;
  TString objnam;
  for(Int_t dd=0;dd<ndeti;dd++) {
    mul_tree->Draw(Form("1e6*asym_%sWS>>hista%d",vBPM[dd],dd),ucut,"goff");
    objnam = Form("hista%d",dd);
    thishist = (TH1D*)gDirectory->FindObject(objnam);
    Aq[dd] = thishist->GetMean();
    eAq[dd]= thishist->GetMeanError();
    rmsAq[dd]= thishist->GetRMS();
    zero[dd]=0;
    // delete thishist;
    mul_tree->Draw(Form("1e3*diff_%sX>>histx%d",vBPM[dd],dd),ucut,"goff");
    objnam = Form("histx%d",dd);
    thishist = (TH1D*)gDirectory->FindObject(objnam);
    Dx[dd] = thishist->GetMean();
    eDx[dd]= thishist->GetMeanError();
    rmsDx[dd]= thishist->GetRMS();
    // delete thishist;
    mul_tree->Draw(Form("1e3*diff_%sY>>histy%d",vBPM[dd],dd),ucut,"goff");
    objnam = Form("histy%d",dd);
    thishist = (TH1D*)gDirectory->FindObject(objnam);
    Dy[dd] = thishist->GetMean();
    eDy[dd]= thishist->GetMeanError();
    rmsDy[dd]= thishist->GetRMS();
    // delete thishist;
    // cout<<vBPM[dd]<<" Aq[ppm](RMS)+-error,Dx[nm](RMS)+-err,Dy[nm](RMS)+-err: "<<Aq[dd]<<" "<<rmsAq[dd]<<" "<<eAq[dd]<<" "<<1e3*Dx[dd]<<" "<<1e3*rmsDx[dd]<<" "<<1e3*eDx[dd]<<" "<<1e3*Dy[dd]<<" "<<1e3*rmsDy[dd]<<" "<<1e3*eDy[dd]<<endl;

  }

  const Int_t numchains=ndeti;
  Double_t chain[numchains];
  Double_t chainerr[numchains];
  for (Int_t i=0;i<numchains;i++){
    chain[i]=i+1;
    chainerr[i]=0.0;
  };

  TGraphErrors *Aqgraph;
  TGraphErrors *Aqrmsgraph;
  TGraphErrors *Dxgraph;
  TGraphErrors *Dygraph;
  TGraphErrors *Dxrmsgraph;
  TGraphErrors *Dyrmsgraph;

  Aqgraph = new TGraphErrors(numchains,chain,&(Aq[0]),chainerr,&(eAq[0]));
  Aqrmsgraph = new TGraphErrors(numchains,chain,&(rmsAq[0]),chainerr,&(zero[0]));
  Dxgraph = new TGraphErrors(numchains,chain,&(Dx[0]),chainerr,&(eDx[0]));
  Dygraph= new TGraphErrors(numchains,chain,&(Dy[0]),chainerr,&(eDy[0]));
  Dxrmsgraph = new TGraphErrors(numchains,chain,&(rmsDx[0]),chainerr,&(zero[0]));
  Dyrmsgraph= new TGraphErrors(numchains,chain,&(rmsDy[0]),chainerr,&(zero[0]));
  
  TMultiGraph *xygraph = new TMultiGraph();
  TMultiGraph *xyrmsgraph = new TMultiGraph();

  // TAxis *ax = Dygraph->GetHistogram()->GetXaxis();
  // Double_t x1 = ax->GetBinLowEdge(1);
  // Double_t x2 = ax->GetBinUpEdge(ax->GetNbins());
  Double_t x1 = chain[0]-0.5;
  Double_t x2 = chain[numchains-1]+0.5;

  TVirtualPad* pad_buff;

  pad_buff= pad1->cd(1);
  Dygraph->SetMarkerStyle(21);
  Dygraph->SetMarkerColor(3);
  Dygraph->SetLineColor(3);

  Dxgraph->SetMarkerStyle(21);
  Dxgraph->SetMarkerColor(4);
  Dxgraph->SetLineColor(4);

  xygraph->Add(Dygraph,"p");
  xygraph->Add(Dxgraph,"p");
  xygraph->Draw("AP");

  xygraph->SetTitle(Form("IHWP=%d, %s ",IHWPstatus,ucut.Data()));
  xygraph->GetYaxis()->SetTitle(" um, blue=Dx, green=Dy");
  xygraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    xygraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }
  pad_buff->SetGrid();

  pad_buff = pad1->cd(3);
  Dyrmsgraph->SetMarkerStyle(21);
  Dyrmsgraph->SetMarkerColor(3);
  Dyrmsgraph->SetLineColor(3);

  Dxrmsgraph->SetMarkerStyle(21);
  Dxrmsgraph->SetMarkerColor(4);
  Dxrmsgraph->SetLineColor(4);

  xyrmsgraph->Add(Dxrmsgraph,"p");
  xyrmsgraph->Add(Dyrmsgraph,"p");
  xyrmsgraph->Draw("AP");
  xyrmsgraph->SetTitle(Form("IHWP=%d,  %s ",IHWPstatus,ucut.Data()));
  xyrmsgraph->GetYaxis()->SetTitle("RMS um, blue=Dx, green=Dy");
  xyrmsgraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    xyrmsgraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }
  pad_buff->SetGrid();

  pad1->cd(2);
  //  ax = Aqgraph->GetHistogram()->GetXaxis();
  //   x1 = ax->GetBinLowEdge(1);
  //   x2 = ax->GetBinUpEdge(ax->GetNbins());
  Aqgraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    //      l2timevschain->GetHistogram()->GetXaxis()->SetBinLabel(k+1,names[k].c_str());
    Aqgraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }

  Aqgraph->SetMarkerStyle(21);
  Aqgraph->SetMarkerColor(1);
  Aqgraph->SetLineColor(1);
  Aqgraph->Draw("AP");
  Aqgraph->SetTitle(Form("IHWP=%d,  %s ",IHWPstatus,ucut.Data()));
  Aqgraph->GetYaxis()->SetTitle("Aq ppm");
  pad1->SetGrid();

  pad1->cd(4);
  Aqrmsgraph->GetHistogram()->GetXaxis()->Set(numchains,x1,x2);
  for(Int_t k=0;k<numchains;k++){
    Aqrmsgraph->GetHistogram()->GetXaxis()->SetBinLabel(k+1,vBPM[k]);
  }

  Aqrmsgraph->SetMarkerStyle(21);
  Aqrmsgraph->SetMarkerColor(1);
  Aqrmsgraph->SetLineColor(1);
  Aqrmsgraph->Draw("AP");
  Aqrmsgraph->SetTitle(Form("IHWP=%d, %s ",IHWPstatus, ucut.Data()));
  Aqrmsgraph->GetYaxis()->SetTitle("RMS ppm");
  pad1->SetGrid();
}

