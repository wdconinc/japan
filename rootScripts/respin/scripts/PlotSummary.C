#include "CheckEventCut.C"
#include "CheckDeviceEventCut.C"
#include "PlotBlocks.C"
#include "PlotBlocks1D.C"
#include "PlotInjBPMS.C"
#include "PlotInjBPMSDr.C"
#include "CheckBCMdd.C"
#include "device_list.h"

void PlotSummary(Int_t run_number){
  TString path = "./japanOutput/";
  TString prefix = "prexRespin1";
  TString file_name = Form("%s_%d.root",prefix.Data(),run_number);
  TFile *rootfile = TFile::Open(path+file_name);

  TString output_path = Form("./summary/run%d/",run_number);
  TString plot_title;

  Int_t nbcm = vBCM.size();
  Int_t nbpm = vBPM.size();
  Int_t nsam = vSAM.size();
  Int_t nblkm = vBlkM.size();

  vector< TString > mulSAM;
  vector< TString > mulBPM;
  vector< TString > mulBCM;
  vector< TString > mulBlkM;
  vector<TString>::iterator it_sam = vSAM.begin();
  vector<TString>::iterator it_bpm = vBPM.begin();
  vector<TString>::iterator it_bcm = vBCM.begin();
  vector<TString>::iterator it_blkm = vBlkM.begin();
  // Concatenation for branch name in pattern tree 
  while(it_blkm!=vBlkM.end()){
    mulBlkM.push_back("asym_"+(*it_blkm));
    it_blkm++;
  }
  while(it_bcm!=vBCM.end()){
    mulBCM.push_back("asym_"+(*it_bcm));
    it_bcm++;
  }
  while(it_sam!=vSAM.end()){
    mulSAM.push_back("asym_"+(*it_sam));
    it_sam++;
  }
  while(it_bpm!=vBPM.end()){
    mulBPM.push_back("diff_"+(*it_bpm+"X"));
    mulBPM.push_back("diff_"+(*it_bpm+"Y"));
    it_bpm++;
  }

  TCanvas *c1 = new TCanvas("c1","c1",2400,600);
  TCanvas *c2 = new TCanvas("c2","c2",1600,1600);
  TCanvas *c3 = new TCanvas("c3","c3",1800,600);
  TCanvas *c4 = new TCanvas("c4","c4",2400,1200);

  TString draw_opts[] = {"COLZ","prof","scat"};
  TString treeNames[] = {"pr","mul"};

  TString treeName;
  
  //===== Event Cuts Plots =====   
  for(int ibcm=0;ibcm<nbcm;ibcm++){
    c1->cd();
    CheckEventCut(vBCM[ibcm]);
    plot_title = Form("run%d_check_EventCut_%s.png",run_number,vBCM[ibcm].Data());
    c1->SaveAs(output_path+ plot_title);
  } // end of BCM loop

  TString bpm_suffix[3]={"X","Y","WS"};
  TString pattern_prefix[3]={"diff_","diff_","asym_"};
  TString bpm_channel;
  for(int ibpm=0;ibpm<nbpm;ibpm++){
    for(int ichannel =0;ichannel<3;ichannel++){
      c1->cd();
      bpm_channel = vBPM[ibpm]+ bpm_suffix[ichannel];
      CheckDeviceEventCut(bpm_channel,pattern_prefix[ichannel]);
      plot_title = Form("run%d_check_EventCut_%s.png",
			run_number,bpm_channel.Data());
      c1->SaveAs(output_path+ plot_title);
    }
  } // end of BPM loop

  for(int isam=0;isam<nsam;isam++){
    c1->cd();
    CheckDeviceEventCut(vSAM[isam],"asym_");
    plot_title = Form("run%d_check_EventCut_%s.png",run_number,vSAM[isam].Data());
    c1->SaveAs(output_path+ plot_title);
  } // end of lumi loop

  gSystem->Exec(Form("convert %s*check_EventCut*.png %srun%d_eventcut.pdf",
		     output_path.Data(),output_path.Data(),run_number));
 
  gSystem->Exec(Form("rm %s*check_EventCut*.png",output_path.Data()));

  //===== BCM Double Difference Plots =====   
  c3->cd();
  for(int iTree = 0; iTree<2 ; iTree++){
    treeName= treeNames[iTree];
    for(int ibcm=0; ibcm<nbcm;ibcm++){
      for(int jbcm=0;jbcm<nbcm;jbcm++){
	if(jbcm<=ibcm)
	  continue;
	c3->Clear("D");
	CheckBCMdd(mulBCM[ibcm],mulBCM[jbcm],treeName);

	plot_title = Form("run%d_%s_vs_%s_DD_%sTree.png",
			  run_number,vBCM[ibcm].Data(),vBCM[jbcm].Data(),treeName.Data());
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
  //===== SubBlocks Plots =====   
  ////========== 2D plots
  for(int iTree = 0; iTree<2 ; iTree++){
    treeName= treeNames[iTree];
    for(int idev=0;idev<nblkm;idev++){
      c2->Clear("D");
      c2->cd();
      PlotBlocks(vBlkM[idev],treeName);
      plot_title = Form("run%d_%s_SubBlocks_%sTree.png",
			run_number,vBlkM[idev].Data(),
			treeName.Data());
      TText *t1 = new TText(0.0,0.007,plot_title);
      t1->SetNDC();
      t1->SetTextSize(0.02);
      c2->cd();
      t1->Draw("same");
      c2->SaveAs(output_path+plot_title);
    } // end of device loop

    ////========== 1D Blocks plots
    for(int idev=0;idev<nblkm;idev++){
      c1->Clear("D");
      c1->cd();
      PlotBlocks1D(vBlkM[idev],treeName);
      plot_title = Form("run%d_%s_SubBlocks1D_%sTree.png",
			run_number,vBlkM[idev].Data(),
			treeName.Data());
      TText *t1 = new TText(0.0,0.007,plot_title);
      t1->SetNDC();
      t1->SetTextSize(0.02);
      c1->cd();
      t1->Draw("same");
      c1->SaveAs(output_path+plot_title);
    } // end of device loop

    TString pdf_filename = Form("run%d_SubBlocks_%sTree.pdf",run_number,treeName.Data());
    gSystem->Exec(Form("convert %srun%d*SubBlock*.png %s%s",
		       output_path.Data(),
		       run_number,
		       output_path.Data(),
		       pdf_filename.Data()));
    gSystem->Exec(Form("rm %srun%d*SubBlock*.png",
		       output_path.Data(),
		       run_number)); 
  }

  // ===== Correlation plots for SAM, BPM before regression
  vector < vector<TString> > Vdv={ mulSAM,mulSAM,mulBPM};
  vector < vector<TString> > Viv={ mulSAM,mulBPM,mulBPM};
  vector<TString> vtag_dv ={"asym_sam","asym_sam","diff_bpm" };
  vector<TString> vtag_iv ={"asym_sam","diff_bpm","diff_bpm" };
  Int_t canvas_width = 600;
  for(int itree=0;itree<2;itree++){
    for(int iplot=0;iplot<3;iplot++){
      for(int iopt=0;iopt<3;iopt++){
	int ny = Vdv[iplot].size();
	int nx = Viv[iplot].size();
	TCanvas* c_this = new TCanvas("","",(nx+1)*canvas_width,ny*canvas_width);
	c_this->cd();
	PlotCorrelation(Vdv[iplot],Viv[iplot],treeNames[itree],draw_opts[iopt]);

	plot_title  = Form("run%d_%s_vs_%s-%s-%sTree.png",
			   run_number,vtag_dv[iplot].Data(),vtag_iv[iplot].Data(),
			   draw_opts[iopt].Data(),treeNames[itree].Data() );
	TText *label = new TText(0.0,0.005,plot_title);     
	label->SetTextFont(23);
	label->SetTextSize(70);
	label->SetNDC();
	c_this->cd();
	label->Draw("same");
	c_this->SaveAs(output_path+plot_title);
      } // end of draw_opts loop
      gSystem->Exec(Form("convert %s/*%sTree.png %s/run%d_%s_vs_%s_%sTree.pdf",
			 output_path.Data(),treeNames[itree].Data(),
			 output_path.Data(),
			 run_number,vtag_dv[iplot].Data(),vtag_iv[iplot].Data(),
			 treeNames[itree].Data()));
      gSystem->Exec(Form("rm %s/*_vs_*.png",output_path.Data()));
    } // end of plots loop
  } // end of tree loop

  //==== Injector BPMs Dx Dy ====
  c4->cd();
  PlotInjBPMS(run_number,vInjBPM,1,"ErrorFlag==0");
  plot_title = Form("run%d_injector_BPM_dxdy.png",run_number);
  c4->SaveAs(output_path+plot_title);

  c1->cd();
  PlotInjBPMSDr(run_number,vIngBPM,1,"ErrorFlag==0");  
  plot_title = Form("run%d_injector_BPM_dr_elli.png",run_number);
  c1->SaveAs(output_path+plot_title);

  TString pdf_filename = Form("run%d_injector_BPM_diff.pdf",run_number);
  gSystem->Exec(Form("convert %srun%d*injector_BPM*.png %s%s",
		     output_path.Data(),
		     run_number,
		     output_path.Data(),
		     pdf_filename.Data()));
  gSystem->Exec(Form("rm %srun%d*injector_BPM*.png",
		     output_path.Data(),
		     run_number)); 
}

