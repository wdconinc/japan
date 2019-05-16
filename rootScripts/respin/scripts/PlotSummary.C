//////////////////////////////////////////////////////////////////////
//
// PlotSummary.C
//   Tao Ye - May 2019
//
//////////////////////////////////////////////////////////////////////

#include "CheckEventCut.C"
#include "CheckDeviceEventCut.C"
#include "PlotBlocks.C"
#include "PlotBlocks1D.C"
#include "PlotInjBPMS.C"
#include "PlotInjBPMSDr.C"
#include "CheckBCMdd.C"
#include "PlotCorrelation.C"
#include "Integrated.C"
#include "PlotErrorCounters.C"
#include "device_list.h"

void PlotSummary(Int_t run_num){
  run_number = run_num;
  path = "./japanOutput/";
  prefix = "prexRespin1";
  file_name = Form("%s_%d.root",prefix.Data(),run_number);
  TFile* rootfile = TFile::Open(path+file_name);
  output_path = Form("./summary_plots/run%d/",run_number);
  TString pdf_filename;
  //===== Error Counter from Evt Tree =====   
  PlotErrorCounters();
  //===== Event Cuts Plots =====   
  CheckEventCut();
  CheckDeviceEventCut();
  gSystem->Exec(Form("convert %s*check_EventCut*.png %srun%d_eventcut.pdf",
		     output_path.Data(),output_path.Data(),run_number));
  gSystem->Exec(Form("rm %s*check_EventCut*.png",output_path.Data()));

  //===== BCM Double Difference Plots =====   
  CheckBCMdd();
  //===== SubBlocks Plots =====   
  PlotBlocks1D();
  PlotBlocks();
  // ===== Integrated Convergence 
  Integrated();
  // ===== Correlation plots for SAM, BPM before regression
  PlotCorrelation();
  // ===== Regression Plots from JAPAN - (To-do)

  //==== Injector BPMs Dx Dy Dr and Ellipticity ====
  PlotInjBPMS();
  PlotInjBPMSDr();
  pdf_filename = Form("run%d_injector_BPM_diff.pdf",run_number);
  gSystem->Exec(Form("convert %srun%d*injector_BPM*.png %s%s",
		     output_path.Data(),
		     run_number,
		     output_path.Data(),
		     pdf_filename.Data()));

  gSystem->Exec(Form("rm %srun%d*injector_BPM*.png",
		     output_path.Data(),
		     run_number)); 

  // gSystem->Exec(Form("pdfunite $(ls -rt %srun%d*.pdf) %srun%d_all.pdf",
  // 		     output_path.Data(),
  // 		     run_number,
  // 		     output_path.Data(),
  // 		     run_number));

}

