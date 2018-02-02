#include "tthAnalysis/HiggsToTauTau/interface/hltPath_LeptonFakeRate.h"

#include <iostream> // std::ostream

hltPath_LeptonFakeRate::hltPath_LeptonFakeRate(const std::string & branchName,
                                               bool is_trigger_1mu,
                                               bool is_trigger_2mu,
                                               bool is_trigger_1e,
                                               bool is_trigger_2e,
                                               double minPt,
                                               double maxPt,
                                               double min_jetPt,
                                               double prescale,
                                               double prescale_rand_mc)
  : hltPath(branchName, minPt, maxPt)
  , branchName_(branchName)
  , value_(-1)
  , minPt_(minPt)
  , maxPt_(maxPt)
  , minJetPt_(min_jetPt)
  , prescale_(prescale)
  , prescale_rand_mc_(prescale_rand_mc)
  , is_trigger_1mu_(is_trigger_1mu)
  , is_trigger_2mu_(is_trigger_2mu)
  , is_trigger_1e_(is_trigger_1e)
  , is_trigger_2e_(is_trigger_2e)
{}

double
hltPath_LeptonFakeRate::getMinJetPt() const
{
  return minJetPt_;
}

double
hltPath_LeptonFakeRate::getPrescale() const
{
  return prescale_;
}

double
hltPath_LeptonFakeRate::getPrescale_rand_mc() const
{
  return prescale_rand_mc_;
}

bool
hltPath_LeptonFakeRate::is_trigger_1mu() const
{
  return is_trigger_1mu_;
}

bool
hltPath_LeptonFakeRate::is_trigger_2mu() const
{
  return is_trigger_2mu_;
}

bool
hltPath_LeptonFakeRate::is_trigger_1e() const
{
  return is_trigger_1e_;
}

bool
hltPath_LeptonFakeRate::is_trigger_2e() const
{
  return is_trigger_2e_;
}

void
hltPaths_LeptonFakeRate_setBranchAddresses(TTree * tree,
                                           const std::vector<hltPath_LeptonFakeRate *> & hltPaths)
{
  for(hltPath_LeptonFakeRate * const & path: hltPaths)
  {
    path->setBranchAddress(tree);
  }
}

std::vector<hltPath_LeptonFakeRate *>
create_hltPaths_LeptonFakeRate(const std::vector<std::string> & branchNames,
                               const edm::ParameterSet & cfg)
{
  const double minPt = cfg.getParameter<double>("cone_minPt");
  const double maxPt = cfg.getParameter<double>("cone_maxPt");
  const double jet_minPt = cfg.getParameter<double>("jet_minPt");
  const double prescale = cfg.getParameter<double>("average_prescale");
  const double prescale_rand_mc = cfg.getParameter<double>("prescale_rand_mc");
  const bool is_trigger_1mu = cfg.getParameter<bool>("is_trigger_1mu");
  const bool is_trigger_2mu = cfg.getParameter<bool>("is_trigger_2mu");
  const bool is_trigger_1e = cfg.getParameter<bool>("is_trigger_1e");
  const bool is_trigger_2e = cfg.getParameter<bool>("is_trigger_2e");

  std::vector<hltPath_LeptonFakeRate *> hltPaths;
  for(const std::string & branchName: branchNames)
  {
    hltPaths.push_back(new hltPath_LeptonFakeRate(
      branchName,
      is_trigger_1mu, is_trigger_2mu, is_trigger_1e, is_trigger_2e,
      minPt, maxPt, jet_minPt, prescale, prescale_rand_mc
    ));
  }
  return hltPaths;
}

void
hltPaths_LeptonFakeRate_delete(const std::vector<hltPath_LeptonFakeRate *> & hltPaths)
{
  for(hltPath_LeptonFakeRate * path: hltPaths)
  {
    delete path;
    path = nullptr;
  }
}

std::ostream &
operator<<(std::ostream & stream,
           const hltPath_LeptonFakeRate & hltPath_iter)
{
  stream << "hltPath = " << hltPath_iter.getBranchName() << ": "
            "value = " << hltPath_iter.getValue() << " ("
            "minPt = " << hltPath_iter.getMinPt() << ", "
            "maxPt = " << hltPath_iter.getMaxPt() << ", "
            "minJetPt = " << hltPath_iter.getMinJetPt() << ", "
            "prescale = " << hltPath_iter.getPrescale() << ", "
            "prescale_rand_mc = " << hltPath_iter.getPrescale_rand_mc()
         << ")\n";
  return stream;
}
