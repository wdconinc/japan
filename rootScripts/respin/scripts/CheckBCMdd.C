// author: Tao Ye
// date: 05-2019
// decription: Macro for showing BCM correlation and double difference 

#include "device_list.h"

void CheckBCMdd(const char* bcm1 , const char* bcm2, TString treeName);  //interface to panguin
void CheckBCMdd(); // interface to summary plot

void CheckBCMdd(){

  Int_t nbcm = vBCM.size();
  TString treeNames[] = {"pr","mul"};
  TString treeName;

  TCanvas *c3 = new TCanvas("c3","c3",1800,600);
  c3->cd();

  for(int iTree = 0; iTree<2 ; iTree++){
    treeName= treeNames[iTree];
    for(int ibcm=0; ibcm<nbcm;ibcm++){
      for(int jbcm=0;jbcm<nbcm;jbcm++){
	if(jbcm<=ibcm)
	  continue;
	c3->Clear("D");
	c3->cd();
	CheckBCMdd(vBCM[ibcm],vBCM[jbcm],treeName);

	plot_title = Form("run%d_%s_vs_%s_DD_%sTree.png",
			  run_number,vBCM[ibcm],vBCM[jbcm],treeName.Data());
	TText *label = new TText(0.0,0.01,plot_title);      
	label->SetNDC();
	c3->cd();
	label->Draw("same");
	c3->SaveAs(output_path+plot_title);
      }
    }
    gSystem->Exec(Form("convert %s*bcm*DD*.png %srun%d_bcm_dd_%sTree.pdf",
		       output_path.Data(),output_path.Data(),
		       run_number,treeName.Data()));
    gSystem->Exec(Form("rm %s*.png",output_path.Data()));
  }
}

void CheckBCMdd(const char* bcm1 , const char* bcm2, TString treeName){
  gStyle->SetOptStat(1);
  TString asym_bcm1 = "asym_"+TString(bcm1);
  TString asym_bcm2 = "asym_"+TString(bcm2);
  TTree* tree = (TTree*)gROOT->FindObject(treeName);

  TPad *pad1 = new TPad("pad1","pad1",0,0,1,1);
  pad1->Divide(3,1);
  pad1->Draw();
  TVirtualPad *pad_buff;
  TProfile *prof_buff;
  TH1D *h_buff;

  pad_buff =pad1->cd(1);
  
  tree->Draw(Form("%s:%s",asym_bcm1.Data(),asym_bcm2.Data()),"ErrorFlag==0","prof");
  gPad->Update();
  prof_buff=(TProfile*)pad_buff->FindObject("htemp");
  TPaveStats*st = (TPaveStats*)prof_buff->FindObject("stats");
  st->SetOptFit(1);
  st->SetOptStat(0);
  st->SetX1NDC(0.1);
  st->SetY1NDC(0.9);
  st->SetY2NDC(0.7);
  st->SetX2NDC(0.5);
  prof_buff->Fit("pol1","Q");
  
  pad1->cd(2);
  tree->Draw(Form("%s:%s",asym_bcm1.Data(),asym_bcm2.Data()),"ErrorFlag==0");

  pad_buff = pad1->cd(3);
  tree->Draw(Form("%s-%s",asym_bcm1.Data(),asym_bcm2.Data()),"ErrorFlag==0");
  h_buff=(TH1D*)pad_buff->FindObject("htemp");

  h_buff->SetName(treeName+"Tree");

}
