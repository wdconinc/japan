/**********************************************************\
* File: QwDataHandlerArray.h                           *
*                                                          *
* Author: P. M. King                                       *
* Time-stamp: <2009-02-04 10:30>                           *
\**********************************************************/

#ifndef __QWDATAHANDLERARRAY__
#define __QWDATAHANDLERARRAY__

#include <vector>
#include <map>
#include "Rtypes.h"
#include "TString.h"
#include "TDirectory.h"
#include <TTree.h>

#include <boost/shared_ptr.hpp>
#include <boost/mem_fn.hpp>

#include "QwDataHandlerArrayEvt.h"
#include "VQwDataHandler.h"
#include "QwOptions.h"
#include "QwBeamMod.h"

// Forward declarations
class QwParityDB;
class QwPromptSummary;

/**
 * \class QwDataHandlerArray
 * \ingroup QwAnalysis
 *
 * \brief Virtual base class for the parity handlers
 *
 *   Virtual base class for the classes containing the
 *   event-based information from each parity handler.
 *   This will define the interfaces used in communicating
 *   with the CODA routines.
 *
 */
class QwDataHandlerArrayEvt:  public std::vector<boost::shared_ptr<VQwDataHandler> > {
 private:
  typedef std::vector<boost::shared_ptr<VQwDataHandler> >  HandlerPtrs;
 public:
  using HandlerPtrs::const_iterator;
  using HandlerPtrs::iterator;
  using HandlerPtrs::begin;
  using HandlerPtrs::end;
  using HandlerPtrs::size;
  using HandlerPtrs::empty;

  private:
    /// Private default constructor
    QwDataHandlerArrayEvt(); // not implement, will thrown linker error on use

  public:
    /// Constructor with options
    QwDataHandlerArrayEvt(QwOptions& options, QwBeamMod& BeamMod, const TString &run);
    /// Copy constructor by reference
    QwDataHandlerArrayEvt(const QwDataHandlerArrayEvt& source);
    /// Default destructor
    virtual ~QwDataHandlerArrayEvt();

  /// \brief Define configuration options for global array
  static void DefineOptions(QwOptions &options);
  /// \brief Process configuration options for the datahandler array itself
  void ProcessOptions(QwOptions &options);

  void LoadDataHandlersFromParameterFile(QwParameterFile& detectors, QwBeamMod& BeamMod, const TString &run);

  /// \brief Add the datahandler to this array
  void push_back(VQwDataHandler* handler);
  void push_back(boost::shared_ptr<VQwDataHandler> handler);

    /// \brief Get the handler with the specified name
    VQwDataHandler* GetDataHandlerByName(const TString& name);

    std::vector<VQwDataHandler*> GetDataHandlerByType(const std::string& type);

    void ConstructTreeBranches(QwRootFile *treerootfile);

    /// \brief Construct a branch and vector for this handler with a prefix
    void ConstructBranchAndVector(TTree *tree, TString& prefix, std::vector <Double_t> &values);

    void FillTreeBranches(QwRootFile *treerootfile);
    /// \brief Fill the vector for this handler
    void FillTreeVector(std::vector<Double_t>& values) const;
    /// \brief Fill the histograms for this handler
    void FillHistograms();

    /// \brief Fill the database
    void FillDB(QwParityDB *db, TString type);
    //    void FillErrDB(QwParityDB *db, TString type);
    //    const QwDataHandlerArray *dummy_source;

    void  ClearEventData();
    void  ProcessEvent();

    /// \brief Assignment operator
    QwDataHandlerArrayEvt& operator=  (const QwDataHandlerArrayEvt &value);
    /*
    /// \brief Addition-assignment operator
    QwDataHandlerArrayEvt& operator+= (const QwDataHandlerArray &value);
    /// \brief Subtraction-assignment operator
    QwDataHandlerArrayEvt& operator-= (const QwDataHandlerArray &value);
    /// \brief Sum of two handler arrays
    void Sum(const QwDataHandlerArray &value1, const QwDataHandlerArray &value2);
    /// \brief Difference of two handler arrays
    void Difference(const QwDataHandlerArray &value1, const QwDataHandlerArray &value2);
    /// \brief Scale this handler array
    void Scale(Double_t factor);
    */

    /// \brief Update the running sums for devices accumulated for the global error non-zero events/patterns
    void AccumulateRunningSum();

    /// \brief Update the running sums for devices accumulated for the global error non-zero events/patterns
    void AccumulateRunningSum(const QwDataHandlerArrayEvt& value);
    /// \brief Update the running sums for devices check only the error flags at the channel level. Only used for stability checks
    void AccumulateAllRunningSum(const QwDataHandlerArrayEvt& value);

    /// \brief Calculate the average for all good events
    void CalculateRunningAverage();

    /// \brief Report the number of events failed due to HW and event cut failures
    void PrintErrorCounters() const;

    /// \brief Print value of all channels
    void PrintValue() const;

    
    void WritePromptSummary(QwPromptSummary *ps, TString type);
    
    
    void ProcessDataHandlerEntry();

    void FinishDataHandler();
  protected:
  /// Filename of the global detector map
  std::string fDataHandlersMapFile;

  /// Pointer for the original data source
  QwBeamMod *fDataSource;

  std::vector<std::string> fDataHandlersDisabledByName; ///< List of disabled types
  std::vector<std::string> fDataHandlersDisabledByType; ///< List of disabled names

  Bool_t fPrintRunningSum;

    /// Test whether this handler array can contain a particular handler
    static Bool_t CanContain(VQwDataHandler* handler) {
      return (dynamic_cast<VQwDataHandler*>(handler) != 0);
    };

}; // class QwDataHandlerArray

#endif // __QWDATAHANDLERARRAY__