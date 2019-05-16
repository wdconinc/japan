// author : Tao Ye
// date : 05-2019
// description: Macro for checking global event cuts
#include "device_list.h"

void CheckEventCut(const char*);
void CheckEventCut();

void CheckEventCut(){
  TCanvas *c1 = new TCanvas("c1","c1",2400,600);
  Int_t nbcm = vBCM.size();
  for(int ibcm=0;ibcm<nbcm;ibcm++){
    c1->cd();
    CheckEventCut(vBCM[ibcm]);
    plot_title = Form("run%d_check_EventCut_%s.png",run_number,vBCM[ibcm]);

    TText *t1 = new TText(0.0,0.007,plot_title);
    t1->SetNDC();
    t1->SetTextSize(0.02);
    c1->cd();
    t1->Draw("same");

    c1->SaveAs(output_path+ plot_title);
    c1->Clear("D");
  } // end of BCM loop
}

void CheckEventCut(const char* device_name){
  gStyle->SetOptStat(1);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  TTree *evt = (TTree*)gROOT->FindObject("evt");
  TTree *mul = (TTree*)gROOT->FindObject("mul");
  TTree *pr = (TTree*)gROOT->FindObject("pr");
  
  TPad *pad1 = new TPad("pad1","pad1",0.0,0.0,1.0,1.0);
  pad1->Draw();
  pad1->Divide(4,1);
  TH1D *h_buff;
  TVirtualPad* pad_buff;
  TGraph* g_buff;

  pad_buff = pad1->cd(1);
  evt->Draw(Form("%s:Entry$",device_name),"","l");
  g_buff = (TGraph*)pad_buff->FindObject("Graph");
  g_buff->SetName("GraphAll");
  evt->Draw(Form("%s:Entry$",device_name),"ErrorFlag!=0","* same");
  g_buff = (TGraph*)pad_buff->FindObject("Graph");
  g_buff->SetMarkerColor(kRed);

  pad1->cd(2);
  evt->Draw(Form("%s:Entry$",device_name),"ErrorFlag==0","*");

  pad_buff = pad1->cd(3);
  mul->Draw(Form("asym_%s",device_name),"ErrorFlag==0");
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  h_buff->SetName("MulTree");

  pad_buff = pad1->cd(4);
  pr->Draw(Form("asym_%s",device_name),"ErrorFlag==0");
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  h_buff->SetName("PairTree");
}
