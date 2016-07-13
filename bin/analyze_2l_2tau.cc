
#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h" // edm::readPSetsFrom()
#include "FWCore/ParameterSet/interface/FileInPath.h" // edm::FileInPath
#include "FWCore/Utilities/interface/Exception.h" // cms::Exception
#include "PhysicsTools/FWLite/interface/TFileService.h" // fwlite::TFileService
#include "DataFormats/FWLite/interface/InputSource.h" // fwlite::InputSource
#include "DataFormats/FWLite/interface/OutputFiles.h" // fwlite::OutputFiles
#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector
#include "DataFormats/Math/interface/deltaR.h" // deltaR

#include <Rtypes.h> // Int_t, Long64_t, Double_t
#include <TChain.h> // TChain
#include <TTree.h> // TTree
#include <TBenchmark.h> // TBenchmark
#include <TString.h> // TString, Form

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h" // RecoHadTau
#include "tthAnalysis/HiggsToTauTau/interface/GenLepton.h" // GenLepton
#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTau.h" // GenHadTau
#include "tthAnalysis/HiggsToTauTau/interface/TMVAInterface.h" // TMVAInterface
#include "tthAnalysis/HiggsToTauTau/interface/mvaInputVariables.h" // auxiliary functions for computing input variables of the MVA used for signal extraction in the 2los_1tau category 
#include "tthAnalysis/HiggsToTauTau/interface/KeyTypes.h"
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronReader.h" // RecoElectronReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonReader.h" // RecoMuonReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTauReader.h" // GenHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionCleaner.h" // RecoElectronCollectionCleaner, RecoMuonCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleaner
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorLoose.h" // RecoHadTauCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorFakeable.h" // RecoHadTauCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/ElectronHistManager.h" // ElectronHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MuonHistManager.h" // MuonHistManager
#include "tthAnalysis/HiggsToTauTau/interface/HadTauHistManager.h" // HadTauHistManager
#include "tthAnalysis/HiggsToTauTau/interface/HadTauFakeRateHistManager.h" // HadTauFakeRateHistManager
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManager.h" // JetHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MEtHistManager.h" // MEtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/EvtHistManager_2l_2tau.h" // EvtHistManager_2l_2tau
#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // getLeptonType, kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/backgroundEstimation.h" // prob_chargeMisId
#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath, create_hltPaths, hltPaths_setBranchAddresses, hltPaths_isTriggered, hltPaths_delete
#include "tthAnalysis/HiggsToTauTau/interface/data_to_MC_corrections.h"
#include "tthAnalysis/HiggsToTauTau/interface/particleIDlooseToTightWeightEntryType.h" // particleIDlooseToTightWeightEntryType
#include "tthAnalysis/HiggsToTauTau/interface/cutFlowTable.h" // cutFlowTableType

#include <boost/range/algorithm/copy.hpp> // boost::copy()
#include <boost/range/adaptor/map.hpp> // boost::adaptors::map_keys

#include <iostream> // std::cerr, std::fixed
#include <iomanip> // std::setprecision(), std::setw()
#include <string> // std::string
#include <vector> // std::vector<>
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <algorithm> // std::sort
#include <fstream> // std::ofstream
#include <assert.h> // assert

#define EPS 1E-2

typedef math::PtEtaPhiMLorentzVector LV;
typedef std::vector<std::string> vstring;
typedef std::vector<double> vdouble;

//--- declare constants
const double z_mass   = 91.1876;
const double z_window = 10.;
const double met_coef =  0.00397;
const double mht_coef =  0.00265;

struct HadTauHistManagerWrapper_eta
{
  HadTauHistManager* histManager_;
  double etaMin_;
  double etaMax_;
};

/**
 * @brief Auxiliary function used for sorting leptons by decreasing pT
 * @param Given pair of leptons
 * @return True, if first lepton has higher pT; false if second lepton has higher pT
 */
bool isHigherPt(const GenParticle* particle1, const GenParticle* particle2)
{
  return (particle1->pt_ > particle2->pt_);
}

/**
 * @brief Produce datacard and control plots for 2l_2tau category.
 */
int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_SUCCESS;
  }

  std::cout << "<analyze_2l_2tau>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("analyze_2l_2tau");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("analyze_2l_2tau") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfg_analyze = cfg.getParameter<edm::ParameterSet>("analyze_2l_2tau");

  std::string treeName = cfg_analyze.getParameter<std::string>("treeName");

  std::string process_string = cfg_analyze.getParameter<std::string>("process");
  bool isSignal = ( process_string == "signal" ) ? true : false;

  vstring triggerNames_1e = cfg_analyze.getParameter<vstring>("triggers_1e");
  std::vector<hltPath*> triggers_1e = create_hltPaths(triggerNames_1e);
  bool use_triggers_1e = cfg_analyze.getParameter<bool>("use_triggers_1e");
  vstring triggerNames_2e = cfg_analyze.getParameter<vstring>("triggers_2e");
  std::vector<hltPath*> triggers_2e = create_hltPaths(triggerNames_2e);
  bool use_triggers_2e = cfg_analyze.getParameter<bool>("use_triggers_2e");
  vstring triggerNames_1mu = cfg_analyze.getParameter<vstring>("triggers_1mu");
  std::vector<hltPath*> triggers_1mu = create_hltPaths(triggerNames_1mu);
  bool use_triggers_1mu = cfg_analyze.getParameter<bool>("use_triggers_1mu");
  vstring triggerNames_2mu = cfg_analyze.getParameter<vstring>("triggers_2mu");
  std::vector<hltPath*> triggers_2mu = create_hltPaths(triggerNames_2mu);
  bool use_triggers_2mu = cfg_analyze.getParameter<bool>("use_triggers_2mu");
  vstring triggerNames_1e1mu = cfg_analyze.getParameter<vstring>("triggers_1e1mu");
  std::vector<hltPath*> triggers_1e1mu = create_hltPaths(triggerNames_1e1mu);
  bool use_triggers_1e1mu = cfg_analyze.getParameter<bool>("use_triggers_1e1mu");

  bool apply_offline_e_trigger_cuts_1e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e");
  bool apply_offline_e_trigger_cuts_2e = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_2e");
  bool apply_offline_e_trigger_cuts_1e1mu = cfg_analyze.getParameter<bool>("apply_offline_e_trigger_cuts_1e1mu");

  enum { kOS, kSS, kDisabled };
  std::string chargeSelection_lepton_string = cfg_analyze.getParameter<std::string>("chargeSelection_lepton");
  int chargeSelection_lepton = -1;
  if      ( chargeSelection_lepton_string == "OS"       ) chargeSelection_lepton = kOS;
  else if ( chargeSelection_lepton_string == "SS"       ) chargeSelection_lepton = kSS;
  else if ( chargeSelection_lepton_string == "disabled" ) chargeSelection_lepton = kDisabled;
  else throw cms::Exception("analyze_2l_2tau") 
    << "Invalid Configuration parameter 'chargeSelection_lepton' = " << chargeSelection_lepton_string << " !!\n";
  std::string chargeSelection_hadTau_string = cfg_analyze.getParameter<std::string>("chargeSelection_hadTau");
  int chargeSelection_hadTau = -1;
  if      ( chargeSelection_hadTau_string == "OS"       ) chargeSelection_hadTau = kOS;
  else if ( chargeSelection_hadTau_string == "SS"       ) chargeSelection_hadTau = kSS;
  else if ( chargeSelection_hadTau_string == "disabled" ) chargeSelection_hadTau = kDisabled;
  else throw cms::Exception("analyze_2l_2tau") 
    << "Invalid Configuration parameter 'chargeSelection_hadTau' = " << chargeSelection_hadTau_string << " !!\n";

  enum { kLoose, kFakeable, kTight };
  std::string hadTauSelection_string = cfg_analyze.getParameter<std::string>("hadTauSelection");
  int hadTauSelection = -1;
  if      ( hadTauSelection_string == "Loose"    ) hadTauSelection = kLoose;
  else if ( hadTauSelection_string == "Fakeable" ) hadTauSelection = kFakeable;
  else if ( hadTauSelection_string == "Tight"    ) hadTauSelection = kTight;
  else throw cms::Exception("analyze_2l_2tau") 
    << "Invalid Configuration parameter 'hadTauSelection' = " << hadTauSelection_string << " !!\n";

  enum { kGenLepton, kGenHadTau, kGenJet, kAll };
  std::string hadTauGenMatch_string = cfg_analyze.getParameter<std::string>("hadTauGenMatch");
  int hadTauGenMatch = -1;
  if      ( hadTauGenMatch_string == "lepton" ) hadTauGenMatch = kGenLepton;
  else if ( hadTauGenMatch_string == "hadTau" ) hadTauGenMatch = kGenHadTau;
  else if ( hadTauGenMatch_string == "jet"    ) hadTauGenMatch = kGenJet;
  else if ( hadTauGenMatch_string == "all"    ) hadTauGenMatch = kAll;  
  else throw cms::Exception("analyze_2l_2tau") 
    << "Invalid Configuration parameter 'hadTauGenMatch' = " << hadTauGenMatch_string << " !!\n";

  vdouble hadTauEtaBins_lead = cfg_analyze.getParameter<vdouble>("hadTauEtaBins_lead"); // CV: eta bins in which jet->tau fake-rates are determined
  vdouble hadTauEtaBins_sublead = cfg_analyze.getParameter<vdouble>("hadTauEtaBins_sublead");

  bool isMC = cfg_analyze.getParameter<bool>("isMC"); 
  std::string central_or_shift = cfg_analyze.getParameter<std::string>("central_or_shift");
  double lumiScale = ( process_string != "data_obs" ) ? cfg_analyze.getParameter<double>("lumiScale") : 1.;

  std::string jet_btagWeight_branch = ( isMC ) ? "Jet_bTagWeight" : "";

  int jetPt_option = RecoJetReader::kJetPt_central;
  int hadTauPt_option = RecoHadTauReader::kHadTauPt_central;
  if ( isMC && central_or_shift != "central" ) {
    TString central_or_shift_tstring = central_or_shift.data();
    std::string shiftUp_or_Down = "";
    if      ( central_or_shift_tstring.EndsWith("Up")   ) shiftUp_or_Down = "Up";
    else if ( central_or_shift_tstring.EndsWith("Down") ) shiftUp_or_Down = "Down";
    else throw cms::Exception("analyze_2l_2tau")
      << "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
    if      ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HF")       ) jet_btagWeight_branch = "Jet_bTagWeightHF" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HFStats1") ) jet_btagWeight_branch = "Jet_bTagWeightHFStats1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_HFStats2") ) jet_btagWeight_branch = "Jet_bTagWeightHFStats2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LF")       ) jet_btagWeight_branch = "Jet_bTagWeightLF" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LFStats1") ) jet_btagWeight_branch = "Jet_bTagWeightLFStats1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_LFStats2") ) jet_btagWeight_branch = "Jet_bTagWeightLFStats2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_cErr1")    ) jet_btagWeight_branch = "Jet_bTagWeightcErr1" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag_cErr2")    ) jet_btagWeight_branch = "Jet_bTagWeightcErr2" + shiftUp_or_Down;
    else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_JES") ) {
      jet_btagWeight_branch = "Jet_bTagWeightJES" + shiftUp_or_Down;
      if      ( shiftUp_or_Down == "Up"   ) jetPt_option = RecoJetReader::kJetPt_jecUp;
      else if ( shiftUp_or_Down == "Down" ) jetPt_option = RecoJetReader::kJetPt_jecDown;
      else assert(0);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_tauES") ) {
      if      ( shiftUp_or_Down == "Up"   ) hadTauPt_option = RecoHadTauReader::kHadTauPt_shiftUp;
      else if ( shiftUp_or_Down == "Down" ) hadTauPt_option = RecoHadTauReader::kHadTauPt_shiftDown;
      else assert(0);
    } else throw cms::Exception("analyze_2l_2tau")
	<< "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
  }

  std::vector<particleIDlooseToTightWeightEntryType*> jetToTauFakeRateWeights;
  bool applyJetToTauFakeRateWeight = cfg_analyze.getParameter<bool>("applyJetToTauFakeRateWeight");
  if ( applyJetToTauFakeRateWeight ) {
    edm::ParameterSet cfg_jetToTauFakeRateWeight = cfg_analyze.getParameter<edm::ParameterSet>("jetToTauFakeRateWeight");

    vdouble hadTauEtaBins_lead = cfg_analyze.getParameter<vdouble>("hadTauEtaBins_lead"); // CV: eta bins in which jet->tau fake-rates have been measured
    vdouble hadTauEtaBins_sublead = cfg_analyze.getParameter<vdouble>("hadTauEtaBins_sublead");
    
    std::string fitFunctionNormName = cfg_jetToTauFakeRateWeight.getParameter<std::string>("fitFunctionNormName");
    std::string fitFunctionShapeName_lead_central = cfg_jetToTauFakeRateWeight.getParameter<std::string>("fitFunctionShapeName_lead_central");
    std::string graphShapeName_lead, fitFunctionShapeName_lead_shift;    
    int applyFitFunction_or_graph_lead = particleIDlooseToTightWeightEntryType::kFitFunction;
    if ( cfg_jetToTauFakeRateWeight.exists("graphShapeName_lead") ) {
      graphShapeName_lead = cfg_jetToTauFakeRateWeight.getParameter<std::string>("graphShapeName_lead");
      fitFunctionShapeName_lead_shift = cfg_jetToTauFakeRateWeight.getParameter<std::string>("fitFunctionShapeName_lead_shift");
      std::string applyFitFunction_or_graph_lead_string = cfg_jetToTauFakeRateWeight.getParameter<std::string>("applyFitFunction_or_graph_lead");
      if      ( applyFitFunction_or_graph_lead_string == "fitFunction" ) applyFitFunction_or_graph_lead = particleIDlooseToTightWeightEntryType::kFitFunction;
      else if ( applyFitFunction_or_graph_lead_string == "graph"       ) applyFitFunction_or_graph_lead = particleIDlooseToTightWeightEntryType::kGraph;
      else if ( applyFitFunction_or_graph_lead_string == "notApplied"  ) applyFitFunction_or_graph_lead = particleIDlooseToTightWeightEntryType::kNotApplied;
      else throw cms::Exception("analyze_2l_2tau") 
        << "Invalid Configuration parameter 'applyFitFunction_or_graph_lead' = " << applyFitFunction_or_graph_lead_string << " !!\n";
    }
    double fitFunctionShapePower_lead = cfg_jetToTauFakeRateWeight.getParameter<double>("fitFunctionShapePower_lead");
    std::string fitFunctionShapeName_sublead_central = cfg_jetToTauFakeRateWeight.getParameter<std::string>("fitFunctionShapeName_sublead_central");
    std::string graphShapeName_sublead, fitFunctionShapeName_sublead_shift;
    int applyFitFunction_or_graph_sublead = particleIDlooseToTightWeightEntryType::kFitFunction;
    if ( cfg_jetToTauFakeRateWeight.exists("graphShapeName_sublead") ) {
      graphShapeName_sublead = cfg_jetToTauFakeRateWeight.getParameter<std::string>("graphShapeName_sublead");
      fitFunctionShapeName_sublead_shift = cfg_jetToTauFakeRateWeight.getParameter<std::string>("fitFunctionShapeName_sublead_shift");
      std::string applyFitFunction_or_graph_sublead_string = cfg_jetToTauFakeRateWeight.getParameter<std::string>("applyFitFunction_or_graph_sublead");
      if      ( applyFitFunction_or_graph_sublead_string == "fitFunction" ) applyFitFunction_or_graph_sublead = particleIDlooseToTightWeightEntryType::kFitFunction;
      else if ( applyFitFunction_or_graph_sublead_string == "graph"       ) applyFitFunction_or_graph_sublead = particleIDlooseToTightWeightEntryType::kGraph;
      else if ( applyFitFunction_or_graph_sublead_string == "notApplied"  ) applyFitFunction_or_graph_sublead = particleIDlooseToTightWeightEntryType::kNotApplied;
      else throw cms::Exception("analyze_2l_2tau") 
        << "Invalid Configuration parameter 'applyFitFunction_or_graph_sublead' = " << applyFitFunction_or_graph_sublead_string << " !!\n";
    }
    double fitFunctionShapePower_sublead = cfg_jetToTauFakeRateWeight.getParameter<double>("fitFunctionShapePower_sublead");

    std::string inputFileName = cfg_jetToTauFakeRateWeight.getParameter<std::string>("inputFileName");
    TFile* inputFile = new TFile(inputFileName.data());

    int numHadTauEtaBins_lead = hadTauEtaBins_lead.size() - 1;
    for ( int idxHadTauEtaBin_lead = 0; idxHadTauEtaBin_lead < numHadTauEtaBins_lead; ++idxHadTauEtaBin_lead ) {
      double hadTauEtaMin_lead = hadTauEtaBins_lead[idxHadTauEtaBin_lead];
      double hadTauEtaMax_lead = hadTauEtaBins_lead[idxHadTauEtaBin_lead + 1];

      int numHadTauEtaBins_sublead = hadTauEtaBins_sublead.size() - 1;
      for ( int idxHadTauEtaBin_sublead = 0; idxHadTauEtaBin_sublead < numHadTauEtaBins_sublead; ++idxHadTauEtaBin_sublead ) {
        double hadTauEtaMin_sublead = hadTauEtaBins_sublead[idxHadTauEtaBin_sublead];
        double hadTauEtaMax_sublead = hadTauEtaBins_sublead[idxHadTauEtaBin_sublead + 1];

        particleIDlooseToTightWeightEntryType* jetToTauFakeRateWeight = new particleIDlooseToTightWeightEntryType(
          inputFile,
          "tau", hadTauEtaMin_lead, hadTauEtaMax_lead, hadTauEtaMin_sublead, hadTauEtaMax_sublead,
          fitFunctionNormName, 
          graphShapeName_lead, fitFunctionShapeName_lead_central, fitFunctionShapeName_lead_shift, applyFitFunction_or_graph_lead, fitFunctionShapePower_lead, 
          graphShapeName_sublead, fitFunctionShapeName_sublead_central, fitFunctionShapeName_sublead_shift, applyFitFunction_or_graph_sublead, fitFunctionShapePower_sublead);
        jetToTauFakeRateWeights.push_back(jetToTauFakeRateWeight);
      }
    }
    delete inputFile;
  }

  std::string selEventsFileName_input = cfg_analyze.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;
  if ( selEventsFileName_input != "" ) {
    edm::ParameterSet cfg_runLumiEventSelector;
    cfg_runLumiEventSelector.addParameter<std::string>("inputFileName", selEventsFileName_input);
    cfg_runLumiEventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfg_runLumiEventSelector);
  }

  std::string selEventsFileName_output = cfg_analyze.getParameter<std::string>("selEventsFileName_output");

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << std::endl;
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TChain* inputTree = new TChain(treeName.data());
  for ( std::vector<std::string>::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    std::cout << "input Tree: adding file = " << (*inputFileName) << std::endl;
    inputTree->AddFile(inputFileName->data());
  }
  
  if ( !(inputTree->GetListOfFiles()->GetEntries() >= 1) ) {
    throw cms::Exception("analyze_2l_2tau") 
      << "Failed to identify input Tree !!\n";
  }
  
  // CV: need to call TChain::LoadTree before processing first event 
  //     in order to prevent ROOT causing a segmentation violation,
  //     cf. http://root.cern.ch/phpBB3/viewtopic.php?t=10062
  inputTree->LoadTree(0);

  std::cout << "input Tree contains " << inputTree->GetEntries() << " Entries in " << inputTree->GetListOfFiles()->GetEntries() << " files." << std::endl;

//--- declare event-level variables
  RUN_TYPE run;
  inputTree->SetBranchAddress(RUN_KEY, &run);
  LUMI_TYPE lumi;
  inputTree->SetBranchAddress(LUMI_KEY, &lumi);
  EVT_TYPE event;
  inputTree->SetBranchAddress(EVT_KEY, &event);
  GENHIGGSDECAYMODE_TYPE genHiggsDecayMode;
  if ( isSignal ) {
    inputTree->SetBranchAddress(GENHIGGSDECAYMODE_KEY, &genHiggsDecayMode);
  }

  hltPaths_setBranchAddresses(inputTree, triggers_1e);
  hltPaths_setBranchAddresses(inputTree, triggers_2e);
  hltPaths_setBranchAddresses(inputTree, triggers_1mu);
  hltPaths_setBranchAddresses(inputTree, triggers_2mu);
  hltPaths_setBranchAddresses(inputTree, triggers_1e1mu);

  MET_PT_TYPE met_pt;
  inputTree->SetBranchAddress(MET_PT_KEY, &met_pt);
  MET_ETA_TYPE met_eta;
  inputTree->SetBranchAddress(MET_ETA_KEY, &met_eta);
  MET_PHI_TYPE met_phi;
  inputTree->SetBranchAddress(MET_PHI_KEY, &met_phi);
  LV met_p4(met_pt, met_eta, met_phi, 0.);

//--- declare particle collections
  RecoMuonReader* muonReader = new RecoMuonReader("nselLeptons", "selLeptons");
  muonReader->setBranchAddresses(inputTree);
  RecoMuonCollectionGenMatcher muonGenMatcher;
  RecoMuonCollectionSelectorLoose preselMuonSelector;
  RecoMuonCollectionSelectorTight tightMuonSelector;

  RecoElectronReader* electronReader = new RecoElectronReader("nselLeptons", "selLeptons");
  electronReader->setBranchAddresses(inputTree);
  RecoElectronCollectionGenMatcher electronGenMatcher;
  RecoElectronCollectionCleaner electronCleaner(0.3);
  RecoElectronCollectionSelectorLoose preselElectronSelector;
  RecoElectronCollectionSelectorTight tightElectronSelector;

  RecoHadTauReader* hadTauReader = new RecoHadTauReader("nTauGood", "TauGood");
  hadTauReader->setHadTauPt_central_or_shift(hadTauPt_option);
  hadTauReader->setBranchAddresses(inputTree);
  RecoHadTauCollectionGenMatcher hadTauGenMatcher;
  RecoHadTauCollectionCleaner hadTauCleaner(0.3);
  RecoHadTauCollectionSelectorLoose preselHadTauSelector;
  RecoHadTauCollectionSelectorFakeable fakeableHadTauSelector_lead(0);
  RecoHadTauCollectionSelectorFakeable fakeableHadTauSelector_sublead(1);
  RecoHadTauCollectionSelectorTight tightHadTauSelector_lead(0);
  RecoHadTauCollectionSelectorTight tightHadTauSelector_sublead(1);
  
  RecoJetReader* jetReader = new RecoJetReader("nJet", "Jet");
  jetReader->setJetPt_central_or_shift(jetPt_option);
  jetReader->setBranchName_BtagWeight(jet_btagWeight_branch);
  jetReader->setBranchAddresses(inputTree);
  RecoJetCollectionGenMatcher jetGenMatcher;
  RecoJetCollectionCleaner jetCleaner(0.5);
  RecoJetCollectionSelector jetSelector;  
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose;
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium;

  GenLeptonReader* genLeptonReader = 0;
  GenHadTauReader* genHadTauReader = 0;
  GenJetReader* genJetReader = 0;
  if ( isMC ) {
    genLeptonReader = new GenLeptonReader("nGenLep", "GenLep");
    genLeptonReader->setBranchAddresses(inputTree);
    genHadTauReader = new GenHadTauReader("nGenHadTaus", "GenHadTaus");
    genHadTauReader->setBranchAddresses(inputTree);
    genJetReader = new GenJetReader("nGenJet", "GenJet");
    genJetReader->setBranchAddresses(inputTree);
  }

//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = new std::ofstream(selEventsFileName_output.data(), std::ios::out);

//--- declare histograms
  std::string charge_and_hadTauSelection = Form("lep%s_tau%s_%s", chargeSelection_lepton_string.data(), chargeSelection_hadTau_string.data(), hadTauSelection_string.data());
  ElectronHistManager preselElectronHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/electrons", charge_and_hadTauSelection.data()), central_or_shift));
  preselElectronHistManager.bookHistograms(fs);
  MuonHistManager preselMuonHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/muons", charge_and_hadTauSelection.data()), central_or_shift));
  preselMuonHistManager.bookHistograms(fs);
  HadTauHistManager preselHadTauHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/hadTaus", charge_and_hadTauSelection.data()), central_or_shift));
  preselHadTauHistManager.bookHistograms(fs);
  HadTauHistManager preselHadTauHistManager_lead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/leadHadTau", charge_and_hadTauSelection.data()), central_or_shift));
  preselHadTauHistManager_lead.bookHistograms(fs);
  HadTauHistManager preselHadTauHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/subleadHadTau", charge_and_hadTauSelection.data()), central_or_shift));
  preselHadTauHistManager_sublead.bookHistograms(fs);
  JetHistManager preselJetHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/jets", charge_and_hadTauSelection.data()), central_or_shift));
  preselJetHistManager.bookHistograms(fs);
  JetHistManager preselBJet_looseHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/BJets_loose", charge_and_hadTauSelection.data()), central_or_shift));
  preselBJet_looseHistManager.bookHistograms(fs);
  JetHistManager preselBJet_mediumHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/BJets_medium", charge_and_hadTauSelection.data()), central_or_shift));
  preselBJet_mediumHistManager.bookHistograms(fs);
  MEtHistManager preselMEtHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/met", charge_and_hadTauSelection.data()), central_or_shift));
  preselMEtHistManager.bookHistograms(fs);
  EvtHistManager_2l_2tau preselEvtHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/presel/evt", charge_and_hadTauSelection.data()), central_or_shift));
  preselEvtHistManager.bookHistograms(fs);

  ElectronHistManager selElectronHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/electrons", charge_and_hadTauSelection.data()), central_or_shift));
  selElectronHistManager.bookHistograms(fs);

  MuonHistManager selMuonHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/muons", charge_and_hadTauSelection.data()), central_or_shift));
  selMuonHistManager.bookHistograms(fs);

  HadTauHistManager selHadTauHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/hadTaus", charge_and_hadTauSelection.data()), central_or_shift));
  selHadTauHistManager.bookHistograms(fs);
  HadTauHistManager selHadTauHistManager_lead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/leadHadTau", charge_and_hadTauSelection.data()), central_or_shift, 0));
  selHadTauHistManager_lead.bookHistograms(fs);
  HadTauHistManager selHadTauHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/subleadHadTau", charge_and_hadTauSelection.data()), central_or_shift, 1));
  selHadTauHistManager_sublead.bookHistograms(fs);
  edm::ParameterSet cfg_selHadTauFakeRateHistManager = makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/hadTauFakeRates", charge_and_hadTauSelection.data()), central_or_shift);
  cfg_selHadTauFakeRateHistManager.addParameter<vdouble>("etaBins_lead", hadTauEtaBins_lead);
  cfg_selHadTauFakeRateHistManager.addParameter<vdouble>("etaBins_sublead", hadTauEtaBins_sublead);
  HadTauFakeRateHistManager selHadTauFakeRateHistManager(cfg_selHadTauFakeRateHistManager);
  selHadTauFakeRateHistManager.bookHistograms(fs);

  JetHistManager selJetHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/jets", charge_and_hadTauSelection.data()), central_or_shift));
  selJetHistManager.bookHistograms(fs);
  JetHistManager selJetHistManager_lead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/leadJet", charge_and_hadTauSelection.data()), central_or_shift, 0));
  selJetHistManager_lead.bookHistograms(fs);
  JetHistManager selJetHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/subleadJet", charge_and_hadTauSelection.data()), central_or_shift, 1));
  selJetHistManager_sublead.bookHistograms(fs);

  JetHistManager selBJet_looseHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/BJets_loose", charge_and_hadTauSelection.data()), central_or_shift));
  selBJet_looseHistManager.bookHistograms(fs);
  JetHistManager selBJet_looseHistManager_lead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/leadBJet_loose", charge_and_hadTauSelection.data()), central_or_shift, 0));
  selBJet_looseHistManager_lead.bookHistograms(fs);
  JetHistManager selBJet_looseHistManager_sublead(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/subleadBJet_loose", charge_and_hadTauSelection.data()), central_or_shift, 1));
  selBJet_looseHistManager_sublead.bookHistograms(fs);
  JetHistManager selBJet_mediumHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/BJets_medium", charge_and_hadTauSelection.data()), central_or_shift));
  selBJet_mediumHistManager.bookHistograms(fs);

  MEtHistManager selMEtHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/met", charge_and_hadTauSelection.data()), central_or_shift));
  selMEtHistManager.bookHistograms(fs);

  EvtHistManager_2l_2tau selEvtHistManager(makeHistManager_cfg(process_string, 
    Form("2l_2tau_%s/sel/evt", charge_and_hadTauSelection.data()), central_or_shift));
  selEvtHistManager.bookHistograms(fs);
  std::map<std::string, EvtHistManager_2l_2tau*> selEvtHistManager_decayMode; // key = decay mode
  const std::map<std::string, GENHIGGSDECAYMODE_TYPE> decayMode_idString = {
    { "ttH_hww", static_cast<GENHIGGSDECAYMODE_TYPE>(24) },
    { "ttH_hzz", static_cast<GENHIGGSDECAYMODE_TYPE>(23) },
    { "ttH_htt", static_cast<GENHIGGSDECAYMODE_TYPE>(15) }
  };
  vstring decayModes_evt;
  decayModes_evt.reserve(decayMode_idString.size());
  boost::copy(decayMode_idString | boost::adaptors::map_keys, std::back_inserter(decayModes_evt));
  if ( isSignal ) {
    for ( vstring::const_iterator decayMode = decayModes_evt.begin();
          decayMode != decayModes_evt.end(); ++decayMode) {
      EvtHistManager_2l_2tau* selEvtHistManager_ptr = new EvtHistManager_2l_2tau(makeHistManager_cfg(decayMode->data(),
        Form("2l_2tau_%s/sel/evt", charge_and_hadTauSelection.data()), central_or_shift));
      selEvtHistManager_ptr->bookHistograms(fs);
      selEvtHistManager_decayMode[*decayMode] = selEvtHistManager_ptr;
    }
  }

  int numEntries = inputTree->GetEntries();
  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  cutFlowTableType cutFlowTable;
  for ( int idxEntry = 0; idxEntry < numEntries && (maxEvents == -1 || idxEntry < maxEvents); ++idxEntry ) {
    if ( idxEntry > 0 && (idxEntry % reportEvery) == 0 ) {
      std::cout << "processing Entry " << idxEntry << " (" << selectedEntries << " Entries selected)" << std::endl;
    }
    ++analyzedEntries;
    
    inputTree->GetEntry(idxEntry);

    if ( run_lumi_eventSelector && !(*run_lumi_eventSelector)(run, lumi, event) ) continue;
    cutFlowTable.update("run:ls:event selection");

    bool isTriggered_1e = hltPaths_isTriggered(triggers_1e);
    bool isTriggered_2e = hltPaths_isTriggered(triggers_2e);
    bool isTriggered_1mu = hltPaths_isTriggered(triggers_1mu);
    bool isTriggered_2mu = hltPaths_isTriggered(triggers_2mu);
    bool isTriggered_1e1mu = hltPaths_isTriggered(triggers_1e1mu);

    bool selTrigger_1e = use_triggers_1e && isTriggered_1e;
    bool selTrigger_2e = use_triggers_2e && isTriggered_2e;
    bool selTrigger_1mu = use_triggers_1mu && isTriggered_1mu;
    bool selTrigger_2mu = use_triggers_2mu && isTriggered_2mu;
    bool selTrigger_1e1mu = use_triggers_1e1mu && isTriggered_1e1mu;
    if ( !(selTrigger_1e || selTrigger_2e || selTrigger_1mu || selTrigger_2mu || selTrigger_1e1mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection." << std::endl; 
	std::cout << " (selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu 
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    }
    
//--- rank triggers by priority and ignore triggers of lower priority if a trigger of higher priority has fired for given event;
//    the ranking of the triggers is as follows: 2mu, 1e1mu, 2e, 1mu, 1e
// CV: this logic is necessary to avoid that the same event is selected multiple times when processing different primary datasets
    if ( !isMC ) {
      if ( selTrigger_1e && (isTriggered_2e || isTriggered_1mu || isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1e = " << selTrigger_1e 
		    << ", isTriggered_2e = " << isTriggered_2e 
		    << ", isTriggered_1mu = " << isTriggered_1mu 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_2e && (isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_2e = " << selTrigger_2e 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_1mu && (isTriggered_2e || isTriggered_2mu || isTriggered_1e1mu) ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1mu = " << selTrigger_1mu 
		    << ", isTriggered_2e = " << isTriggered_2e 
		    << ", isTriggered_2mu = " << isTriggered_2mu 
		    << ", isTriggered_1e1mu = " << isTriggered_1e1mu << ")" << std::endl;
	}
	continue; 
      }
      if ( selTrigger_1e1mu && isTriggered_2mu ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS trigger selection." << std::endl; 
	  std::cout << " (selTrigger_1e1mu = " << selTrigger_1e1mu 
		    << ", isTriggered_2mu = " << isTriggered_2mu << ")" << std::endl;
	}
	continue; 
      }
    }
    cutFlowTable.update("trigger");

    if ( (selTrigger_2e    && !apply_offline_e_trigger_cuts_2e)    ||
	 (selTrigger_1e1mu && !apply_offline_e_trigger_cuts_1e1mu) ||
	 (selTrigger_1e    && !apply_offline_e_trigger_cuts_1e)    ) {
      tightElectronSelector.disable_offline_e_trigger_cuts();
    } else {
      tightElectronSelector.enable_offline_e_trigger_cuts();
    }

//--- build collections of electrons, muons and hadronic taus;
//    resolve overlaps in order of priority: muon, electron,
    std::vector<RecoMuon> muons = muonReader->read();
    std::vector<const RecoMuon*> muon_ptrs = convert_to_ptrs(muons);
    std::vector<const RecoMuon*> cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
    std::vector<const RecoMuon*> preselMuons = preselMuonSelector(cleanedMuons);
    std::vector<const RecoMuon*> selMuons = tightMuonSelector(preselMuons);
    
    std::vector<RecoElectron> electrons = electronReader->read();
    std::vector<const RecoElectron*> electron_ptrs = convert_to_ptrs(electrons);
    std::vector<const RecoElectron*> cleanedElectrons = electronCleaner(electron_ptrs, selMuons);
    std::vector<const RecoElectron*> preselElectrons = preselElectronSelector(cleanedElectrons);
    std::vector<const RecoElectron*> selElectrons = tightElectronSelector(preselElectrons);

    std::vector<RecoHadTau> hadTaus = hadTauReader->read();
    std::vector<const RecoHadTau*> hadTau_ptrs = convert_to_ptrs(hadTaus);
    std::vector<const RecoHadTau*> cleanedHadTaus = hadTauCleaner(hadTau_ptrs, selMuons, selElectrons);
    std::vector<const RecoHadTau*> preselHadTaus = preselHadTauSelector(cleanedHadTaus);
    std::vector<const RecoHadTau*> fakeableHadTaus_lead = fakeableHadTauSelector_lead(preselHadTaus);
    std::vector<const RecoHadTau*> fakeableHadTaus_sublead = hadTauCleaner(fakeableHadTauSelector_sublead(preselHadTaus), fakeableHadTaus_lead);
    assert(fakeableHadTaus_lead.size() <= 1 && fakeableHadTaus_sublead.size() <= 1);
    std::vector<const RecoHadTau*> fakeableHadTaus;
    fakeableHadTaus.insert(fakeableHadTaus.end(), fakeableHadTaus_lead.begin(), fakeableHadTaus_lead.end());
    fakeableHadTaus.insert(fakeableHadTaus.end(), fakeableHadTaus_sublead.begin(), fakeableHadTaus_sublead.end());
    std::vector<const RecoHadTau*> tightHadTaus_lead = tightHadTauSelector_lead(preselHadTaus);
    std::vector<const RecoHadTau*> tightHadTaus_sublead = hadTauCleaner(tightHadTauSelector_sublead(preselHadTaus), tightHadTaus_lead);
    assert(tightHadTaus_lead.size() <= 1 && tightHadTaus_sublead.size() <= 1);
    std::vector<const RecoHadTau*> tightHadTaus;
    tightHadTaus.insert(tightHadTaus.end(), tightHadTaus_lead.begin(), tightHadTaus_lead.end());
    tightHadTaus.insert(tightHadTaus.end(), tightHadTaus_sublead.begin(), tightHadTaus_sublead.end());
    std::vector<const RecoHadTau*> selHadTaus_lead;
    std::vector<const RecoHadTau*> selHadTaus_sublead;
    std::vector<const RecoHadTau*> selHadTaus;
    if ( hadTauSelection == kLoose ) {
      std::vector<const RecoHadTau*> preselHadTaus_lead;
      if ( preselHadTaus.size() >= 1 ) preselHadTaus_lead.push_back(preselHadTaus[0]);
      selHadTaus_lead = preselHadTaus_lead;
      std::vector<const RecoHadTau*> preselHadTaus_sublead;
      if ( preselHadTaus.size() >= 2 ) preselHadTaus_sublead.push_back(preselHadTaus[1]);
      selHadTaus_sublead = preselHadTaus_sublead;
      selHadTaus = preselHadTaus;
    } else if ( hadTauSelection == kFakeable ) {
      selHadTaus_lead = fakeableHadTaus_lead;
      selHadTaus_sublead = fakeableHadTaus_sublead;
      selHadTaus = fakeableHadTaus;
    } else if ( hadTauSelection == kTight ) {
      selHadTaus_lead = tightHadTaus_lead;
      selHadTaus_sublead = tightHadTaus_sublead;
      selHadTaus = tightHadTaus;
    } else assert(0);

//--- build collections of jets and select subset of jets passing b-tagging criteria
    std::vector<RecoJet> jets = jetReader->read();
    std::vector<const RecoJet*> jet_ptrs = convert_to_ptrs(jets);
    std::vector<const RecoJet*> cleanedJets = jetCleaner(jet_ptrs, selMuons, selElectrons, selHadTaus);
    std::vector<const RecoJet*> selJets = jetSelector(cleanedJets);
    std::vector<const RecoJet*> selBJets_loose = jetSelectorBtagLoose(cleanedJets);
    std::vector<const RecoJet*> selBJets_medium = jetSelectorBtagMedium(cleanedJets);    

//--- build collections of generator level particles
    std::vector<GenLepton> genLeptons;
    std::vector<GenLepton> genElectrons;
    std::vector<GenLepton> genMuons;
    std::vector<GenHadTau> genHadTaus;
    std::vector<GenJet> genJets;
    if ( isMC ) {
      genLeptons = genLeptonReader->read();
      for ( std::vector<GenLepton>::const_iterator genLepton = genLeptons.begin();
	    genLepton != genLeptons.end(); ++genLepton ) {
	int abs_pdgId = std::abs(genLepton->pdgId_);
	if      ( abs_pdgId == 11 ) genElectrons.push_back(*genLepton);
	else if ( abs_pdgId == 13 ) genMuons.push_back(*genLepton);
      }
      genHadTaus = genHadTauReader->read();
      genJets = genJetReader->read();
    }

//--- match reconstructed to generator level particles
    if ( isMC ) {
      muonGenMatcher.addGenLeptonMatch(preselMuons, genLeptons, 0.3);
      muonGenMatcher.addGenHadTauMatch(preselMuons, genHadTaus, 0.3);
      muonGenMatcher.addGenJetMatch(preselMuons, genJets, 0.5);

      electronGenMatcher.addGenLeptonMatch(preselElectrons, genLeptons, 0.3);
      electronGenMatcher.addGenHadTauMatch(preselElectrons, genHadTaus, 0.3);
      electronGenMatcher.addGenJetMatch(preselElectrons, genJets, 0.5);

      hadTauGenMatcher.addGenLeptonMatch(preselHadTaus, genLeptons, 0.3);
      hadTauGenMatcher.addGenHadTauMatch(preselHadTaus, genHadTaus, 0.3);
      hadTauGenMatcher.addGenJetMatch(preselHadTaus, genJets, 0.5);

      jetGenMatcher.addGenLeptonMatch(selJets, genLeptons, 0.3);
      jetGenMatcher.addGenHadTauMatch(selJets, genHadTaus, 0.3);
      jetGenMatcher.addGenJetMatch(selJets, genJets, 0.5);
    }

//--- apply preselection
    std::vector<const RecoLepton*> preselLeptons;    
    preselLeptons.reserve(preselElectrons.size() + preselMuons.size());
    preselLeptons.insert(preselLeptons.end(), preselElectrons.begin(), preselElectrons.end());
    preselLeptons.insert(preselLeptons.end(), preselMuons.begin(), preselMuons.end());
    std::sort(preselLeptons.begin(), preselLeptons.end(), isHigherPt);
    // require exactly one lepton passing loose preselection criteria
    if ( !(preselLeptons.size() == 2) ) continue;
    cutFlowTable.update("2 presel lepton");
    const RecoLepton* preselLepton_lead = preselLeptons[0];
    int preselLepton_lead_type = getLeptonType(preselLepton_lead->pdgId_);
    const RecoLepton* preselLepton_sublead = preselLeptons[1];
    int preselLepton_sublead_type = getLeptonType(preselLepton_sublead->pdgId_);

    // require that trigger paths match event category (with event category based on preselLeptons);
    if ( preselElectrons.size() == 2 && !(selTrigger_1e  || selTrigger_2e) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given preselLepton multiplicity." << std::endl; 
	std::cout << " (#preselElectrons = " << preselElectrons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e << ")" << std::endl;
      }
      continue;
    }
    if ( preselMuons.size() == 2 && !(selTrigger_1mu || selTrigger_2mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given preselLepton multiplicity." << std::endl; 
	std::cout << " (#preselMuons = " << preselMuons.size() 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu << ")" << std::endl;
      }
      continue;
    }
    if ( preselElectrons.size() == 1 && preselMuons.size() == 1 && !(selTrigger_1e  || selTrigger_1mu || selTrigger_1e1mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given preselLepton multiplicity." << std::endl; 
	std::cout << " (#preselElectrons = " << preselElectrons.size() 
		  << ", #preselMuons = " << preselMuons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_1mu = " << selTrigger_1mu
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("presel lepton trigger match");

    // require presence of at least two hadronic taus passing loose preselection criteria
    // (do not veto events with more than two loosely selected hadronic tau candidates,
    //  as sample of hadronic tau candidates passing loose preselection criteria contains significant contamination from jets)
    std::sort(preselHadTaus.begin(), preselHadTaus.end(), isHigherPt);
    if ( !(preselHadTaus.size() >= 2) ) continue;
    cutFlowTable.update(">= 2 presel taus");
    const RecoHadTau* preselHadTau_lead = preselHadTaus[0];
    const RecoHadTau* preselHadTau_sublead = preselHadTaus[1];
    double mTauTauVis_presel = (preselHadTau_lead->p4_ + preselHadTau_sublead->p4_).mass();

    // apply requirement on jets (incl. b-tagged jets) on preselection level
    if ( !(selJets.size() >= 2) ) continue;
    cutFlowTable.update(">= 2 jets (1)");
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1) ) continue;
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (1)");

//--- compute MHT and linear MET discriminant (met_LD)
    LV mht_p4(0,0,0,0);
    for ( std::vector<const RecoJet*>::const_iterator jet = selJets.begin();
	  jet != selJets.end(); ++jet ) {
      mht_p4 += (*jet)->p4_;
    }
    for ( std::vector<const RecoLepton*>::const_iterator lepton = preselLeptons.begin();
	  lepton != preselLeptons.end(); ++lepton ) {
      mht_p4 += (*lepton)->p4_;
    }
    for ( std::vector<const RecoHadTau*>::const_iterator hadTau = selHadTaus.begin();
	  hadTau != selHadTaus.end(); ++hadTau ) {
      mht_p4 += (*hadTau)->p4_;
    }
    double met_LD = met_coef*met_p4.pt() + mht_coef*mht_p4.pt();    

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method", 
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
    double evtWeight = lumiScale;
    for ( std::vector<const RecoJet*>::const_iterator jet = selJets.begin();
	  jet != selJets.end(); ++jet ) {
      evtWeight *= (*jet)->BtagWeight_;
    }

//--- apply data/MC corrections for trigger efficiency,
//    and efficiencies for lepton to pass loose identification and isolation criteria
    if ( isMC ) {
      evtWeight *= sf_triggerEff(2,
        preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_);
      evtWeight *= sf_leptonID_and_Iso_loose(2,
        preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_);
    }

//--- fill histograms with events passing preselection
    preselMuonHistManager.fillHistograms(preselMuons, evtWeight);
    preselElectronHistManager.fillHistograms(preselElectrons, evtWeight);
    preselHadTauHistManager.fillHistograms(preselHadTaus, evtWeight);
    preselHadTauHistManager_lead.fillHistograms(preselHadTaus, evtWeight);
    preselHadTauHistManager_sublead.fillHistograms(preselHadTaus, evtWeight);
    preselJetHistManager.fillHistograms(selJets, evtWeight);
    selBJet_looseHistManager.fillHistograms(selBJets_loose, evtWeight);
    selBJet_mediumHistManager.fillHistograms(selBJets_medium, evtWeight);
    preselMEtHistManager.fillHistograms(met_p4, mht_p4, met_LD, evtWeight);
    preselEvtHistManager.fillHistograms(preselElectrons.size(), preselMuons.size(), selHadTaus.size(), 
      selJets.size(), selBJets_loose.size(), selBJets_medium.size(),
      mTauTauVis_presel, evtWeight);

//--- apply final event selection 
    std::vector<const RecoLepton*> selLeptons;    
    selLeptons.reserve(selElectrons.size() + selMuons.size());
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
    selLeptons.insert(selLeptons.end(), selMuons.begin(), selMuons.end());
    std::sort(selLeptons.begin(), selLeptons.end(), isHigherPt);
    // require exactly two leptons passing tight selection criteria of final event selection 
    if ( !(selLeptons.size() == 2) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selLeptons selection." << std::endl;
	std::cout << " (#selLeptons = " << selLeptons.size() << ")" << std::endl;
	for ( size_t idxSelLepton = 0; idxSelLepton < selLeptons.size(); ++idxSelLepton ) {
	  std::cout << "selLepton #" << idxSelLepton << ":" << std::endl;
	  std::cout << (*selLeptons[idxSelLepton]);
	}
      }
      continue;
    }
    cutFlowTable.update("2 sel leptons", evtWeight);
    const RecoLepton* selLepton_lead = selLeptons[0];
    const RecoLepton* selLepton_sublead = selLeptons[1];

    // require that trigger paths match event category (with event category based on selLeptons);
    if ( selElectrons.size() == 2 && !(selTrigger_1e  || selTrigger_2e) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given selLepton multiplicity." << std::endl; 
	std::cout << " (#selElectrons = " << selElectrons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_2e = " << selTrigger_2e << ")" << std::endl;
      }
      continue;
    }
    if ( selMuons.size() == 2 && !(selTrigger_1mu || selTrigger_2mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given selLepton multiplicity." << std::endl; 
	std::cout << " (#selMuons = " << selMuons.size() 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_2mu = " << selTrigger_2mu << ")" << std::endl;
      }
      continue;
    }
    if ( selElectrons.size() == 1 && selMuons.size() == 1 && !(selTrigger_1e  || selTrigger_1mu || selTrigger_1e1mu) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS trigger selection for given selLepton multiplicity." << std::endl; 
	std::cout << " (#selElectrons = " << selElectrons.size() 
		  << ", #selMuons = " << selMuons.size() 
		  << ", selTrigger_1e = " << selTrigger_1e 
		  << ", selTrigger_1mu = " << selTrigger_1mu 
		  << ", selTrigger_1e1mu = " << selTrigger_1e1mu << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("sel lepton trigger match", evtWeight);

    // require presence of exactly two hadronic taus passing tight selection criteria of final event selection
    if ( !(selHadTaus_lead.size() >= 1 && selHadTaus_sublead.size() >= 1) ) continue;
    cutFlowTable.update(">= 2 sel taus", evtWeight);
    const RecoHadTau* selHadTau_lead = selHadTaus_lead[0];
    bool isGenHadTauMatched_lead = selHadTau_lead->genHadTau_;
    bool isGenLeptonMatched_lead = selHadTau_lead->genLepton_ && !isGenHadTauMatched_lead;
    const RecoHadTau* selHadTau_sublead = selHadTaus_sublead[0];
    bool isGenHadTauMatched_sublead = selHadTau_sublead->genHadTau_;
    bool isGenLeptonMatched_sublead = selHadTau_sublead->genLepton_ && !isGenHadTauMatched_sublead;
    
    bool isGen_t = isGenHadTauMatched_lead && isGenHadTauMatched_sublead;
    bool isGen_l = (isGenLeptonMatched_lead || isGenLeptonMatched_sublead) && !isGen_t;
    bool isGen_j = !(isGen_t || isGen_l);
    if ( hadTauGenMatch == kGenLepton && !isGen_l ) continue;
    if ( hadTauGenMatch == kGenHadTau && !isGen_t ) continue;
    if ( hadTauGenMatch == kGenJet    && !isGen_j ) continue;
    cutFlowTable.update("tau gen match", evtWeight);

    double mTauTauVis = (selHadTau_lead->p4_ + selHadTau_sublead->p4_).mass();

    // apply requirement on jets (incl. b-tagged jets) and hadronic taus on level of final event selection
    if ( !(selJets.size() >= 2) ) continue;
    cutFlowTable.update(">= 2 jets (2)", evtWeight);
    if ( !(selBJets_loose.size() >= 2 || selBJets_medium.size() >= 1) ) continue;
    cutFlowTable.update(">= 2 loose b-jets || 1 medium b-jet (2)", evtWeight);
    if ( !(selHadTaus.size() >= 1) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS selHadTaus selection." << std::endl;
	std::cout << " (#selHadTaus = " << selHadTaus.size() << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(">= 1 sel tau (2)", evtWeight);
    
    bool failsLowMassVeto = false;
    for ( std::vector<const RecoLepton*>::const_iterator lepton1 = selLeptons.begin();
	  lepton1 != selLeptons.end(); ++lepton1 ) {
      for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
	    lepton2 != selLeptons.end(); ++lepton2 ) {
	if ( ((*lepton1)->p4_ + (*lepton2)->p4_).mass() < 12. ) {
	  failsLowMassVeto = true;
	}
      }
    }
    if ( failsLowMassVeto ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS low mass lepton pair veto." << std::endl;
      }
      continue;
    }
    cutFlowTable.update("m(ll) > 12 GeV", evtWeight);

    double minPt_lead = 20.;
    double minPt_sublead = selLepton_sublead->is_electron() ? 15. : 10.;
    if ( !(selLepton_lead->pt_ > minPt_lead && selLepton_sublead->pt_ > minPt_sublead) ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton pT selection." << std::endl;
	std::cout << " (leading selLepton pT = " << selLepton_lead->pt_ << ", minPt_lead = " << minPt_lead
		  << ", subleading selLepton pT = " << selLepton_sublead->pt_ << ", minPt_sublead = " << minPt_sublead << ")" << std::endl;
      }
      continue;
    }
    cutFlowTable.update("lead lepton pT > 20 GeV && sublead lepton pT > 15(e)/10(mu) GeV", evtWeight);

    bool isCharge_lepton_SS = selLepton_lead->charge_*selLepton_sublead->charge_ > 0;
    bool isCharge_lepton_OS = selLepton_lead->charge_*selLepton_sublead->charge_ < 0;
    if ( chargeSelection_lepton == kOS && isCharge_lepton_SS ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge_ 
		  << ", subleading selLepton charge = " << selLepton_sublead->charge_ << ", chargeSelection_lepton = OS)" << std::endl;
      }
      continue;
    }
    if ( chargeSelection_lepton == kSS && isCharge_lepton_OS ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS lepton charge selection." << std::endl;
	std::cout << " (leading selLepton charge = " << selLepton_lead->charge_ 
		  << ", subleading selLepton charge = " << selLepton_sublead->charge_ << ", chargeSelection_lepton = SS)" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(Form("lepton-pair %s charge", chargeSelection_lepton_string.data()), evtWeight);

    if ( selLepton_lead->is_electron() && selLepton_sublead->is_electron() ) {
      bool failsZbosonMassVeto = false;
      for ( std::vector<const RecoLepton*>::const_iterator lepton1 = selLeptons.begin();
	    lepton1 != selLeptons.end(); ++lepton1 ) {
	for ( std::vector<const RecoLepton*>::const_iterator lepton2 = lepton1 + 1;
	      lepton2 != selLeptons.end(); ++lepton2 ) {
	  if ( std::fabs(((*lepton1)->p4_ + (*lepton2)->p4_).mass() - z_mass) < z_window ) {
	    failsZbosonMassVeto = true;
	  }
	}
      }
      if ( failsZbosonMassVeto ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS Z-boson veto." << std::endl;
	}
	continue;
      }
    }
    cutFlowTable.update("Z-boson mass veto", evtWeight);
    
    bool isCharge_hadTau_SS = selHadTau_lead->charge_*selHadTau_sublead->charge_ > 0;
    bool isCharge_hadTau_OS = selHadTau_lead->charge_*selHadTau_sublead->charge_ < 0;
    if ( chargeSelection_hadTau == kOS && isCharge_hadTau_SS ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS hadTau charge selection." << std::endl;
	std::cout << " (leading selHadTau charge = " << selHadTau_lead->charge_ 
		  << ", subleading selHadTau charge = " << selHadTau_sublead->charge_ << ", chargeSelection_hadTau = SS)" << std::endl;
      }
      continue;
    }
    if ( chargeSelection_hadTau == kSS && isCharge_hadTau_OS ) {
      if ( run_lumi_eventSelector ) {
	std::cout << "event FAILS hadTau charge selection." << std::endl;
	std::cout << " (leading selHadTau charge = " << selHadTau_lead->charge_ 
		  << ", subleading selHadTau charge = " << selHadTau_sublead->charge_ << ", chargeSelection_hadTau = OS)" << std::endl;
      }
      continue;
    }
    cutFlowTable.update(Form("tau-pair %s charge", chargeSelection_hadTau_string.data()), evtWeight);

    if ( (chargeSelection_lepton == kOS && chargeSelection_hadTau == kOS) ||
	 (chargeSelection_lepton == kSS && chargeSelection_hadTau == kSS) ) {
      if ( (selLepton_lead->charge_ + selLepton_sublead->charge_ + selHadTau_lead->charge_+ selHadTau_sublead->charge_) != 0 ) {
	if ( run_lumi_eventSelector ) {
	  std::cout << "event FAILS lepton+tau charge selection." << std::endl;
	  std::cout << " (leading selLepton charge = " << selLepton_lead->charge_ 
		    << ", subleading selLepton charge = " << selLepton_sublead->charge_ 
		    << ", leading selHadTau charge = " << selHadTau_lead->charge_ 
		    << ", subleading selHadTau charge = " << selHadTau_sublead->charge_ << ")" << std::endl;
	}
	continue;
      }
      cutFlowTable.update("lepton+tau charge");
    }

    if ( hadTauSelection == kFakeable ) {
      if ( tightHadTaus_lead.size() >= 1 && tightHadTaus_sublead.size() >= 1 ) continue; // CV: avoid overlap with signal region
      cutFlowTable.update("signal region veto", evtWeight);
    }

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the tight identification and isolation criteria
    if ( isMC ) {
      double sf_tight_to_loose = sf_leptonID_and_Iso_tight_to_loose(2,
        preselLepton_lead_type, preselLepton_lead->pt_, preselLepton_lead->eta_, 
	preselLepton_sublead_type, preselLepton_sublead->pt_, preselLepton_sublead->eta_);
      evtWeight *= sf_tight_to_loose;
    }

    if ( applyJetToTauFakeRateWeight ) {
      double selHadTau_lead_pt = selHadTau_lead->pt_;
      double selHadTau_lead_absEta = std::fabs(selHadTau_lead->eta_);
      double selHadTau_sublead_pt = selHadTau_sublead->pt_;
      double selHadTau_sublead_absEta = std::fabs(selHadTau_sublead->eta_);
      particleIDlooseToTightWeightEntryType* jetToTauFakeRateWeight_tauEtaBin = 0;
      for ( std::vector<particleIDlooseToTightWeightEntryType*>::const_iterator jetToTauFakeRateWeight = jetToTauFakeRateWeights.begin();
            jetToTauFakeRateWeight != jetToTauFakeRateWeights.end(); ++jetToTauFakeRateWeight ) {
        if ( ((*jetToTauFakeRateWeight)->particle1EtaMin_ < 0. || selHadTau_lead_absEta    > (*jetToTauFakeRateWeight)->particle1EtaMin_) &&
             ((*jetToTauFakeRateWeight)->particle1EtaMax_ > 5. || selHadTau_lead_absEta    < (*jetToTauFakeRateWeight)->particle1EtaMax_) &&
             ((*jetToTauFakeRateWeight)->particle2EtaMin_ < 0. || selHadTau_sublead_absEta > (*jetToTauFakeRateWeight)->particle2EtaMin_) &&
             ((*jetToTauFakeRateWeight)->particle2EtaMax_ > 5. || selHadTau_sublead_absEta < (*jetToTauFakeRateWeight)->particle2EtaMax_) ) {
          jetToTauFakeRateWeight_tauEtaBin = (*jetToTauFakeRateWeight);
          break;
        }
      }
      if ( jetToTauFakeRateWeight_tauEtaBin ) {
        evtWeight *= jetToTauFakeRateWeight_tauEtaBin->weight(selHadTau_lead_pt, selHadTau_sublead_pt);
      } else {
        std::cerr << "Warning: leadHadTauEta = " << selHadTau_lead_absEta << ", subleadHadTauEta = " << selHadTau_sublead_absEta << " outside range !!" << std::endl;
      }
    }

//--- fill histograms with events passing final selection 
    selMuonHistManager.fillHistograms(selMuons, evtWeight);
    selElectronHistManager.fillHistograms(selElectrons, evtWeight);
    selHadTauHistManager_lead.fillHistograms(selHadTaus_lead, evtWeight);
    selHadTauHistManager_sublead.fillHistograms(selHadTaus_sublead, evtWeight);
    selHadTauHistManager.fillHistograms(selHadTaus_lead, evtWeight);
    selHadTauHistManager.fillHistograms(selHadTaus_sublead, evtWeight);
    selHadTauFakeRateHistManager.fillHistograms(selHadTau_lead, selHadTau_sublead, evtWeight);
    selJetHistManager.fillHistograms(selJets, evtWeight);
    selJetHistManager_lead.fillHistograms(selJets, evtWeight);
    selJetHistManager_sublead.fillHistograms(selJets, evtWeight);
    selBJet_looseHistManager.fillHistograms(selBJets_loose, evtWeight);
    selBJet_looseHistManager_lead.fillHistograms(selBJets_loose, evtWeight);
    selBJet_looseHistManager_sublead.fillHistograms(selBJets_loose, evtWeight);
    selBJet_mediumHistManager.fillHistograms(selBJets_medium, evtWeight);
    selMEtHistManager.fillHistograms(met_p4, mht_p4, met_LD, evtWeight);
    selEvtHistManager.fillHistograms(preselElectrons.size(), preselMuons.size(), selHadTaus.size(), 
      selJets.size(), selBJets_loose.size(), selBJets_medium.size(),
      mTauTauVis, evtWeight);
    if ( isSignal ) {
      for ( const auto & kv: decayMode_idString ) {
        if ( std::fabs(genHiggsDecayMode - kv.second) < EPS ) {
          selEvtHistManager_decayMode[kv.first]->fillHistograms(preselElectrons.size(), preselMuons.size(), selHadTaus.size(), 
            selJets.size(), selBJets_loose.size(), selBJets_medium.size(),
	    mTauTauVis, evtWeight);
          break;
        }
      }
    }

    (*selEventsFile) << run << ":" << lumi << ":" << event;

    ++selectedEntries;
    selectedEntries_weighted += evtWeight;
  }

  std::cout << "num. Entries = " << numEntries << std::endl;
  std::cout << " analyzed = " << analyzedEntries << std::endl;
  std::cout << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "cut-flow table" << std::endl;
  cutFlowTable.print(std::cout);
  std::cout << std::endl;

  delete run_lumi_eventSelector;

  delete selEventsFile;

  delete muonReader;
  delete electronReader;
  delete hadTauReader;
  delete jetReader;
  delete genLeptonReader;
  delete genHadTauReader;
  delete genJetReader;

  hltPaths_delete(triggers_1e);
  hltPaths_delete(triggers_2e);
  hltPaths_delete(triggers_1mu);
  hltPaths_delete(triggers_2mu);
  hltPaths_delete(triggers_1e1mu);
  
  clock.Show("analyze_2l_2tau");

  return EXIT_SUCCESS;
}
