#ifndef _DEVICE_LIST_H_
#define _DEVICE_LIST_H_

vector<TString> vBCM={"bcm_an_us", "bcm_an_ds", "bcm_an_ds3", "bcm_dg_us","bcm_dg_ds"};
vector<TString> vBPM={"bpm4a","bpm4e","bpm14","bpm18"};
vector<TString> vCavBPM={"cav4b","cav4c","cav4d"};
vector<TString> vSAM={"sam2","sam4","sam6","sam8"};

vector<TString> vBlkM = {"bcm_an_ds3","sam2","sam4"};  // Block Monitors


vector<TString> IV = {"diff_bpm4aX","diff_bpm4aY",
		      "diff_bpm4eX","diff_bpm4eY", "diff_bpm14X"};

vector<TString> DV_reg={"reg_asym_sam2","reg_asym_sam4",
			"reg_asym_sam6","reg_asym_sam8"};

vector<TString> DV_raw={"asym_sam2","asym_sam4",
			"asym_sam6","asym_sam8"};


//make matrix to hold data points, injector daq can only hold 22 bpms max. 
vector<TString> vInjBPM = {"bpm2i01","bpm2i02","bpm1i02","bpm1i04",
			   "bpm1i06", "bpm0i01","bpm0i01a","bpm0i02",
			   "bpm0i02a","bpm0i05","bpm0i07","bpm0l01",
			   "bpm0l02","bpm0l03","bpm0l04","bpm0l05",
			   "bpm0l06","bpm0l07","bpm0l10", "bpm0r02",
			   "bpm0r05"};
  
/* vector<TString> detnams = {"bpm2i01","bpm2i02","bpm1i02","bpm1i04", */
/* 			 "bpm1i06","bpm0i01","bpm0i01a","bpm0i02", */
/* 			 "bpm0i02a","bpm0i05", "bpm0i07","bpm0l01", */
/* 			 "bpm0l02","bpm0l03","bpm0l04","bpm0l05", */
/* 			 "bpm0l06","bpm0l07","bpm0l08","bpm0l09", */
/* 			 "bpm0l10","bpm0r03","bpm0r05"}; */


#endif

