// latest
// Version: Gen to rec approach.
// data: 20180419, Siddhesh


#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h" // edm::readPSetsFrom()
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
#include <TMatrixD.h> // TMatrixD
#include <TError.h> // gErrorAbortLevel, kError
#include <TMath.h> // TMath::Pi()

#include "tthAnalysis/HiggsToTauTau/interface/RecoLepton.h" // RecoLepton
#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h" // RecoJet
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetAK8.h" // RecoJetAK8
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetAK12.h" // RecoJetAK12
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetHTTv2.h"
#include "tthAnalysis/HiggsToTauTau/interface/GenJet.h" // GenJet
#include "tthAnalysis/HiggsToTauTau/interface/GenHadTau.h" // GenHadTau
#include "tthAnalysis/HiggsToTauTau/interface/TMVAInterface.h" // TMVAInterface
#include "tthAnalysis/HiggsToTauTau/interface/mvaAuxFunctions.h" // check_mvaInputs, get_mvaInputVariables
#include "tthAnalysis/HiggsToTauTau/interface/mvaInputVariables.h" // auxiliary functions for computing input variables of the MVA used for signal extraction in the 2lss_1tau category
#include "tthAnalysis/HiggsToTauTau/interface/LeptonFakeRateInterface.h" // LeptonFakeRateInterface
#include "tthAnalysis/HiggsToTauTau/interface/JetToTauFakeRateInterface.h" // JetToTauFakeRateInterface
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReader.h" // RecoJetReader
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReaderHTTv2.h" // RecoJetReaderHTTv2
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReaderAK12.h" // RecoJetReaderAK12
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetReaderAK8.h" // RecoJetReaderAK8
#include "tthAnalysis/HiggsToTauTau/interface/RecoMEtReader.h" // RecoMEtReader
#include "tthAnalysis/HiggsToTauTau/interface/GenJetReader.h" // GenJetReader
#include "tthAnalysis/HiggsToTauTau/interface/GenParticleReader.h" // GenParticleReader
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoReader.h" // LHEInfoReader
#include "tthAnalysis/HiggsToTauTau/interface/EventInfoReader.h" // EventInfoReader, EventInfo
#include "tthAnalysis/HiggsToTauTau/interface/convert_to_ptrs.h" // convert_to_ptrs
#include "tthAnalysis/HiggsToTauTau/interface/ParticleCollectionGenMatcher.h" // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorLoose.h" // RecoElectronCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoElectronCollectionSelectorTight.h" // RecoElectronCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorLoose.h" // RecoMuonCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorFakeable.h" // RecoMuonCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoMuonCollectionSelectorTight.h" // RecoMuonCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorLoose.h" // RecoHadTauCollectionSelectorLoose
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorFakeable.h" // RecoHadTauCollectionSelectorFakeable
#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauCollectionSelectorTight.h" // RecoHadTauCollectionSelectorTight
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelector.h" // RecoJetCollectionSelector
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorBtag.h" // RecoJetCollectionSelectorBtagLoose, RecoJetCollectionSelectorBtagMedium
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorHTTv2.h" // RecoJetSelectorHTTv2
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorAK12.h" // RecoJetSelectorAK12
#include "tthAnalysis/HiggsToTauTau/interface/RecoJetCollectionSelectorAK8.h" // RecoJetSelectorAK8
#include "tthAnalysis/HiggsToTauTau/interface/RunLumiEventSelector.h" // RunLumiEventSelector
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManager.h" // JetHistManager
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManagerHTTv2.h" // JetHistManagerHTTv2
#include "tthAnalysis/HiggsToTauTau/interface/JetHistManagerAK12.h" // JetHistManagerAK12
#include "tthAnalysis/HiggsToTauTau/interface/MVAInputVarHistManager.h" // MVAInputVarHistManager
#include "tthAnalysis/HiggsToTauTau/interface/WeightHistManager.h" // WeightHistManager
#include "tthAnalysis/HiggsToTauTau/interface/GenEvtHistManager.h" // GenEvtHistManager
#include "tthAnalysis/HiggsToTauTau/interface/LHEInfoHistManager.h" // LHEInfoHistManager
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // getBranchName_bTagWeight, getHadTau_genPdgId, isHigherPt, isMatched
#include "tthAnalysis/HiggsToTauTau/interface/HistManagerBase.h" // HistManagerBase
#include "tthAnalysis/HiggsToTauTau/interface/histogramAuxFunctions.h" // fillWithOverFlow2d
#include "tthAnalysis/HiggsToTauTau/interface/hadTopTaggerAuxFunctions.h" // isGenMatchedJetTriplet
#include "tthAnalysis/HiggsToTauTau/interface/fakeBackgroundAuxFunctions.h" // getWeight_2L, getWeight_3L
#include "tthAnalysis/HiggsToTauTau/interface/hltPath.h" // hltPath, create_hltPaths, hltPaths_setBranchAddresses, hltPaths_isTriggered, hltPaths_delete
#include "tthAnalysis/HiggsToTauTau/interface/Data_to_MC_CorrectionInterface.h" // Data_to_MC_CorrectionInterface
#include "tthAnalysis/HiggsToTauTau/interface/lutAuxFunctions.h" // loadTH2, getSF_from_TH2
#include "tthAnalysis/HiggsToTauTau/interface/cutFlowTable.h" // cutFlowTableType
#include "tthAnalysis/HiggsToTauTau/interface/NtupleFillerBDT.h" // NtupleFillerBDT
#include "tthAnalysis/HiggsToTauTau/interface/HadTopTagger.h" // HadTopTagger
#include "tthAnalysis/HiggsToTauTau/interface/TTreeWrapper.h" // TTreeWrapper
#include "tthAnalysis/HiggsToTauTau/interface/HadTopTaggerFill.h" // HadTopTaggerFill

#include <boost/range/algorithm/copy.hpp> // boost::copy()
#include <boost/range/adaptor/map.hpp> // boost::adaptors::map_keys
#include <boost/math/special_functions/sign.hpp> // boost::math::sign()

#include <iostream> // std::cerr, std::fixed
#include <iomanip> // std::setprecision(), std::setw()
#include <string> // std::string
#include <vector> // std::vector<>
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <algorithm> // std::sort
#include <math.h> // acos, sqrt
#include <fstream> // std::ofstream
#include <assert.h> // assert
#include <numeric> // iota

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

typedef math::PtEtaPhiMLorentzVector LV;
typedef std::vector<std::string> vstring;

//const int hadTauSelection_antiElectron = 1; // vLoose
//const int hadTauSelection_antiMuon = 1; // Loose
const int hadTauSelection_antiElectron = -1; // not applied
const int hadTauSelection_antiMuon = -1; // not applied

enum { kGen_bWj1Wj2, kGen_bWj1, kGen_bWj2, kGen_Wj1Wj2, kGen_b, kGen_Wj1, kGen_Wj2, kGen_none };
bool inAK12 = true;
size_t cutJetCombo = 20;

double square(double x)
{
  return x*x;
}

int main(int argc, char* argv[])
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "<analyze_hadTopTagger>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("analyze_hadTopTagger");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") )
    throw cms::Exception("analyze_hadTopTagger")
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");
  edm::ParameterSet cfg_analyze = cfg.getParameter<edm::ParameterSet>("analyze_hadTopTagger");
  std::string treeName = cfg_analyze.getParameter<std::string>("treeName");
  std::string process_string = cfg_analyze.getParameter<std::string>("process");
  bool isSignal = ( process_string == "signal" ) ? true : false;
  std::string histogramDir = cfg_analyze.getParameter<std::string>("histogramDir");
  std::string era_string = cfg_analyze.getParameter<std::string>("era");
  int era = -1;
  if      ( era_string == "2017" ) era = kEra_2017;
  else throw cms::Exception("analyze_hadTopTagger")
    << "Invalid Configuration parameter 'era' = " << era_string << " !!\n";

  bool use_HIP_mitigation_mediumMuonId = cfg_analyze.getParameter<bool>("use_HIP_mitigation_mediumMuonId");
  std::cout << "use_HIP_mitigation_mediumMuonId = " << use_HIP_mitigation_mediumMuonId << std::endl;

  bool isMC = cfg_analyze.getParameter<bool>("isMC");
  bool isMC_tH = ( process_string == "tH" ) ? true : false;
  std::string central_or_shift = cfg_analyze.getParameter<std::string>("central_or_shift");

  bool isDEBUG = cfg_analyze.getParameter<bool>("isDEBUG");
  if ( isDEBUG ) std::cout << "Warning: DEBUG mode enabled -> trigger selection will not be applied for data !!" << std::endl;

  bool selectBDT = ( cfg_analyze.exists("selectBDT") ) ? cfg_analyze.getParameter<bool>("selectBDT") : false;
  const int jetPt_option     = getJet_option       (central_or_shift, isMC);
  const int jetBtagSF_option = getBTagWeight_option(central_or_shift, isMC);

  std::string branchName_jets = cfg_analyze.getParameter<std::string>("branchName_jets");
  std::string branchName_jetsHTTv2 = cfg_analyze.getParameter<std::string>("branchName_jetsHTTv2");
  std::string branchName_subjetsHTTv2 = cfg_analyze.getParameter<std::string>("branchName_subjetsHTTv2");
  std::string branchName_jetsAK12 = cfg_analyze.getParameter<std::string>("branchName_jetsAK12");
  std::string branchName_subjetsAK12 = cfg_analyze.getParameter<std::string>("branchName_subjetsAK12");
  std::string branchName_jetsAK8 = cfg_analyze.getParameter<std::string>("branchName_jetsAK8");
  std::string branchName_subjetsAK8 = cfg_analyze.getParameter<std::string>("branchName_subjetsAK8");
  std::string branchName_met = cfg_analyze.getParameter<std::string>("branchName_met");
  std::string branchName_genJets = cfg_analyze.getParameter<std::string>("branchName_genJets");
  std::string branchName_genTopQuarks = cfg_analyze.getParameter<std::string>("branchName_genTopQuarks");
  std::string branchName_genBJets = cfg_analyze.getParameter<std::string>("branchName_genBJets");
  std::string branchName_genWBosons = cfg_analyze.getParameter<std::string>("branchName_genWBosons");
  std::string branchName_genWJets = cfg_analyze.getParameter<std::string>("branchName_genWJets");

  bool redoGenMatching = cfg_analyze.getParameter<bool>("redoGenMatching");

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

  TTreeWrapper * inputTree = new TTreeWrapper(treeName.data(), inputFiles.files(), maxEvents);
  std::cout << "Loaded " << inputTree -> getFileCount() << " file(s).\n";

//--- declare event-level variables
  EventInfo eventInfo(isSignal, isMC, isMC_tH);
  EventInfoReader eventInfoReader(&eventInfo);
  inputTree -> registerReader(&eventInfoReader);

//--- declare particle collections
  const bool readGenObjects = isMC && !redoGenMatching;

  RecoJetReader* jetReader = new RecoJetReader(era, isMC, branchName_jets, readGenObjects);
  jetReader->setPtMass_central_or_shift(jetPt_option);
  jetReader->setBranchName_BtagWeight(jetBtagSF_option);
  inputTree -> registerReader(jetReader);
  RecoJetCollectionGenMatcher jetGenMatcher;
  RecoJetCollectionSelector jetSelector(era);
  RecoJetCollectionSelectorBtagLoose jetSelectorBtagLoose(era);
  RecoJetCollectionSelectorBtagMedium jetSelectorBtagMedium(era);

  RecoJetReaderHTTv2* jetReaderHTTv2 = new RecoJetReaderHTTv2(era, branchName_jetsHTTv2, branchName_subjetsHTTv2);
  inputTree -> registerReader(jetReaderHTTv2);
  RecoJetCollectionSelectorHTTv2 jetSelectorHTTv2(era);
	RecoJetSelectorHTTv2 jetSelectorHTTv2_1(era);

  RecoJetReaderAK12* jetReaderAK12 = new RecoJetReaderAK12(era, branchName_jetsAK12, branchName_subjetsAK12);
  inputTree -> registerReader(jetReaderAK12);
  RecoJetCollectionSelectorAK12 jetSelectorAK12(era);
	RecoJetSelectorAK12 jetSelectorAK12_1(era);
	
  RecoJetReaderAK8* jetReaderAK8 = new RecoJetReaderAK8(era, branchName_jetsAK8, branchName_subjetsAK8);
  inputTree -> registerReader(jetReaderAK8);
  RecoJetCollectionSelectorAK8 jetSelectorAK8(era);
	RecoJetSelectorAK8 jetSelectorAK8_1(era);
	
//--- declare generator level information
  GenJetReader* genJetReader = 0;
  LHEInfoReader* lheInfoReader = 0;
  if ( isMC ) {
      if ( branchName_genJets != "" ) {
        genJetReader = new GenJetReader(branchName_genJets);
        inputTree -> registerReader(genJetReader);
      }
    //}
    lheInfoReader = new LHEInfoReader();
    inputTree -> registerReader(lheInfoReader);
  }

  GenParticleReader* genTopQuarkReader = new GenParticleReader(branchName_genTopQuarks);
  GenParticleReader* genBJetReader = new GenParticleReader(branchName_genBJets);
  GenParticleReader* genWBosonReader = new GenParticleReader(branchName_genWBosons);
  GenParticleReader* genWJetReader = new GenParticleReader(branchName_genWJets);

  if ( isMC ) {
    inputTree->registerReader(genTopQuarkReader);
    inputTree->registerReader(genBJetReader);
    inputTree->registerReader(genWBosonReader);
    inputTree->registerReader(genWJetReader);
  }

  HadTopTaggerFill* hadTopTaggerFill = new HadTopTaggerFill();

  NtupleFillerBDT<float, int>* bdt_filler = nullptr;
  typedef std::remove_pointer<decltype(bdt_filler)>::type::float_type float_type;
  typedef std::remove_pointer<decltype(bdt_filler)>::type::int_type int_type;
  if ( selectBDT ) {
    bdt_filler = new std::remove_pointer<decltype(bdt_filler)>::type(
       makeHistManager_cfg(process_string, Form("%s/evtntuple", histogramDir.data()), "central")
       );
    bdt_filler->register_variable<float_type>(
      ///* // computed by (*hadTopTaggerFill)(selBJet, selWJet1, selWJet2);
      "m_bWj1Wj2", "m_Wj1Wj2", "m_bWj1", "m_bWj2",
      "m_Wj1Wj2_div_m_bWj1Wj2",
      "pT_b", "eta_b", //"phi_b", "mass_b",
      "kinFit_pT_b", //"kinFit_eta_b", //"kinFit_phi_b", "kinFit_mass_b",
      "pT_Wj1", "eta_Wj1", //"phi_Wj1", "mass_Wj1",
      "kinFit_pT_Wj1", //"kinFit_eta_Wj1", //"kinFit_phi_Wj1", "kinFit_mass_Wj1",
      "pT_Wj2", "eta_Wj2", //"phi_Wj2", "mass_Wj2",
      "kinFit_pT_Wj2", //"kinFit_eta_Wj2", //"kinFit_phi_Wj2", "kinFit_mass_Wj2",
      "cosTheta_leadWj_restTop","cosTheta_subleadWj_restTop",
      "cosTheta_Kin_leadWj_restTop","cosTheta_Kin_subleadWj_restTop",
      "cosTheta_leadEWj_restTop","cosTheta_subleadEWj_restTop",
      "cosTheta_Kin_leadEWj_restTop","cosTheta_Kin_subleadEWj_restTop",
      "cosThetaW_rest","cosThetaKinW_rest","cosThetaW_lab","cosThetaKinW_lab",
      "cosThetab_rest","cosThetaKinb_rest","cosThetab_lab","cosThetaKinb_lab",
      "Dphi_Wj1_Wj2_lab","Dphi_KinWj1_KinWj2_lab",
      "Dphi_Wb_rest","Dphi_KinWb_rest","Dphi_Wb_lab","Dphi_KinWb_lab",
      "cosThetaWj1_restW","cosThetaKinWj_restW",
      "dR_bWj1", "dR_bWj2", "dR_Wj1Wj2", "dR_bW",
      "statusKinFit", "nllKinFit", "alphaKinFit", //"logPKinFit", "logPErrKinFit",
      "pT_bWj1Wj2", "pT_Wj1Wj2",
      //*/
      "tau32Top", "massTop",
      "tau21W", "massW_SD",
      "genTopPt",
      "btagDisc", "qg_Wj1", "qg_Wj2"
    );


    bdt_filler->register_variable<int_type>(
      "typeTop", "collectionSize", "bjet_tag_position", "bWj1Wj2_isGenMatched", "counter",
      "fatjet_isGenMatched", "b_isGenMatched"
      //"b_isGenMatched", "Wj1_isGenMatched", "Wj2_isGenMatched",
      //"bWj1Wj2_isGenMatched"
    );
	//bdt_filler->register_variable<int_type>("mvaOutput_hadTopTagger");
    bdt_filler->bookTree(fs);
	}

  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  TH1* histogram_analyzedEntries = fs.make<TH1D>("analyzedEntries", "analyzedEntries", 1, -0.5, +0.5);
  TH1* histogram_selectedEntries = fs.make<TH1D>("selectedEntries", "selectedEntries", 1, -0.5, +0.5);
	cutFlowTableType cutFlowTable_ttH;
	cutFlowTableType cutFlowTable_ttH_HTTv2;
	cutFlowTableType cutFlowTable_ttH_AK12;
	cutFlowTableType cutFlowTable_ttH_AK8;
	cutFlowTableType cutFlowTable_ttH_resolved;
	
//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = ( selEventsFileName_output != "" ) ? new std::ofstream(selEventsFileName_output.data(), std::ios::out) : 0;
  std::cout << "selEventsFileName_output = " << selEventsFileName_output << std::endl;

  int countFatTop = 0;
  int countFatAK8 = 0;
  int countFatAK12 = 0;
  int countResolved = 0;
  int hadtruth1 = 0; // counter for diagnosis
  int hadtruth2 = 0; // counter for diagnosis
  int hadtruth3 = 0; // counter for diagnosis

  while(inputTree -> hasNextEvent() && (! run_lumi_eventSelector || (run_lumi_eventSelector && ! run_lumi_eventSelector -> areWeDone())))
  {
    if ( isDEBUG ) {
      std::cout << "processing run = " << eventInfo.run << ", ls = " << eventInfo.lumi << ", event = " << eventInfo.event << std::endl;
    }

    if(inputTree -> canReport(reportEvery))
    {
      std::cout << "processing Entry " << inputTree -> getCurrentMaxEventIdx()
                << " or " << inputTree -> getCurrentEventIdx() << " entry in #"
                << (inputTree -> getProcessedFileCount() - 1)
                << " (" << eventInfo
                << ") file (" << selectedEntries << " Entries selected)" << std::endl;
    }
    ++analyzedEntries;
    histogram_analyzedEntries->Fill(0.);
		cutFlowTable_ttH.update("all events");

    if (run_lumi_eventSelector && !(*run_lumi_eventSelector)(eventInfo))
    {
      continue;
    }

    //std::cout << "before load jets"  << std::endl;
//--- build collections of jets and select subset of jets passing b-tagging criteria
    std::vector<RecoJet> jets = jetReader->read();
    std::vector<const RecoJet*> selJetsraw = convert_to_ptrs(jets);
    std::vector<const RecoJet*> selJets = jetSelector(selJetsraw, isHigherPt);
		//selJets = convert_to_ptrs(jets);
    std::vector<const RecoJet*> selBJets_loose = jetSelectorBtagLoose(selJets);
    std::vector<const RecoJet*> selBJets_medium = jetSelectorBtagMedium(selJets);
    //std::cout << "after load jets"  << std::endl;
//--- build collections of jets reconstructed by hep-top-tagger (HTTv2) algorithm
    std::vector<RecoJetHTTv2> jetsHTTv2 = jetReaderHTTv2->read();
    std::vector<const RecoJetHTTv2*> jet_ptrsHTTv2raw = convert_to_ptrs(jetsHTTv2);
    std::vector<const RecoJetHTTv2*> jet_ptrsHTTv2 =  jetSelectorHTTv2(jet_ptrsHTTv2raw, isHigherPt);
    //std::cout << "after load HTT"  << std::endl;
		//jet_ptrsHTTv2 = convert_to_ptrs(jetsHTTv2);
//--- build collections of jets reconstructed by anti-kT algorithm with dR=1.2 (AK12)
    std::vector<RecoJetAK12> jetsAK12 = jetReaderAK12->read();
    std::vector<const RecoJetAK12*> jet_ptrsAK12raw = convert_to_ptrs(jetsAK12);
    std::vector<const RecoJetAK12*> jet_ptrsAK12 = jetSelectorAK12(jet_ptrsAK12raw, isHigherPt);
		//jet_ptrsAK12 = convert_to_ptrs(jetsAK12);
    //std::cout << "after load ak12"  << std::endl;
//--- build collections of jets reconstructed by anti-kT algorithm with dR=0.8 (AK8)
    std::vector<RecoJetAK8> jetsAK8 = jetReaderAK8->read();
    std::vector<const RecoJetAK8*> jet_ptrsAK8raw = convert_to_ptrs(jetsAK8);
    std::vector<const RecoJetAK8*> jet_ptrsAK8 = jetSelectorAK8(jet_ptrsAK8raw , isHigherPt);
    //std::cout << "after load ak8"  << std::endl;

		
		
//--- build collections of generator level particles (after some cuts are applied, to safe computing time)
    std::vector<GenJet> genJets;
    if ( isMC && redoGenMatching ) {
      if ( genJetReader ) {
				genJets = genJetReader->read();
      }
    }

//--- match reconstructed to generator level particles
    if ( isMC && redoGenMatching ) {
      jetGenMatcher.addGenJetMatch(selJets, genJets, 0.2);
    }

    if ( isMC ) {
      lheInfoReader->read();
    }

    if ( selEventsFile ) {
      (*selEventsFile) << eventInfo.run << ':' << eventInfo.lumi << ':' << eventInfo.event << '\n';
    }

//--- build collections of generator level particles
    std::vector<GenParticle> genTopQuarks = genTopQuarkReader->read();
    std::vector<GenParticle> genBJets = genBJetReader->read();
    std::vector<GenParticle> genWBosons = genWBosonReader->read();
    std::vector<GenParticle> genWJets = genWJetReader->read();
    //if (genWJets.size() < 2) continue;

    if ( !(genTopQuarks.size() == 2) ) {
      if ( isDEBUG ) std::cout << "FAILS '2 genTopQuarks' cut !!" << std::endl;
      continue;
    }
    if ( !(genBJets.size() == 2) ) {
      if ( isDEBUG ) std::cout << "FAILS '2 genBJets' cut !!" << std::endl;
      continue;
    }
    if ( !(genWBosons.size() >= 2) ) { // CV: do not veto events in which Higgs boson decays into W bosons
      if ( isDEBUG ) std::cout << "FAILS '>= 2 genWBosons' cut !!" << std::endl;
      continue;
    }
    if ( !(genWJets.size() >= 2) ) {
      if ( isDEBUG ) std::cout << "FAILS '>= 2 genWJets' cut !!" << std::endl;
      continue;
    }
		cutFlowTable_ttH.update("Gen-level TopQuarks, BJets, WBosons, WJets number are as for ttH signal");
		
    const GenParticle* genTopQuark = 0;
    const GenParticle* genAntiTopQuark = 0;
    for ( std::vector<GenParticle>::const_iterator it = genTopQuarks.begin();
	  it != genTopQuarks.end(); ++it ) {
      if ( it->pdgId() == +6 && !genTopQuark     ) genTopQuark = &(*it);
      if ( it->pdgId() == -6 && !genAntiTopQuark ) genAntiTopQuark = &(*it);
    }
    if ( !(genTopQuark && genAntiTopQuark) ) {
      if ( isDEBUG ) std::cout << "FAILS 'genTopQuark && genAntiTopQuark' cut !!" << std::endl;
      continue;
    }

    std::vector<const GenParticle*> genBJetsFromTop;
    std::vector<const GenParticle*> genBJetsFromAntiTop;
    for ( std::vector<GenParticle>::const_iterator it = genBJets.begin();
	  it != genBJets.end(); ++it ) {
      if ( it->pdgId() == +5 ) genBJetsFromTop.push_back(&(*it));
      if ( it->pdgId() == -5 ) genBJetsFromAntiTop.push_back(&(*it));
    }
    if ( !(genBJetsFromTop.size() >= 1 && genBJetsFromAntiTop.size() >= 1) ) {
      if ( isDEBUG ) std::cout << "FAILS 'genBJetFromTop && genBJetFromAntiTop' cut !!" << std::endl;
      continue;
    }

    std::vector<const GenParticle*> genWBosonsFromTop;
    std::vector<const GenParticle*> genWBosonsFromAntiTop;
    for ( std::vector<GenParticle>::const_iterator it = genWBosons.begin();
	  it != genWBosons.end(); ++it ) {
      if ( it->pdgId() == +24 ) genWBosonsFromTop.push_back(&(*it));
      if ( it->pdgId() == -24 ) genWBosonsFromAntiTop.push_back(&(*it));
    }
    if ( !(genWBosonsFromTop.size() >= 1 && genWBosonsFromAntiTop.size() >= 1) ) {
      if ( isDEBUG ) std::cout << "FAILS 'genWBosonFromTop && genWBosonFromAntiTop' cut !!" << std::endl;
      continue;
    }

    const GenParticle* genBJetFromTop = 0;
    const GenParticle* genWBosonFromTop = 0;
    double genBJet_plus_WBosonMassFromTop = -1.;
    for ( std::vector<const GenParticle*>::const_iterator genBJet = genBJetsFromTop.begin();
	  genBJet != genBJetsFromTop.end(); ++genBJet ) {
      for ( std::vector<const GenParticle*>::const_iterator genWBoson = genWBosonsFromTop.begin();
	    genWBoson != genWBosonsFromTop.end(); ++genWBoson ) {
	double genBJet_plus_WBosonMass = ((*genBJet)->p4() + (*genWBoson)->p4()).mass();
	if ( std::fabs(genBJet_plus_WBosonMass - genTopQuark->mass()) < std::fabs(genBJet_plus_WBosonMassFromTop - genTopQuark->mass())  ||
	     !(genBJetFromTop && genWBosonFromTop) ) {
	  genBJetFromTop = (*genBJet);
	  genWBosonFromTop = (*genWBoson);
	  genBJet_plus_WBosonMassFromTop = genBJet_plus_WBosonMass;
	}
      }
    }
    if ( !(std::fabs((genBJetFromTop->p4() + genWBosonFromTop->p4()).mass() - genTopQuark->mass()) < 15.) ) {
      if ( isDEBUG ) {
	std::cout << "FAILS 'genTopQuark mass' cut !!" << std::endl;
	//std::cout << "genTopQuark: pT = " << genTopQuark->pt() << ", eta = " << genTopQuark->eta() << ","
	//	    << " phi = " << genTopQuark->phi() << ", mass = " << genTopQuark->mass() << std::endl;
	//std::cout << "genBJetFromTop: pT = " << genBJetFromTop->pt() << ", eta = " << genBJetFromTop->eta() << ","
	//	    << " phi = " << genBJetFromTop->phi() << ", mass = " << genBJetFromTop->mass() << std::endl;
	//std::cout << "genWBosonFromTop: pT = " << genWBosonFromTop->pt() << ", eta = " << genWBosonFromTop->eta() << ","
	//	    << " phi = " << genWBosonFromTop->phi() << ", mass = " << genWBosonFromTop->mass() << std::endl;
	//std::cout << "(mass of genBJetFromTop + genWBosonFromTop = " << (genBJetFromTop->p4() + genWBosonFromTop->p4()).mass() << ")" << std::endl;
      }
      continue;
    }
    const GenParticle* genBJetFromAntiTop = 0;
    const GenParticle* genWBosonFromAntiTop = 0;
    double genBJet_plus_WBosonMassFromAntiTop = -1.;
    for ( std::vector<const GenParticle*>::const_iterator genBJet = genBJetsFromAntiTop.begin();
	  genBJet != genBJetsFromAntiTop.end(); ++genBJet ) {
      for ( std::vector<const GenParticle*>::const_iterator genWBoson = genWBosonsFromAntiTop.begin();
	    genWBoson != genWBosonsFromAntiTop.end(); ++genWBoson ) {
	double genBJet_plus_WBosonMass = ((*genBJet)->p4() + (*genWBoson)->p4()).mass();
	if ( std::fabs(genBJet_plus_WBosonMass - genAntiTopQuark->mass()) < std::fabs(genBJet_plus_WBosonMassFromAntiTop - genAntiTopQuark->mass())  ||
	     !(genBJetFromAntiTop && genWBosonFromAntiTop) ) {
	  genBJetFromAntiTop = (*genBJet);
	  genWBosonFromAntiTop = (*genWBoson);
	  genBJet_plus_WBosonMassFromAntiTop = genBJet_plus_WBosonMass;
	}
      }
    }
    if ( !(std::fabs((genBJetFromAntiTop->p4() + genWBosonFromAntiTop->p4()).mass() - genAntiTopQuark->mass()) < 15.) ) {
      if ( isDEBUG ) {
	std::cout << "FAILS 'genAntiTopQuark mass' cut !!" << std::endl;
	//std::cout << "genAntiTopQuark: pT = " << genAntiTopQuark->pt() << ", eta = " << genAntiTopQuark->eta() << ","
	//	    << " phi = " << genAntiTopQuark->phi() << ", mass = " << genAntiTopQuark->mass() << std::endl;
	//std::cout << "genBJetFromAntiTop: pT = " << genBJetFromAntiTop->pt() << ", eta = " << genBJetFromAntiTop->eta() << ","
	//	    << " phi = " << genBJetFromAntiTop->phi() << ", mass = " << genBJetFromAntiTop->mass() << std::endl;
	//std::cout << "genWBosonFromAntiTop: pT = " << genWBosonFromAntiTop->pt() << ", eta = " << genWBosonFromAntiTop->eta() << ","
	//	    << " phi = " << genWBosonFromAntiTop->phi() << ", mass = " << genWBosonFromAntiTop->mass() << std::endl;
	//std::cout << "(mass of genBJetFromAntiTop + genWBosonFromAntiTop = " << (genBJetFromAntiTop->p4() + genWBosonFromAntiTop->p4()).mass() << ")" << std::endl;
      }
      continue;
    }

    std::vector<const GenParticle*> genWJetsFromTop;
    double genWJetsFromTop_mass = -1.;
    std::vector<const GenParticle*> genWJetsFromAntiTop;
    double genWJetsFromAntiTop_mass = -1.;
    for ( std::vector<GenParticle>::const_iterator it1 = genWJets.begin(); it1 != genWJets.end(); ++it1 ) {
      for ( std::vector<GenParticle>::const_iterator it2 = it1 + 1;
	    it2 != genWJets.end(); ++it2 ) {
	double genDijetMass = (it1->p4() + it2->p4()).mass();
	// CV: Matching the generator-level charge of the two quarks to the generator-level charge of the W boson is a bit cumbersome,
	//     because charge of Particles is stored as integer in Ntuple,
	//     so only sign of charge is well defined for quarks (fractional charge is not supported!)
	//
	//     For W->jj decays, the sign of the charge of both jets is equal to the sign of the charge of the W boson,
	//     i.e. the W boson decays either via
	//       W+ -> up-type-quark (q = +2/3) + anti-down-type-quark (+1/3)
	//     or via
	//       W- -> anti-up-type-quark (q = -2/3) + down-type-quark (-1/3)
	if ( boost::math::sign(it1->charge()) == boost::math::sign(genWBosonFromTop->charge()) &&
	     boost::math::sign(it2->charge()) == boost::math::sign(genWBosonFromTop->charge()) ) {
	  if ( std::fabs(genDijetMass - genWBosonFromTop->mass()) < 15. &&
	       (genWJetsFromTop_mass == -1. ||
		std::fabs(genDijetMass - genWBosonFromTop->mass()) < std::fabs(genWJetsFromTop_mass - genWBosonFromTop->mass())) ) {
	    genWJetsFromTop.clear();
	    genWJetsFromTop.push_back(&(*it1));
	    genWJetsFromTop.push_back(&(*it2));
	    genWJetsFromTop_mass = genDijetMass;
	  }
	}
	if ( boost::math::sign(it1->charge()) == boost::math::sign(genWBosonFromAntiTop->charge()) &&
	     boost::math::sign(it2->charge()) == boost::math::sign(genWBosonFromAntiTop->charge()) ) {
	  if ( std::fabs(genDijetMass - genWBosonFromAntiTop->mass()) < 15. &&
	       (genWJetsFromAntiTop_mass == -1. ||
		std::fabs(genDijetMass - genWBosonFromAntiTop->mass()) < std::fabs(genWJetsFromAntiTop_mass - genWBosonFromAntiTop->mass())) ) {
	    genWJetsFromAntiTop.clear();
	    genWJetsFromAntiTop.push_back(&(*it1));
	    genWJetsFromAntiTop.push_back(&(*it2));
	    genWJetsFromAntiTop_mass = genDijetMass;
	  }
	}
      }
    }
    //std::cout << "#genWJetsFromTop = " << genWJetsFromTop.size() << " (mass = " << genWJetsFromTop_mass << ")" << std::endl;
    //std::cout << "#genWJetsFromAntiTop = " << genWJetsFromAntiTop.size() << " (mass = " << genWJetsFromAntiTop_mass << ")" << std::endl;
    if ( !(genWJetsFromTop.size() == 2 || genWJetsFromAntiTop.size() == 2) ) {
      if ( isDEBUG ) std::cout << "FAILS '2 genWJetsFromTop || 2 genWJetsFromAntiTop' cut !!" << std::endl;
      continue;
    }

    const GenParticle* genWJetFromTop_lead = 0;
    const GenParticle* genWJetFromTop_sublead = 0;
    bool failsWbosonMassVeto_top = false;
    if ( genWJetsFromTop.size() == 2 ) {
      std::sort(genWJetsFromTop.begin(), genWJetsFromTop.end(), isHigherPt);
      genWJetFromTop_lead = genWJetsFromTop[0];
      genWJetFromTop_sublead = genWJetsFromTop[1];
      if ( !(std::fabs((genWJetFromTop_lead->p4() + genWJetFromTop_sublead->p4()).mass() - genWBosonFromTop->mass()) < 15.) ) failsWbosonMassVeto_top = true;
    }
    //std::cout << "genWBosonFromTop: mass = " << genWBosonFromTop->mass();
    //if ( genWJetFromTop_lead && genWJetFromTop_sublead ) {
    //  std::cout << "," ;
    //  std::cout << " mass of genWJetsFromTop = " << (genWJetFromTop_lead->p4() + genWJetFromTop_sublead->p4()).mass();
    //}
    //std::cout << std::endl;
    if ( failsWbosonMassVeto_top ) {
      if ( isDEBUG ) std::cout << "FAILS 'genWBosonFromTop mass' cut !!" << std::endl;
      continue;
    }

    const GenParticle* genWJetFromAntiTop_lead = 0;
    const GenParticle* genWJetFromAntiTop_sublead = 0;
    bool failsWbosonMassVeto_antiTop = false;
    if ( genWJetsFromAntiTop.size() == 2 ) {
      std::sort(genWJetsFromAntiTop.begin(), genWJetsFromAntiTop.end(), isHigherPt);
      genWJetFromAntiTop_lead = genWJetsFromAntiTop[0];
      genWJetFromAntiTop_sublead = genWJetsFromAntiTop[1];
      if ( !(std::fabs((genWJetFromAntiTop_lead->p4() + genWJetFromAntiTop_sublead->p4()).mass() - genWBosonFromAntiTop->mass()) < 15.) ) failsWbosonMassVeto_antiTop = true;
    }
    //std::cout << "genWBosonFromAntiTop: mass = " << genWBosonFromAntiTop->mass();
    //if ( genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead ) {
    //  std::cout << "," ;
    //  std::cout << " mass of genWJetsFromAntiTop = " << (genWJetFromAntiTop_lead->p4() + genWJetFromAntiTop_sublead->p4()).mass();
    //}
    //std::cout << std::endl;
    if ( failsWbosonMassVeto_antiTop ) {
      if ( isDEBUG ) std::cout << "FAILS 'genWBosonFromAntiTop mass' cut !!" << std::endl;
      continue;
    }

    if ( isDEBUG ) {
      std::cout << "top:" << (*genTopQuark) << std::endl;
      std::cout << " b:" << (*genBJetFromTop) << std::endl;
      std::cout << " W:" << (*genWBosonFromTop) << std::endl;
      if ( genWJetFromTop_lead && genWJetFromTop_sublead ) {
	std::cout << " Wj1:" << (*genWJetFromTop_lead) << std::endl;
	std::cout << " Wj2:" << (*genWJetFromTop_sublead) << std::endl;
      } else {
	std::cout << "(leptonic W decay)" << std::endl;
      }
      std::cout << "anti-top:" << (*genAntiTopQuark) << std::endl;
      std::cout << " b:" << (*genBJetFromAntiTop) << std::endl;
      std::cout << " W:" << (*genWBosonFromAntiTop) << std::endl;
      if ( genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead ) {
	std::cout << " Wj1:" << (*genWJetFromAntiTop_lead) << std::endl;
	std::cout << " Wj2:" << (*genWJetFromAntiTop_sublead) << std::endl;
      } else {
	std::cout << "(leptonic W decay)" << std::endl;
      }
      std::cout << std::endl;
    }

    if ( !((genBJetFromTop             &&
	    genWJetFromTop_lead        &&
	    genWJetFromTop_sublead    ) ||
	   (genBJetFromAntiTop         &&
	    genWJetFromAntiTop_lead    &&
	    genWJetFromAntiTop_sublead)) ) {
      if ( isDEBUG ) std::cout << "FAILS 'genJet triplet' cut !!" << std::endl;
      continue;
    }
		cutFlowTable_ttH.update("Gen-level Bjet and 2 WJets from top/antitop are as per ttH signal");
		
    Particle::LorentzVector genWBosonFromTopP4;
    if ( genWJetFromTop_lead && genWJetFromTop_sublead ) {
      genWBosonFromTopP4 = genWJetFromTop_lead->p4() + genWJetFromTop_sublead->p4();
    }
    Particle::LorentzVector genWBosonFromAntiTopP4;
    if ( genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead ) {
      genWBosonFromAntiTopP4 = genWJetFromAntiTop_lead->p4() + genWJetFromAntiTop_sublead->p4();
    }

    Particle::LorentzVector genTopP4;
    if ( genBJetFromTop && genWJetFromTop_lead && genWJetFromTop_sublead ) {
      genTopP4 = genBJetFromTop->p4() + genWJetFromTop_lead->p4() + genWJetFromTop_sublead->p4();
    }
    Particle::LorentzVector genAntiTopP4;
    if ( genBJetFromAntiTop && genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead ) {
      genAntiTopP4 = genBJetFromAntiTop->p4() + genWJetFromAntiTop_lead->p4() + genWJetFromAntiTop_sublead->p4();
    }


    int collectionSize = 0;
    Particle::LorentzVector unfittedHadTopP4, selBJet, selWJet1, selWJet2 ;
    bool isGenMatched = false;
		bool fatjet_isGenMatched = false;
    bool b_isGenMatched = false;
    double genTopPtProbeTop = -1.;
    double genTopPtProbeAntiTop = -1.;
    double genTopPt = -1.;
		int typeTop = -1; // 1 - FatTop; 2 - countFatAK8 ? countFatAK12; 3- countResolved;

		bool isCategoryFatTopQuark = false;
		bool isCategoryFatWBoson = false;
		//bool isCategoryResolvedJet = false;
    //-------------------------------------------------------------------------------------------------------------------
    // case 1: all three jets contained within dR=1.5 "fat" jet,
    //         reconstructed by hep-top-tagger (HTTv2) algorithm

		
    if ( (genBJetFromTop     && genWJetFromTop_lead     && genWJetFromTop_sublead     && genTopP4.pt()     > 200.) ||
				 (genBJetFromAntiTop && genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead && genAntiTopP4.pt() > 200.) ) {
			cutFlowTable_ttH_HTTv2.update("HTTv2: gen top above 200GeV");
			
			if ( (genBJetFromTop             && genBJetFromTop->absEta()          < 2.4 &&
						genWJetFromTop_lead        && genWJetFromTop_lead->absEta()     < 2.4 &&
						genWJetFromTop_sublead     && genWJetFromTop_lead->absEta()     < 2.4) ||
					 (genBJetFromAntiTop         && genBJetFromAntiTop->absEta()      < 2.4 &&
						genWJetFromAntiTop_lead    && genWJetFromAntiTop_lead->absEta() < 2.4 &&
						genWJetFromAntiTop_sublead && genWJetFromAntiTop_lead->absEta() < 2.4) ) {
				cutFlowTable_ttH_HTTv2.update("HTTv2: eta < 2.4");
				
				if ( (genBJetFromTop && genWJetFromTop_lead && genWJetFromTop_sublead             &&
							deltaR(genTopP4, genBJetFromTop->p4())                                < 1.5 &&
							deltaR(genTopP4, genWJetFromTop_lead->p4())                           < 1.5 &&
							deltaR(genTopP4, genWJetFromTop_sublead->p4())                        < 1.5) ||
						 (genBJetFromAntiTop && genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead &&
							deltaR(genAntiTopP4, genBJetFromAntiTop->p4())                        < 1.5 &&
							deltaR(genAntiTopP4, genWJetFromAntiTop_lead->p4())                   < 1.5 &&
							deltaR(genAntiTopP4, genWJetFromAntiTop_sublead->p4())                < 1.5) ) {	
					cutFlowTable_ttH_HTTv2.update("HTTv2: dR(top and jets) < 1.5");

					if (jet_ptrsHTTv2.size() >= 1) cutFlowTable_ttH_HTTv2.update("HTTv2: jet_ptrsHTTv2.size() >= 1");

					int nGenMatchedHTTv2 = 0;
					int nGenMatchedHTTv2_1 = 0; 
					typeTop = 1;
					//const RecoJetHTTv2 *recTop = 0;
					for ( std::vector<const RecoJetHTTv2*>::const_iterator jetIter = jet_ptrsHTTv2.begin();
								jetIter != jet_ptrsHTTv2.end(); ++jetIter ) {

						//recTop = (*jetIter);
						if (!((*jetIter)->pt() > 200)) continue;
						if (!((*jetIter)->subJet1()->pt() > 30 && (*jetIter)->subJet2()->pt() > 30 && (*jetIter)->subJet3()->pt() > 30)) continue;
						//if (!( jetSelectorHTTv2_1(*recTop) )) continue;
						if (!( jetSelectorHTTv2_1(**jetIter) )) continue;
						
						unfittedHadTopP4 = (*jetIter)->p4();
						// I just take the subjet with highest b-tag score to be the b-candidate
						const RecoSubjetHTTv2* recSubJet[3];
						recSubJet[0] = (*jetIter)->subJet1();
						recSubJet[1] = (*jetIter)->subJet2();
						recSubJet[2] = (*jetIter)->subJet3();

						/*
						std::vector<double> btag_disc = {recSubJet[0]->BtagCSV(), recSubJet[1]->BtagCSV(), recSubJet[2]->BtagCSV()};
						auto btag_order = sort_indexes(btag_disc);
						selBJet = recSubJet[btag_order[0]]->p4();
						selWJet1 = recSubJet[btag_order[1]]->p4();
						selWJet2 = recSubJet[btag_order[2]]->p4();
						*/

						int perm [5] = { 0, 1, 2, 0, 1 }; // I do not care about the ordering of wj1/2 so no fancy permutation solution is required

						for (int ii = 0; ii < 3; ii++) {
              selBJet = recSubJet[perm[ii]]->p4();
              selWJet1 = recSubJet[perm[ii+1]]->p4();
              selWJet2 = recSubJet[perm[ii+2]]->p4();
							
							std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop, typeTop, unfittedHadTopP4);
							std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop, typeTop, unfittedHadTopP4);
							if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
							if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
							isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
							fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
							b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
							
							if (isGenMatched) nGenMatchedHTTv2++;
							if (isGenMatched && fatjet_isGenMatched) nGenMatchedHTTv2_1++;
						}
					}
					
					if (nGenMatchedHTTv2 >= 1) {
						isCategoryFatTopQuark = true;
						cutFlowTable_ttH_HTTv2.update("HTTv2: nGenMatchedHTTv2 >= 1  ** Rec ttH signal");
					}
					if (nGenMatchedHTTv2_1 >= 1) {
						cutFlowTable_ttH_HTTv2.update("HTTv2: nGenMatchedHTTv2_1 >= 1     ");
					}
				}
			}
		}
    //-------------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------------
    // case 2: two jets from hadronic W boson decay contained within dR=1.2 "fat" jet,
    //         reconstructed by by anti-kT algorithm with dR=1.2 (AK12),
    //         while b-jet from top decay is resolved as separate jets,
    //         reconstructed by anti-kT algorithm with dR=0.4 (AK4)


		//auto btag_order = calRank(selJets);

    if ( (genBJetFromTop                                        && genBJetFromTop->pt()         >  20. &&
					genWJetFromTop_lead && genWJetFromTop_sublead         && genWBosonFromTopP4.pt()      > 100.) ||
				 (genBJetFromAntiTop                                    && genBJetFromAntiTop->pt()     >  20. &&
					genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead && genWBosonFromAntiTopP4.pt()  > 100.) ) {
			cutFlowTable_ttH_AK12.update("AK12: Bjet > 20 GeV and WBoson > 100 GeV");
			
			if ( (genBJetFromTop             && genBJetFromTop->absEta()          < 2.4 &&
						genWJetFromTop_lead        && genWJetFromTop_lead->absEta()     < 2.4 &&
						genWJetFromTop_sublead     && genWJetFromTop_lead->absEta()     < 2.4) ||
					 (genBJetFromAntiTop         && genBJetFromAntiTop->absEta()      < 2.4 &&
						genWJetFromAntiTop_lead    && genWJetFromAntiTop_lead->absEta() < 2.4 &&
						genWJetFromAntiTop_sublead && genWJetFromAntiTop_lead->absEta() < 2.4) ) {
				cutFlowTable_ttH_AK12.update("AK12: eta < 2.4");
				
				if ( (genWJetFromTop_lead && genWJetFromTop_sublead                          &&
							deltaR(genWBosonFromTopP4, genWJetFromTop_lead->p4())            < 1.2 &&
							deltaR(genWBosonFromTopP4, genWJetFromTop_sublead->p4())         < 1.2) ||
						 (genWJetFromAntiTop_lead && genWJetFromAntiTop_sublead                  &&
							deltaR(genWBosonFromAntiTopP4, genWJetFromAntiTop_lead->p4())    < 1.2 &&
							deltaR(genWBosonFromAntiTopP4, genWJetFromAntiTop_sublead->p4()) < 1.2) ) {
					cutFlowTable_ttH_AK12.update("AK12: dR(W and jets) < 1.2");

					if (jet_ptrsAK12.size() >= 1 && selJets.size() >= 1) cutFlowTable_ttH_AK12.update("AK12: jet_ptrsAK12.size() >= 1 && selJets.size() >= 1");

					int nGenMatchedAK12 = 0;
					int nGenMatchedAK12_1 = 
					typeTop = 2; 
					for ( std::vector<const RecoJetAK12*>::const_iterator jetIter = jet_ptrsAK12.begin();
								jetIter != jet_ptrsAK12.end(); ++jetIter ) {

						if (!((*jetIter)->pt() > 130)) continue;
						if (!((*jetIter)->subJet1()->pt() > 10. &&  (*jetIter)->subJet1()->pt() > 10.)) continue;
						if (!(jetSelectorAK12_1(**jetIter))) continue;

							for ( unsigned int bjetCandidate = 0; bjetCandidate < selJets.size(); bjetCandidate++ ) {
							
              // add cleaning
              unfittedHadTopP4 = (*jetIter)->p4() + (*selJets[bjetCandidate]).p4();
              selBJet = (*selJets[bjetCandidate]).p4();

							
              selWJet1 = (*jetIter)->subJet1()->p4();
              selWJet2 = (*jetIter)->subJet2()->p4();
              //std::cout<<"mass "<< unfittedHadTopP4.mass() <<" value "<< (selBJet + selWJet1 + selWJet2).mass() <<std::endl;
              // calculate matching
              std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop, typeTop,(*jetIter)->p4());
              std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop, typeTop,(*jetIter)->p4());
              if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
              if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
              isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
              fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
              b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);

							if (isGenMatched) nGenMatchedAK12++;
							//if (isGenMatched && btag_order[bjetCandidate] > cutJetCombo) nGenMatchedAK12_1++;
							    						

						}
					}

					if (nGenMatchedAK12 >= 1) {
						isCategoryFatWBoson = true;
						cutFlowTable_ttH_AK12.update("AK12: nGenMatchedAK12 >= 1  ** Rec ttH signal");
						if (isCategoryFatTopQuark) cutFlowTable_ttH_AK12.update("AK12: Category 1 and Category2 both");
					}
					//if (nGenMatchedAK12_1 >= 1) cutFlowTable_ttH_AK12.update("AK12: nGenMatchedAK12_1 >= 1  ** Rec ttH signal");
					
				}
			}
		}
    //-------------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------------
    // case 3: all three jets resolved as separate jets,
    //         reconstructed by anti-kT algorithm with dR=0.4 (AK4)

    if ( (genBJetFromTop             && genBJetFromTop->pt()          > 25. &&
					genWJetFromTop_lead        && genWJetFromTop_lead->pt()     > 25. &&
					genWJetFromTop_sublead     && genWJetFromTop_lead->pt()     > 25.) ||
				 (genBJetFromAntiTop         && genBJetFromAntiTop->pt()      > 25. &&
					genWJetFromAntiTop_lead    && genWJetFromAntiTop_lead->pt() > 25. &&
					genWJetFromAntiTop_sublead && genWJetFromAntiTop_lead->pt() > 25.) ) {
			cutFlowTable_ttH_resolved.update("AK4: jets > 25 GeV ");
			
	    if ( (genBJetFromTop             && genBJetFromTop->absEta()          < 2.4 &&
						genWJetFromTop_lead        && genWJetFromTop_lead->absEta()     < 2.4 &&
						genWJetFromTop_sublead     && genWJetFromTop_lead->absEta()     < 2.4) ||
					 (genBJetFromAntiTop         && genBJetFromAntiTop->absEta()      < 2.4 &&
						genWJetFromAntiTop_lead    && genWJetFromAntiTop_lead->absEta() < 2.4 &&
						genWJetFromAntiTop_sublead && genWJetFromAntiTop_lead->absEta() < 2.4) ) {
				cutFlowTable_ttH_resolved.update("AK4: eta < 2.4");

				if ( (genBJetFromTop                                                                &&
							genWJetFromTop_lead                                                           &&
							genWJetFromTop_sublead                                                        &&
							deltaR(genBJetFromTop->p4(),          genWJetFromTop_lead->p4())        > 0.4 &&
							deltaR(genBJetFromTop->p4(),          genWJetFromTop_sublead->p4())     > 0.4 &&
							deltaR(genWJetFromTop_lead->p4(),     genWJetFromTop_sublead->p4())     > 0.4) ||
						 (genBJetFromAntiTop                                                            &&
							genWJetFromAntiTop_lead                                                       &&
							genWJetFromAntiTop_sublead                                                    &&
							deltaR(genBJetFromAntiTop->p4(),      genWJetFromAntiTop_lead->p4())    > 0.4 &&
							deltaR(genBJetFromAntiTop->p4(),      genWJetFromAntiTop_sublead->p4()) > 0.4 &&
							deltaR(genWJetFromAntiTop_lead->p4(), genWJetFromAntiTop_sublead->p4()) > 0.4) ) {
					cutFlowTable_ttH_resolved.update("AK4: dR(W and jets) > 0.4");
																														 
					if (selJets.size() >= 3) cutFlowTable_ttH_resolved.update("AK4: selJets.size() >= 3");

					int nGenMatchedAK4   = 0;
					int nGenMatchedAK4_1 = 0;
					/*
					for ( unsigned int bjetCandidate = 0; bjetCandidate < selJets.size(); bjetCandidate++ )  {
            for ( std::vector<const RecoJet*>::const_iterator selWJet1Candidate = selJets.begin();
									selWJet1Candidate != selJets.end(); ++selWJet1Candidate ) {
							if ( (*selWJet1Candidate)->pt() == (*selJets[bjetCandidate]).pt() ) continue;
							for ( std::vector<const RecoJet*>::const_iterator selWJet2Candidate = selWJet1Candidate + 1;
										selWJet2Candidate != selJets.end(); ++selWJet2Candidate ) {
								if ( (*selWJet2Candidate)->pt() == (*selJets[bjetCandidate]).pt() ) continue;
								if ( (*selWJet2Candidate)->pt() == (*selWJet1Candidate)->pt() ) continue;
								selBJet = (*selJets[bjetCandidate]).p4();
								selWJet1 = (*selWJet1Candidate)->p4();
								selWJet2 = (*selWJet2Candidate)->p4();
								*/
					for ( std::vector<const RecoJet*>::const_iterator selBjetCandidate = selJets.begin();
								 selBjetCandidate != selJets.end(); ++selBjetCandidate ) {
						for ( std::vector<const RecoJet*>::const_iterator selWJet1Candidate = selJets.begin();
									selWJet1Candidate != selJets.end(); ++selWJet1Candidate ) {
							if ( (*selWJet1Candidate)->pt() == (*selBjetCandidate)->pt() ) continue;
							for ( std::vector<const RecoJet*>::const_iterator selWJet2Candidate = selJets.begin();
										selWJet2Candidate != selJets.end(); ++selWJet2Candidate ) {
								if ( (*selWJet2Candidate)->pt() == (*selBjetCandidate)->pt()  ) continue;
								if ( (*selWJet2Candidate)->pt() == (*selWJet1Candidate)->pt() ) continue;
								selBJet  = (*selBjetCandidate)->p4();
								selWJet1 = (*selWJet1Candidate)->p4();
								selWJet2 = (*selWJet2Candidate)->p4();
								
								// calculate gen matching
								std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop);
								std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop);
								if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
								if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
								isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
								fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
								b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
								
								if (isGenMatched) nGenMatchedAK4++;
								if (isGenMatched &&
										deltaR(selBJet, selWJet1) > 0.3 &&
										deltaR(selBJet, selWJet2) > 0.3 &&
										deltaR(selWJet1, selWJet2) > 0.3 ) nGenMatchedAK4_1++;
							}
						}
					}

					if (nGenMatchedAK4 >= 1) {
						cutFlowTable_ttH_resolved.update("AK4: nGenMatchedAK4 >= 1  ** Rec ttH signal");
						//isCategoryResolvedJet = true;

						if (isCategoryFatTopQuark) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatTopQuark and isCategoryResolvedJest");
						if (isCategoryFatWBoson) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatWBoson  and isCategoryResolvedJest");
						if (isCategoryFatWBoson && isCategoryFatTopQuark) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatWBoson  and isCategoryResolvedJest and isCategoryFatTopQuark");
					}
					if (nGenMatchedAK4_1 >= 1) {
						cutFlowTable_ttH_resolved.update("AK4: nGenMatchedAK4_1 >= 1  ** Rec ttH signal");
						//isCategoryResolvedJet = true;

						if (isCategoryFatTopQuark) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatTopQuark and isCategoryResolvedJest_1");
						if (isCategoryFatWBoson) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatWBoson  and isCategoryResolvedJest_1");
						if (isCategoryFatWBoson && isCategoryFatTopQuark) cutFlowTable_ttH_resolved.update("AK4: isCategoryFatWBoson  and isCategoryResolvedJest_1 and isCategoryFatTopQuark");
					}
					
					
				}
			}

		}
		


		continue;











		

		std::cout << "\nTreeEntry "<<analyzedEntries << " passed selection conditions "<< genWJets.size() << std::endl;
    std::cout << "Size of jet collections "
      <<jet_ptrsHTTv2.size()<< " "<< jet_ptrsAK8.size() << " "<< jet_ptrsAK12.size() << " " << selJets.size() << std::endl;
    /*int collectionSize = 0;
    Particle::LorentzVector unfittedHadTopP4, selBJet, selWJet1, selWJet2 ;
    bool isGenMatched = false;
		bool fatjet_isGenMatched = false;
    bool b_isGenMatched = false;
    double genTopPtProbeTop = -1.;
    double genTopPtProbeAntiTop = -1.;
    double genTopPt = -1.;
    int typeTop = -1; // 1 - FatTop; 2 - countFatAK8 ? countFatAK12; 3- countResolved;
		*/

		collectionSize = 0;
		isGenMatched = false;
		fatjet_isGenMatched = false;
		b_isGenMatched = false;
		genTopPtProbeTop = -1.;
		genTopPtProbeAntiTop = -1.;
		genTopPt = -1.;

		typeTop = -1; // 1 - FatTop; 2 - countFatAK8 ? countFatAK12; 3- countResolved;
    if (inAK12) typeTop = getType(jet_ptrsHTTv2.size(), jet_ptrsAK12.size(), selJets.size());
    else typeTop = getType(jet_ptrsHTTv2.size(), jet_ptrsAK8.size(), selJets.size());
    // start loops in jets
    if (typeTop == 1) {
      collectionSize = jet_ptrsHTTv2.size();
      countFatTop++;
      for ( std::vector<const RecoJetHTTv2*>::const_iterator jetIter = jet_ptrsHTTv2.begin();
        jetIter != jet_ptrsHTTv2.end(); ++jetIter ) {
          unfittedHadTopP4 = (*jetIter)->p4();
          // I just take the subjet with highest b-tag score to be the b-candidate
          const RecoSubjetHTTv2* recSubJet[3];
          recSubJet[0] = (*jetIter)->subJet1();
          recSubJet[1] = (*jetIter)->subJet2();
          recSubJet[2] = (*jetIter)->subJet3();
          std::vector<double> btag_disc = {recSubJet[0]->BtagCSV(), recSubJet[1]->BtagCSV(), recSubJet[2]->BtagCSV()};
          auto btag_order = sort_indexes(btag_disc);
          selBJet = recSubJet[btag_order[0]]->p4();
          selWJet1 = recSubJet[btag_order[1]]->p4();
          selWJet2 = recSubJet[btag_order[2]]->p4();
          // checking btag ordering
          std::cout<<"btag ordering  ";
          for (auto i: btag_order) std::cout << i << " ";
          std::cout<<" typeTop = "<<typeTop<<std::endl;
          std::cout<<"btag discriminant  ";
          for (auto i: btag_disc) std::cout << i << " ";
          std::cout<<std::endl;
          std::cout<<"max btag position "<< btag_order[0] <<" value "<< btag_disc[btag_order[0]] <<std::endl;
          //std::cout<<"mass "<< unfittedHadTopP4.mass() <<" value "<< (selBJet + selWJet1 + selWJet2).mass() <<std::endl;
          // calculate matching
          std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop, typeTop, unfittedHadTopP4);
          std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop, typeTop, unfittedHadTopP4);
          if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
          if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
          isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
          fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
          b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
          if (isGenMatched) hadtruth1++;
          if ( bdt_filler ) {
            (*hadTopTaggerFill)(selBJet, selWJet1, selWJet2); // calculates the quantities that take only kinematics
            const std::map<std::string, double>& mvaInputs =  hadTopTaggerFill->mvaInputs();
            for ( std::map<std::string, double>::const_iterator mvaInput = mvaInputs.begin();
              mvaInput != mvaInputs.end(); ++mvaInput ) {
              bdt_filler->operator()(mvaInput->first, mvaInput->second);
            }
            bdt_filler -> operator()({ eventInfo.run, eventInfo.lumi, eventInfo.event })
            ("typeTop",              typeTop)
            ("tau32Top",             (*jetIter)->tau3()/(*jetIter)->tau2())
            ("massTop",              unfittedHadTopP4.mass())
            ("tau21W",               -1.)
            ("massW_SD",             -1.)
            ("btagDisc",             btag_disc[btag_order[0]])
            ("qg_Wj1",                -1.)
            ("qg_Wj2",                -1.)
            ("genTopPt",             genTopPt)
            ("collectionSize",       collectionSize)
            ("bjet_tag_position",    1)
            ("bWj1Wj2_isGenMatched", isGenMatched)
            ("fatjet_isGenMatched",  fatjet_isGenMatched)
            ("b_isGenMatched",       b_isGenMatched)
            ("counter",              (inputTree -> getProcessedFileCount() - 1))
                .fill();
          }
        }
    } else if (typeTop!=-1) {
    // store b-tag classification
    std::cout<<std::endl;
    auto btag_order = calRank(selJets);
    std::cout<<"btag ordering  ";
    for (auto i: btag_order) std::cout << i << " ";
    std::cout<<" typeTop = "<<typeTop<<std::endl;
    // classify between 2 and 3
    if (typeTop == 2 and inAK12) {
      countFatAK12++;
      collectionSize = jet_ptrsAK12.size();
      for ( std::vector<const RecoJetAK12*>::const_iterator jetIter = jet_ptrsAK12.begin();
        jetIter != jet_ptrsAK12.end(); ++jetIter ) {
          for ( unsigned int bjetCandidate = 0; bjetCandidate < selJets.size(); bjetCandidate++ ) {
              // add cleaning
              unfittedHadTopP4 = (*jetIter)->p4() + (*selJets[bjetCandidate]).p4();
              selBJet = (*selJets[bjetCandidate]).p4();
              selWJet1 = (*jetIter)->subJet1()->p4();
              selWJet2 = (*jetIter)->subJet2()->p4();
              //std::cout<<"mass "<< unfittedHadTopP4.mass() <<" value "<< (selBJet + selWJet1 + selWJet2).mass() <<std::endl;
              // calculate matching
              std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop, typeTop,(*jetIter)->p4());
              std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop, typeTop,(*jetIter)->p4());
              if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
              if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
              isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
              fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
              b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
              if (btag_order[bjetCandidate] > cutJetCombo) continue;
              if (isGenMatched) hadtruth2++;
              if ( bdt_filler ) {
                (*hadTopTaggerFill)(selBJet, selWJet1, selWJet2);
                const std::map<std::string, double>& mvaInputs =  hadTopTaggerFill->mvaInputs();
                for ( std::map<std::string, double>::const_iterator mvaInput = mvaInputs.begin();
                  mvaInput != mvaInputs.end(); ++mvaInput ) {
                  bdt_filler->operator()(mvaInput->first, mvaInput->second);
                }
                bdt_filler -> operator()({ eventInfo.run, eventInfo.lumi, eventInfo.event })
                ("typeTop",              typeTop)
                ("tau32Top",             -1.)
                ("massTop",              unfittedHadTopP4.mass())
                ("tau21W",               (*jetIter)->tau2()/(*jetIter)->tau1())
                ("massW_SD",             (*jetIter)->msoftdrop())
                ("btagDisc",             (*selJets[bjetCandidate]).BtagCSV())
                ("qg_Wj1",                -1.)
                ("qg_Wj2",                -1.)
                ("genTopPt",             genTopPt)
                ("collectionSize",       collectionSize)
                ("bjet_tag_position",    btag_order[bjetCandidate])
                ("bWj1Wj2_isGenMatched", isGenMatched)
                ("fatjet_isGenMatched",  fatjet_isGenMatched)
                ("b_isGenMatched",       b_isGenMatched)
                ("counter",              (inputTree -> getProcessedFileCount() - 1))
                    .fill();
              }
          }
        }
    } else if (typeTop == 2) {
        countFatAK8++;
        collectionSize = jet_ptrsAK8.size();
        for ( std::vector<const RecoJetAK8*>::const_iterator jetIter = jet_ptrsAK8.begin();
          jetIter != jet_ptrsAK8.end(); ++jetIter ) {
            for ( unsigned int bjetCandidate = 0; bjetCandidate < selJets.size(); bjetCandidate++ ) {
                unfittedHadTopP4 = (*jetIter)->p4() + (*selJets[bjetCandidate]).p4();
                selBJet = (*selJets[bjetCandidate]).p4();
                selWJet1 = (*jetIter)->subJet1()->p4();
                selWJet2 = (*jetIter)->subJet2()->p4();
                //std::cout<<"mass "<< unfittedHadTopP4.mass() <<" value "<< (selBJet + selWJet1 + selWJet2).mass() <<std::endl;
                // calculate matching
                std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop, typeTop,(*jetIter)->p4());
                std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop, typeTop,(*jetIter)->p4());
                if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
                if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
                isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
                fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
                b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
                if (btag_order[bjetCandidate] > cutJetCombo) continue;
                if (isGenMatched) hadtruth2++;
                if ( bdt_filler ) {
                  (*hadTopTaggerFill)(selBJet, selWJet1, selWJet2);
                  const std::map<std::string, double>& mvaInputs =  hadTopTaggerFill->mvaInputs();
                  for ( std::map<std::string, double>::const_iterator mvaInput = mvaInputs.begin();
                    mvaInput != mvaInputs.end(); ++mvaInput ) {
                    bdt_filler->operator()(mvaInput->first, mvaInput->second);
                  }
                  bdt_filler -> operator()({ eventInfo.run, eventInfo.lumi, eventInfo.event })
                  ("typeTop",              typeTop)
                  ("tau32Top",             -1.)
                  ("massTop",              unfittedHadTopP4.mass())
                  ("tau21W",               (*jetIter)->tau2()/(*jetIter)->tau1())
                  ("massW_SD",             (*jetIter)->msoftdrop())
                  ("btagDisc",             (*selJets[bjetCandidate]).BtagCSV())
                  ("qg_Wj1",                -1.)
                  ("qg_Wj2",                -1.)
                  ("genTopPt",             genTopPt)
                  ("collectionSize",       collectionSize)
                  ("bjet_tag_position",    btag_order[bjetCandidate])
                  ("bWj1Wj2_isGenMatched", isGenMatched)
                  ("fatjet_isGenMatched",  fatjet_isGenMatched)
                  ("b_isGenMatched",       b_isGenMatched)
                  ("counter",              (inputTree -> getProcessedFileCount() - 1))
                      .fill();
                }
            }
            }
        } else if (typeTop == 3) {
          countResolved++;
          collectionSize = selJets.size();
          for ( unsigned int bjetCandidate = 0; bjetCandidate < selJets.size(); bjetCandidate++ )  {
            for ( std::vector<const RecoJet*>::const_iterator selWJet1Candidate = selJets.begin();
              selWJet1Candidate != selJets.end(); ++selWJet1Candidate ) {
               if ( (*selWJet1Candidate)->pt() == (*selJets[bjetCandidate]).pt() ) continue;
               for ( std::vector<const RecoJet*>::const_iterator selWJet2Candidate = selWJet1Candidate + 1;
                 selWJet2Candidate != selJets.end(); ++selWJet2Candidate ) {
                  if ( (*selWJet2Candidate)->pt() == (*selJets[bjetCandidate]).pt() ) continue;
                  if ( (*selWJet2Candidate)->pt() == (*selWJet1Candidate)->pt() ) continue;
                  selBJet = (*selJets[bjetCandidate]).p4();
                  selWJet1 = (*selWJet1Candidate)->p4();
                  selWJet2 = (*selWJet2Candidate)->p4();
                  // calculate gen matching
                  std::map<int, bool> genMatchingTop = isGenMatchedJetTriplet(selBJet, selWJet1, selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenTop, genTopPtProbeTop);
                  std::map<int, bool> genMatchingAntiTop = isGenMatchedJetTriplet(selBJet, selWJet1,  selWJet2, genTopQuarks, genBJets, genWBosons, genWJets, kGenAntiTop, genTopPtProbeAntiTop);
                  if(genMatchingTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeTop;
                  if(genMatchingAntiTop[kGenMatchedTriplet]) genTopPt=genTopPtProbeAntiTop;
                  isGenMatched = (genMatchingTop[kGenMatchedTriplet] || genMatchingAntiTop[kGenMatchedTriplet]);
                  fatjet_isGenMatched = (genMatchingTop[kGenMatchedFatJet] || genMatchingAntiTop[kGenMatchedFatJet]);
                  b_isGenMatched = (genMatchingTop[kGenMatchedBJet] || genMatchingAntiTop[kGenMatchedBJet]);
                  if (btag_order[bjetCandidate] > cutJetCombo) continue;
                  if (isGenMatched) hadtruth3++;
                  //std::cout<<"btag position = "<< btag_order[bjetCandidate] <<std::endl;
                  if ( bdt_filler ) {
                    (*hadTopTaggerFill)(selBJet, selWJet1, selWJet2);
                    const std::map<std::string, double>& mvaInputs =  hadTopTaggerFill->mvaInputs();
                    for ( std::map<std::string, double>::const_iterator mvaInput = mvaInputs.begin();
                      mvaInput != mvaInputs.end(); ++mvaInput ) {
                      bdt_filler->operator()(mvaInput->first, mvaInput->second);
                    }
                  }
                  bdt_filler -> operator()({ eventInfo.run, eventInfo.lumi, eventInfo.event })
                  ("typeTop",              typeTop)
                  ("tau32Top",              -1.)
                  ("massTop",              -1.) // should be the one calculated with the groomed jet
                  ("tau21W",                -1.)
                  ("massW_SD",             -1.) // maybe add the kinfitted mass here
                  ("btagDisc",             (*selJets[bjetCandidate]).BtagCSV())
                  ("qg_Wj1",               (*selWJet1Candidate)->QGDiscr())
                  ("qg_Wj2",               (*selWJet2Candidate)->QGDiscr())
                  ("genTopPt",             genTopPt)
                  ("collectionSize",       collectionSize)
                  ("bjet_tag_position",    btag_order[bjetCandidate])
                  ("bWj1Wj2_isGenMatched", isGenMatched)
                  ("fatjet_isGenMatched",  fatjet_isGenMatched)
                  ("b_isGenMatched",       b_isGenMatched)
                  ("counter",              (inputTree -> getProcessedFileCount() - 1))
                      .fill();
               }
            }
          }
    } //else throw cms::Exception("analyze_hadTopTagger")
      //<< "Does not fall in any category " << " !!\n";
  }

		++selectedEntries;
    //selectedEntries_weighted += evtWeight;
    histogram_selectedEntries->Fill(0.);
  }
  //if(bdt_filler) bdt_filler->write();

  std::cout << "AK8 separation = " << countFatTop << " " << countFatAK8 << " " << countResolved<< " " << countFatTop+countFatAK8+countResolved<<std::endl;
  std::cout << "AK12 separation = " << countFatTop << " " << countFatAK12 << " " << countResolved<< " " << countFatTop+countFatAK12+countResolved<<std::endl;
  std::cout << "truth counters = "<<hadtruth1 << " "<<hadtruth2 << " "<<hadtruth3 << std::endl;


  std::cout << "max num. Entries = " << inputTree -> getCumulativeMaxEventCount()
            << " (limited by " << maxEvents << ") processed in "
            << inputTree -> getProcessedFileCount() << " file(s) (out of "
            << inputTree -> getFileCount() << ")\n"
            << " analyzed = " << analyzedEntries << '\n'
            << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")\n\n"
            << "cut-flow table" << std::endl;

	cutFlowTable_ttH.print(std::cout);
	std::cout << "\nHTTv2:" << std::endl;
	cutFlowTable_ttH_HTTv2.print(std::cout);
	std::cout << "\nAK12:" << std::endl;
	cutFlowTable_ttH_AK12.print(std::cout);
	std::cout << "\nresolved:" << std::endl;
	cutFlowTable_ttH_resolved.print(std::cout);

  delete jetReader;
  delete genJetReader;
  delete lheInfoReader;
  delete hadTopTaggerFill;
  delete inputTree;

  clock.Show("analyze_hadTopTagger");

  return EXIT_SUCCESS;
}
