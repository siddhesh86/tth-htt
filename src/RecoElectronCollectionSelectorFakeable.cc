#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronSelectorFakeable

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2017
#include "tthAnalysis/HiggsToTauTau/interface/cmsException.h" // cmsException(), assert()

RecoElectronSelectorFakeable::RecoElectronSelectorFakeable(int era,
                                                           int index,
                                                           bool debug,
                                                           bool set_selection_flags)
  : era_(era)
  , set_selection_flags_(set_selection_flags)
  , apply_offline_e_trigger_cuts_(true)
  , tightElectronSelector_(era_, index, debug, false)
  , min_cone_pt_(10.)
  , min_lepton_pt_(7.)
  , max_absEta_(2.5)
  , max_dxy_(0.05)
  , max_dz_(0.1)
  , max_relIso_(0.4)
  , max_sip3d_(8.)
  , min_mvaRawPOG_({ 0.0, 0.0, 0.7 })
  , binning_absEta_({ 0.8, 1.479 })
  , min_pt_trig_(-1.) // Was = 30. (AN_2017_029_V5, Lines:237-240) Now changed following sync with Giovanni 
  , max_sigmaEtaEta_trig_({ 0.011, 0.011, 0.030 })
  , max_HoE_trig_({ 0.10, 0.10, 0.07 }) 
  , max_deltaEta_trig_({ 0.01, 0.01, 0.008 })
  , max_deltaPhi_trig_({ 0.04, 0.04, 0.07 })
  , min_OoEminusOoP_trig_(-0.05)
  , max_OoEminusOoP_trig_({ 0.010, 0.010, 0.005 })
  , binning_mvaTTH_({ 0.90 })
  , min_jetPtRatio_({ 0.50, -1.e+3 })
  , apply_conversionVeto_(false) // Was true but changed (Table-7, AN_2017_029_V5)
  , max_nLostHits_(0)
{
  switch(era_)
  {
    case kEra_2017:
    {
      max_jetBtagCSV_ = { 0.3, 0.8484 };
      break;
    }
    default: throw cms::Exception("RecoElectronSelectorFakeable") << "Invalid era: " << era_;
  }
  assert(min_mvaRawPOG_.size() == 3);
  assert(binning_absEta_.size() == 2);
  assert(max_sigmaEtaEta_trig_.size() == 3);
  assert(max_HoE_trig_.size() == 3);
  assert(max_deltaEta_trig_.size() == 3);
  assert(max_deltaPhi_trig_.size() == 3);
  assert(max_OoEminusOoP_trig_.size() == 3);
  assert(binning_mvaTTH_.size() == 1);
  assert(min_jetPtRatio_.size() == 2);
  assert(max_jetBtagCSV_.size() == 2);
}

void
RecoElectronSelectorFakeable::enable_offline_e_trigger_cuts()
{
  apply_offline_e_trigger_cuts_ = true;
}

void
RecoElectronSelectorFakeable::disable_offline_e_trigger_cuts()
{
  apply_offline_e_trigger_cuts_ = false;
}

bool
RecoElectronSelectorFakeable::operator()(const RecoElectron & electron) const
{
  if(electron.cone_pt() >= min_cone_pt_                         &&
     electron.lepton_pt() >= min_lepton_pt_                     &&
     electron.absEta() <= max_absEta_                           &&
     std::fabs(electron.dxy()) <= max_dxy_                      &&
     std::fabs(electron.dz()) <= max_dz_                        &&
     electron.relIso() <= max_relIso_                           &&
     electron.sip3d() <= max_sip3d_                             &&
     electron.nLostHits() <= max_nLostHits_                     &&
     (electron.passesConversionVeto() || ! apply_conversionVeto_))
  {
    std::cout<< "Passed cone_pT, lepton_pT, eta, dxy, dz  cuts "  << std::endl;
    const int idxBin_absEta = electron.absEta() <= binning_absEta_[0] ? 0 :
                             (electron.absEta() <= binning_absEta_[1] ? 1 : 2)
    ;

    double mvaRawPOG = electron.mvaRawPOG_HZZ();
    double mvaRawPOGCut = min_mvaRawPOG_[idxBin_absEta];

    if(mvaRawPOG >= mvaRawPOGCut)
    {
      std::cout<< "Passed mvaRawPOG cut " << std::endl;
      const int idxBin_mvaTTH = electron.mvaRawTTH() <= binning_mvaTTH_[0] ? 0 : 1;

      if(electron.jetPtRatio() >= min_jetPtRatio_[idxBin_mvaTTH] &&
         electron.jetBtagCSV() <= max_jetBtagCSV_[idxBin_mvaTTH])
      {
        if((electron.cone_pt() <= min_pt_trig_ || ! apply_offline_e_trigger_cuts_) || 
	   (electron.cone_pt() > min_pt_trig_ && 
	    electron.sigmaEtaEta() <= max_sigmaEtaEta_trig_[idxBin_absEta] &&
	    electron.HoE() <= max_HoE_trig_[idxBin_absEta]                 &&
	    std::fabs(electron.deltaEta()) <= max_deltaEta_trig_[idxBin_absEta] &&
	    std::fabs(electron.deltaPhi()) <= max_deltaPhi_trig_[idxBin_absEta] &&
	    electron.OoEminusOoP() >= min_OoEminusOoP_trig_  &&
            electron.OoEminusOoP() <= max_OoEminusOoP_trig_[idxBin_absEta]))
	{
	  if(set_selection_flags_)
	  {
            electron.set_isFakeable();
	    std::cout<< "Passed sigmaEtaEta(), HoE(), OoEminusOoP(), deltaEta(), deltaPhi() cuts "  << std::endl;
	    // std::cout<< "std::fabs(electron.deltaEta()) " << std::fabs(electron.deltaEta()) << " max_deltaEta_trig_[idxBin_absEta] " << max_deltaEta_trig_[idxBin_absEta] << std::endl;
	    // std::cout<< "std::fabs(electron.deltaPhi()) " << std::fabs(electron.deltaPhi()) << " max_deltaPhi_trig_[idxBin_absEta] " << max_deltaPhi_trig_[idxBin_absEta] << std::endl;
	    if(tightElectronSelector_(electron))
            {
              electron.set_isTight();
            }
          }
	  // std::cout<< "set_selection_flags_ = " << set_selection_flags_ << std::endl;
          return true;
        } else {
	   std::cout<< "Failed sigmaEtaEta(), HoE(), OoEminusOoP(), deltaEta(), deltaPhi() cuts "  << std::endl;
	   // std::cout<< "std::fabs(electron.deltaEta()) " << std::fabs(electron.deltaEta()) << " max_deltaEta_trig_[idxBin_absEta] " << max_deltaEta_trig_[idxBin_absEta] << std::endl;
	   // std::cout<< "std::fabs(electron.deltaPhi()) " << std::fabs(electron.deltaPhi()) << " max_deltaPhi_trig_[idxBin_absEta] " << max_deltaPhi_trig_[idxBin_absEta] << std::endl;
        }  
      }
    } // mvaRawPOG >= mvaRawPOGCut
  } else {std::cout<< "Failed cone_pT, lepton_pT, eta, dxy, dz  cuts "  << std::endl;}
  return false;
}

RecoElectronCollectionSelectorFakeable::RecoElectronCollectionSelectorFakeable(int era,
                                                                               int index,
                                                                               bool debug,
                                                                               bool set_selection_flags)
  : selIndex_(index)
  , selector_(era, index, debug, set_selection_flags)
{}

void
RecoElectronCollectionSelectorFakeable::enable_offline_e_trigger_cuts()
{
  selector_.enable_offline_e_trigger_cuts();
}

void
RecoElectronCollectionSelectorFakeable::disable_offline_e_trigger_cuts()
{
  selector_.disable_offline_e_trigger_cuts();
}

std::vector<const RecoElectron *>
RecoElectronCollectionSelectorFakeable::operator()(const std::vector<const RecoElectron * > & electrons) const
{
  std::vector<const RecoElectron *> selElectrons;
  int idx = 0;
  for(const RecoElectron * electron: electrons)
  {
    if(selector_(*electron))
    {
      if(idx == selIndex_ || selIndex_ == -1)
      {
        selElectrons.push_back(electron);
      }
      ++idx;
    }
  }
  return selElectrons;
}
