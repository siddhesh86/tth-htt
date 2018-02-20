#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonSelectorFakeable

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2017
#include "tthAnalysis/HiggsToTauTau/interface/cmsException.h" // cmsException()

RecoMuonSelectorFakeable::RecoMuonSelectorFakeable(int era, int index, bool debug, bool set_selection_flags)
  : era_(era)
  , set_selection_flags_(set_selection_flags)
  , tightMuonSelector_(era_, index, debug, false)
  , min_cone_pt_(10.)
  , min_lepton_pt_(5.)
  , max_absEta_(2.4)
  , max_dxy_(0.05)
  , max_dz_(0.1)
  , max_relIso_(0.4)
  , max_sip3d_(8.)
  , apply_looseIdPOG_(true)
  , binning_mvaTTH_({ 0.90 })
  , min_jetPtRatio_({ 0.50, -1.e+3 })
  , apply_mediumIdPOG_(false)
  , min_segmentCompatibility_({0.3, -1.e+3})
{
  switch(era_)
  {
    case kEra_2017:
    {
      max_jetBtagCSV_ = { 0.3, 0.8484 };
      break;
    }
    default: throw cmsException(this) << "Invalid era: " << era_;
  }
}

bool
RecoMuonSelectorFakeable::operator()(const RecoMuon & muon) const
{
  if(muon.cone_pt()        >= min_cone_pt_            &&
     muon.lepton_pt()      >= min_lepton_pt_          &&
     muon.absEta()         <= max_absEta_             &&
     std::fabs(muon.dxy()) <= max_dxy_                &&
     std::fabs(muon.dz())  <= max_dz_                 &&
     muon.relIso()         <= max_relIso_             &&
     muon.sip3d()          <= max_sip3d_              &&
     (muon.passesLooseIdPOG() || ! apply_looseIdPOG_) &&
     (muon.passesMediumIdPOG() || ! apply_mediumIdPOG_))
  {
    const int idxBin = muon.mvaRawTTH() <= binning_mvaTTH_[0] ? 0 : 1;
    if(muon.jetPtRatio() >= min_jetPtRatio_[idxBin] &&
       muon.jetBtagCSV() <= max_jetBtagCSV_[idxBin] &&
       muon.segmentCompatibility() > min_segmentCompatibility_[idxBin])
    {
      if(set_selection_flags_)
      {
        muon.set_isFakeable();
        if(tightMuonSelector_(muon))
        {
          muon.set_isTight();
        }
      } // set_selection_flags
      return true;
    } // muon.jetPtRatio, muon.jetBtagCSV
  }
  return false;
}

RecoMuonCollectionSelectorFakeable::RecoMuonCollectionSelectorFakeable(int era,
                                                                       int index,
                                                                       bool debug,
                                                                       bool set_selection_flags)
  : selIndex_(index)
  , selector_(era, index, debug, set_selection_flags)
{}

std::vector<const RecoMuon *>
RecoMuonCollectionSelectorFakeable::operator()(const std::vector<const RecoMuon *> & muons) const
{
  std::vector<const RecoMuon *> selMuons;
  int idx = 0;
  for(const RecoMuon * const & muon: muons)
  {
    if(selector_(*muon))
    {
      if(idx == selIndex_ || selIndex_ == -1)
      {
        selMuons.push_back(muon);
      }
      ++idx;
    }
  }
  return selMuons;
}
