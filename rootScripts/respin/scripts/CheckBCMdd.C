void CheckBCMdd(TString bcm1 , TString bcm2, TString treeName){

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
