#ifndef tthAnalysis_HiggsToTauTau_GenParticleWriter_h
#define tthAnalysis_HiggsToTauTau_GenParticleWriter_h

#include "tthAnalysis/HiggsToTauTau/interface/GenParticle.h" // GenParticle

#include <Rtypes.h> // Int_t, Double_t
#include <TTree.h> // TTree

#include <string>
#include <vector>

class GenParticleWriter
{
 public:
  GenParticleWriter(const std::string& branchName_num, const std::string& branchName_obj);
  ~GenParticleWriter();

  /**
   * @brief Call tree->Branch for all GenParticle branches
   */
  void setBranches(TTree* tree);

  /**
   * @brief Write collection of GenParticle objects to tree
   */
  void write(const std::vector<GenParticle>& particles);
  
 protected: 
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void setBranchNames();

  const int max_nParticles_;
  std::string branchName_num_;
  std::string branchName_obj_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_charge_;
  std::string branchName_pdgId_;

  Int_t nParticles_;
  Float_t* particle_pt_;
  Float_t* particle_eta_;
  Float_t* particle_phi_;
  Float_t* particle_mass_;
  Float_t* particle_charge_;
  Int_t* particle_pdgId_;
};

#endif // tthAnalysis_HiggsToTauTau_GenParticleWriter_h

