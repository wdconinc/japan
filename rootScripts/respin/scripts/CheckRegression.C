#include <vector>
void PlotCorrelation( Int_t , 
		      vector<TString> ,vector<TString> , 
		      TString tag1, TString tag2,
		      TString draw_opt, TString treeName);

TString output_dir;
void CheckRegression(Int_t run_num){
  gStyle->SetOptFit(1);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.2);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.95);

  output_dir = Form("./summary/run%d/",run_num);

  vector<TString> sam{
    "asym_sam2","asym_sam6","asym_sam4","asym_sam8"};

  vector<TString> bpm{
    "diff_bpm4aX","diff_bpm4aY", "diff_bpm4eX","diff_bpm4eY","diff_bpm14X"};

  vector<TString> reg_sam{
    "reg_asym_sam2","reg_asym_sam6","reg_asym_sam4","reg_asym_sam8"};

  vector<TString> tag1;
  vector<TString> tag2;
  
  vector < vector<TString> > VariableY{
    sam,sam,reg_sam,reg_sam,bpm
      };
  vector < vector<TString> > VariableX{
    bpm,sam,bpm,reg_sam,bpm
      };

  vector<TString> vtag1{
    "sam","sam","reg_sam","reg_sam","bpm"
  };
  vector<TString> vtag2{
    "bpm","sam","bpm","reg_sam","bpm"
  };
  
  Int_t nplot = VariableX.size();

  for(Int_t i=0;i<nplot;i++){
    
    vector<TString> xv = VariableX[i];
    vector<TString> yv = VariableY[i];
    TString tag1 = vtag1[i];
    TString tag2 = vtag2[i];

    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"colz","mul"); 
    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"prof","mul"); 
    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"scat","mul"); 

    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"colz","pr"); 
    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"prof","pr"); 
    PlotCorrelation(run_num, yv, xv, 
		    tag1,tag2,"scat","pr"); 

    gSystem->Exec(Form("convert %s/*prTree.png %s/run%d_%s_vs_%s_prTree.pdf",
		       output_dir.Data(),output_dir.Data(),
		       run_num,tag1.Data(), tag2.Data() ));

    gSystem->Exec(Form("convert %s/*mulTree.png %s/run%d_%s_vs_%s_mulTree.pdf",
		       output_dir.Data(),output_dir.Data(),
		       run_num,tag1.Data(), tag2.Data() ));

    gSystem->Exec(Form("rm %s/*.png",output_dir.Data()));

  }

}

void PlotCorrelation( Int_t run_num, 
		      vector<TString> dv_name,vector<TString> iv_name, 
		      TString tag1, TString tag2,
		      TString draw_opt,TString tree)
{

  TFile* rootfile = TFile::Open(Form("./japanOutput/prexRespin1_%d_regress_%s.root",run_num,tree.Data() ));
  TTree *reg = (TTree*)rootfile ->Get("reg");

  // iv : independent variables
  // dv : dependent variables
  
  TString cuts = "ok_cut";

  Int_t ndvs= dv_name.size();
  Int_t nivs= iv_name.size();
  
  vector<double> iv_mean;
  vector<double> iv_rms;
  vector<double> dv_mean;
  vector<double> dv_rms;

  for(int i=0;i<ndvs;i++){
    reg->Draw(Form("%s>>hdv%d",dv_name[i].Data(),i),cuts);
    TH1D *hbuff = (TH1D*)gDirectory->FindObject(Form("hdv%d",i));
    dv_mean.push_back( hbuff->GetMean());
    dv_rms.push_back( hbuff->GetRMS() );
  }
  for(int i=0;i<nivs;i++){
    reg->Draw(Form("%s>>hiv%d",iv_name[i].Data(),i),cuts);
    TH1D *hbuff = (TH1D*)gDirectory->FindObject(Form("hiv%d",i));
    iv_mean.push_back( hbuff->GetMean());
    iv_rms.push_back( hbuff->GetRMS());
  }

  vector<TText * > dv_txt;
  vector<TText * > iv_txt;

  for(int i=0;i<ndvs;i++){
    TText *txt = new TText(0.0,0.4, dv_name[i]);
    txt->SetTextFont(22);
    txt->SetTextSize(0.15);
    dv_txt.push_back(txt);
  }

  for(int i=0;i<nivs;i++){
    TText *txt = new TText(0.0,0.90, iv_name[i]);
    txt->SetTextFont(22);
    txt->SetTextSize(0.15);
    txt->SetNDC();
    iv_txt.push_back(txt);
  }

  Int_t c_height = 600;
  TCanvas *c1  = new TCanvas("c1","c1",c_height*(nivs+1), c_height*ndvs);
  c1->Divide(nivs+1,ndvs); // columns : iv 

  TString filename = Form("run%d_%s_vs_%s-%s-%sTree.png",
			  run_num,tag1.Data(),tag2.Data(),
			  draw_opt.Data(),tree.Data() );
  TText *label = new TText(0.0,0.005,filename);     
  label->SetTextFont(23);
  label->SetTextSize(70);
  label->SetNDC();
  c1->Clear("D");
  c1->cd();
  label->Draw("same");

  TVirtualPad *c_buff;
  TH1D* h_buff;
  for(int irow=0;irow<ndvs; irow++){
    c1->cd(1 + irow*(nivs+1));
    dv_txt[irow]->Draw();
    for(int icol=0; icol< nivs; icol++){
      c_buff =c1->cd(icol+2+irow*(nivs+1));
      if(draw_opt=="scat"){
	reg->Draw(
		  Form("%s:%s", 
		       dv_name[irow].Data(),iv_name[icol].Data()),
		  cuts);
	h_buff = (TH1D*)c_buff->FindObject("htemp");
	h_buff->SetTitle("");
      }
      else{
	reg->Draw(
		  Form("%s:%s>>h%d_%d", 
		       dv_name[irow].Data(),iv_name[icol].Data(), icol,irow),
		  cuts,draw_opt);
	h_buff = (TH1D*)gDirectory->FindObject(Form("h%d_%d",icol,irow));
	h_buff->SetTitle("");
      }

      if(draw_opt=="prof"){
	gStyle->SetOptStat(0);
	h_buff->Fit("pol1","QR","",iv_mean[icol]-2*iv_rms[icol],iv_mean[icol]+2*iv_rms[icol]);
      }
      iv_txt[icol]->Draw("same");
    }

  }
  c1->SaveAs(output_dir+filename);

}


