// author : Tao Ye
// date : 05-2019
// description: A script for error code plots 

#include "DeviceErrorCounter.C"
#include "ErrorFlagDecoder.C"
#include "device_list.h"

void PlotErrorCounters(){
  TString plot_title;

  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  TCanvas* c2 = new TCanvas("c2","c2",1200,800);
  c2->Divide(2,2);

  c1->cd();
  ErrorFlagDecoder();
  plot_title = Form("run%d_ErrorCounter_global.png",run_number);
  c1->SaveAs(output_path+ plot_title);

  TString sam_array[8]={"sam1","sam2","sam3","sam4",
			"sam5","sam6","sam7","sam8"};
  TString bcm_array[4]={"bcm_an_us","bcm_an_ds",
			"bcm_an_ds3","bcm_an_ds10"};
  TString bpm4a_array[4]={"bpm4aXM","bpm4aXP","bpm4aYP","bpm4aYM"};
  TString bpm4e_array[4]={"bpm4eXM","bpm4eXP","bpm4eYP","bpm4eYM"};
  TString bpm12_array[4]={"bpm12XM","bpm12XP","bpm12YP","bpm12YM"};
  TString bpm14_array[4]={"bpm14XM","bpm14XP","bpm14YP","bpm14YM"};
  TString bpm18_array[4]={"bpm18XM","bpm18XP","bpm18YP","bpm18YM"};
  
  TString* channels[8]={sam_array,sam_array+4,
			bcm_array,
			bpm4a_array,bpm4e_array,
			bpm12_array,bpm14_array,
			bpm18_array};

  for(int ich=0;ich<8;ich++){
    for(int i=0;i<4;i++){
      c2->cd(i+1);
      DeviceErrorCounter(channels[ich][i]);
    }
    plot_title = Form("run%d_ErrorCounter_%d.png",run_number,ich);
    c2->SaveAs(output_path+ plot_title);
  }

  TString pdf_filename = Form("run%d_ErrorCounters.pdf",run_number);
  gSystem->Exec(Form("convert $(ls -rt %srun%d*ErrorCounter*.png) %s%s",
		     output_path.Data(),
		     run_number,
		     output_path.Data(),
		     pdf_filename.Data()));
  gSystem->Exec(Form("rm %srun%d*ErrorCounter*.png",
		     output_path.Data(),
		     run_number)); 
}
