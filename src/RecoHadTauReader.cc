#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTauReader.h" // RecoHadTauReader

#include "FWCore/Utilities/interface/Exception.h"

#include "tthAnalysis/HiggsToTauTau/interface/LocalFileInPath.h" // LocalFileInPath

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // kEra_2015, kEra_2016

#include <TString.h> // Form

std::map<std::string, int> RecoHadTauReader::numInstances_;
std::map<std::string, RecoHadTauReader*> RecoHadTauReader::instances_;

RecoHadTauReader::RecoHadTauReader(int era, bool readGenMatching)
  : tauIdMVArun2dR03DB_wpFile_(0)
  , DBdR03oldDMwLTEff95_(0)
  , mvaOutput_normalization_DBdR03oldDMwLT_(0)
  , era_(era)
  , max_nHadTaus_(32)
  , branchName_num_("nTauGood")
  , branchName_obj_("TauGood")
  , genLeptonReader_(0)
  , genHadTauReader_(0)
  , genJetReader_(0)
  , readGenMatching_(readGenMatching)
  , hadTauPt_option_(RecoHadTauReader::kHadTauPt_central)
  , hadTau_pt_(0)
  , hadTau_eta_(0)
  , hadTau_phi_(0)
  , hadTau_mass_(0)
  , hadTau_charge_(0)
  , hadTau_dxy_(0)
  , hadTau_dz_(0)
  , hadTau_decayMode_(0)
  , hadTau_idDecayMode_(0)
  , hadTau_idDecayModeNewDMs_(0)
  , hadTau_idMVA_dR03_(0)
  , hadTau_rawMVA_dR03_(0)
  , hadTau_idMVA_dR05_(0)
  , hadTau_rawMVA_dR05_(0)
  , hadTau_idCombIso_dR03_(0)
  , hadTau_rawCombIso_dR03_(0)
  , hadTau_idCombIso_dR05_(0)
  , hadTau_rawCombIso_dR05_(0)    
  , hadTau_idAgainstElec_(0)
  , hadTau_idAgainstMu_(0)
{
  if ( readGenMatching_ ) {
    genLeptonReader_ = new GenLeptonReader(Form("%s_genLepton", branchName_num_.data()), Form("%s_genLepton", branchName_obj_.data()));
    genHadTauReader_ = new GenHadTauReader(Form("%s_genHadTau", branchName_num_.data()), Form("%s_genHadTau", branchName_obj_.data()));
    genJetReader_ = new GenJetReader(Form("%s_genJet", branchName_num_.data()), Form("%s_genJet", branchName_obj_.data()));
  }
  setBranchNames();
  readDBdR03oldDMwLTEff95();
}

RecoHadTauReader::RecoHadTauReader(int era, const std::string& branchName_num, const std::string& branchName_obj, bool readGenMatching)
  : tauIdMVArun2dR03DB_wpFile_(0)
  , DBdR03oldDMwLTEff95_(0)
  , mvaOutput_normalization_DBdR03oldDMwLT_(0)
  , era_(era)
  , max_nHadTaus_(32)
  , branchName_num_(branchName_num)
  , branchName_obj_(branchName_obj)
  , genLeptonReader_(0)
  , genHadTauReader_(0)
  , genJetReader_(0)
  , readGenMatching_(readGenMatching)
  , hadTauPt_option_(RecoHadTauReader::kHadTauPt_central)
  , hadTau_pt_(0)
  , hadTau_eta_(0)
  , hadTau_phi_(0)
  , hadTau_mass_(0)
  , hadTau_charge_(0)
  , hadTau_dxy_(0)
  , hadTau_dz_(0)
  , hadTau_decayMode_(0)
  , hadTau_idDecayMode_(0)
  , hadTau_idDecayModeNewDMs_(0)
  , hadTau_idMVA_dR03_(0)
  , hadTau_rawMVA_dR03_(0)
  , hadTau_idMVA_dR05_(0)
  , hadTau_rawMVA_dR05_(0)
  , hadTau_idCombIso_dR03_(0)
  , hadTau_rawCombIso_dR03_(0)
  , hadTau_idCombIso_dR05_(0)
  , hadTau_rawCombIso_dR05_(0)   
  , hadTau_idAgainstElec_(0)
  , hadTau_idAgainstMu_(0)
{
  if ( readGenMatching_ ) {
    genLeptonReader_ = new GenLeptonReader(Form("%s_genLepton", branchName_num_.data()), Form("%s_genLepton", branchName_obj_.data()));
    genHadTauReader_ = new GenHadTauReader(Form("%s_genHadTau", branchName_num_.data()), Form("%s_genHadTau", branchName_obj_.data()));
    genJetReader_ = new GenJetReader(Form("%s_genJet", branchName_num_.data()), Form("%s_genJet", branchName_obj_.data()));
  }
  setBranchNames();
  readDBdR03oldDMwLTEff95();
}

RecoHadTauReader::~RecoHadTauReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if ( numInstances_[branchName_obj_] == 0 ) {
    numInstances_.erase(branchName_obj_);

    int numInstances_total = 0;
    for ( std::map<std::string, int>::const_iterator it = numInstances_.begin();
	  it != numInstances_.end(); ++it ) {
      numInstances_total += it->second;
    }
    if ( numInstances_total == 0 ) {
      RecoHadTauReader* gInstance = instances_.begin()->second;
      assert(gInstance);
      delete gInstance->tauIdMVArun2dR03DB_wpFile_;
    }
    
    RecoHadTauReader* gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete gInstance->genLeptonReader_;
    delete gInstance->genHadTauReader_;
    delete gInstance->genJetReader_;
    delete[] gInstance->hadTau_pt_;
    delete[] gInstance->hadTau_eta_;
    delete[] gInstance->hadTau_phi_;
    delete[] gInstance->hadTau_mass_;
    delete[] gInstance->hadTau_dxy_;
    delete[] gInstance->hadTau_dz_;
    delete[] gInstance->hadTau_decayMode_;
    delete[] gInstance->hadTau_idDecayMode_;
    delete[] gInstance->hadTau_idDecayModeNewDMs_;
    delete[] gInstance->hadTau_idMVA_dR03_;
    delete[] gInstance->hadTau_rawMVA_dR03_;
    delete[] gInstance->hadTau_idMVA_dR05_;
    delete[] gInstance->hadTau_rawMVA_dR05_;
    delete[] gInstance->hadTau_idCombIso_dR03_;
    delete[] gInstance->hadTau_rawCombIso_dR03_;
    delete[] gInstance->hadTau_idCombIso_dR05_;
    delete[] gInstance->hadTau_rawCombIso_dR05_;
    delete[] gInstance->hadTau_idAgainstElec_;
    delete[] gInstance->hadTau_idAgainstMu_;
    delete[] gInstance->hadTau_charge_;

    instances_.erase(branchName_obj_);
  }
}

void RecoHadTauReader::readDBdR03oldDMwLTEff95()
{
  RecoHadTauReader* gInstance = instances_.begin()->second;
  assert(gInstance);
  if ( !gInstance->tauIdMVArun2dR03DB_wpFile_ ) {
    LocalFileInPath tauIdMVArun2dR03DB_wpFilePath = LocalFileInPath("tthAnalysis/HiggsToTauTau/data/wpDiscriminationByIsolationMVARun2v1_DBdR03oldDMwLT.root");
    gInstance->tauIdMVArun2dR03DB_wpFile_ = new TFile(tauIdMVArun2dR03DB_wpFilePath.fullPath().c_str());
  }
  DBdR03oldDMwLTEff95_ = dynamic_cast<TGraph*>(gInstance->tauIdMVArun2dR03DB_wpFile_->Get("DBdR03oldDMwLTEff95"));
  mvaOutput_normalization_DBdR03oldDMwLT_ = dynamic_cast<TFormula*>(gInstance->tauIdMVArun2dR03DB_wpFile_->Get("mvaOutput_normalization_DBdR03oldDMwLT"));
}

void RecoHadTauReader::setBranchNames()
{
  if ( numInstances_[branchName_obj_] == 0 ) {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
    branchName_dxy_ = Form("%s_%s", branchName_obj_.data(), "dxy");
    branchName_dz_ = Form("%s_%s", branchName_obj_.data(), "dz");
    branchName_decayMode_ = Form("%s_%s", branchName_obj_.data(), "decayMode");
    branchName_idDecayMode_ = Form("%s_%s", branchName_obj_.data(), "idDecayMode");
    branchName_idDecayModeNewDMs_ = Form("%s_%s", branchName_obj_.data(), "idDecayModeNewDMs");
    branchName_idMVA_dR03_ = Form("%s_%s", branchName_obj_.data(), "idMVArun2dR03");
    branchName_rawMVA_dR03_ = Form("%s_%s", branchName_obj_.data(), "rawMVArun2dR03");
    branchName_idMVA_dR05_ = Form("%s_%s", branchName_obj_.data(), "idMVArun2");
    branchName_rawMVA_dR05_ = Form("%s_%s", branchName_obj_.data(), "rawMVArun2");
    branchName_idCombIso_dR03_ = Form("%s_%s", branchName_obj_.data(), "idCI3hitdR03");
    //branchName_rawCombIso_dR03_ = Form("%s_%s", branchName_obj_.data(), "isoCI3hitdR03"); // CV: branch does not exist in VHbb Ntuples yet
    branchName_idCombIso_dR05_ = Form("%s_%s", branchName_obj_.data(), "idCI3hit");
    branchName_rawCombIso_dR05_ = Form("%s_%s", branchName_obj_.data(), "isoCI3hit"); 
    branchName_idAgainstElec_ = Form("%s_%s", branchName_obj_.data(), "idAntiErun2");
    branchName_idAgainstMu_ = Form("%s_%s", branchName_obj_.data(), "idAntiMu");
    instances_[branchName_obj_] = this;
  } else {
    if ( branchName_num_ != instances_[branchName_obj_]->branchName_num_ ) {
      throw cms::Exception("RecoHadTauReader") 
	<< "Association between configuration parameters 'branchName_num' and 'branchName_obj' must be unique:"
	<< " present association 'branchName_num' = " << branchName_num_ << " with 'branchName_obj' = " << branchName_obj_ 
	<< " does not match previous association 'branchName_num' = " << instances_[branchName_obj_]->branchName_num_ << " with 'branchName_obj' = " << instances_[branchName_obj_]->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}

namespace
{
  void setValue_int(Int_t* array, int n, Int_t value = 0)
  {
    for ( int idx = 0; idx < n; ++idx ) {
      array[idx] = value;
    }
  }
  void setValue_float(Float_t* array, int n, Float_t value = 0.)
  {
    for ( int idx = 0; idx < n; ++idx ) {
      array[idx] = value;
    }
  }
}

void RecoHadTauReader::setBranchAddresses(TTree* tree)
{
  if ( instances_[branchName_obj_] == this ) {
    if ( readGenMatching_ ) {
      genLeptonReader_->setBranchAddresses(tree);
      genHadTauReader_->setBranchAddresses(tree);
      genJetReader_->setBranchAddresses(tree);  
    }
    tree->SetBranchAddress(branchName_num_.data(), &nHadTaus_);   
    hadTau_pt_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_pt_.data(), hadTau_pt_); 
    hadTau_eta_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_eta_.data(), hadTau_eta_); 
    hadTau_phi_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_phi_.data(), hadTau_phi_); 
    hadTau_mass_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_mass_.data(), hadTau_mass_); 
    hadTau_charge_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_charge_.data(), hadTau_charge_); 
    hadTau_dxy_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_dxy_.data(), hadTau_dxy_);
    hadTau_dz_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_dz_.data(), hadTau_dz_);
    hadTau_decayMode_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_decayMode_.data(), hadTau_decayMode_);
    hadTau_idDecayMode_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idDecayMode_.data(), hadTau_idDecayMode_);
    hadTau_idDecayModeNewDMs_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idDecayModeNewDMs_.data(), hadTau_idDecayModeNewDMs_);
    hadTau_idMVA_dR03_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idMVA_dR03_.data(), hadTau_idMVA_dR03_); 
    hadTau_rawMVA_dR03_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_rawMVA_dR03_.data(), hadTau_rawMVA_dR03_); 
    hadTau_idMVA_dR05_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idMVA_dR05_.data(), hadTau_idMVA_dR05_); 
    hadTau_rawMVA_dR05_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_rawMVA_dR05_.data(), hadTau_rawMVA_dR05_); 
    hadTau_idCombIso_dR03_ = new Int_t[max_nHadTaus_];
    if ( era_ == kEra_2015 ) { // CV: branch does not exist in VHbb Ntuples v24 produced by Andrea for 2016 data
      tree->SetBranchAddress(branchName_idCombIso_dR03_.data(), hadTau_idCombIso_dR03_); 
    } else {
      setValue_int(hadTau_idCombIso_dR03_, max_nHadTaus_, -1);
    }    
    hadTau_rawCombIso_dR03_ = new Float_t[max_nHadTaus_]; 
    setValue_float(hadTau_rawCombIso_dR03_, max_nHadTaus_, -1.); // CV: branch does not exist in VHbb Ntuples yet
    hadTau_idCombIso_dR05_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idCombIso_dR05_.data(), hadTau_idCombIso_dR05_); 
    hadTau_rawCombIso_dR05_ = new Float_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_rawCombIso_dR05_.data(), hadTau_rawCombIso_dR05_);
    hadTau_idAgainstElec_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idAgainstElec_.data(), hadTau_idAgainstElec_); 
    hadTau_idAgainstMu_ = new Int_t[max_nHadTaus_];
    tree->SetBranchAddress(branchName_idAgainstMu_.data(), hadTau_idAgainstMu_);
  }
}

std::vector<RecoHadTau> RecoHadTauReader::read() const
{
  RecoHadTauReader* gInstance = instances_[branchName_obj_];
  assert(gInstance);
  std::vector<RecoHadTau> hadTaus;
  Int_t nHadTaus = gInstance->nHadTaus_;
  if ( nHadTaus > max_nHadTaus_ ) {
    throw cms::Exception("RecoHadTauReader") 
      << "Number of hadronic taus stored in Ntuple = " << nHadTaus << ", exceeds max_nHadTaus = " << max_nHadTaus_ << " !!\n";
  }
  if ( nHadTaus > 0 ) {
    hadTaus.reserve(nHadTaus);
    for ( Int_t idxHadTau = 0; idxHadTau < nHadTaus; ++idxHadTau ) {
      Float_t hadTau_pt = -1.;
      if      ( hadTauPt_option_ == RecoHadTauReader::kHadTauPt_central   ) hadTau_pt = 1.00*gInstance->hadTau_pt_[idxHadTau];
      else if ( hadTauPt_option_ == RecoHadTauReader::kHadTauPt_shiftUp   ) hadTau_pt = 1.03*gInstance->hadTau_pt_[idxHadTau];
      else if ( hadTauPt_option_ == RecoHadTauReader::kHadTauPt_shiftDown ) hadTau_pt = 0.97*gInstance->hadTau_pt_[idxHadTau];
      else assert(0);
      // compute "VVLose" (95% signal efficiency) working point for tau ID MVA trained for dR=0.3 isolation cone,
      // used to enhance background event statistics for training of event-level MVAs that separate ttH signal from backgrounds
      Int_t hadTau_idMVA_dR03 = hadTau_idMVA_dR03_[idxHadTau];
      if ( hadTau_idMVA_dR03 >= 1 ) {
        hadTau_idMVA_dR03 += 1;
      } else {
        assert(DBdR03oldDMwLTEff95_ && mvaOutput_normalization_DBdR03oldDMwLT_);
        if ( mvaOutput_normalization_DBdR03oldDMwLT_->Eval(gInstance->hadTau_rawMVA_dR03_[idxHadTau]) > DBdR03oldDMwLTEff95_->Eval(gInstance->hadTau_pt_[idxHadTau]) ) {
 	  hadTau_idMVA_dR03 = 1;
        } else { 
	  hadTau_idMVA_dR03 = 0;
	}
      }
      hadTaus.push_back(RecoHadTau(
        hadTau_pt,
        gInstance->hadTau_eta_[idxHadTau],
	gInstance->hadTau_phi_[idxHadTau],
	gInstance->hadTau_mass_[idxHadTau],
	gInstance->hadTau_charge_[idxHadTau],
	gInstance->hadTau_dxy_[idxHadTau],
	gInstance->hadTau_dz_[idxHadTau],
	gInstance->hadTau_decayMode_[idxHadTau],
	gInstance->hadTau_idDecayMode_[idxHadTau],
	gInstance->hadTau_idDecayModeNewDMs_[idxHadTau],
	hadTau_idMVA_dR03,
	gInstance->hadTau_rawMVA_dR03_[idxHadTau],
	gInstance->hadTau_idMVA_dR05_[idxHadTau],
	gInstance->hadTau_rawMVA_dR05_[idxHadTau],	
	gInstance->hadTau_idCombIso_dR03_[idxHadTau],
	gInstance->hadTau_rawCombIso_dR03_[idxHadTau],
	gInstance->hadTau_idCombIso_dR05_[idxHadTau],
	gInstance->hadTau_rawCombIso_dR05_[idxHadTau],	
	gInstance->hadTau_idAgainstElec_[idxHadTau],
	gInstance->hadTau_idAgainstMu_[idxHadTau] ));
    }
    readGenMatching(hadTaus);
  }
  return hadTaus;
}

void RecoHadTauReader::readGenMatching(std::vector<RecoHadTau>& hadTaus) const
{
  if ( readGenMatching_ ) {
    assert(genLeptonReader_ && genHadTauReader_ && genJetReader_);
    size_t nHadTaus = hadTaus.size();
    std::vector<GenLepton> matched_genLeptons = genLeptonReader_->read();
    assert(matched_genLeptons.size() == nHadTaus);
    std::vector<GenHadTau> matched_genHadTaus = genHadTauReader_->read();
    assert(matched_genHadTaus.size() == nHadTaus);
    std::vector<GenJet> matched_genJets = genJetReader_->read();
    assert(matched_genJets.size() == nHadTaus);
    for ( size_t idxHadTau = 0; idxHadTau < nHadTaus; ++idxHadTau ) {
      RecoHadTau* hadTau = &hadTaus[idxHadTau];
      const GenLepton& matched_genLepton = matched_genLeptons[idxHadTau];
      if ( matched_genLepton.isValid() ) hadTau->set_genLepton(new GenLepton(matched_genLepton), true);
      const GenHadTau& matched_genHadTau = matched_genHadTaus[idxHadTau];
      if ( matched_genHadTau.isValid() ) hadTau->set_genHadTau(new GenHadTau(matched_genHadTau), true);
      const GenJet& matched_genJet = matched_genJets[idxHadTau];
      if ( matched_genJet.isValid() ) hadTau->set_genJet(new GenJet(matched_genJet), true);
    }
  }
}
