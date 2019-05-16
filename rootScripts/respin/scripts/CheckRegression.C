// CheckRegression.C : 
//	ROOT script for PostPAN regression plots
//
//  author: Tao Ye 
//  	05-2019

#include "device_list.h"
#include "PlotCorrelation.C"

void CheckRegression(Int_t run_num, TString suffix){

  run_number = run_num;
  path = "./japanOutput/";
  prefix = "prexRespin1";
  file_name = Form("%s_%d_regress_%s.root",
		   prefix.Data(),run_number,suffix.Data());
  TFile* rootfile = TFile::Open(path+file_name);
  output_path = Form("./summary_plots/run%d/",run_number);
  TString pdf_filename;

  gStyle->SetOptFit(1);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.2);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.95);

  vector<const char*> vtag1{
    "sam","sam","reg_sam","reg_sam","bpm"
  };
  vector<const char*> vtag2{
    "bpm","sam","bpm","reg_sam","bpm"
  };

  vector<const char*> YV[]={DV_raw,DV_raw,DV_reg,DV_reg,IV};  
  vector<const char*> XV[]={IV,DV_raw,IV,DV_reg,IV};

  Int_t nplot = sizeof(XV)/sizeof(*XV);

  TString draw_opts[] = {"COLZ","fit","scat"};
  Int_t canvas_width = 600;
  TCanvas* c_this = new TCanvas("","",800,800);
  for(Int_t i=0;i<nplot;i++){

    const char* tag1 = vtag1[i];
    const char* tag2 = vtag2[i];

    for(Int_t iopt=0;iopt<3;iopt++){
      int ny = (YV[i]).size();
      int nx = (XV[i]).size();
      c_this->SetWindowSize((nx+1)*canvas_width,ny*canvas_width);
      c_this->cd();

      PlotCorrelation(YV[i],XV[i],"reg",
		      "","",
		      draw_opts[iopt],
		      "ok_cut");

      plot_title  = Form("run%d_%s_vs_%s-%s_postpan_%sTree.png",
			 run_number,tag1,tag2,
			 draw_opts[iopt].Data(),
			 suffix.Data() );
      TText *label = new TText(0.0,0.005,plot_title);     
      label->SetTextFont(23);
      label->SetTextSize(70);
      label->SetNDC();
      c_this->cd();
      label->Draw("same");
      c_this->SaveAs(output_path+plot_title);
      c_this->Clear("D");
    }
  }
  gSystem->Exec(Form("convert $(ls -rt %s/*%sTree.png) %s/run%d_postpan_%sTree.pdf",
		     output_path.Data(),suffix.Data(),
		     output_path.Data(),run_number,suffix.Data() ));
  
  gSystem->Exec(Form("rm %s/*.png",output_path.Data()));

}

