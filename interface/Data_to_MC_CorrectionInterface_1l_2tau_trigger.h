#ifndef tthAnalysis_HiggsToTauTau_Data_to_MC_CorrectionInterface_1l_2tau_trigger_h
#define tthAnalysis_HiggsToTauTau_Data_to_MC_CorrectionInterface_1l_2tau_trigger_h

#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet

#include "tthAnalysis/HiggsToTauTau/interface/lutAuxFunctions.h" // lutWrapperBase, vLutWrapperBase

#include <string>
#include <map>
#include <assert.h> // assert

class Data_to_MC_CorrectionInterface_1l_2tau_trigger
{
 public:
  Data_to_MC_CorrectionInterface_1l_2tau_trigger(const edm::ParameterSet& cfg);
  ~Data_to_MC_CorrectionInterface_1l_2tau_trigger();

  //-----------------------------------------------------------------------------
  // set HLT trigger bits
  // (to be called once per event, before calling any of the getSF.. functions)
  void setTriggerBits(bool isTriggered_1e, bool isTriggered_1e1tau, bool isTriggered_1m, bool isTriggered_1m1tau);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // set lepton type, pT and eta as well as hadTau pT, eta and decay mode
  // (to be called once per event, before calling any of the getSF.. functions)
  void setLeptons(int lepton_type, double lepton_pt, double lepton_eta);

  void setHadTaus(int hadTau1_genPdgId, double hadTau1_pt, double hadTau1_eta, int hadTau1_decayMode, 
		  int hadTau2_genPdgId, double hadTau2_pt, double hadTau2_eta, int hadTau2_decayMode);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // trigger efficiency turn-on curves for Spring16 non-reHLT MC (not yet implemented)
  double getWeight_triggerEff() const { assert(0); }
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC correction for trigger efficiency 
  double getSF_triggerEff() const;
  //-----------------------------------------------------------------------------

 private:

  //-----------------------------------------------------------------------------
  // data/MC corrections for trigger efficiencies in 2016 ReReco data and Summer16 MC
 
  vLutWrapperBase effTrigger_1e_data_;
  vLutWrapperBase effTrigger_1e_mc_;
  vLutWrapperBase effTrigger_1e1tau_lepLeg_data_;
  vLutWrapperBase effTrigger_1e1tau_lepLeg_mc_;
  std::map<int, vLutWrapperBase> effTrigger_1e1tau_tauLeg_data_gentau_;  // key = reconstructed tau decay mode 
  std::map<int, vLutWrapperBase> effTrigger_1e1tau_tauLeg_data_faketau_; // key = reconstructed tau decay mode 
  std::map<int, vLutWrapperBase> effTrigger_1e1tau_tauLeg_mc_gentau_;    // key = reconstructed tau decay mode 
  std::map<int, vLutWrapperBase> effTrigger_1e1tau_tauLeg_mc_faketau_;   // key = reconstructed tau decay mode 
  
  vLutWrapperBase effTrigger_1m_data_;
  vLutWrapperBase effTrigger_1m_mc_;
  vLutWrapperBase effTrigger_1m1tau_lepLeg_data_;
  vLutWrapperBase effTrigger_1m1tau_lepLeg_mc_;
  vLutWrapperBase effTrigger_1m1tau_tauLeg_data_gentau_;
  vLutWrapperBase effTrigger_1m1tau_tauLeg_data_faketau_;
  vLutWrapperBase effTrigger_1m1tau_tauLeg_mc_gentau_;
  vLutWrapperBase effTrigger_1m1tau_tauLeg_mc_faketau_;
  //-----------------------------------------------------------------------------

  std::map<std::string, TFile*> inputFiles_;

  int era_;
  std::string hadTauSelection_;
  bool isDEBUG_;

  bool isTriggered_1e_;
  bool isTriggered_1e1tau_;
  bool isTriggered_1m_;
  bool isTriggered_1m1tau_;

  int lepton_type_;
  double lepton_pt_;
  double lepton_eta_;
  int hadTau1_genPdgId_;
  double hadTau1_pt_;
  double hadTau1_eta_;
  int hadTau1_decayMode_;
  int hadTau2_genPdgId_;
  double hadTau2_pt_;
  double hadTau2_eta_;
  int hadTau2_decayMode_;
};

#endif // tthAnalysis_HiggsToTauTau_data_to_MC_corrections_1l_2tau_trigger_h
