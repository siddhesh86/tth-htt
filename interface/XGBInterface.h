#ifndef tthAnalysis_HiggsToTauTau_XGBInterface_h
#define tthAnalysis_HiggsToTauTau_XGBInterface_h

#include "tthAnalysis/HiggsToTauTau/interface/KeyTypes.h" // RUN_TYPE, LUMI_TYPE, EVT_TYPE
#include <Python.h>
#include <vector>
#include <string>
#include <map>

class XGBInterface
{
 public:
  XGBInterface(const std::string& mvaFileName, const std::vector<std::string>& mvaInputVariables);
  ~XGBInterface();

  /**
   * @brief Calculates MVA output.
   * @param mvaInputs Values of MVA input variables (stored in std::map with key = MVA input variable name)
   * @return          MVA output
   */

  double
  operator()(const std::map<std::string, double>& mvaInputs) const;

 private:
  std::string mvaFileName_;

  std::vector<std::string> mvaInputVariables_; // list of MVA input variables
  PyObject* pkldata_;
  PyObject* moduleMainString_;
  PyObject* moduleMain_;
};

#endif // XGBInterface_h