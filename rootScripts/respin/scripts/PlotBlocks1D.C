void PlotBlocks1D(TString strDeviceName,TString treeName){

  const char* device_name = strDeviceName.Data(); 
  TString draw_opt = "COLZ";
  TTree *tree = (TTree*)gROOT->FindObject(treeName);

  TPad* pad1 = new TPad("pad1","pad1",0.0,0.0,1.0,1.0);
  pad1->Divide(4,1);
  pad1->Draw();

  const char *leaf[]={"block0","block1","block2","block3"};

  TVirtualPad* pad_buff;
  TH1* h_buff;
  TString draw_cmd;

  TPaveText *ptext_buff;

  gStyle->SetStatW(.3);
  gStyle->SetStatH(.4);

  for(int ileaf=0;ileaf<4;ileaf++){
    pad_buff = pad1->cd(ileaf+1);
    draw_cmd = Form("%s.%s",strDeviceName.Data(),leaf[ileaf]);
    tree->Draw(draw_cmd.Data(),"ErrorFlag==0");	
    h_buff = (TH1D*)pad_buff->FindObject("htemp");
    h_buff->SetTitle( Form(" %sTree , %s :%s",
			   treeName.Data(),strDeviceName.Data(),
			   leaf[ileaf]));
  }

}
