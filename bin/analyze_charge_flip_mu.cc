#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h" // edm::readPSetsFrom()
#include "FWCore/Utilities/interface/Exception.h" // cms::Exception
#include "PhysicsTools/FWLite/interface/TFileService.h" // fwlite::TFileService
#include "DataFormats/FWLite/interface/InputSource.h" // fwlite::InputSource
#include "DataFormats/FWLite/interface/OutputFiles.h" // fwlite::OutputFiles
#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector

#include <Rtypes.h> // Int_t, Long64_t, Double_t
#include <TChain.h> // TChain
#include <TTree.h> // TTree
#include <TBenchmark.h> // TBenchmark
#include <TString.h> // TString, Form
#include <TEfficiency.h>
#include <TError.h> // gErrorAbortLevel, kError

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h" // RecoHadTau
#include "tthAnalysis/HiggsToTauTau/interface/GenLepton.h" // GenLepton
#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTau.h" // GenHadTau
#include "tthAnalysis/HiggsToTauTau/interface/TMVAInterface.h" // TMVAInterface
#include "tthAnalysis/HiggsToTauTau/interface/mvaInputVariables.h" // auxiliary functions for computing input variables of the MVA used for signal extraction in the 2lss_1tau category
#include "tthAnalysis/HiggsToTauTau/interface/KeyTypes.h"
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronReader.h" // RecoElectronReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonReader.h" // RecoMuonReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTauReader.h" // GenHadTauReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoReader.h" // LHEInfoReader
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionCleaner.h" // RecoElectronCollectionCleaner, RecoMuonCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleaner
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/MuonHistManager.h" // MuonHistManager
#include "tthAnalysis/HiggsToTauTau/interface/HadTauHistManager.h" // HadTauHistManager
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManager.h" // JetHistManager
#include "tthAnalysis/HiggsToTauTau/interface/MEtHistManager.h" // MEtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/GenEvtHistManager.h" // GenEvtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoHistManager.h" // LHEInfoHistManager
#include "tthAnalysis/HiggsToTauTau/interface/leptonTypes.h" // getLeptonType, kElectron, kMuon
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // getBranchName_bTagWeight, isHigherPt, isMatched
#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath, create_hltPaths, hltPaths_setBranchAddresses, hltPaths_isTriggered, hltPaths_delete
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface.h" // Data_to_MC_CorrectionInterface.h
#include "tthAnalysis/HiggsToTauTau/interface/leptonGenMatchingAuxFunctions.h" // getLeptonGenMatch_definitions_1lepton, getLeptonGenMatch_string, getLeptonGenMatch_int
#include "tthAnalysis/HiggsToTauTau/interface/hadTauGenMatchingAuxFunctions.h" // getHadTauGenMatch_definitions_1tau, getHadTauGenMatch_string, getHadTauGenMatch_int
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEtReader.h" // RecoMEtReader

#include <iostream> // std::cerr, std::fixed
#include <cstring> // std::strncpm
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

const std::string hadTauSelection = "dR03mvaTight";

/**
 * @brief Produce histograms for the charge flip background.
 */
int main(int argc, char* argv[])
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "<analyze_charge_flip_mu>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("analyze_charge_flip_mu");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") )
    throw cms::Exception("analyze_charge_flip_mu")
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  edm::ParameterSet cfg_analyze = cfg.getParameter<edm::ParameterSet>("analyze_charge_flip");
  std::string treeName = cfg_analyze.getParameter<std::string>("treeName");
  std::string process_string = cfg_analyze.getParameter<std::string>("process");

  std::string era_string = cfg_analyze.getParameter<std::string>("era");
  int era = -1;
  if      ( era_string == "2015" ) era = kEra_2015;
  else if ( era_string == "2016" ) era = kEra_2016;
  else throw cms::Exception("analyze_charge_flip_mu") 
    << "Invalid Configuration parameter 'era' = " << era_string << " !!\n";

  std::string histogramDir = "charge_flip";

  vstring triggerNames_1mu = cfg_analyze.getParameter<vstring>("triggers_1mu");
  std::vector<hltPath*> triggers_1mu = create_hltPaths(triggerNames_1mu);
  bool use_triggers_1mu = true; //cfg_analyze.getParameter<bool>("use_triggers_1mu");
  vstring triggerNames_2mu = cfg_analyze.getParameter<vstring>("triggers_2mu");
  std::vector<hltPath*> triggers_2mu = create_hltPaths(triggerNames_2mu);
  bool use_triggers_2mu = true; //cfg_analyze.getParameter<bool>("use_triggers_2mu");

  enum { kLoose, kFakeable, kTight };
  std::string leptonSelection_string = cfg_analyze.getParameter<std::string>("leptonSelection");
  int leptonSelection = -1;
  if      ( leptonSelection_string == "Loose"    ) leptonSelection = kLoose;
  else if ( leptonSelection_string == "Fakeable" ) leptonSelection = kFakeable;
  else if ( leptonSelection_string == "Tight"    ) leptonSelection = kTight;
  else throw cms::Exception("analyze_charge_flip_mu")
    << "Invalid Configuration parameter 'leptonSelection' = " << leptonSelection_string << " !!\n";

  bool apply_leptonGenMatching = true; //cfg_analyze.getParameter<bool>("apply_leptonGenMatching");
  std::vector<leptonGenMatchEntry> leptonGenMatch_definitions = getLeptonGenMatch_definitions_2lepton(apply_leptonGenMatching);
  std::cout << "leptonGenMatch_definitions:" << std::endl;
  std::cout << leptonGenMatch_definitions;

  bool apply_hadTauGenMatching = false;//cfg_analyze.getParameter<bool>("apply_hadTauGenMatching");
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions = getHadTauGenMatch_definitions_1tau(apply_hadTauGenMatching);
  std::cout << "hadTauGenMatch_definitions:" << std::endl;
  std::cout << hadTauGenMatch_definitions;


  bool isMC = cfg_analyze.getParameter<bool>("isMC");
  std::string central_or_shift = cfg_analyze.getParameter<std::string>("central_or_shift");
  std::string central_or_shift_label = central_or_shift == "central" ? "" : "_"+central_or_shift;
  std::string central_or_shift_label_st = central_or_shift == "central" ? "" : central_or_shift+"_";
  double lumiScale = ( process_string != "data_obs" ) ? cfg_analyze.getParameter<double>("lumiScale") : 1.;
  bool apply_trigger_bits = cfg_analyze.getParameter<bool>("apply_trigger_bits"); 

  std::string jet_btagWeight_branch;
  if ( isMC ) {
    if      ( era == kEra_2015 ) jet_btagWeight_branch = "Jet_bTagWeight";
    else if ( era == kEra_2016 ) jet_btagWeight_branch = "Jet_btagWeightCSV";
    else assert(0);
  }

  int jetPt_option = RecoJetReader::kJetPt_central;
  //int lheScale_option = kLHE_scale_central;
  TString central_or_shift_tstring = central_or_shift.data();
  if ( isMC && central_or_shift != "central" ) {
    std::string shiftUp_or_Down = "";
    if      ( central_or_shift_tstring.EndsWith("Up")   ) shiftUp_or_Down = "Up";
    else if ( central_or_shift_tstring.EndsWith("Down") ) shiftUp_or_Down = "Down";
    else throw cms::Exception("analyze_charge_flip_mu")
      << "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
    if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_btag") ) {
      jet_btagWeight_branch = getBranchName_bTagWeight(era, central_or_shift);
    } else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_JES") ) {
      jet_btagWeight_branch = getBranchName_bTagWeight(era, central_or_shift);
      if      ( shiftUp_or_Down == "Up"   ) jetPt_option = RecoJetReader::kJetPt_jecUp;
      else if ( shiftUp_or_Down == "Down" ) jetPt_option = RecoJetReader::kJetPt_jecDown;
      else assert(0);
    } /*else if ( central_or_shift_tstring.BeginsWith("CMS_ttHl_thu_shape") ) {
      if      ( central_or_shift_tstring.EndsWith("x1Down") ) lheScale_option = kLHE_scale_xDown;
      else if ( central_or_shift_tstring.EndsWith("x1Up")   ) lheScale_option = kLHE_scale_xUp;
      else if ( central_or_shift_tstring.EndsWith("y1Down") ) lheScale_option = kLHE_scale_yDown;
      else if ( central_or_shift_tstring.EndsWith("y1Up")   ) lheScale_option = kLHE_scale_yUp;
      else assert(0);
    } */else if (!central_or_shift_tstring.BeginsWith("CMS_ttHl_muonE")) throw cms::Exception("analyze_charge_flip_mu")
	<< "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift << " !!\n";
  }

  edm::ParameterSet cfg_dataToMCcorrectionInterface;
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("era", era_string);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("hadTauSelection", hadTauSelection);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("central_or_shift", central_or_shift);
  Data_to_MC_CorrectionInterface* dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface(cfg_dataToMCcorrectionInterface);

  bool fillGenEvtHistograms = cfg_analyze.getParameter<bool>("fillGenEvtHistograms");

  std::string selEventsFileName_input = cfg_analyze.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;

  if ( selEventsFileName_input != "" ) {
    edm::ParameterSet cfgRunLumiEventSelector;
    cfgRunLumiEventSelector.addParameter<std::string>("inputFileName", selEventsFileName_input);
    cfgRunLumiEventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfgRunLumiEventSelector);
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
    throw cms::Exception("analyze_charge_flip_mu")
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

  hltPaths_setBranchAddresses(inputTree, triggers_1mu);
  hltPaths_setBranchAddresses(inputTree, triggers_2mu);

  PUWEIGHT_TYPE pileupWeight;
  GENWEIGHT_TYPE genWeight = 1.;
  
  if ( isMC ) {
    inputTree->SetBranchAddress(GENWEIGHT_KEY, &genWeight);
    inputTree->SetBranchAddress(PUWEIGHT_KEY, &pileupWeight);
  }

//--- declare particle collections
  RecoMuonReader* muonReader = new RecoMuonReader(era, "nselLeptons", "selLeptons");
  muonReader->setBranchAddresses(inputTree);
  RecoMuonCollectionGenMatcher muonGenMatcher;
  RecoMuonCollectionSelectorLoose preselMuonSelector(era);
  RecoMuonCollectionSelectorFakeable fakeableMuonSelector(era);
  RecoMuonCollectionSelectorTight tightMuonSelector(era);

  RecoElectronReader* electronReader = new RecoElectronReader(era, "nselLeptons", "selLeptons");
  electronReader->setBranchAddresses(inputTree);
  RecoElectronCollectionGenMatcher electronGenMatcher;
  RecoElectronCollectionCleaner electronCleaner(0.3);
  RecoElectronCollectionSelectorLoose preselElectronSelector(era);
  RecoElectronCollectionSelectorFakeable fakeableElectronSelector(era);
  RecoElectronCollectionSelectorTight tightElectronSelector(era);

  RecoHadTauReader* hadTauReader = new RecoHadTauReader(era, "nTauGood", "TauGood");
  hadTauReader->setHadTauPt_central_or_shift(RecoHadTauReader::kHadTauPt_central);
  hadTauReader->setBranchAddresses(inputTree);
  RecoHadTauCollectionGenMatcher hadTauGenMatcher;
  RecoHadTauCollectionCleaner hadTauCleaner(0.3);
  RecoHadTauCollectionSelectorTight hadTauSelector(era);
  hadTauSelector.set(hadTauSelection);

  RecoJetReader* jetReader = new RecoJetReader(era, isMC, "nJet", "Jet");
  jetReader->setJetPt_central_or_shift(jetPt_option);
  jetReader->setBranchName_BtagWeight(jet_btagWeight_branch);
  jetReader->setBranchAddresses(inputTree);
  RecoJetCollectionGenMatcher jetGenMatcher;
  RecoJetCollectionCleaner jetCleaner(0.4);
  RecoJetCollectionSelector jetSelector(era);
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose(era);
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium(era);

  std::string branchName_met = "met";
  RecoMEtReader* metReader = new RecoMEtReader(era, branchName_met);
  metReader->setBranchAddresses(inputTree);

  GenLeptonReader* genLeptonReader = 0;
  GenHadTauReader* genHadTauReader = 0;
  GenJetReader* genJetReader = 0;
  LHEInfoReader* lheInfoReader = 0;
  if ( isMC ) {
    genLeptonReader = new GenLeptonReader("nGenLep", "GenLep", "nGenLepFromTau", "GenLepFromTau");
    genLeptonReader->setBranchAddresses(inputTree);
    genHadTauReader = new GenHadTauReader("nGenHadTaus", "GenHadTaus");
    genHadTauReader->setBranchAddresses(inputTree);
    genJetReader = new GenJetReader("nGenJet", "GenJet");
    genJetReader->setBranchAddresses(inputTree);
    lheInfoReader = new LHEInfoReader();
    lheInfoReader->setBranchAddresses(inputTree);
  }

//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = new std::ofstream(selEventsFileName_output.data(), std::ios::out);

//--- declare histograms
  std::string charge_and_leptonSelectionSS = Form("%s_%s", "SS", leptonSelection_string.data());
  std::string charge_and_leptonSelectionOS = Form("%s_%s", "OS", leptonSelection_string.data());
  MuonHistManager preselMuonHistManagerSS(makeHistManager_cfg(process_string,
    Form("charge_flip_%s/presel/muons", charge_and_leptonSelectionSS.data()), central_or_shift.data()));
  preselMuonHistManagerSS.bookHistograms(fs);
  MuonHistManager preselMuonHistManagerOS(makeHistManager_cfg(process_string,
    Form("charge_flip_%s/presel/muons", charge_and_leptonSelectionOS.data()), central_or_shift.data()));
  preselMuonHistManagerOS.bookHistograms(fs);


  struct selHistManagerType
  {
    MuonHistManager* muons_;
    HadTauHistManager* hadTaus_;
    JetHistManager* jets_;
    JetHistManager* BJets_loose_;
    JetHistManager* BJets_medium_;
    MEtHistManager* met_;
    //EvtHistManager_2lss_1tau* evt_;
  };
  //typedef std::map<int, selHistManagerType*> int_to_selHistManagerMap;
  //std::map<int, int_to_selHistManagerMap> selHistManagers;
  std::map<int, selHistManagerType*> selHistManagers;

  if (central_or_shift == "central"){
    for ( std::vector<leptonGenMatchEntry>::const_iterator leptonGenMatch_definition = leptonGenMatch_definitions.begin();
	  leptonGenMatch_definition != leptonGenMatch_definitions.end(); ++leptonGenMatch_definition ) {
      for ( std::vector<hadTauGenMatchEntry>::const_iterator hadTauGenMatch_definition = hadTauGenMatch_definitions.begin();
	    hadTauGenMatch_definition != hadTauGenMatch_definitions.end(); ++hadTauGenMatch_definition ) {

        std::string process_and_genMatch = process_string;
        if ( apply_leptonGenMatching ) process_and_genMatch += leptonGenMatch_definition->name_;
        if ( apply_leptonGenMatching && apply_hadTauGenMatching ) process_and_genMatch += "&";
        if ( apply_hadTauGenMatching ) process_and_genMatch += hadTauGenMatch_definition->name_;

        int idxLepton = leptonGenMatch_definition->idx_;
        //int idxHadTau = hadTauGenMatch_definition->idx_;

        selHistManagerType* selHistManager = new selHistManagerType();
        //selHistManager->electrons_ = new ElectronHistManager(makeHistManager_cfg(process_and_genMatch, 
        //  Form("%s/presel/electrons", histogramDir.data()), central_or_shift));
        //selHistManager->electrons_->bookHistograms(fs);
        selHistManager->muons_ = new MuonHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/muons", histogramDir.data()), central_or_shift));
        selHistManager->muons_->bookHistograms(fs);
        selHistManager->hadTaus_ = new HadTauHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/hadTaus", histogramDir.data()), central_or_shift));
        selHistManager->hadTaus_->bookHistograms(fs);
        selHistManager->jets_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/jets", histogramDir.data()), central_or_shift));
        selHistManager->jets_->bookHistograms(fs);
        selHistManager->BJets_loose_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/BJets_loose", histogramDir.data()), central_or_shift));
        selHistManager->BJets_loose_->bookHistograms(fs);
        selHistManager->BJets_medium_ = new JetHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/BJets_medium", histogramDir.data()), central_or_shift));
        selHistManager->BJets_medium_->bookHistograms(fs);
        selHistManager->met_ = new MEtHistManager(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/sel/met", histogramDir.data()), central_or_shift));
        selHistManager->met_->bookHistograms(fs);
        /*selHistManager->evt_ = new EvtHistManager_2lss_1tau(makeHistManager_cfg(process_and_genMatch, 
          Form("%s/presel/evt", histogramDir.data()), era_string, central_or_shift));
        selHistManager->evt_->bookHistograms(fs);*/
        selHistManagers[idxLepton] = selHistManager;

      /*  	  std::string decayMode_and_genMatch = (*decayMode);
	    if ( apply_leptonGenMatching ) decayMode_and_genMatch += leptonGenMatch_definition->name_;
	    if ( apply_leptonGenMatching && apply_hadTauGenMatching ) decayMode_and_genMatch += "&";
	    if ( apply_hadTauGenMatching ) decayMode_and_genMatch += hadTauGenMatch_definition->name_;
      */
      }
    }
  }


  vstring categories_etapt = {  //B-barrel, E-endcap, L-low pT (10 <= pT < 25), M-med pT (25 <= pT < 50), H-high pT (pT > 50)
    "BB_LL", "BB_ML", "BB_MM", "BB_HL", "BB_HM", "BB_HH", "EE_LL", "EE_ML", "EE_MM", "EE_HL", "EE_HM", "EE_HH", "BE_LL", "BE_ML", "EB_ML", "BE_MM", "BE_HL", "EB_HL", "BE_HM", "EB_HM", "BE_HH", "total"
  };
  vstring categories_charge = {
    "SS", "OS"
  };
  std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, TH1D*>>>> histos;
  //std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, TH1I*>>>> histos_I;
  std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, TH2D*>>>> histos_2D;
  std::map<std::string, std::map<std::string, std::map<std::string, TH1D*>>> histos_2gen;
  
  
  std::string which = "OS";
  TFileDirectory subDir = fs.mkdir( which.data() );
  //TFileDirectory subDir2 = subDir.mkdir(process_string);
  TFileDirectory subD1 = fs.mkdir("gen");
  TFileDirectory subD = subD1.mkdir(which.data());

  for ( vstring::const_iterator category = categories_etapt.begin(); 	category != categories_etapt.end(); ++category ) {
    TFileDirectory subDir1_5 = subDir.mkdir(category->data());
    TFileDirectory subDir2 = subDir1_5.mkdir(process_string.data());
    TFileDirectory subD2 = subD.mkdir(category->data());
    
    histos[which.data()][*category][process_string]["mass_ll"] = subDir2.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 60,  60., 120. );
    histos[which.data()][*category][process_string]["mass_ll"]->Sumw2();
    
    if (central_or_shift == "central") {
      histos[which.data()][*category][process_string]["delta_phi"] = subDir2.make<TH1D>( Form("%sdelta_phi", central_or_shift_label_st.data()), "delta_phi", 40,  -TMath::Pi(), TMath::Pi() );
      histos[which.data()][*category][process_string]["mass_2"] = subDir2.make<TH1D>( Form("%smass_2", central_or_shift_label_st.data()), "m2_{ll}", 50,  10., 210. );
      histos_2D[which.data()][*category][process_string]["mass_vs_dphi"] = subDir2.make<TH2D>( Form("%smass2_vs_delta_phi", central_or_shift_label_st.data()), "mass2_vs_deltaphi", 50,  10., 210., 40,  -TMath::Pi(), TMath::Pi() );
      histos_2D[which.data()][*category][process_string]["mass_vs_njets"] = subDir2.make<TH2D>( Form("%smass2_vs_njets", central_or_shift_label_st.data()), "mass2_vs_njets", 50,  10., 210., 5,  -0.5, 4.5 );
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_njets"] = subDir2.make<TH2D>( Form("%sdeltaphi_vs_njets", central_or_shift_label_st.data()), "deltaphi_vs_njets", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_bjetsl"] = subDir2.make<TH2D>( Form("%sdeltaphi_vs_bjets_loose", central_or_shift_label_st.data()), "deltaphi_vs_bjets_loose", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["delta_phi"]->Sumw2();
      histos[which.data()][*category][process_string]["mass_2"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["mass_vs_dphi"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["mass_vs_njets"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_njets"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_bjetsl"]->Sumw2();
    
      histos[which.data()][*category][process_string]["nJets"] = subDir2.make<TH1D>( Form("%snJets", central_or_shift_label_st.data()), "n_j", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nBJetsLoose"] = subDir2.make<TH1D>( Form("%snBJetsLoose", central_or_shift_label_st.data()), "n_bjets_loos", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nBJetsMedium"] = subDir2.make<TH1D>( Form("%snBJetsMedium", central_or_shift_label_st.data()), "n_bjets_medium", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nHadTaus"] = subDir2.make<TH1D>( Form("%snHadTaus", central_or_shift_label_st.data()), "n_tau", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nJets"]->Sumw2();
      histos[which.data()][*category][process_string]["nBJetsLoose"]->Sumw2();
      histos[which.data()][*category][process_string]["nBJetsMedium"]->Sumw2();
      histos[which.data()][*category][process_string]["nHadTaus"]->Sumw2();
    }
    
    if (std::strncmp(process_string.data(), "DY", 2) == 0){
        TFileDirectory subDirFake = subDir1_5.mkdir("DY_fake");
        histos[which.data()][*category]["DY_fake"]["mass_ll"] = subDirFake.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 60,  60., 120. );
        histos[which.data()][*category]["DY_fake"]["mass_ll"]->Sumw2();
        if (central_or_shift == "central") {
          histos_2gen[which.data()][*category][process_string] = subD2.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 60,  60., 120. );
          histos_2gen[which.data()][*category][process_string]->Sumw2();
          
          histos[which.data()][*category]["DY_fake"]["delta_phi"] = subDirFake.make<TH1D>( Form("%sdelta_phi", central_or_shift_label_st.data()), "delta_phi", 40,  -TMath::Pi(), TMath::Pi() );
          histos[which.data()][*category]["DY_fake"]["mass_2"] = subDirFake.make<TH1D>( Form("%smass_2", central_or_shift_label_st.data()), "m2_{ll}", 50,  10., 210. );
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_dphi"] = subDirFake.make<TH2D>( Form("%smass2_vs_delta_phi", central_or_shift_label_st.data()), "mass2_vs_deltaphi", 50,  10., 210., 40,  -TMath::Pi(), TMath::Pi() );
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_njets"] = subDirFake.make<TH2D>( Form("%smass2_vs_njets", central_or_shift_label_st.data()), "mass2_vs_njets", 50,  10., 210., 5,  -0.5, 4.5 );
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_njets"] = subDirFake.make<TH2D>( Form("%sdeltaphi_vs_njets", central_or_shift_label_st.data()), "deltaphi_vs_njets", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_bjetsl"] = subDirFake.make<TH2D>( Form("%sdeltaphi_vs_bjets_loose", central_or_shift_label_st.data()), "deltaphi_vs_bjets_loose", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["delta_phi"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["mass_2"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_dphi"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_njets"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_njets"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_bjetsl"]->Sumw2();
        
          histos[which.data()][*category]["DY_fake"]["nJets"] = subDirFake.make<TH1D>( Form("%snJets", central_or_shift_label_st.data()), "n_j", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nBJetsLoose"] = subDirFake.make<TH1D>( Form("%snBJetsLoose", central_or_shift_label_st.data()), "n_bjets_loos", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nBJetsMedium"] = subDirFake.make<TH1D>( Form("%snBJetsMedium", central_or_shift_label_st.data()), "n_bjets_medium", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nHadTaus"] = subDirFake.make<TH1D>( Form("%snHadTaus", central_or_shift_label_st.data()), "n_tau", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nJets"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nBJetsLoose"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nBJetsMedium"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nHadTaus"]->Sumw2();            
        }
    }      
  }

  which = "SS";
  subDir = fs.mkdir( which.data() );
  //TFileDirectory subDir2 = subDir.mkdir(process_string);
  subD1 = fs.mkdir("gen");
  subD = subD1.mkdir( which.data());

  for ( vstring::const_iterator category = categories_etapt.begin(); 	category != categories_etapt.end(); ++category ) {
    TFileDirectory subDir1_5 = subDir.mkdir(category->data());
    TFileDirectory subDir2 = subDir1_5.mkdir(process_string.data());
    TFileDirectory subD2 = subD.mkdir(category->data());
    
    histos[which.data()][*category][process_string]["mass_ll"] = subDir2.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 1,  60., 120. );
    histos[which.data()][*category][process_string]["mass_ll"]->Sumw2();
    
    if (central_or_shift == "central") {
      histos[which.data()][*category][process_string]["delta_phi"] = subDir2.make<TH1D>( Form("%sdelta_phi", central_or_shift_label_st.data()), "delta_phi", 40,  -TMath::Pi(), TMath::Pi() );
      histos[which.data()][*category][process_string]["mass_2"] = subDir2.make<TH1D>( Form("%smass_2", central_or_shift_label_st.data()), "m2_{ll}", 50,  10., 210. );
      histos_2D[which.data()][*category][process_string]["mass_vs_dphi"] = subDir2.make<TH2D>( Form("%smass2_vs_delta_phi", central_or_shift_label_st.data()), "mass2_vs_deltaphi", 50,  10., 210., 40,  -TMath::Pi(), TMath::Pi() );
      histos_2D[which.data()][*category][process_string]["mass_vs_njets"] = subDir2.make<TH2D>( Form("%smass2_vs_njets", central_or_shift_label_st.data()), "mass2_vs_njets", 50,  10., 210., 5,  -0.5, 4.5 );
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_njets"] = subDir2.make<TH2D>( Form("%sdeltaphi_vs_njets", central_or_shift_label_st.data()), "deltaphi_vs_njets", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_bjetsl"] = subDir2.make<TH2D>( Form("%sdeltaphi_vs_bjets_loose", central_or_shift_label_st.data()), "deltaphi_vs_bjets_loose", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nJets"] = subDir2.make<TH1D>( Form("%snJets", central_or_shift_label_st.data()), "n_j", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nBJetsLoose"] = subDir2.make<TH1D>( Form("%snBJetsLoose", central_or_shift_label_st.data()), "n_bjets_loos", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nBJetsMedium"] = subDir2.make<TH1D>( Form("%snBJetsMedium", central_or_shift_label_st.data()), "n_bjets_medium", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["nHadTaus"] = subDir2.make<TH1D>( Form("%snHadTaus", central_or_shift_label_st.data()), "n_tau", 5,  -0.5, 4.5 );
      histos[which.data()][*category][process_string]["delta_phi"]->Sumw2();
      histos[which.data()][*category][process_string]["mass_2"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["mass_vs_dphi"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["mass_vs_njets"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_njets"]->Sumw2();
      histos_2D[which.data()][*category][process_string]["deltaphi_vs_bjetsl"]->Sumw2();
      histos[which.data()][*category][process_string]["nJets"]->Sumw2();
      histos[which.data()][*category][process_string]["nBJetsLoose"]->Sumw2();
      histos[which.data()][*category][process_string]["nBJetsMedium"]->Sumw2();
      histos[which.data()][*category][process_string]["nHadTaus"]->Sumw2();
    }
    
    if (std::strncmp(process_string.data(), "DY", 2) == 0){
        TFileDirectory subDirFake = subDir1_5.mkdir("DY_fake");
        
        histos[which.data()][*category]["DY_fake"]["mass_ll"] = subDirFake.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 1,  60., 120. );
        histos[which.data()][*category]["DY_fake"]["mass_ll"]->Sumw2();
        
        if (central_or_shift == "central") {
          histos_2gen[which.data()][*category][process_string] = subD2.make<TH1D>( Form("%smass_ll", central_or_shift_label_st.data()), "m_{ll}", 1,  60., 120. );
          histos_2gen[which.data()][*category][process_string]->Sumw2();

          histos[which.data()][*category]["DY_fake"]["delta_phi"] = subDirFake.make<TH1D>( Form("%sdelta_phi", central_or_shift_label_st.data()), "delta_phi", 40,  -TMath::Pi(), TMath::Pi() );
          histos[which.data()][*category]["DY_fake"]["mass_2"] = subDirFake.make<TH1D>( Form("%smass_2", central_or_shift_label_st.data()), "m2_{ll}", 50,  10., 210. );
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_dphi"] = subDirFake.make<TH2D>( Form("%smass2_vs_delta_phi", central_or_shift_label_st.data()), "mass2_vs_deltaphi", 50,  10., 210., 40,  -TMath::Pi(), TMath::Pi() );
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_njets"] = subDirFake.make<TH2D>( Form("%smass2_vs_njets", central_or_shift_label_st.data()), "mass2_vs_njets", 50,  10., 210., 5,  -0.5, 4.5 );
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_njets"] = subDirFake.make<TH2D>( Form("%sdeltaphi_vs_njets", central_or_shift_label_st.data()), "mass2_vs_njets", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_bjetsl"] = subDirFake.make<TH2D>( Form("%sdeltaphi_vs_bjets_loose", central_or_shift_label_st.data()), "deltaphi_vs_bjets_loose", 40,  -TMath::Pi(), TMath::Pi(), 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["delta_phi"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["mass_2"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_dphi"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["mass_vs_njets"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_njets"]->Sumw2();
          histos_2D[which.data()][*category]["DY_fake"]["deltaphi_vs_bjetsl"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nJets"] = subDirFake.make<TH1D>( Form("%snJets", central_or_shift_label_st.data()), "n_j", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nBJetsLoose"] = subDirFake.make<TH1D>( Form("%snBJetsLoose", central_or_shift_label_st.data()), "n_bjets_loos", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nBJetsMedium"] = subDirFake.make<TH1D>( Form("%snBJetsMedium", central_or_shift_label_st.data()), "n_bjets_medium", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nHadTaus"] = subDirFake.make<TH1D>( Form("%snHadTaus", central_or_shift_label_st.data()), "n_tau", 5,  -0.5, 4.5 );
          histos[which.data()][*category]["DY_fake"]["nJets"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nBJetsLoose"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nBJetsMedium"]->Sumw2();
          histos[which.data()][*category]["DY_fake"]["nHadTaus"]->Sumw2();
            
          
        
        }
    }      
  }


  
  vstring categories_charge_gen = {
    "ID", "MisID"
  };

  TFileDirectory subDir_fails = fs.mkdir( Form("failed_cuts") );
  TH1I* fail_counter = subDir_fails.make<TH1I>( process_string.data(), "Rejected events", 11,  0, 11 );
  const char *rejcs[11] = {"Trigger", "Trig_2mu1mu", "2mu", "0e", "2mu_trig", "2mu_sel", "2mu_sel_trig", "0e_sel", "pT_mu", "tight_charge", "m_ll"};
  for (int i=1;i<=11;i++) fail_counter->GetXaxis()->SetBinLabel(i,rejcs[i-1]); 


  /*vstring categories_etapt_gen = {  //B-barrel, E-endcap, L-low pT (10 <= pT < 25), M-med pT (25 <= pT < 50), H-high pT (pT > 50)
    "B_L", "B_M", "B_H", "E_L", "E_M", "E_M", "E_H"
  };*/

  Double_t bins_eta[3] = {0, 1.479, 2.5};
  Double_t bins_pt[4] = {10, 25, 50, 1000};

  TEfficiency* gen_eff;
  std::map<std::string, TH2D*> histos_gen;
  if (isMC && central_or_shift == "central"){
    TFileDirectory subD = fs.mkdir("gen_ratio");
    TFileDirectory subD2 = subD.mkdir( process_string );
    for ( vstring::const_iterator which = categories_charge_gen.begin(); 	which != categories_charge_gen.end(); ++which ) {
      //TFileDirectory subDir = fs.mkdir( which->data() );
      histos_gen[which->data()] = subD2.make<TH2D>( Form("pt_eta_%s", which->data()), "pt_eta", 3,  bins_pt, 2, bins_eta );
      histos_gen[which->data()]->Sumw2();
    }
    gen_eff = subD.make<TEfficiency>(Form("pt_eta_%s", process_string.data()),"pt_eta;pT;#eta;charge_misID", 3,  bins_pt, 2, bins_eta);
    gen_eff->SetUseWeightedEvents();
    gen_eff->SetStatisticOption(TEfficiency::kFNormal);
  }

  GenEvtHistManager* genEvtHistManager_beforeCuts = 0;
  GenEvtHistManager* genEvtHistManager_afterCuts = 0;
  LHEInfoHistManager* lheInfoHistManager = 0;
  if ( isMC ) {
    genEvtHistManager_beforeCuts = new GenEvtHistManager(makeHistManager_cfg(process_string, 
      Form("charge_flip_%s/unbiased/genEvt", charge_and_leptonSelectionSS.data()), central_or_shift));
    genEvtHistManager_beforeCuts->bookHistograms(fs);
    genEvtHistManager_afterCuts = new GenEvtHistManager(makeHistManager_cfg(process_string, 
      Form("charge_flip_%s/sel/genEvt", charge_and_leptonSelectionSS.data()), central_or_shift));
    genEvtHistManager_afterCuts->bookHistograms(fs);
    lheInfoHistManager = new LHEInfoHistManager(makeHistManager_cfg(process_string, 
      Form("charge_flip_%s/sel/lheInfo", charge_and_leptonSelectionSS.data()), central_or_shift));
    lheInfoHistManager->bookHistograms(fs);
  }

  int numEntries = inputTree->GetEntries();
  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  for ( int idxEntry = 0; idxEntry < numEntries && (maxEvents == -1 || idxEntry < maxEvents); ++idxEntry ) {
    if ( idxEntry > 0 && (idxEntry % reportEvery) == 0 ) {
      std::cout << "processing Entry " << idxEntry << " (" << selectedEntries << " Entries selected)" << std::endl;
    }
    if(run_lumi_eventSelector && run_lumi_eventSelector -> areWeDone())
    {
      break;
    }
    ++analyzedEntries;

    inputTree->GetEntry(idxEntry);
    
    if ( run_lumi_eventSelector && !(*run_lumi_eventSelector)(run, lumi, event) ) continue;

//--- build collections of generator level particles (before any cuts are applied, to check distributions in unbiased event samples)
    std::vector<GenLepton> genLeptons;
    std::vector<GenLepton> genElectrons;
    std::vector<GenLepton> genMuons;
    std::vector<GenHadTau> genHadTaus;
    std::vector<GenJet> genJets;
    if ( isMC && fillGenEvtHistograms ) {
      genLeptons = genLeptonReader->read();
      for ( std::vector<GenLepton>::const_iterator genLepton = genLeptons.begin();
    	    genLepton != genLeptons.end(); ++genLepton ) {
	          int abs_pdgId = std::abs(genLepton->pdgId());
	          if      ( abs_pdgId == 11 ) genElectrons.push_back(*genLepton);
	          else if ( abs_pdgId == 13 ) genMuons.push_back(*genLepton);
          }
      genHadTaus = genHadTauReader->read();
      genJets = genJetReader->read();
    }

    if ( isMC ) {
      genEvtHistManager_beforeCuts->fillHistograms(genElectrons, genMuons, genHadTaus, genJets);
    }

    bool isTriggered_1mu = hltPaths_isTriggered(triggers_1mu) || (isMC && !apply_trigger_bits);
    bool isTriggered_2mu = hltPaths_isTriggered(triggers_2mu) || (isMC && !apply_trigger_bits);
    
    bool selTrigger_1mu = use_triggers_1mu && isTriggered_1mu;
    bool selTrigger_2mu = use_triggers_2mu && isTriggered_2mu;
    if ( !(selTrigger_1mu || selTrigger_2mu) ) {
        if ( run_lumi_eventSelector ) {
	        std::cout << "event FAILS trigger selection." << std::endl; 
	        std::cout << " (selTrigger_1mu = " << isTriggered_1mu 
			  << ", selTrigger_2mu = " << isTriggered_2mu << ")" << std::endl;
			    //std::cout << hltPaths_isTriggered(triggers_1mu) << " " << isMC << " " << !apply_trigger_bits << " " << hltPaths_isTriggered(triggers_2mu) << " " << use_triggers_1mu << " " << use_triggers_2mu << std::endl;			 
        }
        if (central_or_shift == "central") fail_counter->Fill("Trigger", 1);
        continue;
    }
    
//--- rank triggers by priority and ignore triggers of lower priority if a trigger of higher priority has fired for given event;
//    the ranking of the triggers is as follows: 2mu, 1e1mu, 2e, 1mu, 1e
// CV: this logic is necessary to avoid that the same event is selected multiple times when processing different primary datasets
    if ( !isMC ) {
      if ( selTrigger_1mu && isTriggered_2mu ) {
	      if ( run_lumi_eventSelector ) {
	        std::cout << "event FAILS trigger selection." << std::endl; 
	        std::cout << " (selTrigger_1mu = " << selTrigger_1mu 
		          << ", isTriggered_2mu = " << isTriggered_2mu << ")" << std::endl;
	      }
        if (central_or_shift == "central") fail_counter->Fill("Trig_2mu1mu", 1);
	      continue; 
      }
  }

//--- build collections of electrons, muons and hadronic taus;
//    resolve overlaps in order of priority: muon, electron,
    std::vector<RecoMuon> muons = muonReader->read();
    std::vector<const RecoMuon*> muon_ptrs = convert_to_ptrs(muons);
    std::vector<const RecoMuon*> cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
    std::vector<const RecoMuon*> preselMuons = preselMuonSelector(cleanedMuons);
    std::vector<const RecoMuon*> fakeableMuons = fakeableMuonSelector(preselMuons);
    std::vector<const RecoMuon*> tightMuons = tightMuonSelector(preselMuons);
    std::vector<const RecoMuon*> selMuons;
    if      ( leptonSelection == kLoose    ) selMuons = preselMuons;
    else if ( leptonSelection == kFakeable ) selMuons = fakeableMuons;
    else if ( leptonSelection == kTight    ) selMuons = tightMuons;
    else assert(0);

    std::vector<RecoElectron> electrons = electronReader->read();
    std::vector<const RecoElectron*> electron_ptrs = convert_to_ptrs(electrons);
    std::vector<const RecoElectron*> cleanedElectrons = electronCleaner(electron_ptrs, selMuons);
    std::vector<const RecoElectron*> preselElectrons = preselElectronSelector(cleanedElectrons);
    std::vector<const RecoElectron*> fakeableElectrons = fakeableElectronSelector(preselElectrons);
    std::vector<const RecoElectron*> tightElectrons = tightElectronSelector(preselElectrons);
    std::vector<const RecoElectron*> selElectrons;
    if      ( leptonSelection == kLoose    ) selElectrons = preselElectrons;
    else if ( leptonSelection == kFakeable ) selElectrons = fakeableElectrons;
    else if ( leptonSelection == kTight    ) selElectrons = tightElectrons;
    else assert(0);

    std::vector<RecoHadTau> hadTaus = hadTauReader->read();
    std::vector<const RecoHadTau*> hadTau_ptrs = convert_to_ptrs(hadTaus);
    std::vector<const RecoHadTau*> cleanedHadTaus = hadTauCleaner(hadTau_ptrs, selMuons, selElectrons);
    std::vector<const RecoHadTau*> selHadTaus = hadTauSelector(cleanedHadTaus);

//--- build collections of jets and select subset of jets passing b-tagging criteria
    std::vector<RecoJet> jets = jetReader->read();
    std::vector<const RecoJet*> jet_ptrs = convert_to_ptrs(jets);
    std::vector<const RecoJet*> cleanedJets = jetCleaner(jet_ptrs, selMuons, selElectrons, selHadTaus);
    std::vector<const RecoJet*> selJets = jetSelector(cleanedJets);
    std::vector<const RecoJet*> selBJets_loose = jetSelectorBtagLoose(cleanedJets);
    std::vector<const RecoJet*> selBJets_medium = jetSelectorBtagMedium(cleanedJets);

//--- build collections of generator level particles (after some cuts are applied, to safe computing time)
    if ( isMC && !fillGenEvtHistograms ) {
      genLeptons = genLeptonReader->read();
      for ( std::vector<GenLepton>::const_iterator genLepton = genLeptons.begin();
    	    genLepton != genLeptons.end(); ++genLepton ) {
    	int abs_pdgId = std::abs(genLepton->pdgId());
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
      
      hadTauGenMatcher.addGenLeptonMatch(selHadTaus, genLeptons, 0.2);
      hadTauGenMatcher.addGenHadTauMatch(selHadTaus, genHadTaus, 0.2);
      hadTauGenMatcher.addGenJetMatch(selHadTaus, genJets, 0.2);

      jetGenMatcher.addGenLeptonMatch(selJets, genLeptons, 0.2);
      jetGenMatcher.addGenHadTauMatch(selJets, genHadTaus, 0.2);
      jetGenMatcher.addGenJetMatch(selJets, genJets, 0.2);
    }

//--- apply preselection
    std::vector<const RecoLepton*> preselLeptons;
    preselLeptons.reserve(preselElectrons.size() + preselMuons.size());
    preselLeptons.insert(preselLeptons.end(), preselElectrons.begin(), preselElectrons.end());
    preselLeptons.insert(preselLeptons.end(), preselMuons.begin(), preselMuons.end());
    std::sort(preselLeptons.begin(), preselLeptons.end(), isHigherPt);
    
    // require exactly two preselected muons
    if ( !(preselMuons.size() == 2) ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS 2 presel Muons selection" << std::endl; 
	      std::cout << " #preselMuons = " << preselMuons.size() 
		        << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("2mu", 1);
      continue;
    }
    const RecoLepton* preselLepton_lead = preselLeptons[0];
    int preselLepton_lead_type = getLeptonType(preselLepton_lead->pdgId());
    const RecoLepton* preselLepton_sublead = preselLeptons[1];
    int preselLepton_sublead_type = getLeptonType(preselLepton_sublead->pdgId());
    
    const leptonGenMatchEntry& preselLepton_genMatch = getLeptonGenMatch(leptonGenMatch_definitions, preselLepton_lead, preselLepton_sublead);
    int idxPreselLepton_genMatch = preselLepton_genMatch.idx_;
    assert(idxPreselLepton_genMatch != kGen_LeptonUndefined2);
    
    // require exactly zero preselected electrons
    if ( !(preselElectrons.size() == 0) ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS 0 presel Electrons selection" << std::endl; 
	      std::cout << " (#preselElectrons = " << preselElectrons.size() 
           << ")" << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("0e", 1);
      continue;
    }
    // require that trigger paths match event category (with event category based on preselLeptons);
    if ( preselMuons.size() == 2 && !(selTrigger_1mu  || selTrigger_2mu) ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS trigger selection for given preselLepton multiplicity." << std::endl; 
	      std::cout << " (#preselMuons = " << preselMuons.size() 
		        << ", selTrigger_1mu = " << selTrigger_1mu 
		        << ", selTrigger_2mu = " << selTrigger_2mu << ")" << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("2mu_trig", 1);
      continue;
    }
    
    /*if ( selHadTaus.size() >= 1 ) {
    const RecoHadTau* selHadTau = selHadTaus[0];
    const hadTauGenMatchEntry& selHadTau_genMatch = getHadTauGenMatch(hadTauGenMatch_definitions, selHadTau);
    int idxSelHadTau_genMatch = selHadTau_genMatch.idx_;
    assert(idxSelHadTau_genMatch != kGen_HadTauUndefined1);
    }*/
    
    //--- compute MHT and linear MET discriminant (met_LD)
    RecoMEt met = metReader->read();
    std::vector<const RecoLepton*> fakeableLeptons = mergeLeptonCollections(fakeableElectrons, fakeableMuons);
    Particle::LorentzVector mht_p4 = compMHT(fakeableLeptons, selHadTaus, selJets);
    double met_LD = compMEt_LD(met.p4(), mht_p4);
    
    if ( isMC ) {
      lheInfoReader->read();
    }

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method", 
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
    double evtWeight = 1.;
    if ( isMC ) {
      evtWeight *= lumiScale;
      evtWeight *= pileupWeight;
      evtWeight *= sgn(genWeight);
      for ( std::vector<const RecoJet*>::const_iterator jet = selJets.begin();
	    jet != selJets.end(); ++jet ) {
      	evtWeight *= (*jet)->BtagWeight();
      }
    }    

   if ( isMC ) {
      dataToMCcorrectionInterface->setLeptons(
        preselLepton_lead_type, preselLepton_lead->pt(), preselLepton_lead->eta(), 
	preselLepton_sublead_type, preselLepton_sublead->pt(), preselLepton_sublead->eta());

//--- apply trigger efficiency turn-on curves to Spring16 non-reHLT MC
      if ( !apply_trigger_bits ) {
	evtWeight *= dataToMCcorrectionInterface->getWeight_leptonTriggerEff();
      }

//--- apply data/MC corrections for trigger efficiency,
//    and efficiencies for lepton to pass loose identification and isolation criteria      
      evtWeight *= dataToMCcorrectionInterface->getSF_leptonTriggerEff();
      evtWeight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_loose();
    } 
  
//--- apply final event selection
    std::vector<const RecoLepton*> selLeptons;
    selLeptons.reserve(selElectrons.size() + selMuons.size());
    selLeptons.insert(selLeptons.end(), selElectrons.begin(), selElectrons.end());
    selLeptons.insert(selLeptons.end(), selMuons.begin(), selMuons.end());
    std::sort(selLeptons.begin(), selLeptons.end(), isHigherPt);
    // require exactly two electrons passing selection criteria of final event selection
    if ( selMuons.size() != 2 ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS selMuons selection." << std::endl;
	      std::cout << " (#selMuons = " << selMuons.size() << ")" << std::endl;
	      for ( size_t idxSelMuon = 0; idxSelMuon < selMuons.size(); ++idxSelMuon ) {
	        std::cout << "selMuon #" << idxSelMuon << ":" << std::endl;
	        std::cout << (*selMuons[idxSelMuon]);
	      }
        for ( size_t idxPreSelMuon = 0; idxPreSelMuon < preselMuons.size(); ++idxPreSelMuon ) {
	        std::cout << "preselMuon #" << idxPreSelMuon << ":" << std::endl;
	        std::cout << (*preselMuons[idxPreSelMuon]);
	      }
      }
      if (central_or_shift == "central") fail_counter->Fill("2mu_sel", 1);
      continue;
    }
    
    if ( selMuons.size() == 2 && !(selTrigger_1mu || selTrigger_2mu) ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS trigger selection for given selMuon multiplicity." << std::endl; 
	      std::cout << " (#selMuons = " << selMuons.size() 
		        << ", selTrigger_1mu = " << selTrigger_1mu 
		        << ", selTrigger_2mu = " << selTrigger_2mu << ")" << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("2mu_sel_trig", 1);
      continue;
    }

    if ( selElectrons.size() != 0) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS selection for selElectron = 0." << std::endl; 
	      std::cout << " (#selElectrons = " << selElectrons.size() << ")" << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("0e_sel", 1);
      continue;
    }

    const RecoLepton* selLepton_lead = selLeptons[0];
    const RecoLepton* selLepton_sublead = selLeptons[1];

    // require that trigger paths match event category (with event category based on selLeptons);
    /*if ( selElectrons.size() == 2 &&                         !(selTrigger_1e  || selTrigger_2e)                       ) continue;
    if ( selElectrons.size() == 1 && selMuons.size() == 1 && !(selTrigger_1e  || selTrigger_1mu || selTrigger_1e1mu) ) continue;
    */

    // Determine event category
    std::string stEta;
    std::string stLeadPt;
    std::string stSubPt;

    Double_t etaL0 = std::fabs(selLepton_lead->eta());
    Double_t etaL1 = std::fabs(selLepton_sublead->eta());
    
    double pt0, pt1;
    //std::cout << "Before " << selLepton_lead->pt() << ", " << selLepton_sublead->pt() << "   " << central_or_shift << std::endl;
    pt0 = selLepton_lead->pt();
    pt1 = selLepton_sublead->pt();
    if (central_or_shift == "CMS_ttHl_muonESBarrel1Up") {
      if (etaL0 < 0.9) pt0 *= 1.02;
      if (etaL1 < 0.9) pt1 *= 1.02;
    }
    else if (central_or_shift == "CMS_ttHl_muonESBarrel1Down"){
      if (etaL0 < 0.9) pt0 *= 0.98;
      if (etaL1 < 0.9) pt1 *= 0.98;
    }
    else if (central_or_shift == "CMS_ttHl_muonESBarrel2Up") {
      if (etaL0 >= 0.9 && etaL0 < 1.2) pt0 *= 1.02;
      if (etaL1 >= 0.9 && etaL1 < 1.2) pt1 *= 1.02;
    }
    else if (central_or_shift == "CMS_ttHl_muonESBarrel2Down"){
      if (etaL0 >= 0.9 && etaL0 < 1.2) pt0 *= 0.98;
      if (etaL1 >= 0.9 && etaL1 < 1.2) pt1 *= 0.98;
    }
    else if (central_or_shift == "CMS_ttHl_muonESEndcap1Up") {
      if (etaL0 >= 1.2 && etaL0 < 2.1) pt0 *= 1.02;
      if (etaL1 >= 1.2 && etaL1 < 2.1) pt1 *= 1.02;
    }
    else if (central_or_shift == "CMS_ttHl_muonESEndcap1Down"){
      if (etaL0 >= 1.2 && etaL0 < 2.1) pt0 *= 0.98;
      if (etaL1 >= 1.2 && etaL1 < 2.1) pt1 *= 0.98;
    }
    else if (central_or_shift == "CMS_ttHl_muonESEndcap2Up") {
      if (etaL0 >= 2.1) pt0 *= 1.02;
      if (etaL1 >= 2.1) pt1 *= 1.02;
    }
    else if (central_or_shift == "CMS_ttHl_muonESEndcap2Down"){
      if (etaL0 >= 2.1) pt0 *= 0.98;
      if (etaL1 >= 2.1) pt1 *= 0.98;
    }
    //std::cout << "After: " << pt0 << ", " << pt1 << std::endl;
    
    if(pt1 > pt0){ //Lepton pt order changed due to systematic
      double temp = pt1;
      pt1 = pt0;
      pt0 = temp;
      temp = etaL1;
      etaL1 = etaL0;
      etaL0 = temp;
    }
    
    double minPt_lead = 10.;
    double minPt_sublead = selLepton_sublead->is_electron() ? 15. : 10.;
    if ( !(pt0 > minPt_lead && pt1 > minPt_sublead) ) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS lepton pT selection." << std::endl;
	      std::cout << " (leading selLepton pT = " << selLepton_lead->pt() << ", minPt_lead = " << minPt_lead
		        << ", subleading selLepton pT = " << selLepton_sublead->pt() << ", minPt_sublead = " << minPt_sublead << ")" << std::endl;
      }
      fail_counter->Fill("pT_mu", 1);
      continue;
    }
    
    
    bool failsTightChargeCut = false;
    for ( std::vector<const RecoLepton*>::const_iterator lepton = selLeptons.begin();
	    lepton != selLeptons.end(); ++lepton ) {
      if ( (*lepton)->is_electron() ) {
	      const RecoElectron* electron = dynamic_cast<const RecoElectron*>(*lepton);
	      assert(electron);
  	    if ( electron->tightCharge() < 2 ) failsTightChargeCut = true;
      }
      if ( (*lepton)->is_muon() ) {
	      const RecoMuon* muon = dynamic_cast<const RecoMuon*>(*lepton);
	      assert(muon);
	      if ( muon->tightCharge() < 2 ) failsTightChargeCut = true;
      }
    }
    if ( failsTightChargeCut ) {
      if ( run_lumi_eventSelector ) {
      	std::cout << "event FAILS tight lepton charge requirement." << std::endl;
      }
      continue;
    }
    //cutFlowTable.update("tight lepton charge", evtWeight);
    fail_counter->Fill("tight_charge", 1);
    
      
    bool isCharge_SS = selLepton_lead->charge()*selLepton_sublead->charge() > 0;
    bool isCharge_OS = selLepton_lead->charge()*selLepton_sublead->charge() < 0;

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the tight identification and isolation criteria
    if ( isMC ) {
      if ( leptonSelection == kFakeable ) {
	evtWeight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_fakeable_to_loose();
      } else if ( leptonSelection == kTight ) {
        evtWeight *= dataToMCcorrectionInterface->getSF_leptonID_and_Iso_tight_to_loose_woTightCharge();
      }
    }

//--- fill histograms with events passing final selection
//--- Calclulate mass of lepton system
    math::PtEtaPhiMLorentzVector p4 =
      math::PtEtaPhiMLorentzVector(pt0, preselMuons[0]->eta(), preselMuons[0]->phi(), preselMuons[0]->mass()) +
      math::PtEtaPhiMLorentzVector(pt1, preselMuons[1]->eta(), preselMuons[1]->phi(), preselMuons[1]->mass());
    Double_t mass_ll = p4.M();
    
    Double_t delta_phi = preselMuons[0]->phi() - preselMuons[1]->phi();
    if (delta_phi < -TMath::Pi()) delta_phi += 2*TMath::Pi();
    if (delta_phi > TMath::Pi()) delta_phi -= 2*TMath::Pi();
    math::PtEtaPhiMLorentzVector p4_2 =
      math::PtEtaPhiMLorentzVector(35., preselMuons[0]->eta(), preselMuons[0]->phi(), preselMuons[0]->mass()) +
      math::PtEtaPhiMLorentzVector(35., preselMuons[1]->eta(), preselMuons[1]->phi(), preselMuons[1]->mass());
    Double_t mass_2 = p4_2.M();
    
    //Adjust central value
    //mass_ll *= 1.02;
    
    if (pt0 >= 10 && pt0 < 25) stLeadPt = "L";
    else if (pt0 >= 25 && pt0 < 50) stLeadPt = "M";
    else if (pt0 > 50) stLeadPt = "H";
    if (pt1 >= 10 && pt1 < 25) stSubPt = "L";
    else if (pt1 >= 25 && pt1 < 50) stSubPt = "M";
    else if (pt1 > 50) stSubPt = "H";
    //else assert(0);

    if (etaL0 < 1.479 && etaL1 < 1.479) stEta = "BB";
    else if (etaL0 >= 1.479 && etaL1 >= 1.479) stEta = "EE";
    else if (etaL0 < etaL1) stEta = "BE";
    else
    {
      if (std::strncmp(stLeadPt.data(), stSubPt.data(), 1) == 0) stEta = "BE";       //Symmetric case
      else stEta = "EB";
    }
    //std::cout << stEta << " "<< stLeadPt  <<" " << stSubPt << " " << pt0 << " " << pt1 << " " << etaL0 << " " << etaL1 << std::endl;
  
    std::string category = Form("%s_%s%s", stEta.data(), stLeadPt.data(), stSubPt.data());

    std::string charge_cat = ( isCharge_SS ) ? "SS" : "OS";
  
    //Set all SS events that pass cuts to one bin and to pass selection
    if (isCharge_SS)
      mass_ll = 90.;
    
    if (mass_ll < 60 || mass_ll > 120) {
      if ( run_lumi_eventSelector ) {
	      std::cout << "event FAILS dilepton mass selection." << std::endl;
	      std::cout << " (dilepton mass = " << mass_ll << ")" << std::endl;
      }
      if (central_or_shift == "central") fail_counter->Fill("m_ll", 1);
      continue;
    }
    
    Int_t nJets = selJets.size();
    Int_t nBJetsLoose = selBJets_loose.size();
    Int_t nBJetsMedium = selBJets_medium.size();
    Int_t nHadTaus = selHadTaus.size();
    
    
    if (std::strncmp(process_string.data(), "DY", 2) == 0){    //Split DY
      const GenLepton *gp0 = preselMuons[0]->genLepton();
      const GenLepton *gp1 = preselMuons[1]->genLepton();
      //std::cout << gp0 << " " << gp1 << " " << abs(preselElectrons[0]->pdgId()) << " " << abs(gp0->pdgId()) << " " << abs(preselElectrons[1]->pdgId()) << " " << abs(gp1->pdgId()) << std::endl;
      if (gp0 != 0 && gp1 != 0 && abs(preselMuons[0]->pdgId()) == abs(gp0->pdgId()) && abs(preselMuons[1]->pdgId()) == abs(gp1->pdgId())) {
        math::PtEtaPhiMLorentzVector p4_gen =
            math::PtEtaPhiMLorentzVector(gp0->pt(), gp0->eta(), gp0->phi(), gp0->mass()) +
            math::PtEtaPhiMLorentzVector(gp1->pt(), gp1->eta(), gp1->phi(), gp1->mass());
        Double_t mass_ll_gen = p4_gen.M();
        
        //Adjust central value to better match data shape
        if (isCharge_OS)
        {
          if (central_or_shift == "CMS_ttHl_muonERDown")   
            mass_ll = mass_ll - 0.5 * (mass_ll - mass_ll_gen);
          else if (central_or_shift == "CMS_ttHl_muonERUp")
            mass_ll = mass_ll + 0.5 * (mass_ll - mass_ll_gen);
        }
        //std::cout << "After:  " << mass_ll << std::endl;
        histos[charge_cat][category.data()]["DY"]["mass_ll"]->Fill(mass_ll, evtWeight);
        histos[charge_cat]["total"]["DY"]["mass_ll"]->Fill(mass_ll, evtWeight);
        if (central_or_shift == "central"){
          histos[charge_cat][category.data()]["DY"]["delta_phi"]->Fill(delta_phi, evtWeight);
          histos[charge_cat]["total"]["DY"]["delta_phi"]->Fill(delta_phi, evtWeight);
          histos[charge_cat][category.data()]["DY"]["mass_2"]->Fill(mass_2, evtWeight);
          histos[charge_cat]["total"]["DY"]["mass_2"]->Fill(mass_2, evtWeight);
          histos_2D[charge_cat][category.data()]["DY"]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
          histos_2D[charge_cat]["total"]["DY"]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
          histos_2D[charge_cat][category.data()]["DY"]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight);
          histos_2D[charge_cat]["total"]["DY"]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight);
          histos_2D[charge_cat][category.data()]["DY"]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight);
          histos_2D[charge_cat]["total"]["DY"]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight);  
          histos_2D[charge_cat][category.data()]["DY"]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight);
          histos_2D[charge_cat]["total"]["DY"]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight);  
          
          histos[charge_cat][category.data()]["DY"]["nJets"]->Fill(nJets, evtWeight);
          histos[charge_cat]["total"]["DY"]["nJets"]->Fill(nJets, evtWeight);
          histos[charge_cat][category.data()]["DY"]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
          histos[charge_cat]["total"]["DY"]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
          histos[charge_cat][category.data()]["DY"]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
          histos[charge_cat]["total"]["DY"]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
          histos[charge_cat][category.data()]["DY"]["nHadTaus"]->Fill(nHadTaus, evtWeight);
          histos[charge_cat]["total"]["DY"]["nHadTaus"]->Fill(nHadTaus, evtWeight);
        }
      }
      else if (!central_or_shift_tstring.BeginsWith("CMS_ttHl_muonER")){
        histos[charge_cat][category.data()]["DY_fake"]["mass_ll"]->Fill(mass_ll, evtWeight);
        histos[charge_cat]["total"]["DY_fake"]["mass_ll"]->Fill(mass_ll, evtWeight);
        if (central_or_shift == "central"){
          histos[charge_cat][category.data()]["DY_fake"]["delta_phi"]->Fill(delta_phi, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["delta_phi"]->Fill(delta_phi, evtWeight);
          histos[charge_cat][category.data()]["DY_fake"]["mass_2"]->Fill(mass_2, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["mass_2"]->Fill(mass_2, evtWeight);
          histos_2D[charge_cat][category.data()]["DY_fake"]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
          histos_2D[charge_cat]["total"]["DY_fake"]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
          histos_2D[charge_cat][category.data()]["DY_fake"]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight);
          histos_2D[charge_cat]["total"]["DY_fake"]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight); 
          histos_2D[charge_cat][category.data()]["DY_fake"]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight);
          histos_2D[charge_cat]["total"]["DY_fake"]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight); 
          histos_2D[charge_cat][category.data()]["DY_fake"]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight);
          histos_2D[charge_cat]["total"]["DY_fake"]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight); 
          histos[charge_cat][category.data()]["DY_fake"]["nJets"]->Fill(nJets, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["nJets"]->Fill(nJets, evtWeight);
          histos[charge_cat][category.data()]["DY_fake"]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
          histos[charge_cat][category.data()]["DY_fake"]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
          histos[charge_cat][category.data()]["DY_fake"]["nHadTaus"]->Fill(nHadTaus, evtWeight);
          histos[charge_cat]["total"]["DY_fake"]["nHadTaus"]->Fill(nHadTaus, evtWeight);
        }
      }
    }
    else {//Otherwise
      histos[charge_cat][category.data()][process_string]["mass_ll"]->Fill(mass_ll, evtWeight);
      histos[charge_cat]["total"][process_string]["mass_ll"]->Fill(mass_ll, evtWeight);
      if (central_or_shift == "central"){
        histos[charge_cat][category.data()][process_string]["delta_phi"]->Fill(delta_phi, evtWeight);
        histos[charge_cat]["total"][process_string]["delta_phi"]->Fill(delta_phi, evtWeight);
        histos[charge_cat][category.data()][process_string]["mass_2"]->Fill(mass_2, evtWeight);
        histos[charge_cat]["total"][process_string]["mass_2"]->Fill(mass_2, evtWeight);
        histos_2D[charge_cat][category.data()][process_string]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
        histos_2D[charge_cat]["total"][process_string]["mass_vs_dphi"]->Fill(mass_2, delta_phi, evtWeight);
        histos_2D[charge_cat][category.data()][process_string]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight);
        histos_2D[charge_cat]["total"][process_string]["mass_vs_njets"]->Fill(mass_2, nJets, evtWeight); 
        histos_2D[charge_cat][category.data()][process_string]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight);
        histos_2D[charge_cat]["total"][process_string]["deltaphi_vs_njets"]->Fill(delta_phi, nJets, evtWeight); 
        histos_2D[charge_cat][category.data()][process_string]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight);
        histos_2D[charge_cat]["total"][process_string]["deltaphi_vs_bjetsl"]->Fill(delta_phi, nBJetsLoose, evtWeight); 
        histos[charge_cat][category.data()][process_string]["nJets"]->Fill(nJets, evtWeight);
        histos[charge_cat]["total"][process_string]["nJets"]->Fill(nJets, evtWeight);
        histos[charge_cat][category.data()][process_string]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
        histos[charge_cat]["total"][process_string]["nBJetsLoose"]->Fill(nBJetsLoose, evtWeight);
        histos[charge_cat][category.data()][process_string]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
        histos[charge_cat]["total"][process_string]["nBJetsMedium"]->Fill(nBJetsMedium, evtWeight);
        histos[charge_cat][category.data()][process_string]["nHadTaus"]->Fill(nHadTaus, evtWeight);
        histos[charge_cat]["total"][process_string]["nHadTaus"]->Fill(nHadTaus, evtWeight);
      }
    }    

    if (isMC && central_or_shift == "central" && std::strncmp(process_string.data(), "DY", 2) == 0)
    {
      for (int i = 0; i < 2; i++)
      {
        const GenLepton *gp = preselMuons[i]->genLepton();
        if (gp == 0)
        {
          continue;
        }
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        gen_eff->FillWeighted(preselMuons[i]->charge() != gp->charge(), evtWeight, gp->pt(), std::fabs(gp->eta()));
        if (preselMuons[i]->charge() == gp->charge())
        {
          histos_gen["ID"]->Fill(gp->pt(), std::fabs(gp->eta()), evtWeight);
        }
        if (preselMuons[i]->charge() != gp->charge())
        {
          histos_gen["MisID"]->Fill(gp->pt(), std::fabs(gp->eta()),evtWeight);
        }
        //else assert(0);
      }
      const GenLepton *gen0 = preselMuons[0]->genLepton();
      const GenLepton *gen1 = preselMuons[1]->genLepton();
      if (!(gen0 == 0 || gen1 == 0)){
        const GenLepton *gp0;
        const GenLepton *gp1;
        if (gen1->pt() > gen0->pt()){        
          gp0 = gen1;
          gp1 = gen0;
        }
        else {
          gp0 = gen0;
          gp1 = gen1;
        }
        std::string stEtaGen;
        std::string stLeadPtGen;
        std::string stSubPtGen;
        assert(gp0->pt() >= gp1->pt());
        if (gp0->pt() >= 10 && gp0->pt() < 25) stLeadPtGen = "L";
        else if (gp0->pt() >= 25 && gp0->pt() < 50) stLeadPtGen = "M";
        else if (gp0->pt() > 50) stLeadPtGen = "H";
        if (gp1->pt() >= 10 && gp1->pt() < 25) stSubPtGen = "L";
        else if (gp1->pt() >= 25 && gp1->pt() < 50) stSubPtGen = "M";
        else if (gp1->pt() > 50) stSubPtGen = "H";
        else{
          std::cout << "PT<10 " << gp0->pt() << " " << gp1->pt() << std::endl; 
          stSubPtGen = "L";
          //assert(0);
        }

        Double_t etaL0Gen = std::fabs(gp0->eta());
        Double_t etaL1Gen = std::fabs(gp1->eta());
        if (etaL0Gen < 1.479 && etaL1Gen < 1.479) stEtaGen = "BB";
        else if (etaL0Gen > 1.479 && etaL1Gen > 1.479) stEtaGen = "EE";
        else if (etaL0Gen < etaL1Gen) stEtaGen = "BE";
        else
        {
          if (std::strncmp(stLeadPtGen.data(), stSubPtGen.data(), 1) == 0) stEtaGen = "BE";       //Symmetric case
          else stEtaGen = "EB";
        }
        std::string categoryGen = Form("%s_%s%s", stEtaGen.data(), stLeadPtGen.data(), stSubPtGen.data());
        std::string charge_catGen = ( isCharge_SS ) ? "SS" : "OS";
        histos_2gen[charge_catGen][categoryGen.data()][process_string]->Fill(mass_ll, evtWeight);
        histos_2gen[charge_catGen]["total"][process_string]->Fill(mass_ll, evtWeight);
      }
    }
    if (isCharge_SS)
      preselMuonHistManagerSS.fillHistograms(preselMuons, evtWeight);
    else if (isCharge_OS)
      preselMuonHistManagerOS.fillHistograms(preselMuons, evtWeight);

    if ( isMC ) {
      genEvtHistManager_afterCuts->fillHistograms(genElectrons, genMuons, genHadTaus, genJets);
      lheInfoHistManager->fillHistograms(*lheInfoReader, evtWeight);
    }
    
    if(isCharge_OS == true && central_or_shift == "central"){
      selHistManagerType* selHistManager = selHistManagers[idxPreselLepton_genMatch];
      assert(selHistManager != 0);
      //selHistManager->electrons_->fillHistograms(preselElectrons, 1.);
      selHistManager->muons_->fillHistograms(selMuons, 1.);
      selHistManager->hadTaus_->fillHistograms(selHadTaus, 1.);
      selHistManager->jets_->fillHistograms(selJets, 1.);
      selHistManager->BJets_loose_->fillHistograms(selBJets_loose, 1.);
      selHistManager->BJets_medium_->fillHistograms(selBJets_medium, 1.);
      selHistManager->met_->fillHistograms(met, mht_p4, met_LD, 1.);
      /*selHistManager->evt_->fillHistograms(
        selElectrons.size(), selMuons.size(), selHadTaus.size(), 
        selJets.size(), selBJets_loose.size(), selBJets_medium.size(),
        -1., -1., -1., -1., -1., 
        mTauTauVis1_presel, mTauTauVis2_presel, 
        0, 1.);*/
    }
    (*selEventsFile) << run << ":" << lumi << ":" << event << std::endl;
    (*selEventsFile) << run << ":" << lumi << ":" << event << std::endl;

    ++selectedEntries;
    selectedEntries_weighted += evtWeight;
  }


  std::cout << "num. Entries = " << numEntries << std::endl;
  std::cout << " analyzed = " << analyzedEntries << std::endl;
  std::cout << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")" << std::endl;

  delete dataToMCcorrectionInterface;

  delete run_lumi_eventSelector;

  delete selEventsFile;

  delete muonReader;
  delete electronReader;
  delete hadTauReader;
  delete jetReader;
  delete genLeptonReader;
  delete genHadTauReader;
  delete genJetReader;
  delete lheInfoReader;

  delete genEvtHistManager_beforeCuts;
  delete genEvtHistManager_afterCuts;
  delete lheInfoHistManager;

  hltPaths_delete(triggers_1mu);
  hltPaths_delete(triggers_2mu);

  clock.Show("analyze_charge_flip_mu");

  return EXIT_SUCCESS;
}
