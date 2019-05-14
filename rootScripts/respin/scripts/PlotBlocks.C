void PlotBlocks(TString strDeviceName,TString treeName){
  
  TString draw_opt = "COLZ";
  TTree *tree = (TTree*)gROOT->FindObject(treeName);

  TPad* pad1 = new TPad("pad1","pad1",0.0,0.0,1.0,1.0);
  pad1->Divide(4,4);
  pad1->Draw();

  const char *leaf[]={"block0","block1","block2","block3"};

  TVirtualPad* pad_buff;
  TH1* h_buff;
  TString draw_cmd;
  TPaveText *ptext_buff;
  
  for(int ileaf=0;ileaf<4;ileaf++){
    for(int jleaf=0;jleaf<4;jleaf++){
      pad_buff = pad1->cd(jleaf+1+4*ileaf);
      if(ileaf==jleaf)
	continue;

      draw_cmd = Form("%s.%s:%s.%s",
		      strDeviceName.Data(),leaf[ileaf],
		      strDeviceName.Data(),leaf[jleaf]);
      tree->Draw(draw_cmd,"ErrorFlag==0",draw_opt);
      h_buff = (TH2D*)pad_buff->FindObject("htemp");
      h_buff->SetTitle( Form(" %sTree , %s :%s vs %s",
      			     treeName.Data(),strDeviceName.Data(),
      			     leaf[ileaf],leaf[jleaf]));
    } // end of column loop
  } // end of row loop

}
