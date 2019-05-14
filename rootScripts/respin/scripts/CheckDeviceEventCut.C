void CheckDeviceEventCut(TString strDeviceName, TString strPatternPrefix){
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  const char* device_name = strDeviceName.Data();
  const char* pattern_prefix = strPatternPrefix.Data();

  TTree *evt = (TTree*)gROOT->FindObject("evt");
  TTree *mul = (TTree*)gROOT->FindObject("mul");
  TTree *pr = (TTree*)gROOT->FindObject("pr");

  TString evtcut = Form("ErrorFlag==0");
  TString mulcut = Form("ErrorFlag==0");

  TPad *pad1 = new TPad("pad1","pad1",0.0,0.0,1.0,1.0);
  pad1->Draw();
  pad1->Divide(4,1);
  TH1D *h_buff;
  TVirtualPad* pad_buff;
  TGraph* g_buff;

  pad1->cd(1);
  evt->Draw(Form("%s:Entry$",device_name),evtcut);
  pad_buff=pad1->cd(2);
  evt->Draw(Form("%s",device_name),evtcut);
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  h_buff->SetName("EvtTree");

  evt->Draw(Form("%s",device_name),
	    evtcut+Form("&& %s.Device_Error_Code!=0",device_name),
	    "same"); 
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  if(h_buff!=0)
    h_buff->SetLineColor(kRed);

  pad_buff=pad1->cd(3);
  mul->Draw(Form("%s%s",pattern_prefix,device_name),mulcut);
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  h_buff->SetName("MulTree");

  mul->Draw(Form("%s%s",pattern_prefix,device_name),
	    mulcut+Form("&& %s%s.Device_Error_Code!=0",pattern_prefix,device_name),
	    "same");
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  if(h_buff!=0)
    h_buff->SetLineColor(kRed);

  pad_buff= pad1->cd(4);
  pr->Draw(Form("%s%s",pattern_prefix,device_name),mulcut);
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  h_buff->SetName("PairTree");

  pr->Draw(Form("%s%s",pattern_prefix,device_name),
	   mulcut+Form("&& %s%s.Device_Error_Code!=0",pattern_prefix,device_name),
	   "same");
  h_buff = (TH1D*)pad_buff->FindObject("htemp");
  if(h_buff!=0)
    h_buff->SetLineColor(kRed);

}
