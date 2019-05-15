// author: Tao Ye
// date: 05-2019
// description: Macro for checking device error event cuts
void CheckDeviceEventCut(const char* device_name, const char* pattern_prefix);
void CheckDeviceEventCut();


void CheckDeviceEventCut(){

  Int_t nbpm = vBPM.size();
  Int_t nsam = vSAM.size();

  TCanvas *c1 = new TCanvas("c1","c1",2400,600);
  TString bpm_suffix[3]={"X","Y","WS"};
  const char* pattern_prefix[3]={"diff_","diff_","asym_"};
  TString bpm_channel;
  for(int ibpm=0;ibpm<nbpm;ibpm++){
    for(int ichannel =0;ichannel<3;ichannel++){
      c1->cd();
      bpm_channel = TString(vBPM[ibpm])+ bpm_suffix[ichannel];
      CheckDeviceEventCut(bpm_channel.Data(),
			  pattern_prefix[ichannel]);
      plot_title = Form("run%d_check_EventCut_%s.png",
			run_number,bpm_channel.Data());
      c1->SaveAs(output_path+ plot_title);
    }
  } // end of BPM loop

  for(int isam=0;isam<nsam;isam++){
    c1->cd();
    CheckDeviceEventCut(vSAM[isam],"asym_");
    plot_title = Form("run%d_check_EventCut_%s.png",run_number,vSAM[isam]);
    c1->SaveAs(output_path+ plot_title);
  } // end of lumi loop
}

void CheckDeviceEventCut(const char* device_name, const char* pattern_prefix){
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

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
