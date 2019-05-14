void PlotCorrelation(vector<TString> DVar, vector<TString> IVar,
		     TString treeName, TString draw_opt){
  gStyle->SetStatW(0.2);
  gStyle->SetStatH(0.2);
  gStyle->SetStatX(1);
  gStyle->SetStatY(1);
  
  if(draw_opt=="prof"){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
  }

  TTree *tree = (TTree*)gROOT ->FindObject(treeName);
  TString cuts = "ErrorFlag==0";

  Int_t nDVar= DVar.size();
  Int_t nIVar= IVar.size();
  
  TPad* pad1 = new TPad("pad1","pad1",0,0,1,1);
  pad1->Divide(nIVar+1, nDVar);
  pad1->Draw();
  TVirtualPad *pad_buff;
  TH1D* h_buff;

  vector<double> iv_mean;
  vector<double> iv_rms;
  vector<double> dv_mean;
  vector<double> dv_rms;

  for(int i=0;i<nDVar;i++){
    tree->Draw(Form("%s>>hdv%d",DVar[i].Data(),i),cuts,"goff");
    TH1D *hbuff = (TH1D*)gDirectory->FindObject(Form("hdv%d",i));
    dv_mean.push_back( hbuff->GetMean());
    dv_rms.push_back( hbuff->GetRMS() );
  }

  for(int i=0;i<nIVar;i++){
    tree->Draw(Form("%s>>hiv%d",IVar[i].Data(),i),cuts,"goff");
    TH1D *hbuff = (TH1D*)gDirectory->FindObject(Form("hiv%d",i));
    iv_mean.push_back( hbuff->GetMean());
    iv_rms.push_back( hbuff->GetRMS());
  }

  vector<TText * > dv_txt;
  vector<TText * > iv_txt;

  for(int i=0;i<nDVar;i++){
    TText *txt = new TText(0.0,0.4, DVar[i]);
    txt->SetTextFont(22);
    txt->SetTextSize(0.15);
    dv_txt.push_back(txt);
  }

  for(int i=0;i<nIVar;i++){
    TText *txt = new TText(0.0,0.90, IVar[i]);
    txt->SetTextFont(22);
    txt->SetTextSize(0.15);
    txt->SetNDC();
    iv_txt.push_back(txt);
  }

  for(int irow=0;irow<nDVar; irow++){
    pad1->cd(1 + irow*(nIVar+1));
    dv_txt[irow]->Draw();
    for(int icol=0; icol< nIVar; icol++){
      pad_buff =pad1->cd(icol+2+irow*(nIVar+1));
      if(draw_opt=="scat"){
	tree->Draw(Form("%s:%s",DVar[irow].Data(),IVar[icol].Data()), cuts);
	h_buff = (TH1D*)pad_buff->FindObject("htemp");
	h_buff->SetTitle("");
      }
      else{
	tree->Draw(Form("%s:%s",DVar[irow].Data(),IVar[icol].Data()),
		   cuts,draw_opt);
	h_buff = (TH1D*)pad_buff->FindObject("htemp");
	h_buff->SetTitle("");
      }

      if(draw_opt=="prof"){
	h_buff->Fit("pol1","QR","",iv_mean[icol]-2*iv_rms[icol],iv_mean[icol]+2*iv_rms[icol]);
      }
      iv_txt[icol]->Draw("same");
    }
  }
}
