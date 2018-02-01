#ifndef tthAnalysis_HiggsToTauTau_EvtHistManager_charge_flip_h
#define tthAnalysis_HiggsToTauTau_EvtHistManager_charge_flip_h

/** \class EvtHistManager_charge_flip
 *
 * Book and fill histograms for event-level quantities in electron charge misidentification rate measurement
 *
 * \author Christian Veelken, Tallin
 *
 */

#include "tthAnalysis/HiggsToTauTau/interface/HistManagerBase.h" // HistManagerBase

#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector

#include <map> // std::map

class EvtHistManager_charge_flip
  : public HistManagerBase
{
 public:
  EvtHistManager_charge_flip(edm::ParameterSet const& cfg);
  ~EvtHistManager_charge_flip() {}

  /// book and fill histograms
  void bookHistograms(TFileDirectory& dir);
  void fillHistograms(const math::PtEtaPhiMLorentzVector& selElectron_lead_p4, const math::PtEtaPhiMLorentzVector& selElectron_sublead_p4, double m_ee, bool isCharge_SS, double evtWeight);

 private:
  std::map<std::string, TH1*> histograms_m_ee_SS_; // key = event category, based on pT and eta of the two electrons
  std::map<std::string, TH1*> histograms_m_ee_OS_; // key = event category, based on pT and eta of the two electrons
  
  TH1* histogram_EventCounter_;

  std::vector<TH1*> histograms_;
};

#endif