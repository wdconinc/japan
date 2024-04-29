/**********************************************************\
* File: QwCombinedBCM.cc                                  *
*                                                         *
* Author:                                                 *
* Time-stamp:                                             *
\**********************************************************/

#include "QwCombinedBCM.h"

// System headersF
#include <stdexcept>

// Qweak headers
#ifdef __USE_DATABASE__
#include "QwDBInterface.h"
#endif // __USE_DATABASE__

//  Qweak types that we want to use in this template
#include "QwVQWK_Channel.h"
#include "QwADC18_Channel.h"
#include "QwScaler_Channel.h"
#include "QwMollerADC_Channel.h"


// Randomness generator: Mersenne twister with period 2^19937 - 1
//
// This is defined as static to avoid getting stuck with 100% correlated
// ADC channels when each channel goes through the same list of pseudo-
// random numbers...

template<typename T> std::mt19937 QwCombinedBCM<T>::fRandomnessGenerator;
template<typename T> std::random::uniform_real_distribution<double> QwCombinedBCM<T>::fDistribution;

// The boost::variate_generator has operator() overloaded to get a new random
// value according to the distribution in the second template argument, based
// on the uniform random value generated by the first template argument.
// For example: fNormalRandomVariable() will return a random normal variable.

template<typename T> boost::variate_generator < std::mt19937, std::uniform_real_distribution<double> >
  QwCombinedBCM<T>::fRandomVariable(fRandomnessGenerator, fDistribution);





/********************************************************/

//this is a combined BCM made out of BCMs that are already callibrated and have pedstals removed.
//This will be used for projection of charge at the target

template<typename T>
void QwCombinedBCM<T>::SetBCMForCombo(VQwBCM* bcm, Double_t weight, Double_t sumqw )
{
  // Convert back to QWBCM<T>* from generic VQwBCM*
  fElement.push_back(dynamic_cast<QwBCM<T>* >(bcm));
  fWeights.push_back(weight);
  fSumQweights = sumqw;
}

/********************************************************/

template<typename T>
void  QwCombinedBCM<T>::InitializeChannel(TString name, TString datatosave)
{
  SetPedestal(0.);
  SetCalibrationFactor(1.);
  fLastTripTime = -99999.9;
  this->SetElementName(name);
  this->fBeamCurrent.InitializeChannel(name,"derived");
}

template<typename T>
void  QwCombinedBCM<T>::InitializeChannel(TString subsystem, TString name, TString datatosave)
{
  SetPedestal(0.);
  SetCalibrationFactor(1.);
  fLastTripTime = -99999.9;
  this->SetElementName(name);
  this->fBeamCurrent.InitializeChannel(subsystem, "QwCombinedBCM", name,"derived");
}

template<typename T>
void  QwCombinedBCM<T>::InitializeChannel(TString subsystem, TString name,
    TString type, TString datatosave)
{
  SetPedestal(0.);
  SetCalibrationFactor(1.);
  fLastTripTime = -99999.9;
  this->SetElementName(name);
  this->SetModuleType(type);
  this->fBeamCurrent.InitializeChannel(subsystem, "QwCombinedBCM", name,"derived");
}


/********************************************************/
template<typename T>
void  QwCombinedBCM<T>::ProcessEvent()
{
  static T tmpADC;
  tmpADC.InitializeChannel("tmp","derived");

  this->ClearEventData();

  for (size_t i = 0; i < fElement.size(); i++) {
    tmpADC = fElement[i]->fBeamCurrent;
    tmpADC.Scale(fWeights[i]);
    this->fBeamCurrent += tmpADC;
  }
  this->fBeamCurrent.Scale(1.0/fSumQweights);

  Bool_t ldebug = kFALSE;
  if (ldebug) {
    QwMessage << "*****************" << QwLog::endl;
    QwMessage << "QwCombinedBCM: " << this->GetElementName() << QwLog::endl
	            << "weighted average of hardware sums = " << this->fBeamCurrent.GetValue() << QwLog::endl;
    if (this->fBeamCurrent.GetNumberOfSubelements() > 1) {
      for (size_t i = 0; i < 4; i++) {
        QwMessage << "weighted average of block[" << i << "] = " << this->fBeamCurrent.GetValue(i) << QwLog::endl;
      }
    }
    QwMessage << "*****************" << QwLog::endl;
  }
}

//---------------------------------------------------------------------------------------------------------


template<typename T>
void  QwCombinedBCM<T>::GetProjectedCharge(VQwBCM *device)
{
  (device->GetCharge())->AssignScaledValue(this->fBeamCurrent,1.0);
  //device->PrintInfo();
  device->ApplyResolutionSmearing();
  //device->PrintInfo();
  device->FillRawEventData();
}


template<typename T>
void  QwCombinedBCM<T>::RandomizeEventData(int helicity, double time)
{
  this->fBeamCurrent.RandomizeEventData(helicity, time);
  // do the beam cut routine from MockDataGenerator, but just scale fBeamCurrent instead of re-randomizing
  //  Three time intervals:  fmod(time,period) > (period-tripramp):  Do the ramp scaling
  //                         (period-tripramp) > fmod(time,period) > (period-triplength- tripramp): scale by 0.0
  //                         (period-triplength- tripramp) > fmod(time,period) : leave the variable alone


  //  Determine the probablity of having a beam trip
  if (fLastTripTime<=-99999.9) {
     fLastTripTime = time - fTripLength - fTripRamp;
  }
//  if ( ((time-fLastTripTime) > (fTripPeriod)) ){ //  Make some comparison to a probablilty instead of the time
//  Probablity of a trip happening in one event:  # of trips per hour/3600 * eventtime_in_seconds (which is 0.001s)
  Double_t tmp = fRandomVariable();
//  std::cout << "random value=="<<tmp << "; fProbabilityOfTrip=="<<fProbabilityOfTrip<<std::endl;
  if (tmp < fProbabilityOfTrip) {
     fLastTripTime = time;
     std::cout << "random value=="<<tmp << "; fProbabilityOfTrip=="<<fProbabilityOfTrip<<"; time=="<<time<<std::endl;
  }

  Double_t time_since_trip = time - fLastTripTime;
  if (time_since_trip <= fTripLength) {
    this->fBeamCurrent.Scale(0.0);
    //std::cout << "fBeamCurrent.Scale(0.0)" << std::endl;
  } else if (time_since_trip < (fTripLength + fTripRamp)) {
    Double_t factor = (time_since_trip-fTripLength)/fTripRamp;
    //std::cout << "(fTripPeriod - fmod(time, fTripPeriod))=="<<(fTripPeriod - fmod(time, fTripPeriod))<<"; fTripLength=="<<fTripLength
    //          << "; ((fTripPeriod - fmod(time, fTripPeriod)) / fTripLength)=="<<((fTripPeriod - fmod(time, fTripPeriod)) / fTripLength)
    //          << std::endl;
    this->fBeamCurrent.Scale(factor); 
    //std::cout << "fBeamCurrent.Scale(factor) = " << factor << std::endl;
  }
}


template<typename T>
void  QwCombinedBCM<T>::LoadMockDataParameters(QwParameterFile &paramfile){

  Double_t res=0.0;

  //  Test for the word "beamtrip", and get the tripperiod and tripduration if it is found, otherwise, just pass
  //  the line to the fBeamCurrent object.
  if (paramfile.GetLine().find("beamtrip")!=std::string::npos){
    //  "beamtrip" appears somewhere in the line.  Assume it is the next token and move on...
    paramfile.GetNextToken();  //Throw away this token.  Now the next three should be the beamtrip parameters.

    fTripPeriod = paramfile.GetTypedNextToken<Double_t>() * Qw::sec;
    fTripLength = paramfile.GetTypedNextToken<Double_t>() * Qw::sec;
    fTripRamp   = paramfile.GetTypedNextToken<Double_t>() * Qw::sec;

    fProbabilityOfTrip = 1.0/fTripPeriod;
  } else if (paramfile.GetLine().find("resolution")!=std::string::npos){
    paramfile.GetNextToken();
    res = paramfile.GetTypedNextToken<Double_t>();
    this->SetResolution(res);
  } else {
    //std::cout << "In QwCombinedBCM: ChannelName = " << this->GetElementName() << std::endl;
    this->fBeamCurrent.LoadMockDataParameters(paramfile);
  }
}

//---------------------------------------------------------------------------------------------------------


/********************************************************/
template<typename T>
Bool_t QwCombinedBCM<T>::ApplySingleEventCuts()
{
  
  //This is required to update single event cut faliures in individual channels
  //  First update the error code based on the codes
  //  of the elements.  This requires that the BCMs
  //  have had ApplySingleEventCuts run on them already.
  
  for (size_t i=0;i<fElement.size();i++){
    this->fBeamCurrent.UpdateErrorFlag(fElement.at(i)->fBeamCurrent.GetErrorCode());
  }
  

  //  Everything is identical as for a regular BCM
  return QwBCM<T>::ApplySingleEventCuts();
}

template<typename T>
UInt_t QwCombinedBCM<T>::UpdateErrorFlag(){
  for (size_t i=0;i<fElement.size();i++){
    this->fBeamCurrent.UpdateErrorFlag(fElement.at(i)->fBeamCurrent.GetErrorCode());
  }
  return this->fBeamCurrent.GetEventcutErrorFlag();
}


/********************************************************/
/*
template<typename T>
void QwCombinedBCM<T>::UpdateErrorFlag(const VQwBCM *ev_error){
    
  try {
    if(typeid(*ev_error)==typeid(*this)) {
      // std::cout<<" Here in QwCombinedBCM::UpdateErrorFlag \n";
      if (this->GetElementName()!="") {
        QwCombinedBCM<T>* value_bcm = dynamic_cast<QwCombinedBCM<T>* >(ev_error);
	VQwDataElement *value_data = dynamic_cast<VQwDataElement *>(&(value_bcm->fBeamCurrent));
	fBeamCurrent.UpdateErrorFlag(value_data->GetErrorCode());//the routine GetErrorCode() return the error flag + configuration flag unconditionally
      }
    } else {
      TString loc="Standard exception from QwCombinedBCM::UpdateErrorFlag :"+
        ev_error->GetElementName()+" "+this->GetElementName()+" are not of the "
        +"same type";
      throw std::invalid_argument(loc.Data());
    }
  } catch (std::exception& e) {
    std::cerr<< e.what()<<std::endl;
  }  
    
    QwBCM<T>::UpdateErrorFlag(const ev_error);
};
*/

/********************************************************/
/*
template<typename T>
void QwCombinedBCM<T>::CalculateRunningAverage(){
  fBeamCurrent.CalculateRunningAverage();
=======
  return eventokay;
>>>>>>> .merge-right.r3406
}
*/
/********************************************************/
/*
template<typename T>
<<<<<<< .working
void QwCombinedBCM<T>::AccumulateRunningSum(const VQwBCM &value){
  fBeamCurrent.AccumulateRunningSum(dynamic_cast<const QwCombinedBCM<T>* >(&value)->fBeamCurrent);
}
*/
/********************************************************/
/*
template<typename T>
void QwCombinedBCM<T>::DeaccumulateRunningSum(VQwBCM &value){
  fBeamCurrent.DeaccumulateRunningSum(dynamic_cast<QwCombinedBCM<T>* >(&value)->fBeamCurrent);
}
*/


/********************************************************/
template<typename T>
QwCombinedBCM<T>& QwCombinedBCM<T>::operator= (const QwCombinedBCM<T> &value)
{
  if (this->GetElementName()!="")
    this->fBeamCurrent=value.fBeamCurrent;

  return *this;
}

template<typename T>
VQwBCM& QwCombinedBCM<T>::operator= (const VQwBCM &value)
{
  if (this->GetElementName()!="")
    dynamic_cast<QwCombinedBCM<T>* >(this)->fBeamCurrent=
      dynamic_cast<const QwCombinedBCM<T>* >(&value)->fBeamCurrent;

  return *this;
}


/********************************************************/
/*
template<typename T>
Bool_t QwCombinedBCM<T>::ApplyHWChecks()
{
  // For the combined devices there are no physical channels that we can relate to because they  are being
  // derived from combinations of physical channels. Therefore, this is not exactly a "HW Check"
  // but just a check of the HW checks of the combined channels.

  Bool_t eventokay=kTRUE;

  return eventokay;
}
*/

/*
template<typename T>
Int_t QwCombinedBCM<T>::SetSingleEventCuts(Double_t LL=0, Double_t UL=0){
  fBeamCurrent.SetSingleEventCuts(LL,UL);
  return 1;
}


template<typename T>
void QwCombinedBCM<T>::SetSingleEventCuts(UInt_t errorflag, Double_t LL=0, Double_t UL=0, Double_t stability=0){
  //set the unique tag to identify device type (bcm,bpm & etc)
  errorflag|=kBCMErrorFlag;//currently I use the same flag for bcm & combinedbcm
  QwMessage<<"QwCombinedBCM Error Code passing to QwVQWK_Ch "<<errorflag<<" "<<stability<<QwLog::endl;
  fBeamCurrent.SetSingleEventCuts(errorflag,LL,UL,stability);

}


template<typename T>
void  QwCombinedBCM<T>::ConstructHistograms(TDirectory *folder, TString &prefix)
{
  if (this->GetElementName()=="")
    {
      //  This channel is not used, so skip filling the histograms.
    }
  else
    {
      fBeamCurrent.ConstructHistograms(folder, prefix);
    }
  return;

}

template<typename T>
void  QwCombinedBCM<T>::FillHistograms()
{
 if (this->GetElementName()=="")
    {
      //  This channel is not used, so skip filling the histograms.
    }
  else
    {
      fBeamCurrent.FillHistograms();
    }


  return;
}

template<typename T>
void  QwCombinedBCM<T>::ConstructBranchAndVector(TTree *tree, TString &prefix, std::vector<Double_t> &values)
{
  if (this->GetElementName()==""){
    //  This channel is not used, so skip filling the histograms.
  } else
    {
      fBeamCurrent.ConstructBranchAndVector(tree,prefix,values);
    }
  return;
}

template<typename T>
void  QwCombinedBCM<T>::ConstructBranch(TTree *tree, TString &prefix)
{
  if (this->GetElementName()==""){
    //  This channel is not used, so skip filling the histograms.
  } else
    {
      fBeamCurrent.ConstructBranch(tree,prefix);
    }
  return;
}

template<typename T>
void  QwCombinedBCM<T>::ConstructBranch(TTree *tree, TString &prefix, QwParameterFile& modulelist)
{
  TString devicename;
  devicename=this->GetElementName();
  devicename.ToLower();

  if (this->GetElementName()==""){
    //  This channel is not used, so skip filling the histograms.
  } else
    {
      if (modulelist.HasValue(devicename)){
	fBeamCurrent.ConstructBranch(tree,prefix);
	QwMessage <<" Tree leave added to "<<devicename<<QwLog::endl;
      }
    }
  return;
}


template<typename T>
void  QwCombinedBCM<T>::FillTreeVector(std::vector<Double_t> &values) const
{
  if (this->GetElementName()==""){
    //  This channel is not used, so skip filling the histograms.
  } else
    {
      fBeamCurrent.FillTreeVector(values);
    }
  return;
}

*/

/********************************************************/
template class QwCombinedBCM<QwVQWK_Channel>;
template class QwCombinedBCM<QwADC18_Channel>;
template class QwCombinedBCM<QwSIS3801_Channel>;
template class QwCombinedBCM<QwSIS3801D24_Channel>;
template class QwCombinedBCM<QwMollerADC_Channel>;
